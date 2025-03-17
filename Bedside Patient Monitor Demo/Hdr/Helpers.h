#ifndef HELPERS_H_
#define HELPERS_H_

#include "EVE_Platform.h"

#define ALIGN_TWO_POWER_N(Value, alignval)  (((Value) + (alignval - 1)) & (~(alignval - 1)))

#define INIT_APP_BOX(x_val, y_val, w_val, h_val) \
	(app_box){                                            \
		(x_val),                                 \
		(y_val),                                 \
		(w_val),                                 \
		(h_val),                                 \
		(x_val) + (w_val),                       \
		(y_val) + (h_val),                       \
		(x_val) + (w_val) / 2,                   \
		(y_val) + (h_val) / 2}

#define DRAW_RECT(x, y, w, h)                      \
	{                                              \
		EVE_Cmd_wr32(s_pHalContext, BEGIN(RECTS)); \
		EVE_DRAW_AT(x, y);                         \
		EVE_DRAW_AT(x + w, y + h);                 \
	}

#define DRAW_RECT_BORDER(x, y, w, h, color, border, bcolor)                                                       \
	{                                                                                                             \
		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB((bcolor) & 0xFF, ((bcolor) >> 8) & 0xFF, ((bcolor) >> 16) & 0xFF)); \
		EVE_Cmd_wr32(s_pHalContext, BEGIN(RECTS));                                                                \
		EVE_DRAW_AT(x, y);                                                                                        \
		EVE_DRAW_AT(x + w, y + h);                                                                                \
		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(((color) >> 16) & 0xFF, ((color) >> 8) & 0xFF, ((color) & 0xFF)));  \
		EVE_Cmd_wr32(s_pHalContext, BEGIN(RECTS));                                                                \
		EVE_DRAW_AT(x + border, y + border);                                                                      \
		EVE_DRAW_AT(x + w - border * 2, y + h - border * 2);                                                      \
	}
#define DRAW_BOX(box)                              \
	{                                              \
		EVE_Cmd_wr32(s_pHalContext, BEGIN(RECTS)); \
		EVE_DRAW_AT(box.x, box.y);                 \
		EVE_DRAW_AT(box.x_end, box.y_end);         \
	}

#define DRAW_BOX_BORDER(box, color, border, bcolor)                                                               \
	{                                                                                                             \
		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB((bcolor) & 0xFF, ((bcolor) >> 8) & 0xFF, ((bcolor) >> 16) & 0xFF)); \
		EVE_Cmd_wr32(s_pHalContext, BEGIN(RECTS));                                                                \
		EVE_DRAW_AT(box.x, box.y);                                                                                \
		EVE_DRAW_AT(box.x_end, box.y_end);                                                                        \
		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB((color) & 0xFF, ((color) >> 8) & 0xFF, ((color) >> 16) & 0xFF));    \
		EVE_Cmd_wr32(s_pHalContext, BEGIN(RECTS));                                                                \
		EVE_DRAW_AT(box.x + border, box.y + border);                                                              \
		EVE_DRAW_AT(box.x_end - border, box.y_end - border);                                                      \
	}

#define DRAW_CIRCLE(x, y, r)                                 \
	{                                                        \
		EVE_Cmd_wr32(s_pHalContext, POINT_SIZE(r * 16)); \
		EVE_Cmd_wr32(s_pHalContext, BEGIN(POINTS));          \
		EVE_DRAW_AT(x, y);                                   \
	}

#define DRAW_CIRCLE_WITH_TEXT(x, y, r, txt, font, opt, txtcolor)                   \
	{                                                                              \
		DRAW_CIRCLE(x, y, r);                                                      \
		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB((txtcolor) & 0xFF, ((txtcolor) >> 8) & 0xFF, ((txtcolor) >> 16) & 0xFF));    \
		EVE_CoCmd_text(s_pHalContext, x, y, font, opt | OPT_CENTER, txt); \
	}

