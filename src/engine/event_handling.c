// Keyboard/Mouse/Joystick Input Events
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