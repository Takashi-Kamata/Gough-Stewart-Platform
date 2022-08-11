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

void extend();
void retract();
void stop();

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

bool button = true;
CY_ISR(SWPin_Control)
{
    if (InputPin_Read() == 1u)
    {
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

    PWM3_WriteCompare1(125);// M5
    
    ADC_DelSig_1_StartConvert();
    
    uint32_t output = 0;
    
    for(;;)
    {
        /* Place your application code here. */
        printf("%s", CLEAR_STRING);
        printf("%s", MOVE_CURSOR);
        if (button) {
            LED_Write(1);   
            extend();               
        } else {
            LED_Write(0);   
            retract();
        }
        
        AMux_Select(4);
        while(ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_RETURN_STATUS) == 0){
            printf("Converting MUX 4 \r\n");
        };
        output = ADC_DelSig_1_GetResult16();
        printf("Current Reading Motor 5: %d \r\n", output);
        
        
        
        
        
        CyDelay(100);
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

/* [] END OF FILE */
