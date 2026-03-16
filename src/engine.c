// Includes
#include"include/yearwinter.h"

// Formatting
#define FATAL_ERR_MSG "\x1b[31;1;4;5;7m" "FATAL ERROR:" "\x1b[0m" " "
#define WARN_MSG "\x1b[35;1m" "WARNING:" "\x1b[0m" " "

// For functions with singular scope where the declaration of a variable manually seems like a chore, and calling functions of return type YWE_Err manually.
#define ER_RAISE_ERROR_ERR(game, error) {YWE_Err macro_erval = YWER_ALL_CLEAR; YWER_RAISE_ERROR_ERR((game), macro_erval, (error))}
#define ER_RAISE_ERROR_ERRPTR(game, pointer, error) {YWE_Err macro_erval = YWER_ALL_CLEAR; YWER_RAISE_ERROR_ERRPTR((game), macro_erval, (pointer), (error))}
#define ER_RAISE_ERROR_ERRINT(game, pointer, error) {YWE_Err macro_erval = YWER_ALL_CLEAR; YWER_RAISE_ERROR_ERRINT((game), macro_erval, (integer), (error))}

#define C_RAISE_ERR(error) {ER_RAISE_ERROR_ERR(game, (error))}
#define C_RAISE_ERRPTR(pointer, error) {ER_RAISE_ERROR_ERRPTR(game, (pointer), (error))}
#define C_RAISE_ERRINT(integer, error) {ER_RAISE_ERROR_ERRINT(game, (integer), (error))}

#define PASS_BACK_ERR(call) {YWE_Err macro_erback = call; if(YWER_ERROR(macro_erback)){return macro_erback;}}

YWE_Err YWE_InputEvent(YWE_Engine *game, SDL_Event *event)
{
	switch(event->type)
	{
		case SDL_EVENT_KEY_DOWN:
			switch(((SDL_KeyboardEvent *)event)->key) // Quit on Escape press
			{
				case SDLK_ESCAPE:
					YWE_ExitGame(game, 0);
					break;
				case SDLK_F11:
					SDL_SetWindowFullscreen(game->window, (SDL_GetWindowFlags(game->window) & SDL_WINDOW_FULLSCREEN)? false : true);
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	return YWER_ALL_CLEAR;
}

YWE_Err YWE_RenderRenderUnit(YWE_Engine *game, YWE_RenderUnit *ru)
{
	if(!ru)
	{
		C_RAISE_ERR(YWER_EFNR_ARGS);
	}
	// Get current render target to restore at the end.
	SDL_Texture *curTarget = SDL_GetRenderTarget(game->renderer);
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

	SDL_SetRenderTarget(game->renderer, curTarget);
	if(ru->parent)
	{
		SDL_RenderTexture(game->renderer, ru->tex, YWE_PRUSP(ru), YWE_PRUDP(ru));
	}
	return YWER_ALL_CLEAR;
}

YWE_Err YWE_DrawBackgroundTexture(YWE_Engine *game)
{
	YWE_VN *vn = &(game->vn);
	
	if(!(vn->background.children))
	{
		YWE_ErrPtr retval = YWE_CreateAndAppendRenderUnit(game, &(vn->background));
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
	}
	YWE_RenderRenderUnit(game, &(vn->background));

	return YWER_ALL_CLEAR;
}

YWE_Err YWE_DrawUITexture(YWE_Engine *game)
{
	YWE_VN *vn = &(game->vn);
	vn->ui.no_dst = false;
	return YWER_ALL_CLEAR;
}

YWE_Err YWE_FrameRender(YWE_Engine *game)
{
	YWE_VN *vn = &(game->vn);

	YWE_DrawBackgroundTexture(game);
	YWE_DrawUITexture(game);

	SDL_SetRenderDrawColor(game->renderer, 16, 16, 160, 255);

	SDL_SetRenderTarget(game->renderer, NULL);
	SDL_RenderTexture(game->renderer, vn->background.tex, YWE_RUSP(vn->background), YWE_RUDP(vn->background));
	SDL_RenderTexture(game->renderer, vn->ui.tex, YWE_RUSP(vn->background), YWE_RUDP(vn->ui));

	return YWER_ALL_CLEAR;
}

// Highest framecap is 1frame/ms or 1000 frames per second
void YWE_MarkFrame(YWE_Engine *game)
{
	if(game->frame.frame_target)
	{
		uint64_t frame_quantum = game->frame.frame_target_timescale / game->frame.frame_target;
		if(!frame_quantum)
		{
			frame_quantum = YWE_CONFIG_RENDERING_FALLBACK_FRAME_QUANTUM;
		}
		uint64_t first_quantum = ((game->frame.frame_target_timescale % frame_quantum) + frame_quantum);
		game->frame.next_tick_target = game->frame.last_frame_end + (((game->frame.last_frame_end % game->frame.frame_target_timescale) <= first_quantum)? first_quantum : frame_quantum);
	}
	return;
}

void YWE_MarkFrameEnd(YWE_Engine *game)
{
	if(game->frame.frame_target)
	{
		uint64_t curtick = SDL_GetTicks();
		if(curtick >= game->frame.next_tick_target)
		{
			game->frame.frame_delay = 0;
			YWE_STDERR_OUTPUT(game, WARN_MSG "Frame target missed by %" PRIu64 " ticks. Sleep cancelled. Current Tick: %" PRIu64 YWE_FM_RESET "\n", curtick - game->frame.next_tick_target, curtick);
			game->frame.last_frame_end = curtick;
			return;
		}
		game->frame.last_frame_end = game->frame.next_tick_target;
		game->frame.frame_delay = game->frame.next_tick_target - curtick;
	}
	return;
}