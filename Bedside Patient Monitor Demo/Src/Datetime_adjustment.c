#include <stdio.h>
#include <stdint.h>

#include "Helpers.h"
#include "common.h"
#include "Bedside_Patient_Monitor_Demo.h"

extern EVE_HalContext* s_pHalContext;
// Day (1-31)
static uint32_t dd_list[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
};

// Month (1-12)
static uint32_t mm_list[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12
};

// Year (1900-2100) -> Adjust as needed
static uint32_t yy_list[] = {
    1900, 1901, 1902, 1903, 1904, 1905, 1906, 1907, 1908, 1909,
    1910, 1911, 1912, 1913, 1914, 1915, 1916, 1917, 1918, 1919,
    1920, 1921, 1922, 1923, 1924, 1925, 1926, 1927, 1928, 1929,
    1930, 1931, 1932, 1933, 1934, 1935, 1936, 1937, 1938, 1939,
    1940, 1941, 1942, 1943, 1944, 1945, 1946, 1947, 1948, 1949,
    1950, 1951, 1952, 1953, 1954, 1955, 1956, 1957, 1958, 1959,
    1960, 1961, 1962, 1963, 1964, 1965, 1966, 1967, 1968, 1969,
    1970, 1971, 1972, 1973, 1974, 1975, 1976, 1977, 1978, 1979,
    1980, 1981, 1982, 1983, 1984, 1985, 1986, 1987, 1988, 1989,
    1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999,
    2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009,
    2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019,
    2020, 2021, 2022, 2023, 2024, 2025, 2026, 2027, 2028, 2029,
    2030, 2031, 2032, 2033, 2034, 2035, 2036, 2037, 2038, 2039,
    2040, 2041, 2042, 2043, 2044, 2045, 2046, 2047, 2048, 2049,
    2050, 2051, 2052, 2053, 2054, 2055, 2056, 2057, 2058, 2059,
    2060, 2061, 2062, 2063, 2064, 2065, 2066, 2067, 2068, 2069,
    2070, 2071, 2072, 2073, 2074, 2075, 2076, 2077, 2078, 2079,
    2080, 2081, 2082, 2083, 2084, 2085, 2086, 2087, 2088, 2089,
    2090, 2091, 2092, 2093, 2094, 2095, 2096, 2097, 2098, 2099, 2100
};

// Hour (0-23)
static uint32_t hh_list[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23
};

// Minute (0-59)
static uint32_t mt_list[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59
};

// Second (0-59)
static uint32_t ss_list[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59
};

// Function to find the index of a given number in the array
int find_index(uint32_t arr[], size_t size, uint32_t target) {
    for (size_t i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;  // Return the index if found
        }
    }
    return -1; // Return -1 if not found
}

// Function to return the number of valid left neighbors
int find_left_neighbors(uint32_t arr[], size_t size, int index, int n) {
    int count = 0;
    for (int i = index - n; i < index; i++) {
        if (i >= 0) {  // Ensure the index is valid
            count++;
        }
    }
    return count; // Return the number of valid left neighbors
}

// Function to return the number of valid right neighbors
int find_right_neighbors(uint32_t arr[], size_t size, int index, int n) {
    int count = 0;
    for (int i = index + 1; i <= index + n; i++) {
        if (i < size) {  // Ensure the index is within array bounds
            count++;
        }
    }
    return count; // Return the number of valid right neighbors
}

typedef struct {
    uint32_t* array;
    uint32_t array_count;
    int32_t index;
    int32_t y_offset;
    int32_t frame_x, frame_y, frame_w, frame_h;
    int32_t item_h;
    int32_t tag_val;
    uint8_t* title;
    int32_t velocity;
    int32_t* font_list;
    int32_t num_active_item;
    int rebound;
}scrolling_vertical_array_t;