#define DRAW_CIRCLE_WITH_TEXT_TAG(x, y, r, txt, font, opt, txtcolor, tagval)       \
	{                                                                              \
        EVE_Cmd_wr32(s_pHalContext, TAG(tagval));                                  \
		DRAW_CIRCLE(x, y, r);                                                      \
		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB((txtcolor) & 0xFF, ((txtcolor) >> 8) & 0xFF, ((txtcolor) >> 16) & 0xFF));    \
        EVE_Cmd_wr32(s_pHalContext, TAG(tagval));                                  \
		EVE_CoCmd_text(s_pHalContext, x, y, font, opt | OPT_CENTER, txt); \
	}

#define DRAW_LINE(x, y, w, h)                      \
	{                                              \
		EVE_Cmd_wr32(s_pHalContext, BEGIN(LINES)); \
		EVE_DRAW_AT(x, y);                         \
		EVE_DRAW_AT(x + w, y + h);                 \
	}

#define DRAW_LINE_XY(x, y, x2, y2)                 \
	{                                              \
		EVE_Cmd_wr32(s_pHalContext, BEGIN(LINES)); \
		EVE_DRAW_AT(x, y);                         \
		EVE_DRAW_AT(x2, y2);                       \
	}

#define DRAW_LINE_XY_DEBUG(x, y, x2, y2)                 \
	{                                              \
		printf("DRAW_LINE_XY(%d, %d, %d, %d)\r\n", x, y, x2, y2); \
		EVE_Cmd_wr32(s_pHalContext, BEGIN(LINES)); \
		EVE_DRAW_AT(x, y);                         \
		EVE_DRAW_AT(x2, y2);                       \
	}

#define EVE_VERTEX_FORMAT 2
#define EVE_PRECISION_FACTOR 4 // EVE_VERTEX_FORMAT ^ 2
#define EVE_DRAW_AT(x, y) EVE_Cmd_wr32(s_pHalContext, VERTEX2F((x) * EVE_PRECISION_FACTOR, (y) * EVE_PRECISION_FACTOR))

#define ALIGN_TO_N_2POWER(x, n)    (((x) + ((n) - 1)) & ~((n) - 1)) // Only works correctly if n is a power of 2.
#define ALIGN_UP_TO_N(x, n) (((x) + ((n) - 1)) / (n) * (n))  // Works for any positive integer n (including non-powers of 2).
#define ALIGN_TO_8(x) ALIGN_UP_TO_N((x), 8)

typedef struct
{
	int x, y, w, h, x_end, y_end, x_mid, y_mid;
} app_box;

// Function to calculate and return FPS without float/double
int getFPS();

// Function to format the date 
char* dd_mm_yyyy();
char* dd_mmm_yyyy();
char* dd_month_yyyy();

// Function to format the time as "hh:mm"
char* hh_mm();
char* hh_mm_ss();
char* hh_mm_ss_ms();

// Function to calculate and return the formatted date-time string
char* dd_mm_yyyy_hh_m_ss_ms();

// Function to initialize date-time
void init_datetime(int dd, int mm, int yyyy, int hh, int m, int ss, int ms);


uint32_t get_dd();
uint32_t get_mm();
uint32_t get_yyyy();
uint32_t get_hh();
uint32_t get_mt();
uint32_t get_ss();
uint32_t get_ms();
void dateime_adjustment(EVE_HalContext* phost);

// Function to generate a random number within a given range
int app_random(int range);

// Function to save buffer data to a file
int save_buffer_to_file(const char* filename, const void* buffer, size_t buffer_size);

// Function to take a DDR screenshot
void take_ddr_screenshot(EVE_HalContext* phost, char* name, int ramg_render);

void bt82x_wr_flush(EVE_HalContext* phost, uint32_t addr, uint8_t data32);
void bt82x_wr_mem(EVE_HalContext* phost, uint32_t addr, uint8_t* data, uint32_t data_size);
void bt82x_wr32(EVE_HalContext* phost, uint32_t addr, uint8_t data32);
void bt82x_wr16(EVE_HalContext* phost, uint32_t addr, uint16_t data16);
void bt82x_wr8(EVE_HalContext* phost, uint32_t addr, uint8_t data8);
uint8_t Gesture_GetTag(EVE_HalContext* phost);

