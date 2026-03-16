// Default Title of the main window
#define YWE_CONFIG_MAIN_WINDOW_DEF_TITLE "Yearwinter"

// Creates a prototype to allow choosing render driver name programatically. If in doubt, leave commented.
// #define YWE_RENDER_DRIVER_NAME_CHOICE

// Default Window Size
#define YWE_CONFIG_MAIN_WINDOW_DEF_WIDTH_RATIO 16
#define YWE_CONFIG_MAIN_WINDOW_DEF_HEIGHT_RATIO 9
#define YWE_CONFIG_MAIN_WINDOW_DEF_DIMENSION_SCALE 100
#define YWE_CONFIG_MAIN_WINDOW_DEF_MIN_SCALE 40
#define YWE_CONFIG_MAIN_WINDOW_DEF_RESIZABLE true
#define YWE_CONFIG_MAIN_WINDOW_DEF_PRESERVE_ASPECT true

// Frame Target (Attempt to stay as close to the target as possible, not strictly under or above)
#define YWE_CONFIG_RENDERING_DEF_FRAME_TARGET 60
// Timescale to achieve target in ticks (ms)
#define YWE_CONFIG_RENDERING_DEF_FRAME_TARGET_TIMESCALE 1000

// Fallback frame_quantum for YWE_MarkFrame if frame_target exceeds timescale
#define YWE_CONFIG_RENDERING_FALLBACK_FRAME_QUANTUM 1

// Debug Configuration
#define YWE_CONFIG_STDERR_OUTPUT_DEF_UNSUPPRESSED false
