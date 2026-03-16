/*---------------------------------------------------------Struct Prototypes---------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------|-----------------------------------------------------------------*/

struct YWE_GameEngine;
typedef struct YWE_GameEngine YWE_Engine;

/*------------------------------------------------------------Error Types------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------|-----------------------------------------------------------------*/

// Should have Valid value from 0 to 65,535
#ifndef YWE_ERR_TYPE
		#define ERR_TYPE uint16_t
		typedef ERR_TYPE YWE_err;
#endif

struct YWE_ReturnErrorWithPointer
{
	void *value;
	YWE_err ret;
}; typedef struct YWE_ReturnErrorWithPointer YWE_ErrPtr;

struct YWE_ReturnErrorWithInteger
{
	int value;
	YWE_err ret;
}; typedef struct YWE_ReturnErrorWithInteger YWE_ErrInt;

/*----------------------------------------------------------Wrapper Structs----------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------|-----------------------------------------------------------------*/

typedef struct YWE_DoublyLinkedListNode
{
	struct YWE_DoublyLinkedListNode *next;
	struct YWE_DoublyLinkedListNode *prev;

	struct YWE_DoublyLinkedListNode *to_free;

	void *data;
} YWE_DNode;

// Prototypes to Initialize and Destroy these structs
YWE_ErrPtr YWE_AppendDnodeList(YWE_Engine *game, YWE_DNode *node, size_t data_sz);
YWE_err YWE_RemoveDnodeList(YWE_Engine *game, YWE_DNode *node);

YWE_ErrPtr YWE_AllocPushFreelist(YWE_Engine *game, YWE_DNode *node, size_t size);
YWE_err YWE_FreeFreelist(YWE_Engine *game, YWE_DNode *node);


/*------------------------------------------------------------Engine Core------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------|-----------------------------------------------------------------*/

typedef struct YWE_RenderUnit
{
	struct YWE_RenderUnit *parent;
	YWE_DNode *children;
	SDL_Texture *tex;
	SDL_Rect rect;
	bool to_free;
} YWE_RenderUnit;

typedef struct YWE_VNScreen
{
	YWE_RenderUnit background;
	YWE_RenderUnit ui;
	SDL_Surface *dialog_text_surface;
} YWE_VN;

typedef struct YWE_WindowProperties
{
	unsigned int aspect_x;
	unsigned int aspect_y;
	unsigned int min_scale;
	bool preserve_aspect;
} YWE_WindowProperties;

typedef struct YWE_FrameManagement
{
	uint64_t last_frame_end;
	uint64_t next_tick_target;
	uint64_t frame_target;
	uint64_t frame_target_timescale;
	uint64_t frame_delay;
} YWE_FrameManagement;

struct YWE_GameEngine
{
	// SDL objects
	SDL_Window *window;
	SDL_Renderer *renderer;

	// Rendering State
	YWE_VN vn;

	// Properties
	YWE_WindowProperties winprop;
	YWE_FrameManagement frame;

	// Miscallaneous
	void *data; // Free to use for any purpose by the game programmer. The engine doesn't mess with this.
};

// Init and Destroy Prototypes

// Render Unit
YWE_ErrPtr YWE_InitRenderUnit(YWE_Engine *game, YWE_RenderUnit *ru, bool to_free);
YWE_err YWE_DestroyRenderUnit(YWE_Engine *game, YWE_RenderUnit *ru);

// VN
YWE_err YWE_InitVN(YWE_Engine *game, YWE_VN *vn);
YWE_err YWE_DestroyVN(YWE_Engine *game, YWE_VN *vn);

// Engine
YWE_err YWE_InitEngine(YWE_Engine *game);
YWE_err YWE_DestroyEngine(YWE_Engine *game);