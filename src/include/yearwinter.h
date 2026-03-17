/*--------------------------------------------------------------Version--------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------|-----------------------------------------------------------------*/

#define YWE_ENVER_MAJ 0
#define YWE_ENVER_MIN 1

/*-------------------------------------------------------VT100 Text Formatting-------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------|-----------------------------------------------------------------*/

#define YWE_FM_COL_RD "\x1b[31m"
#define YWE_FM_COL_GR "\x1b[32m"
#define YWE_FM_COL_MG "\x1b[35m"

#define YWE_FM_BG_RD "\x1b[41m"
#define YWE_FM_BG_GR "\x1b[42m"
#define YWE_FM_BG_MG "\x1b[45m"

#define YWE_FM_BOLD "\x1b[1m"
#define YWE_FM_THIN "\x1b[2m"
#define YWE_FM_ITAL "\x1b[3m"
#define YWE_FM_UNLN "\x1b[4m"
#define YWE_FM_BLN1 "\x1b[5m"
#define YWE_FM_BLN2 "\x1b[6m"
#define YWE_FM_INVR "\x1b[7m"
#define YWE_FM_DISA "\x1b[8m"
#define YWE_FM_STRK "\x1b[9m"

#define YWE_FM_RESET "\x1b[0m"

/*----------------------------------------------------------Header Includes----------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------|-----------------------------------------------------------------*/

// These are not prepended by YWE_ since they are regarding inclusions of external libraries.
// Library inclusions
#ifndef STDINT_INCLUDED
	#include<stdint.h>
	#define STDINT_INCLUDED
#endif
#ifndef STDDEF_INCLUDED
	#include<stddef.h>
	#define STDDEF_INCLUDED
#endif
#ifndef STDIO_INCLUDED
	#include<stdio.h>
	#define STDIO_INCLUDED
#endif
#ifndef STDLIB_INCLUDED
	#include<stdlib.h>
	#define STDLIB_INCLUDED
#endif
#ifndef STDBOOL_INCLUDED
	#include<stdbool.h>
	#define STDBOOL_INCLUDED
#endif
#ifndef INTTYPES_INCLUDED
	#include <inttypes.h>
	#define INTTYPES_INCLUDED
#endif

// SDL Headers
#ifndef SDLHEADERS_INCLUDED
	#include<SDL3/sdl.h>
	#include<SDL3_image/SDL_image.h>
	#include<SDL3_ttf/SDL_ttf.h>
	#include<SDL3_mixer/SDL_mixer.h>
	#define SDLHEADERS_INCLUDED
#endif

// Local Headers
#ifndef YWE_LOCALHEADERS_INCLUDED
	#include"types.h"
	#include"error.h"
	#include"config.h"
	#define YWE_LOCALHEADERS_INCLUDED
#endif

/*--------------------------------------------------------Engine Declarations--------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------|-----------------------------------------------------------------*/

#ifndef YWE_SDL_INIT_FLAGS
	#define YWE_SDL_INIT_FLAGS (SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS | SDL_INIT_SENSOR | SDL_INIT_CAMERA)
#endif
#ifndef YWE_MAIN_WINDOW_DEF_FLAGS
	#define YWE_MAIN_WINDOW_DEF_FLAGS (SDL_WINDOW_RESIZABLE)
#endif
#ifndef YWE_VN_TOP_LEVEL_RENDER_UNITS
	#define YWE_VN_TOP_LEVEL_RENDER_UNITS 8
#endif

/*---------------------------------------------------------Engine Prototypes---------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------|-----------------------------------------------------------------*/

#ifndef YWE_RENDER_DRIVER_NAME_CONFIGURED
	#ifdef YWE_RENDER_DRIVER_NAME_CHOICE
		char *YWE_GetRenderDriverName(YWE_Engine *game);
	#else
		#define YWE_GetRenderDriverName(game) ((char *)NULL)
	#endif
	#define YWE_RENDER_DRIVER_NAME_CONFIGURED
#endif

#ifndef YWE_STDERR_OUTPUT
		#define YWE_STDERR_OUTPUT(game, ...) {if(game->progprop.stderr_output_enabled){fprintf(stderr, __VA_ARGS__);}}
#endif

#ifndef YWE_CORE_PROTOTYPES
	void YWE_ExitGame(YWE_Engine *game, int exitcode);

	// Rendering Functions
	YWE_Err YWE_FrameRender(YWE_Engine *game);
	YWE_Err YWE_DrawBackgroundTexture(YWE_Engine *game);
	YWE_Err YWE_DrawUITexture(YWE_Engine *game);

	YWE_Err YWE_RenderRenderUnit(YWE_Engine *game, YWE_RenderUnit *ru);

	// Frame Timing
	void YWE_MarkFrame(YWE_Engine *game);
	void YWE_MarkFrameEnd(YWE_Engine *game);

	// Event callbacks
	YWE_Err YWE_InputEvent(YWE_Engine *game, SDL_Event *event);
	YWE_Err YWE_ResizeEvent(YWE_Engine *game, SDL_WindowEvent *event);

	// Error callback
	YWE_Err YWE_ErrorHandler(YWE_Engine *game, YWE_Err code);

	// Prototypes to Initialize and Destroy List Structures
	YWE_ErrPtr YWE_AppendDnodeList(YWE_Engine *game, YWE_DNode *node, size_t data_sz);
	YWE_Err YWE_RemoveDnodeList(YWE_Engine *game, YWE_DNode *node);

	YWE_ErrPtr YWE_AllocPushFreelist(YWE_Engine *game, YWE_DNode *node, size_t size);
	YWE_Err YWE_FreeFreelist(YWE_Engine *game, YWE_DNode *node);

	// Init and Destroy Prototypes

	// Render Unit
	YWE_ErrPtr YWE_InitRenderUnit(YWE_Engine *game, YWE_RenderUnit *ru, bool to_free);
	YWE_Err YWE_DestroyRenderUnit(YWE_Engine *game, YWE_RenderUnit *ru);

	YWE_ErrPtr YWE_CreateAndAppendRenderUnit(YWE_Engine *game, YWE_RenderUnit *ru);
	YWE_Err YWE_DestroyAndRemoveRenderUnit(YWE_Engine *game, YWE_RenderUnit *ru, YWE_DNode *node);

	// VN
	YWE_Err YWE_InitVN(YWE_Engine *game, YWE_VN *vn);
	YWE_Err YWE_DestroyVN(YWE_Engine *game, YWE_VN *vn);

	// Engine
	YWE_Err YWE_InitEngine(YWE_Engine *game);
	YWE_Err YWE_DestroyEngine(YWE_Engine *game);
#endif