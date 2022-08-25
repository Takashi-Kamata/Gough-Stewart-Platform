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
#define MAX_SPEED 0
#define MIN_SPEED 180
#define SYSTICK_RELOAD 24000U // when 0.01s is target, reload val is 240000, since 0.01s / (1s/24MHz)
#define TOLERANCE 30 // ADC Tolerance
#define POINTS 10 // Number of Points

/*** Private Prototypes ***/

bool set_speed(uint8_t M, uint8_t speed);
bool extend(uint8_t M);
bool retract(uint8_t M);
bool stop(uint8_t M);
void make_1d_points(float* points, float setpoint, uint32_t adc);

/*** Private Variables ***/

// PID
struct pid_controller ctrldata[MOTOR_NUM];
pids_t pid[MOTOR_NUM];
float input[MOTOR_NUM] = {0.0};
float output[MOTOR_NUM] = {0.0};
float setpoint[MOTOR_NUM] = {10000.0, 20000.0, 30000.0, 40000.0, 50000.0, 60000.0};
float points[MOTOR_NUM][POINTS] = {{0.00}};

float kp[MOTOR_NUM] = {0.005, 0.005, 0.005, 0.005, 0.005, 0.005};
float ki[MOTOR_NUM] = {0.01, 0.01, 0.001, 0.01, 0.01, 0.01};
float kd[MOTOR_NUM] = {0.0008, 0.0008, 0.0008, 0.0008, 0.0008, 0.0008};

// For Keyboard Lock
bool manual = false;
bool all;
uint8_t counter = 1;
uint8_t at_point[MOTOR_NUM] = {0};

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
                
                //UART_TXDATA_REG = rxData; // echo back by directly changing TX register
                //printf("%d\r\n",rxData);
                if (rxData == 65) {// UP
                    for (uint8_t i = 0; i< MOTOR_NUM; i++)
                    {
                        set_speed(i, 127);
                        extend(i);
                    }
                    manual = true;
                } else if (rxData == 66) {// DOWN
                    for (uint8_t i = 0; i< MOTOR_NUM; i++)
                    {
                        set_speed(i, 127);
                        retract(i);
                    }
                    manual = true;
                } else if (rxData == 67) {// RIGHT
                } else if (rxData == 68) {// LEFT
                } else if (rxData == 13) {// ENTER
                    manual = false;   
                    counter = 0;
                    for (uint8_t i = 0; i<MOTOR_NUM; i++)
                    {
                        setpoint[i] = points[i][counter]; 
                        pid_reset(pid[i]);
                        at_point[i] = 0;
                    }
                    counter++;
                }
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
        }
        else
        { // Initial Run.... when button = false;
            LED_Write(0);
        }
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
    for (uint8_t i = 0; i<MOTOR_NUM; i++)
    {
        pid[i] = pid_create(&ctrldata[i], &input[i], &output[i], &setpoint[i], kp[i], ki[i], kd[i]);
        // Set controler output limits from 0 to 200
    	pid_limits(pid[i], -MIN_SPEED, MIN_SPEED);
    	// Allow PID to compute and change output
    	pid_auto(pid[i]);
    }
    
    /**
     * Initialise Points
     */
    all = false;
    counter = 1;
    for (uint8_t i = 0; i<MOTOR_NUM; i++)
    {
        at_point[i] = 0;
    }
    
    for (uint8_t i = 0; i<MOTOR_NUM; i++)
    {
        AMux_Select(i);
        CyDelay(2); // ~1 ms is the min time required for amux to swtich, using 2 ms for safety
        uint32_t temp = ADC_DelSig_1_GetResult32();
        (void)make_1d_points(points[i], setpoint[i], temp);  
        setpoint[i] = points[i][0];// first point
    }
    
    /**
     * Loop
     */
    for (;;)
    {
        uint32_t sec = tick_get() / 1000;
        
        printf("%s", CLEAR_STRING);
        printf("%s", MOVE_CURSOR);
        printf("Run Time (s) : %d\r\n", sec);
        printf("Current Tick (ms) : %d\r\n", tick_get());
        printf("\r\n");
        
        printf("Mode is : %s\r\n", (manual == 0) ? "Auto" : "Manual");
        printf("Press Enter to leave Manual mode...\r\n");
        printf("\r\n");
        
        if (!manual) {
            //printf("Auto mode output...\r\n");
            
            // Check if need to compute PID
            for (uint8_t i = 0; i < MOTOR_NUM; i++)
            {
                AMux_Select(i);
                CyDelay(2); // ~1 ms is the min time required for amux to switch, using 2 ms for safety
                uint32_t temp_adc = ADC_DelSig_1_GetResult32();
                if (pid_need_compute(pid[i])) {
                    // Read ADC
        			input[i] = temp_adc;
        			// Compute new PID output value
        			pid_compute(pid[i]);
        		} else {
                    printf("Sampling Too Fast!! Adjust delay.\r\n");  
                }  
                if (button) {
                    uint8_t new_speed = 255 - abs((int)output[i]);
                    //printf("Output %d\r\n", (int)output[i]);
                    if ((int)output[i] > TOLERANCE)
                    {
                        extend(i);
                        set_speed(i, new_speed);
                    } else if ((int)output[i] < -TOLERANCE) {
                        retract(i);
                        set_speed(i, new_speed);
                    } else {  
                        stop(i);
                        new_speed = MIN_SPEED;
                        at_point[i] = 1;
                    }
                }
               // printf("OUTPUT %d %d \r\n", i, (int)output[i]);
            }
            
            all = true;
                
            for (uint8_t q = 0; q<MOTOR_NUM; q++)
            {
                if (at_point[q] == 0) {all = false;}
            }
            
            if (all && (counter < POINTS - 1))
            {
                for (uint8_t w = 0; w<MOTOR_NUM; w++)
                {
                    setpoint[w] = points[w][counter]; 
                    pid_reset(pid[w]);
                    at_point[w] = 0;
                }
                
                counter++;
            }
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

/**
 * @brief Divide setpoints by number of points from current position
 *
 * @param float* pointer to an array
 * @param float target point
 * @param uint16_t current position
 *
 * @return bool True if successful
 */
void make_1d_points(float* points, float setpoint, uint32_t adc) {
    for (uint16_t i = 0; i < POINTS; i++) {
        points[i] = (float)adc + (float)((setpoint - (float)adc) / POINTS * (i+1));
    }
}
/* [] END OF FILE */
