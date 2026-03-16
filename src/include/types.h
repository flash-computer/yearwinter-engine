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
		typedef ERR_TYPE YWE_Err;
#endif

struct YWE_ReturnErrorWithPointer
{
	void *value;
	YWE_Err ret;
}; typedef struct YWE_ReturnErrorWithPointer YWE_ErrPtr;

struct YWE_ReturnErrorWithInteger
{
	int value;
	YWE_Err ret;
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


/*------------------------------------------------------------Engine Core------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------|-----------------------------------------------------------------*/

typedef struct YWE_RenderUnit
{
	struct YWE_RenderUnit *parent;
	YWE_DNode *children;
	SDL_Texture *tex;
	bool no_src;
	SDL_FRect src;
	bool no_dst;
	SDL_FRect dst;
	bool to_free;
} YWE_RenderUnit;
// Render Unit Rect Pointer easy access
#define YWE_RUSP(ru) (((ru).no_src)?NULL:(&((ru).src)))
#define YWE_PRUSP(ru) (((ru)->no_src)?NULL:(&((ru)->src)))

#define YWE_RUDP(ru) (((ru).no_dst)?NULL:(&((ru).dst)))
#define YWE_PRUDP(ru) (((ru)->no_dst)?NULL:(&((ru)->dst)))

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
	Sint32 min_scale;
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

typedef struct YWE_ProgramProperties
{
	bool stderr_output_enabled;
} YWE_ProgramProperties;

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
	YWE_ProgramProperties progprop;

	// Miscallaneous
	void *data; // Free to use for any purpose by the game programmer. The engine doesn't mess with this.
};