/*-----------------------------------------------------------Frame Timings-----------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------|-----------------------------------------------------------------*/

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