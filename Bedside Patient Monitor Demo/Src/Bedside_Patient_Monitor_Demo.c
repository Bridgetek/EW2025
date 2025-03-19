/**
 * @file Bedside_Patient_Monitor_Demo.c
 * @brief Source file for the Bedside Patient Monitoring Demo application
 *
 * This file contains the implementation of the Bedside Patient Monitoring Demo application.
 * The application demonstrates the use of EVE graphics and touch capabilities in a
 * healthcare environment, providing a user interface for monitoring patient data at the bedside.
 *
 * @author Bridgetek
 *
 * @date 2025
 * @license MIT License
 *
 * Copyright (c) [2019] [Bridgetek Pte Ltd (BRTChip)]
 */

#include "Common.h"
#include "EVE_Platform.h"
#include "EVE_CoCmd.h"
#include "Bedside_Patient_Monitor_Demo.h"
#include "Helpers.h"
#include "Gesture.h"

// Definitions -------------------------------------------
#define F_ADDR 0
#define F_SIZE 1
#define ENABLE_FONT_CACHE 1
#define ENABLE_SHOW_FPS 0

#define MONTH_MODE_DIGIT 0
#define MONTH_MODE_STR3 1
#define MONTH_MODE_STRS 2

#define TIME_MODE_HH_MM 0
#define TIME_MODE_HH_MM_SS 1
#define TIME_MODE_HH_MM_SS_MS 2

#define COLOR_CODE_WINDOW_BAR COLOR_RGB(0, 120, 215)

// Structs -----------------------------------------------
typedef struct
{
	uint32_t flash_addr;
	uint32_t flash_size;

	uint32_t xfont_addr;
	uint32_t xfont_size;
	uint32_t handler;
	uint32_t *xfont;
	uint32_t cache_addr;
	uint32_t cache_size;
} app_font_t;

typedef struct
{
	uint32_t ramg_address;
	uint32_t flash_address;
	uint32_t size_on_flash;
} app_image_from_flash_t;

// Function declarations ---------------------------------
uint32_t graph_l1_rotate_init(app_box *box_heartbeat, app_box *box_pleth, app_box *box_co2);
void graph_l1_rotate_draw();

// Variables ---------------------------------------------
EVE_HalContext s_halContext;
EVE_HalContext *s_pHalContext;
uint8_t time_mode = TIME_MODE_HH_MM;
uint8_t month_mode = MONTH_MODE_DIGIT;

/* Font and image data from ew2025_bedside_patient_monitor_demo_bt81x.map
	arial_25_ASTC.glyph                                              : 649344   : 52416
	arial_25_ASTC.xfont                                              : 701760   : 176
	arial_25_ASTC.xfont.padding                                      : 701936   : 16
	ARIALNB_88_ASTC.glyph                                            : 10943360 : 66560
	ARIALNB_88_ASTC.xfont                                            : 11009920 : 176
	ARIALNB_88_ASTC.xfont.padding                                    : 11010096 : 16
	zoom-in-25-white_28x28_COMPRESSED_RGBA_ASTC_4x4_KHR.raw          : 11010112 : 784
	zoom-in-25-white_28x28_COMPRESSED_RGBA_ASTC_4x4_KHR.raw.padding  : 11010896 : 48
	zoom-out-25-white_28x28_COMPRESSED_RGBA_ASTC_4x4_KHR.raw         : 11010944 : 784
	zoom-out-25-white_28x28_COMPRESSED_RGBA_ASTC_4x4_KHR.raw.padding : 11011728 : 48
*/
static const uint32_t arial_25_ASTC_xfont[] = {701760, 176};
static const uint32_t ARIALNB_88_ASTC_xfont[] = {11009920, 176};
app_image_from_flash_t zoom_in = {0, 11010112, 784 + 48};
app_image_from_flash_t zoom_out = {0, 11010944, 784 + 48};

// Font list
app_font_t font0 = {.xfont = &ARIALNB_88_ASTC_xfont};
app_font_t font2 = {.xfont = &arial_25_ASTC_xfont};
app_font_t *fonts[] = {&font0, &font2};

int32_t g_graph_zoom_lv = 3;

app_box box_menu_top;
app_box box_ecg;
app_box box_pth;
app_box box_co2;
app_box box_menu_bottom;
app_box box_graph_ecg;
app_box box_graph_pth;
app_box box_graph_co2;

