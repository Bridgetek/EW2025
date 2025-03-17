/**
 * @file Paging.c
 * @brief Paging module for handling page transitions and logic.
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include "Paging.h"

void utils_pageInit(utils_paging *p) {}

void utils_pageRun1(utils_paging *p)
{
    if (p->isActive)
    {
        if (!p->isInitialized)
        {
            p->isInitialized = true;
            p->init();
        }
        if (PAGE_FINISH == p->draw())
        {
            p->isActive = false;
            p->isInitialized = false;
            p->deinit();

            if (p->nextPage)
                p->nextPage->isActive = true;
        }
    }
}
void utils_pageLoad(utils_paging **pages, int count)
{
    for (int i = 0; i < count; i++)
    {
        utils_paging *p = pages[i];
        if (!p)
            continue;

        if (p->load)
        {
            p->load();
        }
    }
}
void utils_pageRun(utils_paging **pages, int count)
{
    for (int i = 0; i < count; i++)
    {
        utils_paging *p = pages[i];
        if (!p){
            continue;
        }
        utils_pageRun1(p);
    }
}
