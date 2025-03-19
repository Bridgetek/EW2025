/**
 * @file Graph_L1_rotate.c
 * @brief Implements a L1 rotated graph for display purposes.
 *
 * @author Bridgetek
 *
 * @date 2025
 * @license MIT License
 *
 * Copyright (c) [2019] [Bridgetek Pte Ltd (BRTChip)]
 */

#include "Helpers.h"
#include "Bedside_Patient_Monitor_Demo.h"

extern EVE_HalContext s_halContext;
extern EVE_HalContext *s_pHalContext;

int32_t new_data_heartbeat(int **data, int *data_size);
int32_t new_data_pleth(int **data, int *data_size);
int32_t new_data_co2(int **data, int *data_size);

#define GRAPH_BIT_PER_PIXEL 1												  // l1 1 bit per pixel
#define GRAPH_BIT_PER_LINE (GRAPH_BIT_PER_PIXEL * GRAPH_W)					  // bbp * width // 160
#define GRAPH_BYTE_PER_LINE (GRAPH_BIT_PER_LINE / BIT_PER_CHAR)				  // 160 / 8
#define GRAPH_BYTE_PER_BUFFER (GRAPH_H * (GRAPH_BIT_PER_LINE / BIT_PER_CHAR)) // 160 / 8 * 1000
#define GRAPH_BUFFER_NUM 3													  // display from buffer 1, append to buffer 2, loopback to buffer 0
#define GRAPH_BUFFER_SIZE (GRAPH_BYTE_PER_BUFFER * GRAPH_BUFFER_NUM)

typedef struct
{
	int32_t handler;
	int32_t bitmap_rp;				   // bitmap read pointer, use buffer 0 and 1
	int32_t bitmap_wp;				   // bitmap write pointer, use buffer 1 and 2
	int32_t bitmap_wplb;			   // bitmap write pointer loopback, use buffer 0
	int32_t buffer0, buffer1, buffer2; // 3 buffer on ramg
	int32_t loopback_addr;
	int32_t buffer2_end; // end addreff of buffer 2
	int32_t x, y, w, h;
	uint32_t rgba;
	int32_t x_graph_last;
} app_graph_t;

static app_graph_t graph_heartbeat;
static app_graph_t graph_pleth;
static app_graph_t graph_co2;

/**
 * @brief Set a pixel color on/off in a graph buffer according to input coordinates and color.
 *
 * @param buffer The graph buffer to draw on.
 * @param x The x-coordinate of the pixel to draw.
 * @param y The y-coordinate of the pixel to draw.
 * @param rgba The color of the pixel. Non-zero values set the pixel color to on.
 *
 * @return None
 */
static void draw_pixel(SIGNALS_DATA_TYPE *buffer, int32_t x, int32_t y, uint32_t rgba)
{
	if (y > GRAPH_H * 2)
	{
		goto err;
	}

	if (x < 0)
	{
		goto err;
	}
	if (x < 0 || x >= GRAPH_W)
	{
		goto err;
	}

	// set pixel color on/off
	int32_t pixel_nth = x + y * GRAPH_W;
	int32_t byte_index = pixel_nth / 8;	   // byte nth
	int32_t bit_index = 7 - pixel_nth % 8; // bit nth

	if (byte_index >= GRAPH_BYTE_PER_LINE * g_graph_zoom_lv)
	{
		goto err;
	}

	buffer[byte_index] |= (1 << bit_index);
	return;

err:
	printf("Skip a pixel at (%d, %d)\n", x, y);
	return;
}

/**
 * @brief Draws a line on the given buffer using Bresenham's line drawing algorithm
 *
 * @param buffer The buffer to draw the line on.
 * @param x1 The x-coordinate of the starting pixel.
 * @param y1 The y-coordinate of the starting pixel.
 * @param x2 The x-coordinate of the ending pixel.
 * @param y2 The y-coordinate of the ending pixel.
 * @param rgba The color of the line.
 *
 * @return None
 */
