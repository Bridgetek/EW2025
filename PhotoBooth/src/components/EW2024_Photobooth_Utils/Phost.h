/**
 * @file Phost.h
 * @brief EVE phost handle management functions
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#ifndef PHOST_H_
#define PHOST_H_

#include "EW2024_Photobooth_Utils.h"

#define ENABLE_PHOST_EXTERN 1

void utils_hostRemove(EVE_HalContext *phost);
void utils_hostAdd(EVE_HalContext *phost);
void utils_hostInit(EVE_HalContext *phost);
void utils_hostDeinit();
EVE_HalContext *utils_hostGet(int index);

#if ENABLE_PHOST_EXTERN
// fast accessing without a variable guard
#define MAX_HOST 10
extern EVE_HalContext g_HalContext;
extern EVE_HalContext *g_pHalContext;
extern EVE_HalContext *g_pHalContexts[MAX_HOST];
#define PHOST g_pHalContext // reduce a function call compare to utils_hostGet(0)
#define PHOST1 g_pHalContexts[1]
#define PHOST2 g_pHalContexts[2]
#define PHOST3 g_pHalContexts[3]
#define PHOST4 g_pHalContexts[4]
#define PHOST5 g_pHalContexts[5]
#define PHOST6 g_pHalContexts[6]
#define PHOST7 g_pHalContexts[7]
#define PHOST8 g_pHalContexts[8]
#define PHOST9 g_pHalContexts[9]

#else
// slow accessing with a variable guard
#define PHOST utils_hostGet(0)
#define PHOST1 utils_hostGet(1)
#define PHOST2 utils_hostGet(2)
#define PHOST3 utils_hostGet(3)
#define PHOST4 utils_hostGet(4)
#define PHOST5 utils_hostGet(5)
#define PHOST6 utils_hostGet(6)
#define PHOST7 utils_hostGet(7)
#define PHOST8 utils_hostGet(8)
#define PHOST9 utils_hostGet(9)

#endif // ENABLE_PHOST_EXTERN

#endif // PHOST_H_