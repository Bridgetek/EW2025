/**
 * @file Phost.c
 * @brief Provides utility functions for the EVE display host interface
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include "EW2024_Photobooth_Utils.h"

#define MAX_HOST 10 // number of EVE connected to the host (host = PC/mcu board)
static uint8_t host_count = 0;
EVE_HalContext g_HalContext;
EVE_HalContext *g_pHalContext;
EVE_HalContext *g_pHalContexts[MAX_HOST];

void utils_hostAdd(EVE_HalContext *phost)
{
    g_pHalContexts[host_count++] = phost;
}

void utils_hostRemove(EVE_HalContext *phost)
{
    for (int i = 0; i < host_count; i++)
    {
        if (g_pHalContexts[i] == phost)
        {
            g_pHalContexts[i] = NULL;
            host_count--;
        }
        if (g_pHalContext == phost)
        {
            g_pHalContext = NULL;
        }
    }
}

void utils_hostInit(EVE_HalContext *phost)
{
    g_pHalContext = phost;
    g_pHalContexts[0] = phost;
    host_count = 1;
}
void utils_hostDeinit()
{
    g_pHalContext = NULL;
    host_count = 0;
}

EVE_HalContext *utils_hostGet(int index)
{
    if (index >= host_count)
    {
        return NULL;
    }
    return g_pHalContexts[index];
}
