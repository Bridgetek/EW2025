/**
 * @file Page_Keyboard.c
 * @brief A simple keyboard application
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "EW2024_Photobooth.h"
#include "EW2024_Photobooth_Utils.h"

typedef struct
{
    char r, g, b;
    char *name;
} app_color_t;
app_color_t app_color[] = {
    {0x00, 0x33, 0x66, "#003366"},
    {0x33, 0x66, 0x99, "#336699"},
    {0x33, 0x66, 0xcc, "#3366cc"},
    {0x00, 0x33, 0x99, "#003399"},
    {0x00, 0x00, 0x99, "#000099"},
    {0x00, 0x00, 0xcc, "#0000cc"},
    {0x00, 0x00, 0x66, "#000066"},
    {0x00, 0x66, 0x66, "#006666"},
    {0x00, 0x66, 0x99, "#006699"},
    {0x00, 0x99, 0xcc, "#0099cc"},
    {0x00, 0x66, 0xcc, "#0066cc"},
    {0x00, 0x33, 0xcc, "#0033cc"},
    {0x00, 0x00, 0xff, "#0000ff"},
    {0x33, 0x33, 0xff, "#3333ff"},
    {0x33, 0x33, 0x99, "#333399"},
    {0x66, 0x99, 0x99, "#669999"},
    {0x00, 0x99, 0x99, "#009999"},
    {0x33, 0xcc, 0xcc, "#33cccc"},
    {0x00, 0xcc, 0xff, "#00ccff"},
    {0x00, 0x99, 0xff, "#0099ff"},
    {0x00, 0x66, 0xff, "#0066ff"},
    {0x33, 0x66, 0xff, "#3366ff"},
    {0x33, 0x33, 0xcc, "#3333cc"},
    {0x66, 0x66, 0x99, "#666699"},
    {0x33, 0x99, 0x66, "#339966"},
    {0x00, 0xcc, 0x99, "#00cc99"},
    {0x00, 0xff, 0xcc, "#00ffcc"},
    {0x00, 0xff, 0xff, "#00ffff"},
    {0x33, 0xcc, 0xff, "#33ccff"},
    {0x33, 0x99, 0xff, "#3399ff"},
    {0x66, 0x99, 0xff, "#6699ff"},
    {0x66, 0x66, 0xff, "#6666ff"},
    {0x66, 0x00, 0xff, "#6600ff"},
    {0x66, 0x00, 0xcc, "#6600cc"},
    {0x33, 0x99, 0x33, "#339933"},
    {0x00, 0xcc, 0x66, "#00cc66"},
    {0x00, 0xff, 0x99, "#00ff99"},
    {0x66, 0xff, 0xcc, "#66ffcc"},
    {0x66, 0xff, 0xff, "#66ffff"},
    {0x66, 0xcc, 0xff, "#66ccff"},
    {0x99, 0xcc, 0xff, "#99ccff"},
    {0x99, 0x99, 0xff, "#9999ff"},
    {0x99, 0x66, 0xff, "#9966ff"},
    {0x99, 0x33, 0xff, "#9933ff"},
    {0x99, 0x00, 0xff, "#9900ff"},
    {0x00, 0x66, 0x00, "#006600"},
    {0x00, 0xcc, 0x00, "#00cc00"},
    {0x00, 0xff, 0x00, "#00ff00"},
    {0x66, 0xff, 0x99, "#66ff99"},
    {0x99, 0xff, 0xcc, "#99ffcc"},
    {0xcc, 0xff, 0xff, "#ccffff"},
    {0xcc, 0xcc, 0xff, "#ccccff"},
    {0xcc, 0x99, 0xff, "#cc99ff"},
    {0xcc, 0x66, 0xff, "#cc66ff"},
    {0xcc, 0x33, 0xff, "#cc33ff"},
    {0xcc, 0x00, 0xff, "#cc00ff"},
    {0x99, 0x00, 0xcc, "#9900cc"},
    {0x00, 0x33, 0x00, "#003300"},
    {0x00, 0x99, 0x33, "#009933"},
    {0x33, 0xcc, 0x33, "#33cc33"},
    {0x66, 0xff, 0x66, "#66ff66"},
    {0x99, 0xff, 0x99, "#99ff99"},
    {0xcc, 0xff, 0xcc, "#ccffcc"},
    {0xff, 0xff, 0xff, "#ffffff"},
    {0xff, 0xcc, 0xff, "#ffccff"},
    {0xff, 0x99, 0xff, "#ff99ff"},
    {0xff, 0x66, 0xff, "#ff66ff"},
    {0xff, 0x00, 0xff, "#ff00ff"},
    {0xcc, 0x00, 0xcc, "#cc00cc"},
    {0x66, 0x00, 0x66, "#660066"},
    {0x33, 0x66, 0x00, "#336600"},
    {0x00, 0x99, 0x00, "#009900"},
    {0x66, 0xff, 0x33, "#66ff33"},
    {0x99, 0xff, 0x66, "#99ff66"},
    {0xcc, 0xff, 0x99, "#ccff99"},
    {0xff, 0xff, 0xcc, "#ffffcc"},
    {0xff, 0xcc, 0xcc, "#ffcccc"},
    {0xff, 0x99, 0xcc, "#ff99cc"},
    {0xff, 0x66, 0xcc, "#ff66cc"},
    {0xff, 0x33, 0xcc, "#ff33cc"},
    {0xcc, 0x00, 0x99, "#cc0099"},
    {0x99, 0x33, 0x99, "#993399"},
    {0x33, 0x33, 0x00, "#333300"},
    {0x66, 0x99, 0x00, "#669900"},
    {0x99, 0xff, 0x33, "#99ff33"},
    {0xcc, 0xff, 0x66, "#ccff66"},
    {0xff, 0xff, 0x99, "#ffff99"},
    {0xff, 0xcc, 0x99, "#ffcc99"},
    {0xff, 0x99, 0x99, "#ff9999"},
    {0xff, 0x66, 0x99, "#ff6699"},
    {0xff, 0x33, 0x99, "#ff3399"},
    {0xcc, 0x33, 0x99, "#cc3399"},
    {0x99, 0x00, 0x99, "#990099"},
    {0x66, 0x66, 0x33, "#666633"},
    {0x99, 0xcc, 0x00, "#99cc00"},
    {0xcc, 0xff, 0x33, "#ccff33"},
    {0xff, 0xff, 0x66, "#ffff66"},
    {0xff, 0xcc, 0x66, "#ffcc66"},
    {0xff, 0x99, 0x66, "#ff9966"},
    {0xff, 0x66, 0x66, "#ff6666"},
    {0xff, 0x00, 0x66, "#ff0066"},
    {0xcc, 0x66, 0x99, "#cc6699"},
    {0x99, 0x33, 0x66, "#993366"},
    {0x99, 0x99, 0x66, "#999966"},
    {0xcc, 0xcc, 0x00, "#cccc00"},
    {0xff, 0xff, 0x00, "#ffff00"},
    {0xff, 0xcc, 0x00, "#ffcc00"},
    {0xff, 0x99, 0x33, "#ff9933"},
    {0xff, 0x66, 0x00, "#ff6600"},
    {0xff, 0x50, 0x50, "#ff5050"},
    {0xcc, 0x00, 0x66, "#cc0066"},
    {0x66, 0x00, 0x33, "#660033"},
    {0x99, 0x66, 0x33, "#996633"},
    {0xcc, 0x99, 0x00, "#cc9900"},
    {0xff, 0x99, 0x00, "#ff9900"},
    {0xcc, 0x66, 0x00, "#cc6600"},
    {0xff, 0x33, 0x00, "#ff3300"},
    {0xff, 0x00, 0x00, "#ff0000"},
    {0xcc, 0x00, 0x00, "#cc0000"},
    {0x99, 0x00, 0x33, "#990033"},
    {0x66, 0x33, 0x00, "#663300"},
    {0x99, 0x66, 0x00, "#996600"},
    {0xcc, 0x33, 0x00, "#cc3300"},
    {0x99, 0x33, 0x00, "#993300"},
    {0x99, 0x00, 0x00, "#990000"},
    {0x80, 0x00, 0x00, "#800000"},
    {0x99, 0x33, 0x33, "#993333"},
};

#define font_english_46_L8 "assets_sdcard_eve/photobooth2024/font/font_english_46_L8.reloc"
#define font_english_47_L8 "assets_sdcard_eve/photobooth2024/font/font_english_47_L8.reloc"
#define font_english_48_L8 "assets_sdcard_eve/photobooth2024/font/font_english_48_L8.reloc"
#define font_english_49_L8 "assets_sdcard_eve/photobooth2024/font/font_english_49_L8.reloc"
#define font_english_50_L8 "assets_sdcard_eve/photobooth2024/font/font_english_50_L8.reloc"
#define font_english_51_L8 "assets_sdcard_eve/photobooth2024/font/font_english_51_L8.reloc"
#define font_english_52_L8 "assets_sdcard_eve/photobooth2024/font/font_english_52_L8.reloc"
#define font_english_53_L8 "assets_sdcard_eve/photobooth2024/font/font_english_53_L8.reloc"
#define font_english_54_L8 "assets_sdcard_eve/photobooth2024/font/font_english_54_L8.reloc"
#define font_english_55_L8 "assets_sdcard_eve/photobooth2024/font/font_english_55_L8.reloc"
#define font_english_60_L8 "assets_sdcard_eve/photobooth2024/font/font_english_60_L8.reloc"
#define font_english_65_L8 "assets_sdcard_eve/photobooth2024/font/font_english_65_L8.reloc"
#define font_english_70_L8 "assets_sdcard_eve/photobooth2024/font/font_english_70_L8.reloc"

static char *key_shift[] = {
    "QWERTYUIOP{}|", // 13
    "ASDFGHJKL:\"",  // 11
    "ZXCVBNM<>?",    // 10
};

static char *key_normal[] = {
    "qwertyuiop[]\\",
    "asdfghjkl;'",
    "zxcvbnm,./",
};
static char *key_special[] = {
    "~`!@#$%^&*()_",
    "-=[]\\;',./",
    "<>?:{}|\"  ",
};

extern uint32_t taskbar_icon_x;
extern uint32_t taskbar_icon_y;

static int tag_btn_showhide = 0, show_keyboard = 1;
static int tag_key_back = 0;
static int tag_key_tab = 0;
static int tag_key_shift = 0;
static int tag_key_space = 0;
static int tag_key_caps = 0;
static int tag_key_enter = 0;
static int tag_key_ctrl = 0;
static int tag_key_alt = 0;
static int tag_key_change = 0;
static int tag_key_color = 0;

#define MAX_LINE_LENGTH 100
static int8_t notepad[MAX_LINE_LENGTH] = {{0}};
int notepad_len = 0;

const int KEY_MULTIPLE = 0;
const int KEY_SINGLE = 1;
const int BUTTON = 2;

typedef struct
{
    char *fontfile;
    int fontsize;
    uint32_t ptr;
    int bitmap_handler;
} app_font_t;

app_font_t app_fonts[] = {
    {font_english_46_L8, 46, 0, 0},
    {font_english_47_L8, 47, 0, 0},
    {font_english_48_L8, 48, 0, 0},
    {font_english_49_L8, 49, 0, 0},
    {font_english_50_L8, 50, 0, 0},
    {font_english_51_L8, 51, 0, 0},
    {font_english_52_L8, 52, 0, 0},
    {font_english_53_L8, 53, 0, 0},
    {font_english_54_L8, 54, 0, 0},
    {font_english_55_L8, 55, 0, 0},
    {font_english_60_L8, 60, 0, 0},
    {font_english_65_L8, 65, 0, 0},
    {font_english_70_L8, 70, 0, 0},
};

#define app_fonts_count sizeof(app_fonts) / sizeof(app_font_t)

#define font_handle_keyboard 35
int font_handle_draw = 36;

static int font_key_id = 0;
static int font_draw_id = 4;

static const char *toprow = "~1234567890-+";
static char **keys_list[] = {
    key_normal,
    key_shift,
    key_special};
static char **keys = key_normal;
int pressed = 0, tag_special_mode = 0, tag_special_key = 0;
;

char txtFont[20] = "Font 36";
char *txtcolor = 0;

#define color_num sizeof(app_color) / sizeof(app_color_t)
int key_color = 0;

int loadXfont(int font_id, int bitmap_handler_offset)
{
    if (font_id >= app_fonts_count)
    {
        printf("Skip font id %d\n", font_id);
        return 0;
    }

    uint32_t result = 0;
    uint32_t handler = bitmap_handler_offset;

    app_font_t font = app_fonts[font_id];

    font.bitmap_handler = handler++;

    printf("Loading font %s\n", font.fontfile);
    cmd_fssource(PHOST, font.fontfile, &result);
    finish(PHOST);
    if (result != 0)
    {
        if (result == 2)
        {
            APP_INF("cmd_fssource error: file not found");
        }
        else if (result == 5)
        {
            APP_INF("cmd_fssource error: IO error");
        }
        else
        {
            APP_INF("cmd_fssource error: Unexpected error");
        }
        return 3;
    }
    cmd_loadasset(PHOST, font.ptr, OPT_FS);
    finish(PHOST);

    printf("Setfont handler = %d, at ptr= %d\n", font.bitmap_handler, font.ptr);
    cmd_setfont(PHOST, font.bitmap_handler, font.ptr, 32);
    finish(PHOST);
}

// Function to push a character onto the stack
void txtpush(char ch)
{
    if (notepad_len >= MAX_LINE_LENGTH - 1)
    {
        printf("Stack overflow, cannot push '%c'.\n", ch);
        return;
    }
    notepad[notepad_len] = ch;
    notepad_len++;
}

// Function to pop a character from the stack
char txtpop()
{
    if (notepad_len == -1)
    {
        printf("Stack underflow, cannot pop.\n");
        return '\0'; // Return null character if stack is empty
    }
    char ch = notepad[notepad_len];
    notepad[notepad_len] = 0;
    notepad_len--;
    return ch;
}

static int event()
{
    Gesture_Touch_t *ges = utils_gestureGet();
    pressed = 0;
    tag_special_key = ges->tagPressed;
    static char char_list[] = " ~1234567890-+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?qwertyuiop[]\\asdfghjkl;'zxcvbnm,./~`!@#$%^&*()_-=[]\\;',./<>?:{}|\"";
    for (int i = 0; i < sizeof(char_list); i++)
    {
        if (ges->tagPressed == char_list[i])
        {
            pressed = ges->tagPressed;
            break;
        }
    }

    if (ges->tagReleased == tag_btn_showhide)
    {
        show_keyboard = !show_keyboard;
    }

    if (ges->tagPressed1 == tag_key_shift)
    {
        static char **key_active = key_shift;
        if (keys == key_active)
        {
            tag_special_mode = 0;
            keys = key_normal;
        }
        else
        {
            tag_special_mode = ges->tagPressed1;
            keys = key_active;
        }
    }
    else if (ges->tagPressed1 == tag_key_ctrl)
    {
        static char **key_active = key_special;
        if (keys == key_active)
        {
            tag_special_mode = 0;
            keys = key_normal;
        }
        else
        {
            tag_special_mode = ges->tagPressed1;
            keys = key_active;
        }
    }
    else if (ges->tagPressed1 == tag_key_alt)
    {
        tag_special_mode = 0;
        memset(notepad, 0, sizeof(notepad));
        notepad_len = 0;
    }
    else if (ges->tagPressed1 == tag_key_tab)
    {
        tag_special_mode = 0;
    }
    else if (ges->tagPressed1 == tag_key_back)
    {
        txtpop();
    }
    else if (ges->tagPressed1 == tag_key_color)
    {
        key_color++;
        if (key_color >= color_num)
        {
            key_color = 0;
        }
        txtcolor = app_color[key_color].name;
    }
    else if (ges->tagPressed1 == tag_key_change)
    {
        font_handle_draw++;
        tag_special_mode = 0;
        if (font_handle_draw >= (35 + app_fonts_count))
        {
            font_handle_draw = 36;
        }

        snprintf(txtFont, sizeof(txtFont), "Font %d", font_handle_draw);
    }
    else if (ges->tagReleased > 0)
    {
        for (int i = 0; i < sizeof(char_list); i++)
        {
            if (ges->tagReleased == char_list[i])
            {
                txtpush(ges->tagReleased);
                break;
            }
        }
    }

    return PAGE_CONTINUE;
}

static int drawing_taskbar()
{
    int w = 350;
    int h = 70;
    int x = PHOST->Width / 2 - w / 2;

    int y = 1080 + (120 / 2 - h / 2);
    int font = 29;
    uint16_t options = 0;

    EVE_CoCmd_fgColor(PHOST, 0xFF8C1A);
    tag(PHOST, tag_btn_showhide);

    EVE_CoCmd_button(PHOST, x, y, w, h, font, options, show_keyboard ? "Hide keyboard" : "Show keyboard");
    EVE_CoCmd_fgColor(PHOST, 0x101010);

#define ENABLE_FONT_SIZE_ADJUSTMENT 0
#if ENABLE_FONT_SIZE_ADJUSTMENT
    box sliderbox = {taskbar_icon_x + 100, taskbar_icon_y, 500, 30};
    tag(PHOST, tag_font_size);
    cmd_track(PHOST, sliderbox.x, sliderbox.y, sliderbox.w, sliderbox.h, tag_font_size);

    char msg[100];
    snprintf(msg, 100, "Font size: %d", font_draw_id);
    cmd_text(PHOST, sliderbox.x + sliderbox.w / 2, sliderbox.y + sliderbox.h + 15, 29, OPT_CENTERX, msg);
    EVE_CoCmd_slider(PHOST, sliderbox.x, sliderbox.y, sliderbox.w, sliderbox.h, 0, slider_val, 65535);
#endif
    return 0;
}

static void drawNextKey(int8_t keyType, int16_t *x, int16_t y, int16_t w, int16_t h, int16_t font, uint16_t options, const int8_t *s, uint32_t tagval)
{
    uint8_t str[2];
    uint8_t *tobeDraw = s;

    str[0] = s[0];
    str[1] = 0;
    if (keyType == KEY_SINGLE)
    {
        tobeDraw = str;
    }

    if (keyType == BUTTON)
    {
        if (tag_special_mode == tagval || tag_special_key == tagval)
        {
            // button is pressed
            color_rgb(PHOST, 0x00, 0x00, 0xff);
        }
        EVE_Cmd_wr32(PHOST, TAG(tagval));
        EVE_CoCmd_button(PHOST, *x, y, w, h, font, options, tobeDraw);
        if (tag_special_mode == tagval || tag_special_key == tagval)
        {
            // button is pressed
            color_rgb(PHOST, 255, 255, 255);
        }
    }
    else
    {
        // KEY_SINGLE and KEY_MULTIPLE go here
        // Note: BT820 does not support keys with unicode
        EVE_CoCmd_keys(PHOST, *x, y, w, h, font, pressed, tobeDraw);
    }
    *x += (w + 3); // 3 is the gap
}
static int drawing_keyboard()
{
    const int LEN_ROW1 = 13; // "~1234567890-+"
    const int LEN_ROW2 = 13; // "QWERTYUIOP{}|",//13
    const int LEN_ROW3 = 11; // "ASDFGHJKL:\"",//11
    const int LEN_ROW4 = 10; // "ZXCVBNM<>?",//10

    int key_w = 90, key_h = 70, key_col_num = 15, key_gap = 2;
    box keyboard;
    keyboard.w = LEN_ROW1 * (key_w) + (key_w + key_gap) * 3 / 2; // "~1234567890-+" and tab key
    keyboard.h = (key_h + key_gap) * 5;
    keyboard.x = (PHOST->Width - keyboard.w) / 2;
    keyboard.y = (PHOST->Height - key_h) / 2;
    int x = keyboard.x, y = keyboard.y;
    int font_specials = font_handle_keyboard;

    color_rgb(PHOST, 0x99, 0x99, 0x99);
    int kborder = 10;
    utils_drawRect(keyboard.x - kborder, keyboard.y - kborder, keyboard.w + kborder * 2, keyboard.h + kborder * 2);
    color_rgb(PHOST, 255, 255, 255);

    // styling
    EVE_Cmd_wr32(PHOST, TAG_MASK(1)); // enable tagbuffer updation
    EVE_CoCmd_fgColor(PHOST, 0x703800);
    EVE_CoCmd_bgColor(PHOST, 0x703800);

    // first row
    drawNextKey(KEY_MULTIPLE, &x, y, key_w * LEN_ROW1, key_h, font_handle_keyboard, 0, toprow, 0);
    drawNextKey(BUTTON, &x, y, key_w * 3 / 2, key_h, font_specials, 0, "<-", tag_key_back);

    // second row
    x = keyboard.x;
    y += key_h + key_gap;
    drawNextKey(BUTTON, &x, y, key_w * 3 / 2, key_h, font_specials, 0, "Tab", tag_key_tab);
    drawNextKey(KEY_MULTIPLE, &x, y, key_w * LEN_ROW2, key_h, font_handle_keyboard, 0, keys[0], 0);

    // third row
    x = keyboard.x;
    y += key_h + key_gap;
    drawNextKey(BUTTON, &x, y, key_w * 3 / 2, key_h, font_specials, 0, "Caps", tag_key_shift);
    drawNextKey(KEY_MULTIPLE, &x, y, key_w * LEN_ROW3, key_h, font_handle_keyboard, 0, keys[1], 0);
    drawNextKey(BUTTON, &x, y, key_w * 2, key_h, font_specials, 0, "Enter", tag_key_enter);

    // fourth row
    x = keyboard.x;
    y += key_h + key_gap;
    drawNextKey(BUTTON, &x, y, key_w * 2, key_h, font_specials, 0, "Shift", tag_key_shift);
    drawNextKey(KEY_MULTIPLE, &x, y, key_w * LEN_ROW4, key_h, font_handle_keyboard, 0, keys[2], 0);
    drawNextKey(BUTTON, &x, y, key_w * 5 / 2, key_h, font_specials, 0, "Shift", tag_key_shift);

    // fifth row - display list may overflow
    x = keyboard.x;
    y += key_h + key_gap;
    drawNextKey(BUTTON, &x, y, key_w * 2 - 2, key_h, font_specials, 0, "Ctrl", tag_key_ctrl);
    drawNextKey(BUTTON, &x, y, key_w, key_h, font_specials, 0, "Cls", tag_key_alt);
    drawNextKey(BUTTON, &x, y, key_w * 6 + key_w / 2, key_h, 26, 0, "Space", ' ');
    EVE_Cmd_wr32(PHOST, COLOR_RGB(app_color[key_color].r, app_color[key_color].g, app_color[key_color].b));
    drawNextKey(BUTTON, &x, y, key_w * 3, key_h, font_specials, 0, txtcolor, tag_key_color);
    color_rgb(PHOST, 255, 255, 255);
    drawNextKey(BUTTON, &x, y, key_w * 2 - 2, key_h, font_specials, 0, txtFont, tag_key_change);
}

box textbox = {0, 400, 1920 * 2 / 3, 120};
static int counter = 0;
const int timeval = 10;
static int drawing_text()
{
    int y = 50;

    EVE_Cmd_wr32(PHOST, COLOR_RGB(200, 200, 200));
    if (show_keyboard)
        utils_drawBox(&textbox);

    EVE_Cmd_wr32(PHOST, SCISSOR_XY(textbox.x, textbox.y));
    EVE_Cmd_wr32(PHOST, SCISSOR_SIZE(textbox.w, textbox.h));
    if (notepad[0] != 0)
    {
        EVE_Cmd_wr32(PHOST, COLOR_RGB(app_color[key_color].r, app_color[key_color].g, app_color[key_color].b));
        tag(PHOST, 0);
        EVE_CoCmd_text(PHOST, textbox.x + textbox.w - 2, textbox.y, font_handle_draw, OPT_RIGHTX, notepad);
    }

    EVE_Cmd_wr32(PHOST, SCISSOR_XY(0, 0));
    EVE_Cmd_wr32(PHOST, SCISSOR_SIZE(2048, 2048));

    if (counter > timeval)
    {
        EVE_Cmd_wr32(PHOST, COLOR_RGB(0, 0, 255));
        EVE_CoCmd_text(PHOST, textbox.x + textbox.w - 1, textbox.y, 30, 0, "|");
    }
    EVE_Cmd_wr32(PHOST, COLOR_RGB(255, 255, 255));
}

static int drawing()
{
    if (show_keyboard)
        drawing_keyboard();
    drawing_text();
    drawing_taskbar();

    return 0;
}

static void page_keyboard_init_one()
{
    static bool is_page_keyboard_init = 0;
    if (is_page_keyboard_init == 1)
        return;
    is_page_keyboard_init = 1;

    txtcolor = app_color[key_color].name;

    // Clear Linebuffer
    memset(notepad, 0, sizeof(notepad));

    textbox.x = PHOST->Width / 2 - textbox.w / 2;

    tag_btn_showhide = utils_tagAlloc(TAG_INC);
    tag_key_back = utils_tagAlloc(TAG_INC);
    tag_key_tab = utils_tagAlloc(TAG_INC);
    tag_key_shift = utils_tagAlloc(TAG_INC);
    tag_key_space = utils_tagAlloc(TAG_INC);
    tag_key_caps = utils_tagAlloc(TAG_INC);
    tag_key_enter = utils_tagAlloc(TAG_INC);
    tag_key_ctrl = utils_tagAlloc(TAG_INC);
    tag_key_alt = utils_tagAlloc(TAG_INC);
    tag_key_change = utils_tagAlloc(TAG_INC);
    tag_key_color = utils_tagAlloc(TAG_INC);

    const int font_byte_num = 32768 * 128;
    int startfont = font_handle_keyboard;
    for (int i = 0; i < app_fonts_count; i++)
    {
        app_font_t *font = &app_fonts[i];
        font->ptr = utils_ddrAllocAlignment(font_byte_num, 32);
        loadXfont(i, startfont);
        startfont++;
    }
}

static void page_keyboard_deinit_one()
{
    static bool is_page_keyboard_deinit = 0;
    if (is_page_keyboard_deinit == 1)
        return;
    is_page_keyboard_deinit = 1;
}

void page_keyboard_load()
{
}

int page_keyboard_draw()
{
    int ret = event();
    drawing();
    return ret;
}

void page_keyboard_init()
{
    page_keyboard_init_one();
    cmd_romfont(PHOST, 32, 32);
    cmd_romfont(PHOST, 33, 33);
    cmd_romfont(PHOST, 34, 34);
}

void page_keyboard_deinit()
{
    page_keyboard_deinit_one();
}