int32_t index_max_min(int index, int32_t arr_count, int32_t display_count) {
    int32_t index_max = (arr_count - (display_count + 1) / 2);
    int32_t index_min = -display_count / 2;
    index = max(index, index_min);
    index = min(index, index_max);
    return index;

}
// return the selected index
int32_t scrollable_list(scrolling_vertical_array_t *sc) {
    Gesture_Touch_t* ges = utils_gestureGet(s_pHalContext);
    
    int32_t dragY = 0;
    int32_t item_start = 0; // 0 to -item_num / 2
    int32_t item_end = 0; // 0 to item_num / 2

    int32_t ret_index = 0;

    if (ges->tagPressed== sc->tag_val) {
        dragY = ges->distanceY;
    }

    else if (ges->tagReleased == sc->tag_val) {
        sc->y_offset += ges->distanceY;
        sc->index -= sc->y_offset / sc->item_h;
        sc->y_offset = sc->y_offset % sc->item_h;

        int32_t index_max = (sc->array_count - (sc->num_active_item + 1) / 2);
        int32_t index_min = -sc->num_active_item / 2;
        sc->index = max(sc->index, index_min);
        sc->index = min(sc->index, index_max);
    }

    else if (ges->velocityY != 0 && ges->tagVelocity == sc->tag_val) {
        sc->velocity = ges->velocityY;
        stopVelocity();
    }
    
    if (sc->velocity != 0) {
        const int32_t change_level = 35;
        const int32_t speed_level = 9;
        int32_t change = sc->velocity / change_level;

        sc->y_offset -= change;

        sc->index -= sc->y_offset / sc->item_h;
        sc->y_offset = sc->y_offset % sc->item_h;

        int32_t index_max = (sc->array_count - (sc->num_active_item + 1) / 2);
        int32_t index_min = -sc->num_active_item / 2;
        sc->index = index_max_min(sc->index, sc->array_count, sc->num_active_item);

        if (abs(sc->velocity) < 2)
            sc->velocity = 0;
         else
            sc->velocity = sc->velocity * speed_level / 10;

        // move velocity to item boundary
        if (sc->index == index_min) {
            sc->velocity = (sc->velocity < -sc->item_h) ? -sc->item_h : sc->velocity;
        }
        else if (sc->index == index_max) {
            sc->velocity = (sc->velocity > sc->item_h) ? sc->item_h : sc->velocity;
        }
    }
    else{ // stopped scoling, adjustment to center
#define ENABLE_INDEX_ADJUSTMENT_AFTER_SCROLLING 0
#if ENABLE_INDEX_ADJUSTMENT_AFTER_SCROLLING
        if (sc->y_offset > 0) {
            if (sc->y_offset < sc->item_h / 2) {
                sc->y_offset--;
            }
            else if (sc->y_offset > sc->item_h / 2) {
                sc->y_offset++;
                if (sc->y_offset == sc->item_h) {
                    sc->y_offset = 0;
                    sc->index--;
                    sc->index = index_max_min(sc->index, sc->array_count, sc->num_active_item);
                }
            }
        }
        else if (sc->y_offset < 0) {
            if (sc->y_offset > -sc->item_h / 2) {
                sc->y_offset++;
            }
            else if (sc->y_offset < 0 && sc->y_offset < -sc->item_h / 2) {
                sc->y_offset--;
                if (sc->y_offset == -sc->item_h) {
                    sc->y_offset = 0;
                    sc->index++;
                    sc->index = index_max_min(sc->index, sc->array_count, sc->num_active_item);
                }
            }
        }
#else
        if (sc->y_offset > 0) {
            sc->y_offset--;
        }
        else if (sc->y_offset < 0) {
            sc->y_offset++;
        }
#endif
    }

    int32_t drag_index = -dragY / sc->item_h;
    int32_t drag_y = dragY % sc->item_h;

    // draw the title
    EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(0, 0, 0));
    EVE_CoCmd_text(s_pHalContext, sc->frame_x + sc->frame_w / 2, sc->frame_y - 30, 28, OPT_CENTERX, sc->title);

    EVE_Cmd_wr32(s_pHalContext, SCISSOR_XY(sc->frame_x, sc->frame_y));
    EVE_Cmd_wr32(s_pHalContext, SCISSOR_SIZE(sc->frame_w, sc->item_h * sc->num_active_item));
    int32_t text_x = sc->frame_x + sc->frame_w / 2;
    for (int i = 0; i < sc->num_active_item+1; i++) {
        int arr_index = i + sc->index + drag_index;
        int text_y = drag_y + sc->y_offset + sc->frame_y + sc->item_h * i;
        if (arr_index >= 0 && arr_index < sc->array_count) {
            EVE_CoCmd_text(s_pHalContext, text_x, text_y, sc->font_list[i], OPT_FORMAT | OPT_CENTERX, "%d", sc->array[arr_index]);
            if (i == sc->num_active_item / 2) {
                ret_index = arr_index;
            }
        }
    }

    // transparent drawing_frame
    EVE_Cmd_wr32(s_pHalContext, COLOR_A(110));
    EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(0, 0, 0));
    EVE_Cmd_wr32(s_pHalContext, TAG(sc->tag_val));
    DRAW_RECT(sc->frame_x, sc->frame_y, sc->frame_w, sc->frame_h);
    EVE_Cmd_wr32(s_pHalContext, COLOR_A(255));

    EVE_Cmd_wr32(s_pHalContext, SCISSOR_XY(0, 0));
    EVE_Cmd_wr32(s_pHalContext, SCISSOR_SIZE(2048, 2048));

    return sc->array[ret_index];
}

