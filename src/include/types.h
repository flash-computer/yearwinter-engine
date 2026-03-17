/*---------------------------------------------------------Struct Prototypes---------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------|-----------------------------------------------------------------*/

struct YWE_GameEngine;
typedef struct YWE_GameEngine YWE_Engine;

/*---------------------------------------------------------------Enums---------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------|-----------------------------------------------------------------*/

#ifndef YWE_ENUMS
	typedef enum YWE_VolatileMode
	{
		YWE_VOLATILE_PRESERVE, YWE_VOLATILE_TOGGLE, YWE_VOLATILE_TRUE, YWE_VOLATILE_FALSE
	} YWE_VolatileMode;
	#define YWE_ENUMS
#endif

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

typedef struct YWE_RenderUnitProperties
{
	bool no_src;
	bool src_is_rel;
	bool no_dst;
	bool dst_is_rel;
	bool volatile_target;
	bool to_free;

	YWE_VolatileMode volatile_target_mode;
} YWE_RenderUnitProperties;

typedef struct YWE_RenderUnit
{
	char name[32];

	struct YWE_RenderUnit *parent;
	struct YWE_RenderUnit *children;
	size_t children_count;

	SDL_Texture *tex;
	SDL_Surface *surface;

	SDL_FRect src;
	SDL_FRect dst;

	SDL_FRect abs_src;
	SDL_FRect abs_dst;

	YWE_RenderUnitProperties prop;
} YWE_RenderUnit;

typedef struct YWE_VNScreen
{
	YWE_RenderUnit units[8];
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