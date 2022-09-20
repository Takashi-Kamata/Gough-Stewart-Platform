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
#define MOTOR_NUM 6U
#define SAMPLE_NUM 10000
#define TARGET 5U
#define MAX_SPEED 399U

bool set_speed(uint8_t M, uint8_t speed);
bool extend(uint8_t M);
bool retract(uint8_t M);
bool stop(uint8_t M);

static uint16_t buffer[SAMPLE_NUM];
uint16_t output = 0;
uint32_t counter = 0;
bool measured = false;

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
                if (rxData == 65) {// UP
                    for (uint8_t i = 0; i< MOTOR_NUM; i++)
                    {
                        set_speed(i, MAX_SPEED/2);
                        extend(i);
                    }
                } else if (rxData == 66) {// DOWN
                    for (uint8_t i = 0; i< MOTOR_NUM; i++)
                    {
                        set_speed(i, MAX_SPEED/2);
                        retract(i);
                    }
                }else if (rxData == 13) // Enter
                {
                    counter = 0;
                    for (int i=0; i<SAMPLE_NUM; i++)
                    {
                        printf("%d, %d\r\n", i, buffer[i]);
                        buffer[i] = 0;
                    }   
                } else if (rxData == 114)
                {
                }
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
            for (int i=0; i<6; i++)
            {
                extend(i);
            }
        } else {
            for(int i=0; i<6; i++)
            {
                retract(i);
            }
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
    
    PWM1_Start();
    PWM2_Start();
    PWM3_Start();

    set_speed(TARGET, MAX_SPEED/2);// 50%
    set_speed(TARGET-1, MAX_SPEED/2);// 50%
    
    AMux_Select(TARGET);// Select motor
    
    ADC_DelSig_1_StartConvert();
    
    
    for(int i=0; i<6; i++)
    {
        retract(i);
    }
    while(button);
    
    printf("%s", CLEAR_STRING);
    printf("%s", MOVE_CURSOR);
    printf("Start... \r\n");
    LED_Write(0);
    
    for(;;)
    {
        output = ADC_DelSig_1_GetResult16();
        
        if (output >= 20000 && counter<SAMPLE_NUM)
        {
            // measure into buffer
            buffer[counter] = output;
            counter++;
        }        
        if (counter == SAMPLE_NUM)
        {
            LED_Write(1); 
        }
        /*
        if (counter == SAMPLE_NUM && measured == false) 
        {
            printf("Stopped measuring at %d, counter : %d \r\n", output, counter);
            measured = true;
            for (int i=0; i<SAMPLE_NUM; i++)
            {
                printf("%d, %d\r\n", i, buffer[i]);
            }
            printf("Finished measuring at %d, counter : %d \r\n", output, counter);

        }
        */
        /*
        if (output >= 40030 && measured == false)
        {
            measured = true;
            for (int i=0; i<SAMPLE_NUM; i++)
            {
                printf("%d, %d\r\n", i, buffer[i]);
            }
            printf("Finished measuring at %d, counter : %d \r\n", output, counter);
        }
        */
        
        CyDelayUs(100);
    }
}

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
