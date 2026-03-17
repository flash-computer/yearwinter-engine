// Render to Render Units

YWE_Err YWE_RenderRenderUnit(YWE_Engine *game, YWE_RenderUnit *ru, YWE_VolatileMode volatile_target_mode, bool root)
{
	if(!ru)
	{
		C_RAISE_ERR(YWER_EFNR_ARGS);
	}
	// Get current render target to restore at the end.
	SDL_Texture *curTarget = SDL_GetRenderTarget(game->renderer);
	if(ru->prop.volatile_target)
	{
		if(!(ru->tex))
		{
			int width = 0;
			int height = 0;
			if(ru->prop.no_src)
			{
				if(!SDL_GetRenderOutputSize(game->renderer, &width, &height))
				{
					C_RAISE_ERR(YWER_EGNS_GENERAL);
				}
			}
			ru->tex = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (ru->prop.no_src)?width:ru->src.w, (ru->prop.no_src)?height:ru->src.h);
			if(!(ru->tex))
			{
				C_RAISE_ERR(YWER_EGNS_GENERAL);
			}
			// Set Target as fully transparent
			SDL_SetTextureAlphaMod(ru->tex, 0);
		}
		// If the texture has SDL_TextureAccess Set texture as render target
		SDL_PropertiesID tex_properties = SDL_GetTextureProperties(ru->tex);
		if(!tex_properties)
		{
			C_RAISE_ERR(YWER_EGNS_GENERAL);
		}
		Sint64 access = SDL_GetNumberProperty(tex_properties, SDL_PROP_TEXTURE_ACCESS_NUMBER, 64+SDL_TEXTUREACCESS_TARGET);
		if(access == SDL_TEXTUREACCESS_TARGET)
		{
			// Texture is eligible to be a RenderTarget
			SDL_SetRenderTarget(game->renderer, ru->tex);

			// Recursively Render all child RenderUnits onto the current RenderUnit
			for(size_t i=0; i<ru->children_count; i++)
			{
				PASS_BACK_ERR(YWE_RenderRenderUnit(game, ru->children + i, ru->children[i].prop.volatile_target_mode, false));
			}
		}
		else
		{
			if(ru->children)
			{
				// Consider instead outputting the children to higher RenderTarget which is targeteable.
				YWE_STDERR_OUTPUT(game, WARN_MSG "Children on Render Unit without SDL_TEXTUREACCESS_TARGET. Skipping\n");
			}
		}
	}

	switch(volatile_target_mode)
	{
		case YWE_VOLATILE_TOGGLE:
			ru->prop.volatile_target = !(ru->prop.volatile_target);
			break;
		case YWE_VOLATILE_TRUE:
			ru->prop.volatile_target = true;
			break;
		case YWE_VOLATILE_FALSE:
			ru->prop.volatile_target = false;
			break;
		default:
			break;
	}

	SDL_SetRenderTarget(game->renderer, curTarget);
	if(!(root))
	{
		if(ru->parent)
		{
			SDL_RenderTexture(game->renderer, ru->tex, YWE_PRUSP(ru), YWE_PRUDP(ru));
		}
		else
		{
			YWE_STDERR_OUTPUT(game, WARN_MSG "Non root Target with missing parent. Not Rendered.\n");
		}
	}
	return YWER_ALL_CLEAR;
}

// Placeholder until the rendering process is properly decided
YWE_Err YWE_DrawBackgroundTexture(YWE_Engine *game)
{
	YWE_VN *vn = &(game->vn);

	if(!(vn->units[0].children))
	{
		YWE_ErrPtr retval = YWE_CreateRenderUnitArray(game, vn->units + 0, 4);
		if(YWER_ERROR(retval.ret))
		{
			return retval.ret;
		}
		vn->units[0].children = (YWE_RenderUnit *)(retval.value);
		if(!(vn->units[0].children[0].tex))
		{
			vn->units[0].children[0].tex = IMG_LoadTexture(game->renderer, "assets/images/background.png");
			if(!(vn->units[0].children[0].tex))
			{
				C_RAISE_ERR(YWER_EGNS_GENERAL);
			}
		}
		SDL_SetTextureAlphaMod(vn->units[0].children[0].tex, 128);

		if(!(vn->units[0].children[1].tex))
		{
			vn->units[0].children[1].tex = IMG_LoadTexture(game->renderer, "assets/images/textbox.png");
			if(!(vn->units[0].children[1].tex))
			{
				C_RAISE_ERR(YWER_EGNS_GENERAL);
			}
			vn->units[0].children[1].dst = (SDL_FRect){0.083, 0.7, 0.833, 0.3};
			vn->units[0].children[1].prop.no_dst = false;
		}

		vn->units[0].prop.volatile_target = true;
		vn->units[0].prop.volatile_target_mode = YWE_VOLATILE_FALSE;
	}

	YWE_RenderRenderUnit(game, vn->units + 0, vn->units[0].prop.volatile_target_mode, true);
	return YWER_ALL_CLEAR;
}

// Render the frame to the offscreen buffer
YWE_Err YWE_FrameRender(YWE_Engine *game)
{
	YWE_VN *vn = &(game->vn);

	YWE_DrawBackgroundTexture(game);

	SDL_SetRenderDrawColor(game->renderer, 16, 16, 160, 255);

	SDL_SetRenderTarget(game->renderer, NULL);
	for(int i=0; i<YWE_VN_TOP_LEVEL_RENDER_UNITS; i++)
	{
			SDL_RenderTexture(game->renderer, vn->units[i].tex, YWE_RUSP(vn->units[i]), YWE_RUDP(vn->units[i]));
	}

	return YWER_ALL_CLEAR;
}