uint8_t *const btnStartTxtActive = "START";
uint8_t *const btnStartTxtInActive = "STOP";
uint8_t *btnStartTxt = 0;
uint8_t btnStartState = BTN_START_INACTIVE;
uint32_t graph_size_ramg = 0;

void load_app_assets(uint32_t ramg_offset)
{
	// font addr calculation
	const uint8_t font_handler_start = 9;
	uint32_t num_font = sizeof(fonts) / sizeof(app_font_t *);

	// prepare the font
	for (int32_t i = 0; i < num_font; i++)
	{
		app_font_t *f = fonts[i];

		f->flash_addr = f->xfont[F_ADDR];
		f->flash_size = f->xfont[F_SIZE];
		f->handler = i + font_handler_start;
		f->xfont_addr = ALIGN_UP_TO_N(ramg_offset, 32); // SetFont2 32 bit align
		f->xfont_size = f->flash_size;
		ramg_offset = f->xfont_addr + f->xfont_size;

		// prepare font handler
		Display_Start(s_pHalContext);
		EVE_CoCmd_flashRead(s_pHalContext, f->xfont_addr, f->flash_addr, f->flash_size);
		EVE_CoCmd_setFont2(s_pHalContext, f->handler, f->xfont_addr, 0);
		Display_End(s_pHalContext);
	}

	// zoom icon
	zoom_in.ramg_address = ramg_offset;
	zoom_out.ramg_address = zoom_in.ramg_address + zoom_in.size_on_flash;
	EVE_CoCmd_flashRead(s_pHalContext, zoom_in.ramg_address, zoom_in.flash_address, zoom_in.size_on_flash);
	EVE_CoCmd_flashRead(s_pHalContext, zoom_out.ramg_address, zoom_out.flash_address, zoom_out.size_on_flash);

#if ENABLE_FONT_CACHE
	// caching the font, only BT817/8, BT82x incompatible
	ramg_offset += zoom_in.size_on_flash + zoom_out.size_on_flash;
	uint32_t ramg_remain = RAM_G_SIZE - ramg_offset;
	uint32_t cache_size_per_font = ramg_remain / num_font;
	for (int32_t i = 0; i < num_font; i++)
	{
		app_font_t *f = fonts[i];

		uint32_t cache_size = min(cache_size_per_font, RAM_G_SIZE - ramg_offset);
		if (cache_size < 16 * 1024)
		{
			printf("Warning: RAMG free < 16Kb\n");
			break;
		}
		if (ramg_offset > RAM_G_SIZE)
		{
			printf("Warning: RAMG full\n");
			break;
		}

		f->cache_addr = ALIGN_UP_TO_N(ramg_offset, 64); // 64-byte aligned.
		f->cache_size = ALIGN_UP_TO_N(cache_size, 4);	// 4 byte aligned. Must be at least 16 Kbytes.

		Display_Start(s_pHalContext);
		EVE_CoCmd_fontCache(s_pHalContext, f->handler, f->cache_addr, f->cache_size);
		Display_End(s_pHalContext);
		ramg_offset = f->cache_addr + f->cache_size;
	}
#endif
}

void draw_app_window(app_box app_window)
{
	int32_t border = 5;
	int32_t bx = app_window.x;
	int32_t by = app_window.y;
	int32_t bw = app_window.w + border;
	int32_t bh = app_window.h + border;
	char border_color[3] = {134, 134, 134};

	EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(0, 0, 0));
	EVE_Cmd_wr32(s_pHalContext, BEGIN(RECTS));
	EVE_DRAW_AT(0, 0);
	EVE_DRAW_AT(s_pHalContext->Width, s_pHalContext->Height);
}

