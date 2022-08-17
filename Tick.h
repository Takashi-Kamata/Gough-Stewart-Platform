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
    
#define TICK_SECOND 1

#ifdef	__cplusplus
extern "C" {
#endif

    /**
	 * @brief Enables automatic control using PID
	 *
	 * Enables the PID control loop. If manual output adjustment is needed you can
	 * disable the PID control loop using pid_manual(). This function enables PID
	 * automatic control at program start or after calling pid_manual()
	 *
	 * @param pid The PID controller instance to enable
	 */
	uint32_t get_tick();

#ifdef	__cplusplus
}
#endif

#endif
/* [] END OF FILE */
