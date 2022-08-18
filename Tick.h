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
#ifndef TICK_H
#define TICK_H

#include <stdbool.h>
#include <stdint.h>

#define TICKS_PER_SECOND 1000U
#define TICK_SECOND	((unsigned long long)TICKS_PER_SECOND)

#ifdef	__cplusplus
extern "C" {
#endif
    
    /**
	 * @brief Increment tick
     * 
	 * @return bool True if successful
	 */
    void tick_init();
    
    /**
	 * @brief Increment tick
     * 
	 * @return bool True if successful
	 */
    bool tick_inc();
    
    /**
	 * @brief Set tick
     * 
     * @param uint32_t tick
     *
	 * @return bool True if successful
	 */
    bool tick_set(uint32_t tick);

    /**
	 * @brief Get current tick
     *
	 * @return uint32_t tick
	 */
	uint32_t tick_get();

#ifdef	__cplusplus
}
#endif

#endif
/* [] END OF FILE */
