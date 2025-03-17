#ifndef APP_H_
#define APP_H_

#include "EVE_Platform.h"

// Path to UI assets Folder
#if defined(MSVC_PLATFORM) || defined(BT8XXEMU_PLATFORM)
#define TEST_DIR                            "..\\..\\..\\Test\\Flash\\"
#else
#define TEST_DIR                            "/"
#endif

#define GET_CALIBRATION                     1

#define WINDOW_W 1280
#define WINDOW_H 800
#define GRAPH_W 160
#define GRAPH_H 1000
#define GRAPH_SIZE (GRAPH_W * GRAPH_H)

#define CHAR_BIT 8
#define BITS_IN_TYPE(type) (sizeof(type) * CHAR_BIT)
#define BIT_PER_CHAR (BITS_IN_TYPE(char))

#define MSG printf
#define SIGNALS_DATA_TYPE unsigned char 

extern int32_t g_graph_zoom_lv;
#define GRAPH_ZOOM_LV_MAX 8

#define TAG_ZOOM_UP 1
#define TAG_ZOOM_DOWN 2
#define TAG_START_STOP 3
#define TAG_MONTH_STR 4
#define TAG_TIME_STR 5

#define BTN_START_ACTIVE 0
#define BTN_START_INACTIVE 1

#define FONT_32 2 // note: BT81x maximum handler is 31
#define FONT_33 3
#define FONT_34 4

extern uint8_t btnStartState;
extern uint32_t grid_bytes;

#endif /* APP_H_ */
