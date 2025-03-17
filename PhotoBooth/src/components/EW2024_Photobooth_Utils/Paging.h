/**
 * @file Paging.h
 * @brief Page management utility for the application
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#ifndef EVEUTILS_PAGING_H_
#define EVEUTILS_PAGING_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "EW2024_Photobooth_Utils.h"

#define PAGE_FINISH 1
#define PAGE_CONTINUE 0

#define PAGE(x, ui_name)             \
    void page_##x##_load();          \
    void page_##x##_init();          \
    void page_##x##_deinit();        \
    int page_##x##_draw();           \
    utils_paging page_##x = {        \
        .name = "page_" #x,          \
        .uiname = ui_name,           \
        .load = page_##x##_load,     \
        .init = page_##x##_init,     \
        .deinit = page_##x##_deinit, \
        .draw = page_##x##_draw,     \
        .isActive = 0,               \
        .isInitialized = 0,          \
        .nextPage = 0};

typedef struct utils_paging_t
{
    char name[100];
    char uiname[100];
    int (*load)(void); // load before application start
    void (*init)(void);
    void (*deinit)(void);
    int (*draw)(void); // return non 0 to finish
    bool isActive;
    bool isInitialized;
    struct utils_paging_t *nextPage;
} utils_paging;

void utils_pageInit();
void utils_pageLoad(utils_paging **pages, int count);
void utils_pageRun(utils_paging **pages, int count);
void utils_pageRun1(utils_paging *p);

#endif // EVEUTILS_PAGING_H_