void process_event()
{
	Gesture_Touch_t *ges = utils_gestureRenew(s_pHalContext);
	if (ges->tagReleased == TAG_ZOOM_DOWN)
	{
		g_graph_zoom_lv--;
		g_graph_zoom_lv = max(g_graph_zoom_lv, 1);
	}

	else if (ges->tagReleased == TAG_ZOOM_UP)
	{
		g_graph_zoom_lv++;
		g_graph_zoom_lv = min(g_graph_zoom_lv, GRAPH_ZOOM_LV_MAX);
	}

	else if (ges->tagReleased == TAG_START_STOP)
	{
		btnStartState = !btnStartState;
		if (BTN_START_ACTIVE == btnStartState)
		{
			btnStartTxt = btnStartTxtActive;
		}
		else
		{
			btnStartTxt = btnStartTxtInActive;
		}
	}
	else if (ges->tagPressed1 == TAG_MONTH_STR)
	{
		month_mode++;
		month_mode = month_mode % 3; // set 0 when reached max
	}
	else if (ges->tagPressed1 == TAG_TIME_STR)
	{
		time_mode++;
		time_mode = time_mode % 3; // set 0 when reached max
	}

	if (ges->distanceX > 300)
	{
		// Swift 300 pixels to active the date time adjustment window
		dateime_adjustment(s_pHalContext);
		// reset graph before active the graph window again
		graph_size_ramg = graph_l1_rotate_init(&box_graph_ecg, &box_graph_pth, &box_graph_co2);
		load_app_assets(graph_size_ramg);
	}
}

void draw_grid_by_cocmd(int32_t x, int32_t y, int32_t w, int32_t h, int32_t cell_w, int32_t cell_h)
{
	const uint32_t ACC_FACTOR = 100; // accuracy factor to simulate 0.01 pixel precision

	// Multiply all coordinates by ACC_FACTOR
	x *= ACC_FACTOR;
	y *= ACC_FACTOR;
	w *= ACC_FACTOR;
	h *= ACC_FACTOR;
	cell_w *= ACC_FACTOR;
	cell_h *= ACC_FACTOR;

	// Compute the number of columns and rows
	uint32_t num_cols = w / cell_w;
	uint32_t num_rows = h / cell_h;

	// Adjust cell size to fit perfectly
	cell_w = w / num_cols;
	cell_h = h / num_rows;

	uint32_t y_end = y + h;
	uint32_t x_end = x + w;

	EVE_Cmd_wr32(s_pHalContext, LINE_WIDTH(6));
	EVE_Cmd_wr32(s_pHalContext, COLOR_A(80));

	// Draw vertical lines
	for (uint32_t i = 0; i <= num_cols; i++)
	{
		uint32_t line_x = x + i * cell_w;
		DRAW_LINE_XY(line_x / ACC_FACTOR, y / ACC_FACTOR, line_x / ACC_FACTOR, y_end / ACC_FACTOR);
	}

	// Draw horizontal lines
	for (uint32_t j = 0; j <= num_rows; j++)
	{
		uint32_t line_y = y + j * cell_h;
		DRAW_LINE_XY(x / ACC_FACTOR, line_y / ACC_FACTOR, x_end / ACC_FACTOR, line_y / ACC_FACTOR);
	}
}

void draw_grid_box(app_box box_graph)
{
	int32_t x = box_graph.x;
	int32_t y = box_graph.y;
	int32_t w = box_graph.w;
	int32_t h = box_graph.h;
	int32_t cell_w = 15;
	int32_t cell_h = 15;
	draw_grid_by_cocmd(x, y, w, h, cell_w, cell_h);
}

