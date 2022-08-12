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

#define CLEAR_STRING "\033[2J"
#define MOVE_CURSOR "\033[0;0H"
#define SAMPLE_NUM 10000
#define MAX_ADC 57500
#define MIN_ADC 7000
#define MAX_ADC_DIFF 50500
#define ERROR_MULTIPLIER 100
#define MAX_SPEED 0
#define MIN_SPEED 255
void extend();
void retract();
void stop();
int16_t PID(int16_t[3], int16_t, int16_t[3], uint8_t);

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

bool button = false;
CY_ISR(SWPin_Control)
{
    if (InputPin_Read() == 1u)
    {
        if (button) {
            LED_Write(1);        
        } else {
            LED_Write(0);   
        }
        
        button = !button;
    }

    InputPin_ClearInterrupt();
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_Start(); /* Start communication component */
    AMux_Start();
    ADC_DelSig_1_Start();
    
    isr_rx_StartEx(RxIsr);
    InputInterrupt_StartEx(SWPin_Control);
    
    PWM3_Start();

    PWM3_WriteCompare1(127);// M5 speed
    
    AMux_Select(4);
    ADC_DelSig_1_StartConvert();
    
    int32_t target = MAX_ADC / 2;
    int32_t output = 0;
    
    int16_t PID_output = 0;
    int16_t integral = 0;
    int16_t errors[3] = {0};
    int16_t params[3] = {1, 500, 1};
    
    uint8_t counter = 0;
    uint8_t speed = 0;
    
    printf("%s", CLEAR_STRING);
    printf("%s", MOVE_CURSOR);
    printf("\r");
    
    printf("Retracting...");
    retract();
    while(button);
    
    
    for(int i = 0; i < 3; i++)
    {
        output = ADC_DelSig_1_GetResult32();
        // rio's code
        errors[counter] = (int16_t) ((target - output) * ERROR_MULTIPLIER / MAX_ADC_DIFF);
        counter = (counter + 1) % 3;
        
        CyDelay(100);
    }
    
    for(;;)
    {
        output = ADC_DelSig_1_GetResult32();
        // rio's code
        errors[counter] = (int16_t) (((target - output) * ERROR_MULTIPLIER) / MAX_ADC_DIFF);
        
        printf("%s", CLEAR_STRING);
        printf("%s", MOVE_CURSOR);
        printf("output %li \r\n", output);
        printf("Error %d \r\n", errors[counter]);
        printf("PID %d \r\n", PID_output);
        printf("speed %d \r\n", speed);
        
        integral += errors[counter];
        PID_output = PID(errors, integral, params, counter);
        
        
        if (abs(PID_output) > ERROR_MULTIPLIER) {
            speed = MAX_SPEED;
        } else {
            speed = (uint8_t) (((int16_t) MIN_SPEED * (ERROR_MULTIPLIER - PID_output)) / ERROR_MULTIPLIER);
        }
        
        PWM3_WriteCompare1(speed);
        
        if (errors[counter] > (ERROR_MULTIPLIER / 200)) {
            extend();
        } else if (errors[counter] < -(ERROR_MULTIPLIER / 200)) {
            retract();
        } else {
            stop();
        }
        
        counter = (counter + 1) % 3;
        CyDelay(200);
    }
}

void extend() {
    M5_IN1_Write(1U);
    M5_IN2_Write(0U);
}

void retract() {
    M5_IN1_Write(0U);
    M5_IN2_Write(1U);
}

void stop() {
    M5_IN1_Write(0U);
    M5_IN2_Write(0U);
}

int16_t PID(int16_t errors[3], int16_t integral, int16_t params[3], uint8_t counter) {
    // P = errors[counter] * params[0]
    // I = integral + errors[counter] * params[1]
    // D = (errors[counter] - errors[(counter + 1) % 3]) / 2 * params[2]
    return errors[counter] / params[0] + integral / params[1] + (errors[counter] - errors[(counter + 1) % 3]) / 2 / params[2];
}

/* [] END OF FILE */
