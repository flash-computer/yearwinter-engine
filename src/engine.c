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

// Easy Access to Rects (No Back Propagation)
#define YWE_RUSP(ru) (YWE_GetRenderUnitRectSrc(game, &ru).value)
#define YWE_PRUSP(ru) (YWE_GetRenderUnitRectSrc(game, ru).value)

#define YWE_RUDP(ru) (YWE_GetRenderUnitRectDst(game, &ru).value)
#define YWE_PRUDP(ru) (YWE_GetRenderUnitRectDst(game, ru).value)

// Sub Sources

// Event Handling
#include"engine/event_handling.c"

// Rendering Core
#include"engine/rendering_core.c"

// Frame Timings
#include"engine/frame_timings.c"