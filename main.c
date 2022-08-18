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
#if defined(__GNUC__)
asm(".global _printf_float");
#endif

#include "project.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "PID.h"

/*** Private Definitions ***/

#define CLEAR_STRING "\033[2J"
#define MOVE_CURSOR "\033[0;0H"
#define MOTOR_NUM 6U
#define MAX_ADC 57500U
#define MIN_ADC 7000U // 0.5V when fully retracted
#define MAX_SPEED 0U
#define MIN_SPEED 180U
#define SYSTICK_RELOAD 24000U // when 0.01s is target, reload val is 240000, since 0.01s / (1s/24MHz)
#define TOLERANCE 10 // ADC Tolerance

/*** Private Prototypes ***/

bool set_speed(uint8_t M, uint8_t speed);
bool extend(uint8_t M);
bool retract(uint8_t M);
bool stop(uint8_t M);

/*** Private Variables ***/
struct pid_controller ctrldata;
pids_t pid;

float input = 0, output = 0;
float setpoint = 30000.0;

float kp = 0.01, ki =  0.0, kd =  0.0005;

/**
 * UART ISR
 */
uint8 errorStatus = 0u;
CY_ISR(RxIsr)
{
    uint8 rxStatus;
    uint8 rxData;
    do
    {
        /* Read receiver status register */
        rxStatus = UART_RXSTATUS_REG;

        if ((rxStatus & (UART_RX_STS_BREAK | UART_RX_STS_PAR_ERROR |
                         UART_RX_STS_STOP_ERROR | UART_RX_STS_OVERRUN)) != 0u)
        {
            /* ERROR handling. */
            errorStatus |= rxStatus & (UART_RX_STS_BREAK | UART_RX_STS_PAR_ERROR |
                                       UART_RX_STS_STOP_ERROR | UART_RX_STS_OVERRUN);
        }

        if ((rxStatus & UART_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read data from the RX data register */
            rxData = UART_RXDATA_REG;
            if (errorStatus == 0u)
            {
                /*
                UART_TXDATA_REG = rxData;

                printf("11\r\n");
                */
            }
        }
    } while ((rxStatus & UART_RX_STS_FIFO_NOTEMPTY) != 0u);
}

/**
 * Button ISR
 */
bool button = false;
CY_ISR(SWPin_Control)
{
    if (InputPin_Read() == 1u)
    {
        if (button)
        {
            LED_Write(1);
            retract(1);
        }
        else
        { // Initial Run.... when button = false;
            LED_Write(0);
            extend(1);
        }
        set_speed(1, 0);
        button = !button;
    }

    InputPin_ClearInterrupt();
}

/**
 * SysTick ISR
 */
CY_ISR(isr_systick)
{
    tick_inc();
}

int main(void)
{ 
    CyGlobalIntEnable; /* Enable global interrupts. */

    /**
     * UART, AMUX & ADC Start
     */
    UART_Start();
    AMux_Start();
    ADC_DelSig_1_Start();

    /**
     * ISR Start UART & Button
     */
    isr_rx_StartEx(RxIsr);
    InputInterrupt_StartEx(SWPin_Control);

    /**
     * SysTick Setting, ISR & Start
     */
    CySysTickEnable();
    CySysTickStart();                     /* Start the systick */
    CySysTickSetReload(SYSTICK_RELOAD);   /* Make sure start() is called */
    CySysTickSetCallback(0, isr_systick); /* Add the Systick callback */
    CySysTickClear();

    /**
     * PWM Modules Start
     */
    PWM1_Start();
    PWM2_Start();
    PWM3_Start();

    /**
     * ADC Initialisation
     */
    ADC_DelSig_1_StartConvert();
    CyDelay(500);
    for (uint8_t i = 0; i < MOTOR_NUM; i++) // Initialise ADC register, first read is always bad
    {
        AMux_Select(i);
        CyDelay(2); // ~1 ms is the min time required for amux to swtich, using 2 ms for safety
        (void)ADC_DelSig_1_GetResult32();
    }
    CyDelay(500); // rest
    
    /**
     * Initialise Tick
     */
    tick_init();
    
    /**
     * Clear UART Console & Start...
     */
    printf("%s", CLEAR_STRING);
    printf("%s", MOVE_CURSOR);
    printf("Starting...\r\n");
    
    /**
     * Initialise PID
     */

    
    pid = pid_create(&ctrldata, &input, &output, &setpoint, kp, ki, kd);
	// Set controler output limits from 0 to 200
	pid_limits(pid, -255, 255);
	// Allow PID to compute and change output
	pid_auto(pid);
    // Reverse Direction
    //pid_direction(pid, E_PID_REVERSE);
    
    retract(0); // Start retracted
    /**
     * Loop
     */
    uint32_t counter = 0;
    for (;;)
    {
        printf("%s", CLEAR_STRING);
        printf("%s", MOVE_CURSOR);
        uint32_t sec = tick_get() / 1000;
        printf("Run Time (s) : %d\r\n", sec);
        printf("Current Tick (ms) : %d\r\n", tick_get());
        
        AMux_Select(1);
        CyDelay(2); // ~1 ms is the min time required for amux to swtich, using 2 ms for safety
        uint32_t M1_ADC = ADC_DelSig_1_GetResult32();
        //printf("M1 ADC %d \r\n", M1_ADC);
        
        // Check if need to compute PID
		if (pid_need_compute(pid)) {
			// Read process feedback
            // Read ADC
			input = M1_ADC;
			// Compute new PID output value
			pid_compute(pid);
            
			//Change actuator value
            // Set PWM
			//set_speed(0, output);
		} else {
            printf("Sampling Too Fast!! Change PID setting.\r\n");  
        }
        
        printf("Output %i \r\n", (int)output);
        if (button) {
            uint8_t new_speed = 255 - abs((int)output);
            
            if (((int)output + TOLERANCE) > 0)
            {
                extend(1);
            } else if (((int)output - TOLERANCE) < 0) {
                retract(1);
            } else {
                stop(1);
                new_speed = MIN_SPEED;
            }
            set_speed(1, new_speed);
        }
        CyDelay(100); // rest
    }
}

/*** Private Functions ***/

/**
 * @brief Set motor speed by changing PWM value
 *
 * @param uint8_t Motor number 0-5
 * @param uint8_t Speed between MAX_SPEED & MIN_SPEED
 *
 * @return bool True if successful
 */
bool set_speed(uint8_t M, uint8_t speed)
{
    switch (M)
    {
    case 0:
        PWM1_WriteCompare1(speed);
        break;
    case 1:
        PWM1_WriteCompare2(speed);
        break;
    case 2:
        PWM2_WriteCompare1(speed);
        break;
    case 3:
        PWM2_WriteCompare2(speed);
        break;
    case 4:
        PWM3_WriteCompare1(speed);
        break;
    case 5:
        PWM3_WriteCompare2(speed);
        break;
    default:
        return false;
        break;
    }
    return true;
}

/**
 * @brief Extend actuator by configuring direction pins
 *
 * @param uint8_t Motor number 0-5
 *
 * @return bool True if successful
 */
bool extend(uint8_t M)
{
    switch (M)
    {
    case 0:
        M1_IN1_Write(1U);
        M1_IN2_Write(0U);
        break;
    case 1:
        M2_IN1_Write(1U);
        M2_IN2_Write(0U);
        break;
    case 2:
        M3_IN1_Write(1U);
        M3_IN2_Write(0U);
        break;
    case 3:
        M4_IN1_Write(1U);
        M4_IN2_Write(0U);
        break;
    case 4:
        M5_IN1_Write(1U);
        M5_IN2_Write(0U);
        break;
    case 5:
        M6_IN1_Write(1U);
        M6_IN2_Write(0U);
        break;
    default:
        return false;
        break;
    }
    return true;
}

/**
 * @brief Retract actuator by configuring direction pins
 *
 * @param uint8_t Motor number 0-5
 *
 * @return bool True if successful
 */
bool retract(uint8_t M)
{
    switch (M)
    {
    case 0:
        M1_IN1_Write(0U);
        M1_IN2_Write(1U);
        break;
    case 1:
        M2_IN1_Write(0U);
        M2_IN2_Write(1U);
        break;
    case 2:
        M3_IN1_Write(0U);
        M3_IN2_Write(1U);
        break;
    case 3:
        M4_IN1_Write(0U);
        M4_IN2_Write(1U);
        break;
    case 4:
        M5_IN1_Write(0U);
        M5_IN2_Write(1U);
        break;
    case 5:
        M6_IN1_Write(0U);
        M6_IN2_Write(1U);
        break;
    default:
        return false;
        break;
    }
    return true;
}

/**
 * @brief Stop actuator by configuring direction pins
 *
 * @param uint8_t Motor number 0-5
 *
 * @return bool True if successful
 */
bool stop(uint8_t M)
{
    switch (M)
    {
    case 0:
        M1_IN1_Write(0U);
        M1_IN2_Write(0U);
        break;
    case 1:
        M2_IN1_Write(0U);
        M2_IN2_Write(0U);
        break;
    case 2:
        M3_IN1_Write(0U);
        M3_IN2_Write(0U);
        break;
    case 3:
        M4_IN1_Write(0U);
        M4_IN2_Write(0U);
        break;
    case 4:
        M5_IN1_Write(0U);
        M5_IN2_Write(0U);
        break;
    case 5:
        M6_IN1_Write(0U);
        M6_IN2_Write(0U);
        break;
    default:
        return false;
        break;
    }
    return true;
}

/* [] END OF FILE */
