// Render to Render Units
YWE_Err YWE_RenderRenderUnit(YWE_Engine *game, YWE_RenderUnit *ru)
{
	if(!ru)
	{
		C_RAISE_ERR(YWER_EFNR_ARGS);
	}
	// Get current render target to restore at the end.
	SDL_Texture *curTarget = SDL_GetRenderTarget(game->renderer);
	if(ru->target)
	{
		if(!(ru->tex))
		{
			int width = 0;
			int height = 0;
			if(ru->no_src)
			{
				if(!SDL_GetRenderOutputSize(game->renderer, &width, &height))
				{
					C_RAISE_ERR(YWER_EGNS_GENERAL);
				}
			}
			ru->tex = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (ru->no_src)?width:ru->src.w, (ru->no_src)?height:ru->src.h);
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
			YWE_DNode *temp = ru->children;
			while(temp)
			{
				YWE_RenderRenderUnit(game, (YWE_RenderUnit *)temp->data);
				temp = temp->prev;
			}
		}
		else
		{
			if(ru->children)
			{
				// Consider instead outputting the children to higher RenderTarget which is targeteable.
				YWE_STDERR_OUTPUT(game, "Children on Render Unit without SDL_TEXTUREACCESS_TARGET. Skipping\n");
			}
		}
		ru->target = false;
	}

	SDL_SetRenderTarget(game->renderer, curTarget);
	if(ru->parent)
	{
		SDL_RenderTexture(game->renderer, ru->tex, YWE_PRUSP(ru), YWE_PRUDP(ru));
	}
	return YWER_ALL_CLEAR;
}

// Placeholder until the rendering process is properly decided
YWE_Err YWE_DrawBackgroundTexture(YWE_Engine *game)
{
	YWE_VN *vn = &(game->vn);

	if(!(vn->units[0].children))
	{
		YWE_ErrPtr retval = YWE_CreateAndAppendRenderUnit(game, vn->units + 0);
		if(YWER_ERROR(retval.ret))
		{
			return retval.ret;
		}
		YWE_RenderUnit *bgimage = (YWE_RenderUnit *)(((YWE_DNode *)(retval.value))->data);
		if(!(bgimage->tex))
		{
			bgimage->tex = IMG_LoadTexture(game->renderer, "assets/images/background.png");
			if(!(bgimage->tex))
			{
				C_RAISE_ERR(YWER_EGNS_GENERAL);
			}
		}
		SDL_SetTextureAlphaMod(bgimage->tex, 255);
		vn->units[0].target = true;
	}

	YWE_RenderRenderUnit(game, vn->units + 0);
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