int32_t main(int32_t argc, char *argv[])
{
	s_pHalContext = &s_halContext;
	Gpu_Init(s_pHalContext);

	EVE_Util_clearScreen(s_pHalContext);

	// read and store calibration setting
	Calibration_New(s_pHalContext);
	Flash_Init(s_pHalContext, TEST_DIR "ew2025_bedside_patient_monitor_demo_bt81x.bin", "ew2025_bedside_patient_monitor_demo_bt81x.bin", 0);
	FlashHelper_SwitchFullMode(s_pHalContext);

	// register big font 32 33 34
	Display_Start(s_pHalContext);
	EVE_CoCmd_romFont(s_pHalContext, FONT_32, 32);
	EVE_CoCmd_romFont(s_pHalContext, FONT_33, 33);
	EVE_CoCmd_romFont(s_pHalContext, FONT_34, 34);
	Display_End(s_pHalContext);

	btnStartTxt = btnStartTxtInActive;

	app_box lcd_size = {0, 0, s_pHalContext->Width, s_pHalContext->Height, s_pHalContext->Width, s_pHalContext->Height};
	app_box app_window = INIT_APP_BOX((s_pHalContext->Width - WINDOW_W) / 2, (s_pHalContext->Height - WINDOW_H) / 2, WINDOW_W, WINDOW_H);
	int32_t graph_start = app_window.x + 35;

	box_menu_top = INIT_APP_BOX(app_window.x, app_window.y, app_window.w * 80 / 100, app_window.h * 8 / 100);
	box_ecg = INIT_APP_BOX(graph_start, box_menu_top.y_end, box_menu_top.w, app_window.h * 28 / 100);
	box_pth = INIT_APP_BOX(graph_start, box_ecg.y_end, box_menu_top.w, box_ecg.h);
	box_co2 = INIT_APP_BOX(graph_start, box_pth.y_end, box_menu_top.w, box_ecg.h);
	box_menu_bottom = INIT_APP_BOX(app_window.x, box_co2.y_end, box_menu_top.w, box_menu_top.h);

	int32_t graph_margin_t = box_ecg.h / 4;
	int32_t graph_margin_l = box_ecg.w / 40;
	int32_t graph_margin_r = 5;
	int32_t graph_margin_b = box_ecg.h / 10;
	int32_t graph_margin_w = graph_margin_l + graph_margin_r + graph_start;
	int32_t graph_margin_h = graph_margin_t + graph_margin_b;

	box_graph_ecg = INIT_APP_BOX(box_ecg.x + graph_margin_l, box_ecg.y + graph_margin_t, box_ecg.w - graph_margin_w, box_ecg.h - graph_margin_h);
	box_graph_pth = INIT_APP_BOX(box_pth.x + graph_margin_l, box_pth.y + graph_margin_t, box_pth.w - graph_margin_w, box_pth.h - graph_margin_h);
	box_graph_co2 = INIT_APP_BOX(box_co2.x + graph_margin_l, box_co2.y + graph_margin_t, box_co2.w - graph_margin_w, box_co2.h - graph_margin_h);

	int32_t x = box_menu_top.x_end;
	int32_t w = app_window.w - box_menu_top.w;
	int32_t h = (app_window.h - box_menu_top.h - box_menu_bottom.h) / 4;
	int32_t y = box_menu_top.y_end;
	int32_t border = 2;

	app_box box_right1 = INIT_APP_BOX(x, y + h * 0 + 1, w, h);
	app_box box_right2 = INIT_APP_BOX(x, y + h * 1, w, h);
	app_box box_right3 = INIT_APP_BOX(x, y + h * 2, w, h);
	app_box box_right4 = INIT_APP_BOX(x, y + h * 3, w, h);

	init_datetime(11, 12, 2024, 9, 11, 0, 0);
	dateime_adjustment(s_pHalContext); // set date and time at initialize

	int32_t time_start_ms = 0;
	int32_t val_hr = 66;
	int32_t val_spo2 = 96;
	int32_t val_co2 = 22;
	int32_t val_sys = 156;
	int32_t val_dias = 93;

	static int32_t screenshot_counter = 0;

	graph_size_ramg = graph_l1_rotate_init(&box_graph_ecg, &box_graph_pth, &box_graph_co2);
	load_app_assets(graph_size_ramg);

	while (1)
	{
		Display_Start(s_pHalContext);
		EVE_Cmd_wr32(s_pHalContext, VERTEX_FORMAT(EVE_VERTEX_FORMAT));

		process_event();

		draw_app_window(app_window);

		// the grid
		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(255, 255, 255));
		EVE_Cmd_wr32(s_pHalContext, COLOR_A(100));
		draw_grid_box(box_graph_ecg);
		draw_grid_box(box_graph_pth);
		draw_grid_box(box_graph_co2);
		EVE_Cmd_wr32(s_pHalContext, LINE_WIDTH(16)); // reset line_width, without this, graphics become transparent 50%
		EVE_Cmd_wr32(s_pHalContext, COLOR_A(255));

		graph_l1_rotate_draw();

		// Top menu box
		EVE_Cmd_wr32(s_pHalContext, COLOR_CODE_WINDOW_BAR);
		DRAW_BOX(box_menu_top);
		// Bottom menu box
		DRAW_BOX(box_menu_bottom);

		int32_t graph_border = 3;
		int32_t brace_y = box_ecg.h / 7, brace_w = 5, brace_h = box_ecg.h * 4 / 5;

		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(255, 255, 255));
		// Graph ECG braces outside
		DRAW_RECT(box_ecg.x, box_ecg.y + brace_y, brace_w, brace_h);
		// Graph pleth braces outside
		DRAW_RECT(box_pth.x, box_pth.y + brace_y, brace_w, brace_h);
		// Graph co2 braces outside
		DRAW_RECT(box_co2.x, box_co2.y + brace_y, brace_w, brace_h);

		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(0, 0, 0));
		// Graph ecg braces inside
		DRAW_RECT(box_ecg.x + graph_border, box_ecg.y + brace_y + border, brace_w, brace_h - border * 2);
		// Graph pleth braces inside
		DRAW_RECT(box_pth.x + graph_border, box_pth.y + brace_y + border, brace_w, brace_h - border * 2);
		// Graph co2 braces inside
		DRAW_RECT(box_co2.x + graph_border, box_co2.y + brace_y + border, brace_w, brace_h - border * 2);

		EVE_Cmd_wr32(s_pHalContext, COLOR_CODE_WINDOW_BAR);
		// right menu top
		DRAW_RECT(box_menu_top.x_end, box_menu_top.y, WINDOW_W - box_menu_top.w, box_menu_top.h);
		// right menu bottom
		DRAW_RECT_BORDER(box_menu_bottom.x_end, box_menu_bottom.y, WINDOW_W - box_menu_bottom.w, box_menu_bottom.h, 0x0078d7, border, 0xffffff);

		// right menu HR
		DRAW_BOX_BORDER(box_right1, 0x000000, border, 0xffffff);
		// right menu spO2
		DRAW_BOX_BORDER(box_right2, 0x000000, border, 0xffffff);
		// right menu etCO2
		DRAW_BOX_BORDER(box_right3, 0x000000, border, 0xffffff);
		// right menu NIBP
		DRAW_BOX_BORDER(box_right4, 0x000000, border, 0xffffff);

		// buttons
		int32_t x = box_menu_top.x;
		int32_t y = box_menu_top.y_mid;
		int32_t btn_w = 180;
		int32_t btn_h = 50;
		int32_t btn_margin = 50;

		// Stop button
		if (btnStartState == BTN_START_INACTIVE)
		{
			EVE_CoCmd_fgColor(s_pHalContext, 0x0078D4);
			EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(255, 255, 255));
		}
		else
		{
			EVE_CoCmd_fgColor(s_pHalContext, 0xC2C2C2);
			EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(53, 53, 53));
		}
		EVE_Cmd_wr32(s_pHalContext, TAG(TAG_START_STOP));
		EVE_CoCmd_button(s_pHalContext, box_menu_top.x + btn_w * 2 + btn_margin, box_menu_bottom.y + 7, btn_w, btn_h, 30, 0, btnStartTxt);
		EVE_Cmd_wr32(s_pHalContext, TAG(0));
		if (btnStartState != BTN_START_ACTIVE)
		{
			EVE_CoCmd_fgColor(s_pHalContext, 0xC2C2C2);
		}

		// Record/NIBP/EXIT button
		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(97, 97, 97));
		EVE_CoCmd_fgColor(s_pHalContext, 0xC2C2C2);
		EVE_CoCmd_button(s_pHalContext, box_menu_top.x + btn_w * 1, box_menu_bottom.y + 7, btn_w, btn_h, 30, OPT_FLAT, "RECORD");
		EVE_CoCmd_button(s_pHalContext, box_menu_top.x + btn_w * 3 + btn_margin * 2, box_menu_bottom.y + 7, btn_w, btn_h, 30, OPT_FLAT, "NIBP");
		EVE_CoCmd_button(s_pHalContext, box_right4.x + (box_right4.w / 2 - btn_w / 2), box_menu_bottom.y + 7, btn_w, btn_h, 30, OPT_FLAT, "EXIT");

