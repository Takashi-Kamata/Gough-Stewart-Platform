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
#include "UART_Com.h"

/*** Private Definitions ***/

#define CLEAR_STRING "\033[2J"
#define MOVE_CURSOR "\033[0;0H"
#define MOTOR_NUM 6U
#define MIN_PWM 50U
#define MAX_PWM 380U
#define SYSTICK_RELOAD 24000U // when 0.01s is target1, reload val is 240000, since 0.01s / (1s/24MHz)
#define TOLERANCE 7 // PID Tolerance
#define POINTS 10 // Number of Points
#define SAMPLE_NUM 800
#define SPEED 0U
#define ROD 12U //inch

#define KP 0.05
#define KI 0.0
#define KD 0.0

/*** Calibrated Values ***/

uint16_t min_adc[MOTOR_NUM] = {6342,6464,6464,6317,6392,6383};
uint16_t max_adc[MOTOR_NUM] = {48182,48182,48186,48182,48183,48181};

/*** Private Prototypes ***/

bool set_speed(uint8_t M, uint16_t speed);
bool extend(uint8_t M);
bool retract(uint8_t M);
bool stop(uint8_t M);
void reset_pid();
float inch_adc(uint8_t m, float inch);
void flip(uint8_t m, bool* flag, uint16_t speed);

/*** Private Variables ***/
// PID
struct pid_controller ctrldata[MOTOR_NUM];
pids_t pid[MOTOR_NUM];
float input[MOTOR_NUM] = {0.0};
float output[MOTOR_NUM] = {0.0};
float setpoint[MOTOR_NUM] = {0};
static float target1[MOTOR_NUM][64] = {0};
static float target2[MOTOR_NUM][64] = {0};

float kp[MOTOR_NUM] = {0.0};
float ki[MOTOR_NUM] = {0.0};
float kd[MOTOR_NUM] = {0.0};

bool stopped[MOTOR_NUM] = {0};
uint16_t max_point1 = 0;
uint16_t max_point2 = 0;
// For Keyboard Lock
bool manual = true;
bool wave = false;
uint32_t counter = 0;
uint16_t point = 0;
uint8_t wave_target = 1;

// Switches
bool manual_m1 = true;
bool manual_m2 = true;
bool manual_m3 = true;
bool manual_m4 = true;
bool manual_m5 = true;
bool manual_m6 = true;

//buffer to hold application settings
typedef struct TParamBuffer{
    float A, B, C, D, E, F;//some other integer params
} ParamBuffer; //settings
volatile ParamBuffer PB;     //volatile struct TParamBuffer PB;//else

char strMsg1[64];//output UART buffer

/**
 * UART ISR
 */

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

uint32_t data_received[MOTOR_NUM] = {0};

