// Includes
#include"include/yearwinter.h"

// Formatting
#define FATAL_ERR_MSG "\x1b[31;1;4;5;7m" "FATAL ERROR:" "\x1b[0m" " "
#define WARN_MSG "\x1b[35;1m" "WARNING:" "\x1b[0m" " "

// For functions with singular scope where the declaration of a variable manually seems like a chore, and calling functions of return type YWE_Err manually.
#define ER_RAISE_ERROR_ERR(game, error) {YWE_err macro_erval = YWER_ALL_CLEAR; YWER_RAISE_ERROR_ERR((game), macro_erval, (error))}
#define ER_RAISE_ERROR_ERRPTR(game, pointer, error) {YWE_err macro_erval = YWER_ALL_CLEAR; YWER_RAISE_ERROR_ERRPTR((game), macro_erval, (pointer), (error))}
#define ER_RAISE_ERROR_ERRINT(game, pointer, error) {YWE_err macro_erval = YWER_ALL_CLEAR; YWER_RAISE_ERROR_ERRINT((game), macro_erval, (integer), (error))}

#define C_RAISE_ERR(error) {ER_RAISE_ERROR_ERR(game, (error))}
#define C_RAISE_ERRPTR(pointer, error) {ER_RAISE_ERROR_ERRPTR(game, (pointer), (error))}
#define C_RAISE_ERRINT(integer, error) {ER_RAISE_ERROR_ERRINT(game, (integer), (error))}

#define PASS_BACK_ERR(call) {YWE_err macro_erback = call; if(YWER_ERROR(macro_erback)){return macro_erback;}}

YWE_err YWE_InputEvent(YWE_Engine *game, SDL_Event *event)
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

YWE_err YWE_DrawBackgroundTexture(YWE_Engine *game)
{
	YWE_VN *vn = &(game->vn);
	if(!(vn->background.tex))
	{
		vn->background.tex = IMG_LoadTexture(game->renderer, "assets/images/background.png");
		if(!(vn->background.tex))
		{
			ER_RAISE_ERROR_ERR(game, YWER_EGNS_GENERAL);
		}
	}
	return YWER_ALL_CLEAR;
}

YWE_err YWE_FrameRender(YWE_Engine *game)
{
	YWE_VN *vn = &(game->vn);

	YWE_DrawBackgroundTexture(game);

	SDL_SetRenderDrawColor(game->renderer, 16, 16, 160, 255);
	SDL_RenderTexture(game->renderer, vn->background.tex, NULL, NULL);

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
			fprintf(stderr, WARN_MSG "Frame target missed by %" PRIu64 " ticks. Sleep cancelled. Current Tick: %" PRIu64 YWE_FM_RESET "\n", curtick - game->frame.next_tick_target, curtick);
			game->frame.last_frame_end = curtick;
			return;
		}
		game->frame.last_frame_end = game->frame.next_tick_target;
		game->frame.frame_delay = game->frame.next_tick_target - curtick;
	}
	return;
}