#if ENABLE_SHOW_FPS
		// Frame per second measurement
		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(0, 0, 0));
		EVE_CoCmd_text(s_pHalContext, app_window.x + 10, app_window.y_end - 40, 30, OPT_FORMAT, "fps = %d", getFPS());
#endif

		x = box_menu_top.x;
		y = box_menu_top.y_mid;
		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(255, 255, 255));

		EVE_CoCmd_text(s_pHalContext, x + 5, y, FONT_32, OPT_CENTERY, "Bed");
		EVE_CoCmd_text(s_pHalContext, x + 155, y, 31, OPT_CENTERY, "No. 5");
		EVE_Cmd_wr32(s_pHalContext, TAG(TAG_TIME_STR));
		if (time_mode == TIME_MODE_HH_MM)
		{
			EVE_CoCmd_text(s_pHalContext, box_menu_top.x_end - 10, y, 31, OPT_CENTERY | OPT_RIGHTX, hh_mm());
		}
		else if (time_mode == TIME_MODE_HH_MM_SS)
		{
			EVE_CoCmd_text(s_pHalContext, box_menu_top.x_end - 10, y, 31, OPT_CENTERY | OPT_RIGHTX, hh_mm_ss());
		}
		else if (time_mode == TIME_MODE_HH_MM_SS_MS)
		{
			EVE_CoCmd_text(s_pHalContext, box_menu_top.x_end - 10, y, 31, OPT_CENTERY | OPT_RIGHTX, hh_mm_ss_ms());
		}

		EVE_Cmd_wr32(s_pHalContext, TAG(TAG_MONTH_STR));
		if (month_mode == MONTH_MODE_DIGIT)
		{
			EVE_CoCmd_text(s_pHalContext, box_menu_top.x_mid, y, 31, OPT_CENTER, dd_mm_yyyy());
		}
		else if (month_mode == MONTH_MODE_STR3)
		{
			EVE_CoCmd_text(s_pHalContext, box_menu_top.x_mid, y, 31, OPT_CENTER, dd_mmm_yyyy());
		}
		else if (month_mode == MONTH_MODE_STRS)
		{
			EVE_CoCmd_text(s_pHalContext, box_menu_top.x_mid, y, 31, OPT_CENTER, dd_month_yyyy());
		}

		// zoom level control
		btn_w = 120;
		btn_h = 40;
		x = box_menu_top.x_end + (app_window.x_end - box_menu_top.x_end) / 2 - btn_w / 2;
		y = box_menu_top.y_mid - btn_h / 2;
		int zoom_icon_wh = 28;
		int zoom_icon_padding = 5;
		app_box zoombox = INIT_APP_BOX(x, y, btn_w, btn_h); // top right side
		EVE_Cmd_wr32(s_pHalContext, TAG(0));
		EVE_CoCmd_button(s_pHalContext, zoombox.x, zoombox.y, zoombox.w, zoombox.h, 28, 0, "");
		EVE_Cmd_wr32(s_pHalContext, BITMAP_HANDLE(0));
		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(0, 0, 0));
		EVE_CoCmd_setBitmap(s_pHalContext, zoom_out.ramg_address, COMPRESSED_RGBA_ASTC_4x4_KHR, zoom_icon_wh, zoom_icon_wh);
		EVE_Cmd_wr32(s_pHalContext, BEGIN(BITMAPS));
		EVE_DRAW_AT(zoombox.x + zoom_icon_padding, zoombox.y_mid - zoom_icon_wh / 2); // 14 is zoom icon height / 2
		EVE_CoCmd_setBitmap(s_pHalContext, zoom_in.ramg_address, COMPRESSED_RGBA_ASTC_4x4_KHR, zoom_icon_wh, zoom_icon_wh);
		EVE_Cmd_wr32(s_pHalContext, BEGIN(BITMAPS));
		EVE_DRAW_AT(zoombox.x_end - zoom_icon_wh - zoom_icon_padding, zoombox.y_mid - zoom_icon_wh / 2);
		EVE_CoCmd_text(s_pHalContext, zoombox.x_mid, zoombox.y_mid, font2.handler, OPT_FORMAT | OPT_CENTER, "%d", g_graph_zoom_lv);
		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(255, 255, 255));
		EVE_Cmd_wr32(s_pHalContext, COLOR_A(0));
		// transparent touch circle
		EVE_Cmd_wr32(s_pHalContext, TAG(TAG_ZOOM_DOWN));
		DRAW_CIRCLE(zoombox.x + zoom_icon_wh / 2 + zoom_icon_padding, zoombox.y_mid, 40);
		EVE_Cmd_wr32(s_pHalContext, TAG(TAG_ZOOM_UP));
		DRAW_CIRCLE(zoombox.x_end - zoom_icon_padding, zoombox.y_mid, 40);
		EVE_Cmd_wr32(s_pHalContext, COLOR_A(255));

		// Graph title text information
		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(255, 255, 255));
		EVE_CoCmd_text(s_pHalContext, box_ecg.x + box_ecg.w / 100, box_ecg.y + box_ecg.h / 10, font2.handler, 0, "ECG");
		EVE_CoCmd_text(s_pHalContext, box_pth.x + box_pth.w / 100, box_pth.y + box_pth.h / 10, font2.handler, 0, "PLETH");
		EVE_CoCmd_text(s_pHalContext, box_co2.x + box_co2.w / 100, box_co2.y + box_co2.h / 10, font2.handler, 0, "CO2");

		// create random data change
		int32_t time_end_ms = EVE_millis();
		int32_t duration = time_end_ms - time_start_ms;
		if (duration > (200 + app_random(100) - 50))
		{
			if (app_random(10) % 9 == 0)
				val_hr = 66 + app_random(8) - 5;
			if (app_random(10) % 5 == 0)
				val_spo2 = 90 + app_random(12) - 6;
			if (app_random(10) % 5 == 0)
				val_co2 = 22 + app_random(4) - 2;
			if (app_random(10) % 2 == 0)
				val_sys = 156 + app_random(8) - 4;
			if (app_random(10) % 3 == 0)
				val_dias = 93 + app_random(8) - 4;

			time_start_ms = time_end_ms;
		}

		EVE_Cmd_wr32(s_pHalContext, BITMAP_HANDLE(0));
		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(0, 255, 0));
		// Heart rate
		EVE_CoCmd_text(s_pHalContext, box_right1.x + 5, box_right1.y + 5, font2.handler, 0, "HR");
		EVE_CoCmd_number(s_pHalContext, box_right1.x_mid, box_right1.y_mid, font0.handler, OPT_CENTER, val_hr);
		EVE_CoCmd_text(s_pHalContext, box_right1.x_mid + 40, box_right1.y_mid, font2.handler, OPT_CENTERY, "bpm");

		// SPO2
		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(0, 255, 255));
		EVE_CoCmd_text(s_pHalContext, box_right2.x + 5, box_right2.y + 5, font2.handler, 0, "spO2");
		EVE_CoCmd_number(s_pHalContext, box_right2.x_mid, box_right2.y_mid, FONT_33, OPT_CENTER, val_spo2);
		EVE_CoCmd_text(s_pHalContext, box_right2.x_mid + 40, box_right2.y_mid, font2.handler, OPT_CENTERY, "%");

		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(255, 255, 0));
		// ETCO2
		EVE_CoCmd_text(s_pHalContext, box_right3.x + 5, box_right3.y + 5, font2.handler, 0, "etCO2");
		EVE_CoCmd_number(s_pHalContext, box_right3.x_mid, box_right3.y_mid, FONT_33, OPT_CENTER, val_co2);
		EVE_CoCmd_text(s_pHalContext, box_right3.x_mid + 40, box_right3.y_mid, font2.handler, OPT_CENTERY, "mmHg");

		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(255, 255, 255));
		// NIBP
		EVE_CoCmd_text(s_pHalContext, box_right4.x + 5, box_right4.y + 5, font2.handler, 0, "NIBP");
		EVE_CoCmd_number(s_pHalContext, box_right4.x + 2, box_right4.y + 55, FONT_32, 0, val_sys);
		EVE_CoCmd_text(s_pHalContext, box_right4.x_mid + 4, box_right4.y_mid, font2.handler, OPT_CENTERX, "mmHg");
		EVE_CoCmd_number(s_pHalContext, box_right4.x_mid + 50, box_right4.y + 55, FONT_32, 0, val_dias);
		EVE_CoCmd_text(s_pHalContext, box_right4.x + 35, box_right4.y_end - 40, font2.handler, 0, "sys");
		EVE_CoCmd_text(s_pHalContext, box_right4.x_end - 70, box_right4.y_end - 40, font2.handler, 0, "dias");

		Display_End(s_pHalContext);
	}
	return 0;
};