static void bresenham_line(SIGNALS_DATA_TYPE *buffer, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t rgba)
{
	int32_t dx = abs(x2 - x1);
	int32_t dy = abs(y2 - y1);
	int32_t sx = (x2 > x1) ? 1 : -1;
	int32_t sy = (y2 > y1) ? 1 : -1;
	int32_t err = dx - dy;

	while (1)
	{
		// Draw the current pixel
		draw_pixel(buffer, x1, y1, rgba);

		// Check if the endpoint is reached
		if (x1 == x2 && y1 == y2)
		{
			break;
		}

		int32_t e2 = 2 * err;

		// Adjust error and coordinates
		if (e2 > -dy)
		{
			err -= dy;
			x1 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y1 += sy;
		}
	}
}

/**
 * @brief Normalizes a sensor value to fit within the graph's y-axis range
 *
 * @param graph The graph to normalize the sensor value for
 * @param sensor_value The sensor value to normalize
 * @param sensor_min The minimum value of the sensor range
 * @param sensor_max The maximum value of the sensor range
 *
 * @return The normalized value in the range [graph_min, graph_max]
 */
static int32_t normalize_to_graph(app_graph_t *graph, int32_t sensor_value, int32_t sensor_min, int32_t sensor_max)
{
#define graph_max graph->h
#define graph_min 1

	// Ensure the sensor range is valid to prevent division by zero
	if (sensor_max == sensor_min)
	{
		return 0; // Default to minimum y-value if range is zero
	}

	/* Normalize the sensor value to the range[graph_min, graph_max]
	   here is the formular:
		 data range:  sensor_min  --> sensor_value ------> sensor_max
		 graph range: graph_min   --> x            ------> graph_max
		 find x: => (graph_max - x) / (graph_max - graph_min) = (sensor_max - sensor_value) / (sensor_max - sensor_min)
		 find x: => (graph_max - x) = (graph_max - graph_min) * (sensor_max - sensor_value) / (sensor_max - sensor_min)
		 find x: => x = graph_max - (graph_max - graph_min) * (sensor_max - sensor_value) / (sensor_max - sensor_min)
	*/
	return graph_max - (graph_max - graph_min) * (sensor_max - sensor_value) / (sensor_max - sensor_min);
}

/**
 * @brief Appends a new graph line to the current graph.
 *
 * @param graph The graph to append to.
 * @param lines The new line data to append, where each element is a byte representing a y-axis value.
 * @param line_count The number of elements in lines.
 *
 */
static void graph_append(app_graph_t *graph, SIGNALS_DATA_TYPE *lines, int32_t line_count)
{
	uint8_t buffer_1line[GRAPH_BYTE_PER_LINE * GRAPH_ZOOM_LV_MAX] = {{0}};
	uint32_t addr = graph->bitmap_wp;
	for (int32_t i = 0; i < line_count; i++)
	{
		memset(buffer_1line, 0, sizeof(buffer_1line));
		SIGNALS_DATA_TYPE x = lines[i] & 0xFF;
		x = normalize_to_graph(graph, x, 0, 255);
		bresenham_line(buffer_1line, x, g_graph_zoom_lv - 1, graph->x_graph_last, 0, graph->rgba);
		graph->x_graph_last = x;
		EVE_Hal_wrMem(s_pHalContext, addr, buffer_1line, GRAPH_BYTE_PER_LINE * g_graph_zoom_lv);
		addr += GRAPH_BYTE_PER_LINE * g_graph_zoom_lv;
	}
}

/**
 * @brief Displays a graph on the screen
 *
 * @param graph The graph to display
 *
 * This function displays a graph on the screen, with the graph's bitmap as the
 * background and the graph's color as the foreground. The graph's bitmap is
 * rotated by 90 degrees to fit the screen vertically. The graph's color is
 * extracted from the rgba field of the graph struct.
 */
