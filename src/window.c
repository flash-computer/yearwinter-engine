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

// Cleanup SDL and exit
void YWE_ExitGame(YWE_Engine *game, int exit_code)
{
	YWE_DestroyEngine(game);
	exit(exit_code);
}

// Error Handler
YWE_Err YWE_ErrorHandler(YWE_Engine *game, YWE_Err code)
{
	if(YWER_ERROR(code))
	{
		YWE_STDERR_OUTPUT(game, FATAL_ERR_MSG "Something broke. Shit is fucked%s%s" YWE_FM_RESET "\n", (code & 0x100)?" in SDL ― ":"", SDL_GetError());
	}
	YWE_ExitGame(game, 1);
	return code;
}

YWE_Err YWE_ResizeEvent(YWE_Engine *game, SDL_WindowEvent *event)
{
	if(game->winprop.preserve_aspect == false)
	{
		return YWER_ALL_CLEAR;
	}
	Sint32 x_mag = ((event->data1)/(game->winprop.aspect_x));
	Sint32 y_mag = ((event->data2)/(game->winprop.aspect_y));
	Sint32 low_mag = ((x_mag > y_mag)?y_mag:x_mag);
	Sint32 min_magnitude = (low_mag > game->winprop.min_scale)?low_mag:game->winprop.min_scale;
	Sint32 new_x = min_magnitude * game->winprop.aspect_x;
	Sint32 new_y = min_magnitude * game->winprop.aspect_y;
	if(SDL_SetWindowSize(game->window, new_x, new_y))
	{
		return YWER_ALL_CLEAR;
	}
	C_RAISE_ERR(YWER_EGNS_GENERAL);

	// Remake the Top Level Textures in VN
	YWE_VN *vn = &(game->vn);
	for(int i=0; i<YWE_VN_TOP_LEVEL_RENDER_UNITS; i++)
	{
		SDL_DestroyTexture(vn->units[i].tex);
		vn->units[i].tex = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, new_x, new_y);
		if(!(vn->units[i].tex))
		{
			C_RAISE_ERR(YWER_EGNS_GENERAL);
		}
	}

	return YWER_ALL_CLEAR;
}

// Undef call and raise macros because of two reasons: 1. game is a YWE_Engine and not a YWE_Engine * in main. 2. Main's return type is not YWE_Err
#undef PASS_BACK_ERR

#define CALL_EXIT_ERR_MAIN(call) {YWE_Err macro_erback = (call); if(YWER_ERROR(macro_erback)){YWE_ExitGame(&game, 1);}}

// Entry Function
int main(int nargs, char **args)
{
	YWE_Engine game = {
		.window = NULL,
		.renderer = NULL,
	};

	CALL_EXIT_ERR_MAIN(YWE_InitEngine(&game));

	if(nargs > 1)
	{
		for(int i=1; i<nargs; i++)
		{
			// Currently, we only care about some option arguments
			if(args[i][0] == '-')
			{
				char *itr = (args[i]+1);
				while(*itr)
				{
					switch(*itr)
					{
						case 's':
							game.progprop.stderr_output_enabled = !(game.progprop.stderr_output_enabled);
							break;
						default:
							break;
					}
					itr++;
				}
			}
		}
	}

	while(true)
	{
		// Event Management
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_EVENT_QUIT:
					YWE_ExitGame(&game, 0); // Quit on Quit
					break;
				case SDL_EVENT_WINDOW_RESIZED:
					CALL_EXIT_ERR_MAIN(YWE_ResizeEvent(&game, (SDL_WindowEvent *)&event));
					break;
				case SDL_EVENT_KEY_UP:
				case SDL_EVENT_KEY_DOWN:
					CALL_EXIT_ERR_MAIN(YWE_InputEvent(&game, &event));
					break;
				default:
					break;
			}
		}
		SDL_RenderClear(game.renderer);
		YWE_MarkFrame(&game);

		// Frame Time Debug print #1
		//fprintf(stderr, "Flag 0: %" PRIu64 ", %" PRIu64 ", %" PRIu64 "\n", game.frame.last_frame_end, game.frame.next_tick_target, SDL_GetTicks());


		// YWE_PreFrameLogic(&game);
		YWE_FrameRender(&game);
		// YWE_PostFrameLogic(&game);

		// Render to inactive canvas
		SDL_RenderPresent(game.renderer);
		YWE_MarkFrameEnd(&game);

		// Frame Time Debug print #2
		// fprintf(stderr, "Flag 1: %" PRIu64 ", %" PRIu64 ", %" PRIu64 "\n", game.frame.last_frame_end, game.frame.next_tick_target, SDL_GetTicks());

		if(game.frame.frame_delay > 0)
		{
			SDL_Delay(game.frame.frame_delay);
		}
	}
	
	YWE_ExitGame(&game, 0);

	// Redundant
	return 0;
}