void ProcessCommandMsg(void)
{    
    switch (RB.cmd)
    {
    case 'A': // For Real-time Motion
        if (strlen(RB.valstr) > 0) PB.A = atoi(RB.valstr) / 100.0;
        break;
    case 'B':
        if (strlen(RB.valstr) > 0) PB.B = atoi(RB.valstr) / 100.0;
        break;
    case 'C':
        if (strlen(RB.valstr) > 0) PB.C = atoi(RB.valstr) / 100.0;
        break;
    case 'D':
        if (strlen(RB.valstr) > 0) PB.D = atoi(RB.valstr) / 100.0;
        break;
    case 'E':
        if (strlen(RB.valstr) > 0) PB.E = atoi(RB.valstr) / 100.0;
        break;
    case 'F':
        if (strlen(RB.valstr) > 0) PB.F = atoi(RB.valstr) / 100.0;
        break;
    case 'G': // Common Commands
        manual = true;
        for (uint8_t i = 0; i < MOTOR_NUM; i++) 
        {
            stop(i);
            set_speed(i, MAX_PWM);
        }
        printf("STOP ALL\r\n");
        break;
    case 'H':
        manual = true;
        for (uint8_t i = 0; i < MOTOR_NUM; i++) 
        {
            set_speed(i, SPEED);
            extend(i);
        }
        printf("EXTEND ALL\r\n");
        break;
    case 'I':
        manual = true;
        for (uint8_t i = 0; i < MOTOR_NUM; i++) 
        {
            set_speed(i, SPEED);
            retract(i);
        }
        printf("RETRACT ALL\r\n");
        break;
    case 'J': // Auto Mode
        setpoint[0] = inch_adc(0, PB.A);
        setpoint[1] = inch_adc(1, PB.B);
        setpoint[2] = inch_adc(2, PB.C);
        setpoint[3] = inch_adc(3, PB.D);
        setpoint[4] = inch_adc(4, PB.E);
        setpoint[5] = inch_adc(5, PB.F);
        reset_pid();
        manual = false;
        printf("MOVING\r\n");
        break;
    case 'K': // Manual Mode + Stop
        manual = true;
        for (uint8_t i = 0; i < MOTOR_NUM; i++) 
        {
            set_speed(i, MAX_PWM);
            stop(i);
        }
        printf("MANUAL\r\n");
        printf("STOP ALL\r\n");
        break;
    case 'L':
        manual = true;
        printf("MANUAL\r\n");
        break;
    case 'M':
        for (uint8_t i = 0; i < MOTOR_NUM; i++) 
        {
            data_received[i] = 0;
        }
        printf("M\r\n");
        break;
        // For Wave Motion 
    case 'N':
        if (wave_target == 1) {
            max_point1 = atoi(RB.valstr);
        } else if (wave_target == 2) {
            max_point2 = atoi(RB.valstr);
        }
        for (uint8_t i = 0; i < MOTOR_NUM; i++) 
        {
            data_received[i] = 0;
        }
        point = 0;
        printf("N\r\n");
        break;
    case 'O':
        PB.A = atoi(RB.valstr) / 100.0;//set new value, else report old 
        if (wave_target == 1) {
            target1[0][data_received[0]] = inch_adc(0, atoi(RB.valstr) / 100.0);
        } else if (wave_target == 2) {
            target2[0][data_received[0]] = inch_adc(0, atoi(RB.valstr) / 100.0);
        }
        data_received[0]++;
        printf("O\r\n");
        break;
    case 'P':
        PB.B = atoi(RB.valstr) / 100.0;//set new value, else report old 
        if (wave_target == 1) {
            target1[1][data_received[1]] = inch_adc(1, atoi(RB.valstr) / 100.0);
        } else if (wave_target == 2) {
            target2[1][data_received[1]] = inch_adc(1, atoi(RB.valstr) / 100.0);
        }
        data_received[1]++;
        printf("O\r\n");
        break;
    case 'Q':
        PB.C = atoi(RB.valstr) / 100.0;//set new value, else report old
        if (wave_target == 1) {
            target1[2][data_received[2]] = inch_adc(2, atoi(RB.valstr) / 100.0);
        } else if (wave_target == 2) {
            target2[2][data_received[2]] = inch_adc(2, atoi(RB.valstr) / 100.0);
        }
        data_received[2]++;
        printf("O\r\n");
        break;
    case 'R':
        PB.D = atoi(RB.valstr) / 100.0;//set new value, else report old 
        if (wave_target == 1) {
            target1[3][data_received[3]] = inch_adc(3, atoi(RB.valstr) / 100.0);
        } else if (wave_target == 2) {
            target2[3][data_received[3]] = inch_adc(3, atoi(RB.valstr) / 100.0);
        }
        data_received[3]++;
        printf("O\r\n");
        break;
    case 'S':
        PB.E = atoi(RB.valstr) / 100.0;//set new value, else report old 
        if (wave_target == 1) {
            target1[4][data_received[4]] = inch_adc(4, atoi(RB.valstr) / 100.0);
        } else if (wave_target == 2) {
            target2[4][data_received[4]] = inch_adc(4, atoi(RB.valstr) / 100.0);
        }
        data_received[4]++;
        printf("O\r\n");
        break;
    case 'T':
        PB.F = atoi(RB.valstr) / 100.0;//set new value, else report old 
        if (wave_target == 1) {
            target1[5][data_received[5]] = inch_adc(5, atoi(RB.valstr) / 100.0);
        } else if (wave_target == 2) {
            target2[5][data_received[5]] = inch_adc(5, atoi(RB.valstr) / 100.0);
        }
        data_received[5]++;
        printf("O\r\n");
        break;
    case 'U':
        wave_target = 1;
        wave = true;
        point = 0;
        setpoint[0] = target1[0][point];
        setpoint[1] = target1[1][point];
        setpoint[2] = target1[2][point];
        setpoint[3] = target1[3][point];
        setpoint[4] = target1[4][point];
        setpoint[5] = target1[5][point];
        break;
    case 'V':
        wave_target = 2;
        wave = true;
        point = 0;
        setpoint[0] = target2[0][point];
        setpoint[1] = target2[1][point];
        setpoint[2] = target2[2][point];
        setpoint[3] = target2[3][point];
        setpoint[4] = target2[4][point];
        setpoint[5] = target2[5][point];
        break;
    case 'W':
        wave = false;
        point = 0;
        break;
    default:
        printf("UNKNOWN\r\n");
        break;
    }
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
    
    /**
     * Initialise PID
     */
    reset_pid();
    
    for (uint8_t i = 0; i<MOTOR_NUM; i++)
    {
        setpoint[i] = inch_adc(i, 6.0);
        pid[i] = pid_create(&ctrldata[i], &input[i], &output[i], &setpoint[i], kp[i], ki[i], kd[i], - (float)MAX_PWM, (float)MAX_PWM);
    	// Allow PID to compute and change output
    	pid_auto(pid[i]);
    }
    
    CyDelay(500); // rest
    
    /**
     * Initialise Tick
     */
    tick_init();
    
    
    /**
     * Loop
     */
    for (;;)
    {
        if(IsCharReady()) {
            if (GetRxStr()) {
                ProcessCommandMsg();
            }
        }
        
            
        if (!manual) {
            // Check if need to compute PID
            for (uint8_t i = 0; i < MOTOR_NUM; i++)
            {
                AMux_Select(i);
                CyDelay(1); // ~1 ms is the min time required for amux to swtich, using 2 ms for safety
                uint32_t temp_adc = ADC_DelSig_1_GetResult32();
                if (pid_need_compute(pid[i])) {
                    // Read ADC
        			input[i] = temp_adc;
        			// Compute new PID output value
        			pid_compute(pid[i]);
        		} else {
                    printf("Sampling Too Fast!! Adjust delay.\r");  
                }  
                uint16_t new_speed = MAX_PWM - abs((int)output[i]);
                if ((int)output[i] > 0)
                {
                    extend(i);
                } else if ((int)output[i] < 0) {
                    retract(i);
                } 
                //printf("Set Speed %d \r\n", new_speed);
                set_speed(i, new_speed);
                
                if (new_speed > (MAX_PWM - 200))
                {
                    stopped[i] = true;
                } else {
                    stopped[i] = false;
                }
            }
            
            /*
             * Wave - Step through points
             */
            if (wave)
            {
               
                bool send = false;
                for (uint8_t i = 0; i < MOTOR_NUM; i++)
                {
                    if (stopped[i] == true)
                    {
                        send = true;
                        stopped[i] = false;
                        break;
                    }
                }
                
                if (send == true)
                {
                    if (wave_target == 1) {
                        LED_Write(1);
                        setpoint[0] = target1[0][point];
                        setpoint[1] = target1[1][point];
                        setpoint[2] = target1[2][point];
                        setpoint[3] = target1[3][point];
                        setpoint[4] = target1[4][point];
                        setpoint[5] = target1[5][point];
                        if (point == max_point1-1)
                        {
                            point = 0;
                        } else {
                            point++;
                        }
                    } else if (wave_target == 2) {
                        LED_Write(0);
                        setpoint[0] = target2[0][point];
                        setpoint[1] = target2[1][point];
                        setpoint[2] = target2[2][point];
                        setpoint[3] = target2[3][point];
                        setpoint[4] = target2[4][point];
                        setpoint[5] = target2[5][point];
                        if (point == max_point2-1)
                        {
                            point = 0;
                        } else {
                            point++;
                        }
                    }

                    reset_pid();   
                    for (uint8_t i = 0; i < MOTOR_NUM; i++)
                    {
                        pid_compute(pid[i]);
                    }
                }
            }
        }
        CyDelay(100);
        
    }
        
}

/*** Private Functions ***/

/**
 * @brief Set motor speed by changing PWM value
 *
 * @param uint8_t Motor number 0-5
 * @param uint8_t Speed between MIN_PWM & MAX_PWM
 *
 * @return bool True if successful
 */
bool set_speed(uint8_t M, uint16_t speed)
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
 * @brief Reset PID settings for new destination
 */
void reset_pid() {
    for (uint8_t i = 0; i < MOTOR_NUM; i++) {
        input[i] = 0.0;
        output[i] = 0.0;
        kp[i] = KP;
        ki[i] = KI;
        kd[i] = KD;
    }
}

/**
 * @brief Convert destination inch to a calibrated ADC value 
 */
float inch_adc(uint8_t m, float inch) {
    // Get specific leg's min & max ADC vals
    uint16_t min = min_adc[m];// as 0  inch
    uint16_t max = max_adc[m];// as 12 inch
    
    return (float)(max-min) * inch/((float)ROD) + (float)min;
}

void flip(uint8_t m, bool* flag, uint16_t speed) {
    set_speed(m, speed);
    if (*flag == true) {
        extend(m);
    } else {
        retract(m);   
    }
    *flag = !(*flag);
}

/* [] END OF FILE */