static void graph_display(app_graph_t *graph)
{
	int32_t bformat = L1;
	int32_t lw = max(graph->w, GRAPH_W);
	int32_t lh = max(graph->w, GRAPH_W);
#define MAX_ANGLE 360
#define MAX_CIRCLE_UNIT 65536
	int32_t rotation_angle = -90;

	// display bitmap
	EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(255, 255, 255));
	EVE_CoCmd_setBitmap(s_pHalContext, graph->bitmap_rp, bformat, GRAPH_W, graph->w);
	EVE_Cmd_wr32(s_pHalContext, PALETTE_SOURCE(0));
	EVE_Cmd_wr32(s_pHalContext, BITMAP_SIZE(NEAREST, BORDER, BORDER, lw, lh));
	EVE_Cmd_wr32(s_pHalContext, BITMAP_SIZE_H(lw >> 9, lh >> 9));
	EVE_Cmd_wr32(s_pHalContext, SAVE_CONTEXT());
	EVE_CoCmd_loadIdentity(s_pHalContext);
	EVE_CoCmd_translate(s_pHalContext, 0, GRAPH_W * MAX_CIRCLE_UNIT);
	EVE_CoCmd_rotate(s_pHalContext, rotation_angle * MAX_CIRCLE_UNIT / MAX_ANGLE);
	EVE_CoCmd_setMatrix(s_pHalContext);

	// coloring
	int32_t r = (graph->rgba >> 24) & 0xFF; // Extract Red (most significant byte)
	int32_t g = (graph->rgba >> 16) & 0xFF; // Extract Green
	int32_t b = (graph->rgba >> 8) & 0xFF;	// Extract Blue
	int32_t a = graph->rgba & 0xFF;			// Extract Alpha (least significant byte)
	EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(r, g, b));

	// vertex
	EVE_Cmd_wr32(s_pHalContext, BEGIN(BITMAPS));
	int32_t x = graph->x;
	int32_t y = graph->y - (GRAPH_W - graph->h);
	EVE_DRAW_AT(x, y);
	EVE_Cmd_wr32(s_pHalContext, RESTORE_CONTEXT());
}

/**
 * @brief Appends new line data to the graph and updates its display.
 *
 * This function appends a series of new data lines to the graph and manages
 * the graph's memory buffers to ensure the display remains current and accurate.
 *
 * @param graph The graph to which the lines are appended.
 * @param lines The new line data to append, where each element is a byte representing a y-axis value.
 * @param line_count The number of elements in lines.
 *
 */
static void graph_append_and_display(app_graph_t *graph, SIGNALS_DATA_TYPE *lines, int32_t line_count)
{
	// skip overflow lines
	if (line_count > min(line_count, graph->w / g_graph_zoom_lv))
	{
		return;
	}

	// write data to ramg
	while (btnStartState == BTN_START_INACTIVE)
	{
		int32_t bytes_count = line_count * g_graph_zoom_lv * GRAPH_BYTE_PER_LINE;

		// if data size >= buffer2, write to buffer0
		if (graph->bitmap_wp + bytes_count >= graph->buffer2_end)
		{
			graph->bitmap_wp = graph->bitmap_wplb;
			graph_append(graph, lines, line_count);
			graph->bitmap_wplb = graph->buffer0;
			graph->bitmap_wp += bytes_count;
			graph->bitmap_rp = graph->bitmap_wp - GRAPH_BYTE_PER_BUFFER;
			break;
		}

		graph_append(graph, lines, line_count);

		// loopback
		if (graph->bitmap_wp + bytes_count > graph->loopback_addr)
		{
			EVE_CoCmd_memCpy(s_pHalContext, graph->bitmap_wplb, graph->bitmap_wp, bytes_count);
			graph->bitmap_wplb += bytes_count;
		}

		// increase write pointer
		graph->bitmap_rp += bytes_count;
		graph->bitmap_wp += bytes_count;
		break;
	}

	graph_display(graph);
}

