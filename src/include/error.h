/*------------------------------------------------------------Error Codes------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------|-----------------------------------------------------------------*/

/* If the high byte is not 0x00 or 0x01, then it's not an error.
 * If the low byte is 0x00, then it's not an error
 * If the high byte is 0x00, the highest bit of the lower byte is reset(0) and the lower byte is not 0x00, it's an engine error separate from SDL3
 * If the high byte is 0x00, the highest bit of the lower byte is set(1), it's an game specific error separate from the engine
 * If the high byte is 0x01, and the lower byte is not 0x00, it's an engine error related to SDL3
 * Use YWER_ERROR to check if the return value is an error
 * use YWER_TESTCATEGORY to check if the return value is an error belonging to a particular category
 * Category codes are in the format YWER_E{XX}R_NIB_HI for non-SDL related errors(the R stands for Regular) and YWER_E{XX}S_NIB_HI for SDL related errors (The S stands for SDL).
*/

#define YWER_ERROR(code) (!((code) & 0xFE00) && ((code) & 0xFF))
#define YWER_TESTCATEGORY(code, category) (((((code) & 0xF0) ^ ((category) & 0xF0))? 0 : 1) && YWER_ERROR(code) && ((code) & 0x100 == (category) & 0x100))

/* Code 0 for every category is reserved for Miscellaneous errors that don't fit neatly under any preexisting category, with the exception of 0x00 */

// Non-SDL Errors
#define YWER_EFNR_NIB_HI	0x60  // Program execution errors
#define YWER_EFNR_ARGS	(0x1 | YWER_EFNR_NIB_HI)	// Malformed Arguments
#define YWER_EFNR_DECOMP	(0x2 | YWER_EFNR_NIB_HI)	// Decompression error
#define YWER_EFNR_COMP	(0x3 | YWER_EFNR_NIB_HI)	// Compression error

#define YWER_EFLR_NIB_HI	0x40	// File Handling Errors
#define YWER_EFLR_OPEN	(0x1 | YWER_EFLR_NIB_HI)
#define YWER_EFLR_CLOSE	(0x2 | YWER_EFLR_NIB_HI)
#define YWER_EFLR_READ	(0x3 | YWER_EFLR_NIB_HI)
#define YWER_EFLR_WRITE	(0x4 | YWER_EFLR_NIB_HI)
#define YWER_EFLR_SEEK	(0x5 | YWER_EFLR_NIB_HI)

#define YWER_EMMR_NIB_HI	0x20	// Memory Errors
#define YWER_EMMR_ALLOC	(0x1 | YWER_EMMR_NIB_HI)	// Error during memory allocation

#define YWER_ALL_CLEAR	0x0		// All clear

// SDL-Related Errors
#define YWER_EGNS_NIB_HI	0x110	// General SDL3 Error (Placeholder until I figure out a better framework for reporting SDL3 Errors)
#define YWER_EGNS_GENERAL	(0x1 | YWER_EGNS_NIB_HI)	// Something fucked up in regards to SDL3

/*-----------------------------------------------------------Error Handler-----------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------|-----------------------------------------------------------------*/
YWE_err YWE_ErrorHandler(YWE_Engine *game, YWE_err code);

// Convinient Macros to Raise Errors
#define YWER_RAISE_ERROR_ERR(game, variable, error) {(variable)=YWE_ErrorHandler((game), (error));if(YWER_ERROR(variable))return(variable);}
#define YWER_RAISE_ERROR_ERRPTR(game, variable, pointer, error) {(variable)=YWE_ErrorHandler((game), (error));if(YWER_ERROR(variable))return((YWE_ErrPtr){(pointer), (variable)});}
#define YWER_RAISE_ERROR_ERRINT(game, variable, integer, error) {(variable)=YWE_ErrorHandler((game), (error));if(YWER_ERROR(variable))return((YWE_ErrInt){(integer), (variable)});}