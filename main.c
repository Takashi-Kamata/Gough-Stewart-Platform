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
#define ERROR_MULTIPLIER 1000
#define MAX_SPEED 0
#define MIN_SPEED 180
#define N_PNTS 10

void set_speed(uint8_t M, uint8_t speed);
void extend(uint8_t M);
void retract(uint8_t M);
void stop(uint8_t M);
void make_points();
void reset();
int32_t PID(int32_t[3], int32_t*, int32_t[3], uint8_t);
int32_t target[6] = {MAX_ADC / 2, MAX_ADC / 2, MAX_ADC / 2, MAX_ADC / 2, MAX_ADC / 2, MAX_ADC / 2};
int32_t points[6][N_PNTS];

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
            for (int i = 0; i<6; i++) {
                target[i] = MIN_ADC + (MAX_ADC - MIN_ADC)/6*i;   
            }
            make_points();
            reset();
        } else { // Initial Run.... when button = false;
            LED_Write(0);   
            for (int i = 0; i<6; i++) {
                target[i] = MIN_ADC;
            }
            make_points();
            reset();
        }
        button = !button;
    }

    InputPin_ClearInterrupt();
}

int32_t output = 0;
    
int32_t PID_output[6] ={0};
int32_t integral[6] = {0};
int32_t errors[6][3] = {0};
int32_t params[3] = {1, 500, 1};

uint8_t counter = 0;


int32_t next_target[6] = {0};
int32_t next_target_flag[6] = {0};
int32_t next_point = 0;

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

    ADC_DelSig_1_StartConvert();
    CyDelay(1000);
    
    // Initialise ADC register, first read is always bad
    for (int i = 0; i<6; i++)
    {
        AMux_Select(i);
        CyDelay(2);// ~1 ms is the min time required for amux to swtich, using 2 ms for safety
        uint32_t temp = ADC_DelSig_1_GetResult32();
    }
    CyDelay(1000);
    
    
    
    
    printf("%s", CLEAR_STRING);
    printf("%s", MOVE_CURSOR);
    printf("\r");
    
    printf("Retracting...\r\n");
    /*
    for(int i = 0; i < 3; i++)
    {
        for (int j=0; j<6; j++)
        {
            AMux_FastSelect(j);
            CyDelay(2);// Important! minimum 1ms required for amux to catchup
            output = ADC_DelSig_1_GetResult32();
            errors[j][i] = (int32_t) ((target[j] - output) * ERROR_MULTIPLIER / MAX_ADC_DIFF);
            printf("%d %d \r\n", j, output);
            PID_output[j] = PID(errors[j], integral[j], params, i);
        }
    }
    
    
    for(int i = 0; i < 3; i++)
    {
        for (int j = 0; j<6; j++)
        {
            printf("%d %d %d  %d\r\n", i, j, errors[j][i], 1);
        }
    }
    */
    
    

    for(;;)
    {       
        /* pid */ 
        for (int i = 0; i<6; i++) {
            AMux_Select(i);
            CyDelay(2);
            output = ADC_DelSig_1_GetResult32();
            
            //errors[i][counter] = ((target[i] - output) * ERROR_MULTIPLIER) / MAX_ADC_DIFF;
            errors[i][counter] = ((points[i][next_point] - output) * ERROR_MULTIPLIER) / MAX_ADC_DIFF;

            PID_output[i] = PID(errors[i], &integral[i], params, counter);
            
            if (abs(PID_output[i]) > ERROR_MULTIPLIER) {
                set_speed(i, MAX_SPEED);
            } else {
                set_speed(i, (uint8_t) (MAX_SPEED + ((MIN_SPEED - MAX_SPEED) * (ERROR_MULTIPLIER - abs(PID_output[i]))) / ERROR_MULTIPLIER));
            }
            
            if ((errors[i][counter]) * 20 > ERROR_MULTIPLIER) {
                extend(i);
            } else if ((errors[i][counter]) * 20 < -ERROR_MULTIPLIER) {
                retract(i);
            } else {
                stop(i); // Stop the motor
                next_target_flag[i] = 1;
                /*
                if (next_target[i] < N_PNTS - 1) // check if the current target is less than 9 (final)
                {
                    next_target[i]++;
                } else if (next_target_flag[i] == 0){
                    next_target_flag[i] = 1;
                    printf("M%d is at the final dest. Point : %d \r\n", i, next_target[i]);
                } else {
                }
                */
                bool all_check = true;
                for (int j = 0; j < 6; j++)
                {
                    if (next_target_flag[j] != 1)
                    {
                        all_check = false;
                        break;
                    }
                }
                if (all_check && next_point < N_PNTS - 1) 
                {   
                    next_point++;
                    memset(next_target_flag, 0, sizeof next_target_flag); 
                    printf("All motors reached point, next point is %d \r\n", next_point);
                }
            }
        }
        //printf("%d %d %d \r\n", errors[0][counter], PID_output[0], MIN_SPEED * (ERROR_MULTIPLIER - abs(PID_output[0])) / ERROR_MULTIPLIER);
        counter = (counter + 1) % 3;
        CyDelay(10);
    }
}

void set_speed(uint8_t M, uint8_t speed) {
    switch(M) {
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
       default :
        break;
    }
}

void extend(uint8_t M) {
    switch(M) {
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
       default :
        break;
    }
}

void retract(uint8_t M) {
    switch(M) {
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
       default :
        break;
    }
}

void stop(uint8_t M) {
    switch(M) {
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
       default :
        break;
    }
}

int32_t PID(int32_t errors[3], int32_t *integral, int32_t params[3], uint8_t counter) {
    // P = errors[counter] * params[0]
    // I = integral + errors[counter] * params[1]
    // D = (errors[counter] - errors[(counter + 1) % 3]) / 2 * params[2]
    *integral += errors[counter] / params[1];
    //printf("%d\r\n", *integral);
    return errors[counter] / params[0] + (*integral) + (errors[counter] - errors[(counter + 1) % 3]) / 2 / params[2];
}

void make_points() {
    printf("Make Points\r\n");
    int32_t temp;
    for (int i=0; i < 6; i++) {
        AMux_Select(i);
        CyDelay(2);
        temp = ADC_DelSig_1_GetResult32();
        for (int j = 0; j < N_PNTS; j++) {
            points[i][j] = temp + (target[i] - temp) / N_PNTS * (j+1);
            // printf("%d %d %d \r\n", i, j, points[i][j]);
        }
    }
}

void reset() {
    memset(PID_output, 0, sizeof PID_output); 
    memset(integral, 0, sizeof integral); 
    memset(errors, 0, sizeof errors); 
    memset(params, 0, sizeof params); 
    counter = 0;
    memset(next_target, 0, sizeof next_target); 
    next_point = 0;
    memset(next_target_flag, 0, sizeof next_target_flag); 
}

/* [] END OF FILE */
