/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "PID.h"

/*** Private Prototypes ***/
static bool mInit = false;
static uint32_t mTick;

void tick_init() {
    if (!mInit) {
        mInit = true;
        mTick = 0;
    }
}

bool tick_inc() {
    if (!mInit) {
        return false;   
    }
    mTick++;
    return true;
}

bool tick_set(uint32_t tick) {
    if (!mInit) {
        return false;   
    }
    mTick = tick;
    return true;
}

uint32_t tick_get() {
    if (!mInit) {
        return false;   
    }
    return mTick;
}

/* [] END OF FILE */
