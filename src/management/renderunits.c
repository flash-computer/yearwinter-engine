// Destroy a RenderUnit
YWE_Err YWE_DestroyRenderUnit(YWE_Engine *game, YWE_RenderUnit *ru)
{
	if(!ru)
	{
		return YWER_ALL_CLEAR;
	}
	for(size_t i=0; i < ru->children_count; i++)
	{
		PASS_BACK_ERR(YWE_DestroyRenderUnit(game, ru->children + i));
	}
	ru->children_count = 0;
	free(ru->children);
	SDL_DestroyTexture(ru->tex);
	SDL_DestroySurface(ru->surface);

	return YWER_ALL_CLEAR;
}

// Initialize a RenderUnit
YWE_ErrPtr YWE_InitRenderUnit(YWE_Engine *game, YWE_RenderUnit *ru)
{
	if(!ru)
	{
		ru = malloc(sizeof(YWE_RenderUnit));
		if(!ru)
		{
			C_RAISE_ERRPTR(NULL, YWER_EMMR_ALLOC);
		}
	}

	ru->name[0] = '\0';

	ru->parent = NULL;
	ru->children = NULL;
	ru->children_count = 0;

	ru->tex = NULL;
	ru->surface = NULL;

	ru->src = (SDL_FRect){0, 0, 0, 0};
	ru->dst = (SDL_FRect){0, 0, 0, 0};
	ru->abs_src = (SDL_FRect){0, 0, 0, 0};
	ru->abs_dst = (SDL_FRect){0, 0, 0, 0};

	ru->prop.no_src = true;
	ru->prop.no_dst = true;
	ru->prop.src_is_rel = true;
	ru->prop.dst_is_rel = true;
	ru->prop.volatile_target_mode = YWE_VOLATILE_PRESERVE;
	ru->prop.volatile_target = false;
	ru->prop.to_free = (!ru) ? true : false;
	return (YWE_ErrPtr){ru, YWER_ALL_CLEAR};
}

YWE_ErrPtr YWE_CreateRenderUnitArray(YWE_Engine *game, YWE_RenderUnit *parent, size_t count)
{
	if((!count) || ((SIZE_MAX / count) < sizeof(YWE_RenderUnit)))
	{
		C_RAISE_ERRPTR(NULL, YWER_EFNR_ARGS);
	}

	YWE_RenderUnit *array = (YWE_RenderUnit *)malloc(sizeof(YWE_RenderUnit) * count);
	if(!array)
	{
		C_RAISE_ERRPTR(NULL, YWER_EMMR_ALLOC);
	}

	for(size_t i=0; i<count; i++)
	{
		YWE_InitRenderUnit(game, array+i);
		array[i].parent = parent;
	}

	if(parent)
	{
		parent->children_count = count;
	}
	return (YWE_ErrPtr){array, YWER_ALL_CLEAR};
}

YWE_Err YWE_DestroyRenderUnitArray(YWE_Engine *game, YWE_RenderUnit *array, size_t size)
{
	if(!array)
	{
		return YWER_ALL_CLEAR;
	}
	for(size_t i=0; i<size; i++)
	{
		PASS_BACK_ERR(YWE_DestroyRenderUnit(game, array + i));
	}
	return YWER_ALL_CLEAR;
}

YWE_ErrPtr YWE_GetRenderUnitRectSrc(YWE_Engine *game, YWE_RenderUnit *ru)
{
	if(!ru)
	{
		C_RAISE_ERRPTR(NULL, YWER_EFNR_ARGS);
	}
	if(ru->prop.no_src)
	{
		return (YWE_ErrPtr){NULL, YWER_ALL_CLEAR};
	}
	if(ru->prop.src_is_rel)
	{
		int width = 0;
		int height = 0;
		if(!SDL_GetRenderOutputSize(game->renderer, &width, &height))
		{
			C_RAISE_ERRPTR(NULL, YWER_EGNS_GENERAL);
		}
		ru->abs_src.x = ru->src.x * width;
		ru->abs_src.w = ru->src.w * width;
		ru->abs_src.y = ru->src.y * height;
		ru->abs_src.h = ru->src.h * height;
	}
	return (YWE_ErrPtr){&(ru->abs_src), YWER_ALL_CLEAR};
}

YWE_ErrPtr YWE_GetRenderUnitRectDst(YWE_Engine *game, YWE_RenderUnit *ru)
{
	if(!ru)
	{
		C_RAISE_ERRPTR(NULL, YWER_EFNR_ARGS);
	}
	if(ru->prop.no_dst)
	{
		return (YWE_ErrPtr){NULL, YWER_ALL_CLEAR};
	}
	if(ru->prop.dst_is_rel)
	{
		int width = 0;
		int height = 0;
		if(!SDL_GetRenderOutputSize(game->renderer, &width, &height))
		{
			C_RAISE_ERRPTR(NULL, YWER_EGNS_GENERAL);
		}
		ru->abs_dst.x = ru->dst.x * width;
		ru->abs_dst.w = ru->dst.w * width;
		ru->abs_dst.y = ru->dst.y * height;
		ru->abs_dst.h = ru->dst.h * height;
	}
	return (YWE_ErrPtr){&(ru->abs_dst), YWER_ALL_CLEAR};
}