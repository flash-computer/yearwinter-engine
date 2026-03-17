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

// Initialize SDL
YWE_Err sdl_initialize(YWE_Engine *game)
{
	if(!SDL_Init(YWE_SDL_INIT_FLAGS))
	{
		C_RAISE_ERR(YWER_EGNS_GENERAL);
	}
	if(!SDL_CreateWindowAndRenderer(YWE_CONFIG_MAIN_WINDOW_DEF_TITLE, YWE_CONFIG_MAIN_WINDOW_DEF_WIDTH_RATIO * YWE_CONFIG_MAIN_WINDOW_DEF_DIMENSION_SCALE, YWE_CONFIG_MAIN_WINDOW_DEF_HEIGHT_RATIO * YWE_CONFIG_MAIN_WINDOW_DEF_DIMENSION_SCALE, YWE_MAIN_WINDOW_DEF_FLAGS, &(game->window), &(game->renderer)))
	{
		C_RAISE_ERR(YWER_EGNS_GENERAL);
	}
	if(!TTF_Init())
	{
		C_RAISE_ERR(YWER_EGNS_GENERAL);
	}
	return YWER_ALL_CLEAR;
}

// Sub sources
#include"management/renderunits.c"

// Destroy a VN
YWE_Err YWE_DestroyVN(YWE_Engine *game, YWE_VN *vn)
{
	for(int i=0; i<YWE_VN_TOP_LEVEL_RENDER_UNITS; i++)
	{
		PASS_BACK_ERR(YWE_DestroyRenderUnit(game, vn->units + i));
	}
	return YWER_ALL_CLEAR;
}

// Initialize a VN
YWE_Err YWE_InitVN(YWE_Engine *game, YWE_VN *vn)
{
	int scr_width = 0;
	int scr_height = 0;
	if(!SDL_GetRenderOutputSize(game->renderer, &scr_width, &scr_height))
	{
		C_RAISE_ERR(YWER_EGNS_GENERAL);
	}
	for(int i=0; i<YWE_VN_TOP_LEVEL_RENDER_UNITS; i++)
	{
		YWE_InitRenderUnit(game, vn->units + i);
		vn->units[i].tex = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, scr_width, scr_height);
		if(!(vn->units[i].tex))
		{
			C_RAISE_ERR(YWER_EGNS_GENERAL);
		}
	}
	return YWER_ALL_CLEAR;
}

// Destroy a Game Engine
YWE_Err YWE_DestroyEngine(YWE_Engine *game)
{
	PASS_BACK_ERR(YWE_DestroyVN(game, &(game->vn)));
	TTF_Quit();
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
	return YWER_ALL_CLEAR;
}

// Initialize a Game Engine
YWE_Err YWE_InitEngine(YWE_Engine *game)
{
	PASS_BACK_ERR(sdl_initialize(game));

	// YWE_VN initialization
	PASS_BACK_ERR(YWE_InitVN(game, &(game->vn)));

	// YWE_WindowProperties initialization
	if(!SDL_SetWindowResizable(game->window, YWE_CONFIG_MAIN_WINDOW_DEF_RESIZABLE))
	{
		C_RAISE_ERR(YWER_EGNS_GENERAL);
	}
	game->winprop.aspect_x = YWE_CONFIG_MAIN_WINDOW_DEF_WIDTH_RATIO;
	game->winprop.aspect_y = YWE_CONFIG_MAIN_WINDOW_DEF_HEIGHT_RATIO;
	game->winprop.min_scale = YWE_CONFIG_MAIN_WINDOW_DEF_MIN_SCALE;
	game->winprop.preserve_aspect = YWE_CONFIG_MAIN_WINDOW_DEF_PRESERVE_ASPECT;

	// YWE_FrameManagement initialization
	game->frame.last_frame_end = SDL_GetTicks();
	game->frame.frame_target = YWE_CONFIG_RENDERING_DEF_FRAME_TARGET;
	game->frame.frame_target_timescale = YWE_CONFIG_RENDERING_DEF_FRAME_TARGET_TIMESCALE;
	game->frame.frame_delay = 16;
	YWE_MarkFrame(game);

	// YWE_ProgramProperties initialization
	game->progprop.stderr_output_enabled = YWE_CONFIG_STDERR_OUTPUT_DEF_UNSUPPRESSED;

	return YWER_ALL_CLEAR;
}

/*----------------------------------------------------------Wrapper Structs----------------------------------------------------------*/
/*----------------------------------------------------------Implementations----------------------------------------------------------*/
/*-----------------------------------------------------------------|-----------------------------------------------------------------*/

YWE_ErrPtr YWE_AllocPushFreelist(YWE_Engine *game, YWE_DNode *node, size_t size)
{
	if(!node)
	{
		C_RAISE_ERRPTR(NULL, YWER_EFNR_ARGS);
	}
	void *ret_ptr = malloc(size);
	if(!ret_ptr)
	{
		C_RAISE_ERRPTR(NULL, YWER_EMMR_ALLOC);
	}

	YWE_ErrPtr ret_node = YWE_AppendDnodeList(game, node->to_free, 0);
	if(YWER_ERROR(ret_node.ret))
	{
		free(ret_node.value);
		free(ret_ptr);
		return(YWE_ErrPtr){NULL, ret_node.ret};
	}
	YWE_DNode *new_node = ret_node.value;
	new_node->data = ret_ptr;
	node->to_free = new_node;

	return (YWE_ErrPtr){ret_ptr, YWER_ALL_CLEAR};
}

YWE_Err YWE_FreeFreelist(YWE_Engine *game, YWE_DNode *node)
{
	YWE_DNode *temp;
	while(node)
	{
		temp = node;
		node = node->prev;
		free(temp->data);
		PASS_BACK_ERR(YWE_RemoveDnodeList(game, temp));
	}
	return YWER_ALL_CLEAR;
}

YWE_ErrPtr YWE_AppendDnodeList(YWE_Engine *game, YWE_DNode *node, size_t data_sz)
{
	YWE_DNode *new_node = malloc(sizeof(YWE_DNode) + data_sz);
	if(!new_node)
	{
		C_RAISE_ERRPTR(NULL, YWER_EMMR_ALLOC);
	}

	new_node->prev = node;
	if(data_sz)
	{
		new_node->data = (void *)((char *)new_node + sizeof(YWE_DNode));
	}
	else
	{
		new_node->data = NULL;
	}
	new_node->to_free = NULL;

	if(node)
	{
		if(node->next)
		{
			node->next->prev = new_node;
		}
		new_node->next = node->next;
		node->next = new_node;
		return (YWE_ErrPtr){new_node, YWER_ALL_CLEAR};
	}
	new_node->next = NULL;
	return (YWE_ErrPtr){new_node, YWER_ALL_CLEAR};
}

YWE_Err YWE_RemoveDnodeList(YWE_Engine *game, YWE_DNode *node)
{
	if(!node)
	{
		C_RAISE_ERR(YWER_EFNR_ARGS);
	}
	if(node->to_free)
	{
		YWE_FreeFreelist(game, node->to_free);
	}
	if(node->prev)
	{
		node->prev->next = node->next;
	}
	if(node->next)
	{
		node->next->prev = node->prev;
	}
	free(node);
	return YWER_ALL_CLEAR;
}