/**
 * @brief Initializes the graph buffers and settings for each graph type (heartbeat, pleth, CO2).
 *
 * This function sets up three graph structures for displaying different types of data: heartbeat, pleth, and CO2.
 * Each graph is initialized with separate buffers, memory addresses, and display parameters. The function also
 * clears the memory for each graph buffer and assigns specific colors to each graph type.
 *
 * @param box_heartbeat Pointer to the app_box structure defining the display area for the heartbeat graph.
 * @param box_pleth Pointer to the app_box structure defining the display area for the pleth graph.
 * @param box_co2 Pointer to the app_box structure defining the display area for the CO2 graph.
 *
 * @return The ending address of the buffer for the CO2 graph, indicating the total memory used by all graph buffers.
 */
uint32_t graph_l1_rotate_init(app_box *box_heartbeat, app_box *box_pleth, app_box *box_co2)
{
	//      ramg: <-------- read pointer -------->lb<- when write pointer reached here, start loopback to buffer 0
	//      ramg: -------------------------------------------------
	//      ramg: |   buffer 0    |   buffer 1    |   buffer 2    |
	//      ramg: -------------------------------------------------
	//      ramg:                 <-------- write pointer -------->

	memset(&graph_heartbeat, 0, sizeof(app_graph_t));
	memset(&graph_pleth, 0, sizeof(app_graph_t));
	memset(&graph_co2, 0, sizeof(app_graph_t));

	app_graph_t *graphs[] = {&graph_heartbeat, &graph_pleth, &graph_co2};
	app_box *boxs[] = {box_heartbeat, box_pleth, box_co2};
	int32_t colors[] = {0x00ff0000, 0x00ffff00, 0xffff0000};

	for (int32_t i = 0; i < 3; i++)
	{
		app_graph_t *gh = graphs[i];
		gh->buffer0 = i * GRAPH_BUFFER_SIZE; // loopback buffer
		gh->buffer1 = gh->buffer0 + GRAPH_BYTE_PER_BUFFER;
		gh->buffer2 = gh->buffer1 + GRAPH_BYTE_PER_BUFFER;

		gh->loopback_addr = gh->buffer2;
		gh->buffer2_end = gh->buffer2 + GRAPH_BYTE_PER_BUFFER;

		gh->bitmap_rp = gh->buffer0;   // display from buffer 0 and continue to buffer 1
		gh->bitmap_wp = gh->buffer1;   // append to buffer 1 and continue to buffer 2
		gh->bitmap_wplb = gh->buffer0; // loopback from buffer 0
		gh->x = boxs[i]->x;
		gh->y = boxs[i]->y;
		gh->w = boxs[i]->w;
		gh->h = boxs[i]->h;
		gh->handler = i + 1;
		gh->rgba = colors[i];
		EVE_CoCmd_memSet(s_pHalContext, gh->buffer0, 0, GRAPH_BUFFER_SIZE);
	}
	return graph_co2.buffer2_end;
}

/**
 * @brief Collects new data samples for each graph type and updates their display.
 *
 * This function retrieves new data samples for the heartbeat, plethysmography, and CO2 graphs
 * by calling their respective data simulation functions. It then appends these data samples
 * to each graph and updates their display on the screen.
 */
void graph_l1_rotate_draw()
{
	SIGNALS_DATA_TYPE *data_heartbeat;
	SIGNALS_DATA_TYPE *data_pleth;
	SIGNALS_DATA_TYPE *data_co2;
	int32_t data_heartbeat_size = 0;
	int32_t data_pleth_size = 0;
	int32_t data_co2_size = 0;

	int32_t x1 = new_data_heartbeat(&data_heartbeat, &data_heartbeat_size);
	int32_t x2 = new_data_pleth(&data_pleth, &data_pleth_size);
	int32_t x3 = new_data_co2(&data_co2, &data_co2_size);

	graph_append_and_display(&graph_heartbeat, data_heartbeat, data_heartbeat_size);
	graph_append_and_display(&graph_pleth, data_pleth, data_pleth_size);
	graph_append_and_display(&graph_co2, data_co2, data_co2_size);
}