void dateime_adjustment(EVE_HalContext* phost) {
    uint32_t dd = get_dd();
    uint32_t mm = get_mm();
    uint32_t yy = get_yyyy();
    uint32_t hh = get_hh();
    uint32_t mt = get_mt();
    uint32_t ss = get_ss();
    uint32_t ms = get_ms();

    const uint8_t dd_text[]  = "Date"  ;
    const uint8_t mm_text[]  = "Month" ;
    const uint8_t yy_text[]  = "year"  ;
    const uint8_t hh_text[]  = "Hour"  ;
    const uint8_t mt_text[]  = "Minute";
    const uint8_t ss_text[]  = "Second";

    int dd_count = sizeof(dd_list) / sizeof(uint32_t);
    int mm_count = sizeof(mm_list) / sizeof(uint32_t);
    int yy_count = sizeof(yy_list) / sizeof(uint32_t);
    int hh_count = sizeof(hh_list) / sizeof(uint32_t);
    int mt_count = sizeof(mt_list) / sizeof(uint32_t);
    int ss_count = sizeof(ss_list) / sizeof(uint32_t);

    const int32_t dd_tag = 1;
    const int32_t mm_tag = 2;
    const int32_t yy_tag = 3;
    const int32_t hh_tag = 4;
    const int32_t mt_tag = 5;
    const int32_t ss_tag = 6;
    const int32_t tag_btn_ok = 7;
    const int32_t tag_btn_cancel = 8;

    app_box box_datetime = INIT_APP_BOX(phost->Width / 2 - phost->Width * 2 / 6, phost->Height / 2 - phost->Height * 2 / 6, phost->Width * 2 / 3, phost->Height * 2 / 3);

    int h = 50, w = 100;
    int x = phost->Width / 2 - (200 + w * 5) / 2; // x center screen
    int y = phost->Height / 2 - h * 5 / 2; // y center screen
    app_box yy_frame = INIT_APP_BOX(x, y, 200, h);
    app_box mm_frame = INIT_APP_BOX(yy_frame.x_end + 1, y, w, h);
    app_box dd_frame = INIT_APP_BOX(mm_frame.x_end + 1, y, w, h);
    app_box hh_frame = INIT_APP_BOX(dd_frame.x_end + 1, y, w, h);
    app_box mt_frame = INIT_APP_BOX(hh_frame.x_end + 1, y, w, h);
    app_box ss_frame = INIT_APP_BOX(mt_frame.x_end + 1, y, w, h);

    int32_t fonts[] = { 30, 30, FONT_32, 30, 30, 30 };

    scrolling_vertical_array_t yy_scroller;
    yy_scroller.array = yy_list;
    yy_scroller.array_count = yy_count;
    yy_scroller.num_active_item = 5;
    yy_scroller.item_h = yy_frame.h;
    yy_scroller.frame_x = yy_frame.x;
    yy_scroller.frame_y = yy_frame.y;
    yy_scroller.frame_w = yy_frame.w;
    yy_scroller.frame_h = yy_scroller.num_active_item * yy_scroller.item_h;
    yy_scroller.y_offset = 0;
    yy_scroller.index = find_index(yy_list, yy_count, yy);
    yy_scroller.tag_val = yy_tag;
    yy_scroller.title = yy_text;
    yy_scroller.velocity = 0;
    yy_scroller.font_list = fonts;
    yy_scroller.index = index_max_min(yy_scroller.index - yy_scroller.num_active_item / 2, yy_scroller.array_count, yy_scroller.num_active_item);

    scrolling_vertical_array_t mm_scroller;
    mm_scroller.array = mm_list;
    mm_scroller.array_count = mm_count;
    mm_scroller.num_active_item = 5;
    mm_scroller.item_h = mm_frame.h;
    mm_scroller.frame_x = mm_frame.x;
    mm_scroller.frame_y = mm_frame.y;
    mm_scroller.frame_w = mm_frame.w;
    mm_scroller.frame_h = mm_scroller.num_active_item * mm_scroller.item_h;
    mm_scroller.y_offset = 0;
    mm_scroller.index = find_index(mm_list, mm_count, mm);
    mm_scroller.tag_val = mm_tag;
    mm_scroller.title = mm_text;
    mm_scroller.velocity = 0;
    mm_scroller.font_list = fonts;
    mm_scroller.index = index_max_min(mm_scroller.index - mm_scroller.num_active_item / 2, mm_scroller.array_count, mm_scroller.num_active_item);

    scrolling_vertical_array_t dd_scroller;
    dd_scroller.array = dd_list;
    dd_scroller.array_count = dd_count;
    dd_scroller.num_active_item = 5;
    dd_scroller.item_h = dd_frame.h;
    dd_scroller.frame_x = dd_frame.x;
    dd_scroller.frame_y = dd_frame.y;
    dd_scroller.frame_w = dd_frame.w;
    dd_scroller.frame_h = dd_scroller.num_active_item * dd_scroller.item_h;
    dd_scroller.y_offset = 0;
    dd_scroller.index = find_index(dd_list, dd_count, dd);
    dd_scroller.tag_val = dd_tag;
    dd_scroller.title = dd_text;
    dd_scroller.velocity = 0;
    dd_scroller.font_list = fonts;
    dd_scroller.index = index_max_min(dd_scroller.index - dd_scroller.num_active_item / 2, dd_scroller.array_count, dd_scroller.num_active_item);

    scrolling_vertical_array_t hh_scroller;
    hh_scroller.array = hh_list;
    hh_scroller.array_count = hh_count;
    hh_scroller.num_active_item = 5;
    hh_scroller.item_h = hh_frame.h;
    hh_scroller.frame_x = hh_frame.x;
    hh_scroller.frame_y = hh_frame.y;
    hh_scroller.frame_w = hh_frame.w;
    hh_scroller.frame_h = hh_scroller.num_active_item * hh_scroller.item_h;
    hh_scroller.y_offset = 0;
    hh_scroller.index = find_index(hh_list, hh_count, hh);
    hh_scroller.tag_val = hh_tag;
    hh_scroller.title = hh_text;
    hh_scroller.velocity = 0;
    hh_scroller.font_list = fonts;
    hh_scroller.index = index_max_min(hh_scroller.index - hh_scroller.num_active_item / 2, hh_scroller.array_count, hh_scroller.num_active_item);

    scrolling_vertical_array_t mt_scroller;
    mt_scroller.array = mt_list;
    mt_scroller.array_count = mt_count;
    mt_scroller.num_active_item = 5;
    mt_scroller.item_h = mt_frame.h;
    mt_scroller.frame_x = mt_frame.x;
    mt_scroller.frame_y = mt_frame.y;
    mt_scroller.frame_w = mt_frame.w;
    mt_scroller.frame_h = mt_scroller.num_active_item * mt_scroller.item_h;
    mt_scroller.y_offset = 0;
    mt_scroller.index = find_index(mt_list, mt_count, mt);
    mt_scroller.tag_val = mt_tag;
    mt_scroller.title = mt_text;
    mt_scroller.velocity = 0;
    mt_scroller.font_list = fonts;
    mt_scroller.index = index_max_min(mt_scroller.index - mt_scroller.num_active_item / 2, mt_scroller.array_count, mt_scroller.num_active_item);

    scrolling_vertical_array_t ss_scroller;
    ss_scroller.array = ss_list;
    ss_scroller.array_count = ss_count;
    ss_scroller.num_active_item = 5;
    ss_scroller.item_h = ss_frame.h;
    ss_scroller.frame_x = ss_frame.x;
    ss_scroller.frame_y = ss_frame.y;
    ss_scroller.frame_w = ss_frame.w;
    ss_scroller.frame_h = ss_scroller.num_active_item * ss_scroller.item_h;
    ss_scroller.y_offset = 0;
    ss_scroller.index = find_index(ss_list, ss_count, ss);
    ss_scroller.tag_val = ss_tag;
    ss_scroller.title = ss_text;
    ss_scroller.velocity = 0;
    ss_scroller.font_list = fonts;
    ss_scroller.index = index_max_min(ss_scroller.index - ss_scroller.num_active_item / 2, ss_scroller.array_count, ss_scroller.num_active_item);

    while (1) {
        Gesture_Touch_t* ges = utils_gestureRenew(s_pHalContext);
        if (ges->tagReleased == tag_btn_ok) {
            init_datetime(dd, mm, yy, hh, mt, ss, 0);
            return;
        }
        else if (ges->tagReleased == tag_btn_cancel) {
            return;
        }

        Display_Start(s_pHalContext);
        EVE_Cmd_wr32(s_pHalContext, VERTEX_FORMAT(EVE_VERTEX_FORMAT));

        DRAW_BOX_BORDER(box_datetime, 0x00FFFFFF, 2, 0x00000000);
        EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(0, 0, 0));
        EVE_CoCmd_text(s_pHalContext, box_datetime.x + box_datetime.w / 2, box_datetime.y + 30, 31, OPT_CENTERX, "Select the date time");

        int btn_w = 100, btn_distance = 20;
        int btn_x = (box_datetime.x + box_datetime.w / 2) - (btn_w * 2 + btn_distance) / 2; // x start of the 2 buttons center screen
        int btn_y = ss_frame.y + ss_frame.h * 5 + 10;
        EVE_CoCmd_fgColor(s_pHalContext, 0x144344);
        EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(255, 255, 255));
        EVE_Cmd_wr32(s_pHalContext, TAG(tag_btn_ok));
        EVE_CoCmd_button(s_pHalContext, btn_x, btn_y, btn_w, 30, 28, 0, "Ok");
        EVE_Cmd_wr32(s_pHalContext, TAG(tag_btn_cancel));
        EVE_CoCmd_button(s_pHalContext, btn_x + btn_w + btn_distance, btn_y, btn_w, 30, 28, 0, "Cancel");
        EVE_Cmd_wr32(s_pHalContext, TAG(tag_btn_cancel));
        EVE_CoCmd_button(s_pHalContext, box_datetime.x + 5, box_datetime.y + 5, 80, 30, 28, 0, "Back");

        yy=scrollable_list(&yy_scroller);
        mm=scrollable_list(&mm_scroller);
        dd=scrollable_list(&dd_scroller);
        
        hh=scrollable_list(&hh_scroller);
        mt=scrollable_list(&mt_scroller);
        ss=scrollable_list(&ss_scroller);

        // reset scissor
        EVE_Cmd_wr32(s_pHalContext, SCISSOR_XY(0, 0));
        EVE_Cmd_wr32(s_pHalContext, SCISSOR_SIZE(2048, 2048));

        Display_End(s_pHalContext);
    }
}