#ifdef BT82X_ENABLE
#define EVE2_wr8  bt82x_wr8
#define EVE2_wr16 bt82x_wr16
#define EVE2_wr32 bt82x_wr32
#define EVE2_wrmem bt82x_wr_mem
#define EVE2_flush bt82x_wr_flush
#else
#define EVE2_wr8  EVE_Hal_wr8
#define EVE2_wr16 EVE_Hal_wr16
#define EVE2_wr32 EVE_Hal_wr32
#define EVE2_wrmem EVE_Hal_wrmem
#define EVE2_flush EVE_Hal_flush
#endif // BT82X_ENABLE


// coloring
static uint8_t color_table[] = {
	// blue , green, red   , alpha
	0    , 0    , 0     , 255  , /* Black			 */
	255  , 255  , 255   , 255  , /* White			 */
	0    , 0    , 255   , 255  , /* Red				 */
	0    , 255  , 0     , 255  , /* Green			 */
	255  , 0    , 0     , 255  , /* Blue			 */
	0    , 255  , 255   , 255  , /* Yellow			 */
	255  , 255  , 0     , 255  , /* Cyan			 */
	255  , 0    , 255   , 255  , /* Magenta			 */
	192  , 192  , 192   , 255  , /* Light Gray		 */
	128  , 128  , 128   , 255  , /* Dark Gray		 */
	0    , 0    , 128   , 255  , /* Dark Red		 */
	0    , 128  , 0     , 255  , /* Dark Green		 */
	128  , 0    , 0     , 255  , /* Dark Blue		 */
	0    , 128  , 128   , 255  , /* Dark Yellow		 */
	128  , 128  , 0     , 255  , /* Dark Cyan		 */
	128  , 0    , 128   , 255  , /* Dark Magenta	 */
	128  , 128  , 255   , 255  , /* Light Red		 */
	128  , 255  , 128   , 255  , /* Light Green		 */
	255  , 128  , 128   , 255  , /* Light Blue		 */
	128  , 255  , 255   , 255  , /* Light Yellow	 */
	255  , 255  , 128   , 255  , /* Light Cyan		 */
	255  , 128  , 255   , 255  , /* Light Magenta	 */
	64   , 64   , 64    , 255  , /* Very Dark Gray	 */
	64   , 64   , 192   , 255  , /* Warm Red		 */
	64   , 192  , 64    , 255  , /* Warm Green		 */
	192  , 64   , 64    , 255  , /* Warm Blue		 */
	64   , 192  , 192   , 255  , /* Olive			 */
	192  , 192  , 64    , 255  , /* Teal			 */
	192  , 64   , 192   , 255  , /* Purple			 */
	64   , 64   , 0     , 255  , /* Deep Teal		 */
	64   , 0    , 64    , 255    /* Deep Purple      */
};

enum ColorIndex
{
	ARGB_BLACK = 0,
	ARGB_WHITE,
	ARGB_RED,
	ARGB_GREEN,
	ARGB_BLUE,
	ARGB_YELLOW,
	ARGB_CYAN,
	ARGB_MAGENTA,
	ARGB_LIGHT_GRAY,
	ARGB_DARK_GRAY,
	ARGB_DARK_RED,
	ARGB_DARK_GREEN,
	ARGB_DARK_BLUE,
	ARGB_DARK_YELLOW,
	ARGB_DARK_CYAN,
	ARGB_DARK_MAGENTA,
	ARGB_LIGHT_RED,
	ARGB_LIGHT_GREEN,
	ARGB_LIGHT_BLUE,
	ARGB_LIGHT_YELLOW,
	ARGB_LIGHT_CYAN,
	ARGB_LIGHT_MAGENTA,
	ARGB_VERY_DARK_GRAY,
	ARGB_WARM_RED,
	ARGB_WARM_GREEN,
	ARGB_WARM_BLUE,
	ARGB_OLIVE,
	ARGB_TEAL,
	ARGB_PURPLE,
	ARGB_DEEP_TEAL,
	ARGB_DEEP_PURPLE
};

#endif /* HELPERS_H_ */
