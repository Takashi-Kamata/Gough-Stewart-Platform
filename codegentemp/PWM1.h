/*******************************************************************************
* File Name: PWM1.h
* Version 3.30
*
* Description:
*  Contains the prototypes and constants for the functions available to the
*  PWM user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PWM_PWM1_H)
#define CY_PWM_PWM1_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 PWM1_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define PWM1_Resolution                     (16u)
#define PWM1_UsingFixedFunction             (0u)
#define PWM1_DeadBandMode                   (0u)
#define PWM1_KillModeMinTime                (0u)
#define PWM1_KillMode                       (0u)
#define PWM1_PWMMode                        (1u)
#define PWM1_PWMModeIsCenterAligned         (0u)
#define PWM1_DeadBandUsed                   (0u)
#define PWM1_DeadBand2_4                    (0u)

#if !defined(PWM1_PWMUDB_genblk8_stsreg__REMOVED)
    #define PWM1_UseStatus                  (0u)
#else
    #define PWM1_UseStatus                  (0u)
#endif /* !defined(PWM1_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(PWM1_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define PWM1_UseControl                 (1u)
#else
    #define PWM1_UseControl                 (0u)
#endif /* !defined(PWM1_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define PWM1_UseOneCompareMode              (0u)
#define PWM1_MinimumKillTime                (1u)
#define PWM1_EnableMode                     (0u)

#define PWM1_CompareMode1SW                 (0u)
#define PWM1_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define PWM1__B_PWM__DISABLED 0
#define PWM1__B_PWM__ASYNCHRONOUS 1
#define PWM1__B_PWM__SINGLECYCLE 2
#define PWM1__B_PWM__LATCHED 3
#define PWM1__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define PWM1__B_PWM__DBMDISABLED 0
#define PWM1__B_PWM__DBM_2_4_CLOCKS 1
#define PWM1__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define PWM1__B_PWM__ONE_OUTPUT 0
#define PWM1__B_PWM__TWO_OUTPUTS 1
#define PWM1__B_PWM__DUAL_EDGE 2
#define PWM1__B_PWM__CENTER_ALIGN 3
#define PWM1__B_PWM__DITHER 5
#define PWM1__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define PWM1__B_PWM__LESS_THAN 1
#define PWM1__B_PWM__LESS_THAN_OR_EQUAL 2
#define PWM1__B_PWM__GREATER_THAN 3
#define PWM1__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define PWM1__B_PWM__EQUAL 0
#define PWM1__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!PWM1_UsingFixedFunction)
        uint16 PWMUdb;               /* PWM Current Counter value  */
        #if(!PWM1_PWMModeIsCenterAligned)
            uint16 PWMPeriod;
        #endif /* (!PWM1_PWMModeIsCenterAligned) */
        #if (PWM1_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (PWM1_UseStatus) */

        /* Backup for Deadband parameters */
        #if(PWM1_DeadBandMode == PWM1__B_PWM__DBM_256_CLOCKS || \
            PWM1_DeadBandMode == PWM1__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(PWM1_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (PWM1_KillModeMinTime) */

        /* Backup control register */
        #if(PWM1_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (PWM1_UseControl) */

    #endif /* (!PWM1_UsingFixedFunction) */

}PWM1_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    PWM1_Start(void) ;
void    PWM1_Stop(void) ;

#if (PWM1_UseStatus || PWM1_UsingFixedFunction)
    void  PWM1_SetInterruptMode(uint8 interruptMode) ;
    uint8 PWM1_ReadStatusRegister(void) ;
#endif /* (PWM1_UseStatus || PWM1_UsingFixedFunction) */

#define PWM1_GetInterruptSource() PWM1_ReadStatusRegister()

#if (PWM1_UseControl)
    uint8 PWM1_ReadControlRegister(void) ;
    void  PWM1_WriteControlRegister(uint8 control)
          ;
#endif /* (PWM1_UseControl) */

#if (PWM1_UseOneCompareMode)
   #if (PWM1_CompareMode1SW)
       void    PWM1_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (PWM1_CompareMode1SW) */
#else
    #if (PWM1_CompareMode1SW)
        void    PWM1_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (PWM1_CompareMode1SW) */
    #if (PWM1_CompareMode2SW)
        void    PWM1_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (PWM1_CompareMode2SW) */
#endif /* (PWM1_UseOneCompareMode) */

#if (!PWM1_UsingFixedFunction)
    uint16   PWM1_ReadCounter(void) ;
    uint16 PWM1_ReadCapture(void) ;

    #if (PWM1_UseStatus)
            void PWM1_ClearFIFO(void) ;
    #endif /* (PWM1_UseStatus) */

    void    PWM1_WriteCounter(uint16 counter)
            ;
#endif /* (!PWM1_UsingFixedFunction) */

void    PWM1_WritePeriod(uint16 period)
        ;
uint16 PWM1_ReadPeriod(void) ;

#if (PWM1_UseOneCompareMode)
    void    PWM1_WriteCompare(uint16 compare)
            ;
    uint16 PWM1_ReadCompare(void) ;
#else
    void    PWM1_WriteCompare1(uint16 compare)
            ;
    uint16 PWM1_ReadCompare1(void) ;
    void    PWM1_WriteCompare2(uint16 compare)
            ;
    uint16 PWM1_ReadCompare2(void) ;
#endif /* (PWM1_UseOneCompareMode) */


#if (PWM1_DeadBandUsed)
    void    PWM1_WriteDeadTime(uint8 deadtime) ;
    uint8   PWM1_ReadDeadTime(void) ;
#endif /* (PWM1_DeadBandUsed) */

#if ( PWM1_KillModeMinTime)
    void PWM1_WriteKillTime(uint8 killtime) ;
    uint8 PWM1_ReadKillTime(void) ;
#endif /* ( PWM1_KillModeMinTime) */

void PWM1_Init(void) ;
void PWM1_Enable(void) ;
void PWM1_Sleep(void) ;
void PWM1_Wakeup(void) ;
void PWM1_SaveConfig(void) ;
void PWM1_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define PWM1_INIT_PERIOD_VALUE          (399u)
#define PWM1_INIT_COMPARE_VALUE1        (199u)
#define PWM1_INIT_COMPARE_VALUE2        (199u)
#define PWM1_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    PWM1_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWM1_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWM1_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWM1_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define PWM1_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  PWM1_CTRL_CMPMODE2_SHIFT)
#define PWM1_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  PWM1_CTRL_CMPMODE1_SHIFT)
#define PWM1_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (PWM1_UsingFixedFunction)
   #define PWM1_PERIOD_LSB              (*(reg16 *) PWM1_PWMHW__PER0)
   #define PWM1_PERIOD_LSB_PTR          ( (reg16 *) PWM1_PWMHW__PER0)
   #define PWM1_COMPARE1_LSB            (*(reg16 *) PWM1_PWMHW__CNT_CMP0)
   #define PWM1_COMPARE1_LSB_PTR        ( (reg16 *) PWM1_PWMHW__CNT_CMP0)
   #define PWM1_COMPARE2_LSB            (0x00u)
   #define PWM1_COMPARE2_LSB_PTR        (0x00u)
   #define PWM1_COUNTER_LSB             (*(reg16 *) PWM1_PWMHW__CNT_CMP0)
   #define PWM1_COUNTER_LSB_PTR         ( (reg16 *) PWM1_PWMHW__CNT_CMP0)
   #define PWM1_CAPTURE_LSB             (*(reg16 *) PWM1_PWMHW__CAP0)
   #define PWM1_CAPTURE_LSB_PTR         ( (reg16 *) PWM1_PWMHW__CAP0)
   #define PWM1_RT1                     (*(reg8 *)  PWM1_PWMHW__RT1)
   #define PWM1_RT1_PTR                 ( (reg8 *)  PWM1_PWMHW__RT1)

#else
   #if (PWM1_Resolution == 8u) /* 8bit - PWM */

       #if(PWM1_PWMModeIsCenterAligned)
           #define PWM1_PERIOD_LSB      (*(reg8 *)  PWM1_PWMUDB_sP16_pwmdp_u0__D1_REG)
           #define PWM1_PERIOD_LSB_PTR  ((reg8 *)   PWM1_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #else
           #define PWM1_PERIOD_LSB      (*(reg8 *)  PWM1_PWMUDB_sP16_pwmdp_u0__F0_REG)
           #define PWM1_PERIOD_LSB_PTR  ((reg8 *)   PWM1_PWMUDB_sP16_pwmdp_u0__F0_REG)
       #endif /* (PWM1_PWMModeIsCenterAligned) */

       #define PWM1_COMPARE1_LSB        (*(reg8 *)  PWM1_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define PWM1_COMPARE1_LSB_PTR    ((reg8 *)   PWM1_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define PWM1_COMPARE2_LSB        (*(reg8 *)  PWM1_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define PWM1_COMPARE2_LSB_PTR    ((reg8 *)   PWM1_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define PWM1_COUNTERCAP_LSB      (*(reg8 *)  PWM1_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define PWM1_COUNTERCAP_LSB_PTR  ((reg8 *)   PWM1_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define PWM1_COUNTER_LSB         (*(reg8 *)  PWM1_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define PWM1_COUNTER_LSB_PTR     ((reg8 *)   PWM1_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define PWM1_CAPTURE_LSB         (*(reg8 *)  PWM1_PWMUDB_sP16_pwmdp_u0__F1_REG)
       #define PWM1_CAPTURE_LSB_PTR     ((reg8 *)   PWM1_PWMUDB_sP16_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(PWM1_PWMModeIsCenterAligned)
               #define PWM1_PERIOD_LSB      (*(reg16 *) PWM1_PWMUDB_sP16_pwmdp_u0__D1_REG)
               #define PWM1_PERIOD_LSB_PTR  ((reg16 *)  PWM1_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #else
               #define PWM1_PERIOD_LSB      (*(reg16 *) PWM1_PWMUDB_sP16_pwmdp_u0__F0_REG)
               #define PWM1_PERIOD_LSB_PTR  ((reg16 *)  PWM1_PWMUDB_sP16_pwmdp_u0__F0_REG)
            #endif /* (PWM1_PWMModeIsCenterAligned) */

            #define PWM1_COMPARE1_LSB       (*(reg16 *) PWM1_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define PWM1_COMPARE1_LSB_PTR   ((reg16 *)  PWM1_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define PWM1_COMPARE2_LSB       (*(reg16 *) PWM1_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define PWM1_COMPARE2_LSB_PTR   ((reg16 *)  PWM1_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define PWM1_COUNTERCAP_LSB     (*(reg16 *) PWM1_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define PWM1_COUNTERCAP_LSB_PTR ((reg16 *)  PWM1_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define PWM1_COUNTER_LSB        (*(reg16 *) PWM1_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define PWM1_COUNTER_LSB_PTR    ((reg16 *)  PWM1_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define PWM1_CAPTURE_LSB        (*(reg16 *) PWM1_PWMUDB_sP16_pwmdp_u0__F1_REG)
            #define PWM1_CAPTURE_LSB_PTR    ((reg16 *)  PWM1_PWMUDB_sP16_pwmdp_u0__F1_REG)
        #else
            #if(PWM1_PWMModeIsCenterAligned)
               #define PWM1_PERIOD_LSB      (*(reg16 *) PWM1_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
               #define PWM1_PERIOD_LSB_PTR  ((reg16 *)  PWM1_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #else
               #define PWM1_PERIOD_LSB      (*(reg16 *) PWM1_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
               #define PWM1_PERIOD_LSB_PTR  ((reg16 *)  PWM1_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
            #endif /* (PWM1_PWMModeIsCenterAligned) */

            #define PWM1_COMPARE1_LSB       (*(reg16 *) PWM1_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define PWM1_COMPARE1_LSB_PTR   ((reg16 *)  PWM1_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define PWM1_COMPARE2_LSB       (*(reg16 *) PWM1_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define PWM1_COMPARE2_LSB_PTR   ((reg16 *)  PWM1_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define PWM1_COUNTERCAP_LSB     (*(reg16 *) PWM1_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define PWM1_COUNTERCAP_LSB_PTR ((reg16 *)  PWM1_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define PWM1_COUNTER_LSB        (*(reg16 *) PWM1_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define PWM1_COUNTER_LSB_PTR    ((reg16 *)  PWM1_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define PWM1_CAPTURE_LSB        (*(reg16 *) PWM1_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
            #define PWM1_CAPTURE_LSB_PTR    ((reg16 *)  PWM1_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define PWM1_AUX_CONTROLDP1          (*(reg8 *)  PWM1_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)
       #define PWM1_AUX_CONTROLDP1_PTR      ((reg8 *)   PWM1_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (PWM1_Resolution == 8) */

   #define PWM1_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  PWM1_PWMUDB_sP16_pwmdp_u0__A1_REG)
   #define PWM1_AUX_CONTROLDP0          (*(reg8 *)  PWM1_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)
   #define PWM1_AUX_CONTROLDP0_PTR      ((reg8 *)   PWM1_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (PWM1_UsingFixedFunction) */

#if(PWM1_KillModeMinTime )
    #define PWM1_KILLMODEMINTIME        (*(reg8 *)  PWM1_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define PWM1_KILLMODEMINTIME_PTR    ((reg8 *)   PWM1_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (PWM1_KillModeMinTime ) */

#if(PWM1_DeadBandMode == PWM1__B_PWM__DBM_256_CLOCKS)
    #define PWM1_DEADBAND_COUNT         (*(reg8 *)  PWM1_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define PWM1_DEADBAND_COUNT_PTR     ((reg8 *)   PWM1_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define PWM1_DEADBAND_LSB_PTR       ((reg8 *)   PWM1_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define PWM1_DEADBAND_LSB           (*(reg8 *)  PWM1_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(PWM1_DeadBandMode == PWM1__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (PWM1_UsingFixedFunction)
        #define PWM1_DEADBAND_COUNT         (*(reg8 *)  PWM1_PWMHW__CFG0)
        #define PWM1_DEADBAND_COUNT_PTR     ((reg8 *)   PWM1_PWMHW__CFG0)
        #define PWM1_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << PWM1_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define PWM1_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define PWM1_DEADBAND_COUNT         (*(reg8 *)  PWM1_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define PWM1_DEADBAND_COUNT_PTR     ((reg8 *)   PWM1_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define PWM1_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << PWM1_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define PWM1_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (PWM1_UsingFixedFunction) */
#endif /* (PWM1_DeadBandMode == PWM1__B_PWM__DBM_256_CLOCKS) */



#if (PWM1_UsingFixedFunction)
    #define PWM1_STATUS                 (*(reg8 *) PWM1_PWMHW__SR0)
    #define PWM1_STATUS_PTR             ((reg8 *) PWM1_PWMHW__SR0)
    #define PWM1_STATUS_MASK            (*(reg8 *) PWM1_PWMHW__SR0)
    #define PWM1_STATUS_MASK_PTR        ((reg8 *) PWM1_PWMHW__SR0)
    #define PWM1_CONTROL                (*(reg8 *) PWM1_PWMHW__CFG0)
    #define PWM1_CONTROL_PTR            ((reg8 *) PWM1_PWMHW__CFG0)
    #define PWM1_CONTROL2               (*(reg8 *) PWM1_PWMHW__CFG1)
    #define PWM1_CONTROL3               (*(reg8 *) PWM1_PWMHW__CFG2)
    #define PWM1_GLOBAL_ENABLE          (*(reg8 *) PWM1_PWMHW__PM_ACT_CFG)
    #define PWM1_GLOBAL_ENABLE_PTR      ( (reg8 *) PWM1_PWMHW__PM_ACT_CFG)
    #define PWM1_GLOBAL_STBY_ENABLE     (*(reg8 *) PWM1_PWMHW__PM_STBY_CFG)
    #define PWM1_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) PWM1_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define PWM1_BLOCK_EN_MASK          (PWM1_PWMHW__PM_ACT_MSK)
    #define PWM1_BLOCK_STBY_EN_MASK     (PWM1_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define PWM1_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define PWM1_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define PWM1_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define PWM1_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define PWM1_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define PWM1_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define PWM1_CTRL_ENABLE            (uint8)((uint8)0x01u << PWM1_CTRL_ENABLE_SHIFT)
    #define PWM1_CTRL_RESET             (uint8)((uint8)0x01u << PWM1_CTRL_RESET_SHIFT)
    #define PWM1_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << PWM1_CTRL_CMPMODE2_SHIFT)
    #define PWM1_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << PWM1_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define PWM1_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define PWM1_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << PWM1_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define PWM1_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define PWM1_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define PWM1_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define PWM1_STATUS_TC_INT_EN_MASK_SHIFT            (PWM1_STATUS_TC_SHIFT - 4u)
    #define PWM1_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define PWM1_STATUS_CMP1_INT_EN_MASK_SHIFT          (PWM1_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define PWM1_STATUS_TC              (uint8)((uint8)0x01u << PWM1_STATUS_TC_SHIFT)
    #define PWM1_STATUS_CMP1            (uint8)((uint8)0x01u << PWM1_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define PWM1_STATUS_TC_INT_EN_MASK              (uint8)((uint8)PWM1_STATUS_TC >> 4u)
    #define PWM1_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)PWM1_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define PWM1_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define PWM1_RT1_MASK              (uint8)((uint8)0x03u << PWM1_RT1_SHIFT)
    #define PWM1_SYNC                  (uint8)((uint8)0x03u << PWM1_RT1_SHIFT)
    #define PWM1_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define PWM1_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << PWM1_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define PWM1_SYNCDSI_EN            (uint8)((uint8)0x0Fu << PWM1_SYNCDSI_SHIFT)


#else
    #define PWM1_STATUS                (*(reg8 *)   PWM1_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define PWM1_STATUS_PTR            ((reg8 *)    PWM1_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define PWM1_STATUS_MASK           (*(reg8 *)   PWM1_PWMUDB_genblk8_stsreg__MASK_REG)
    #define PWM1_STATUS_MASK_PTR       ((reg8 *)    PWM1_PWMUDB_genblk8_stsreg__MASK_REG)
    #define PWM1_STATUS_AUX_CTRL       (*(reg8 *)   PWM1_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define PWM1_CONTROL               (*(reg8 *)   PWM1_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define PWM1_CONTROL_PTR           ((reg8 *)    PWM1_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define PWM1_CTRL_ENABLE_SHIFT      (0x07u)
    #define PWM1_CTRL_RESET_SHIFT       (0x06u)
    #define PWM1_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define PWM1_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define PWM1_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define PWM1_CTRL_ENABLE            (uint8)((uint8)0x01u << PWM1_CTRL_ENABLE_SHIFT)
    #define PWM1_CTRL_RESET             (uint8)((uint8)0x01u << PWM1_CTRL_RESET_SHIFT)
    #define PWM1_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << PWM1_CTRL_CMPMODE2_SHIFT)
    #define PWM1_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << PWM1_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define PWM1_STATUS_KILL_SHIFT          (0x05u)
    #define PWM1_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define PWM1_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define PWM1_STATUS_TC_SHIFT            (0x02u)
    #define PWM1_STATUS_CMP2_SHIFT          (0x01u)
    #define PWM1_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define PWM1_STATUS_KILL_INT_EN_MASK_SHIFT          (PWM1_STATUS_KILL_SHIFT)
    #define PWM1_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (PWM1_STATUS_FIFONEMPTY_SHIFT)
    #define PWM1_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (PWM1_STATUS_FIFOFULL_SHIFT)
    #define PWM1_STATUS_TC_INT_EN_MASK_SHIFT            (PWM1_STATUS_TC_SHIFT)
    #define PWM1_STATUS_CMP2_INT_EN_MASK_SHIFT          (PWM1_STATUS_CMP2_SHIFT)
    #define PWM1_STATUS_CMP1_INT_EN_MASK_SHIFT          (PWM1_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define PWM1_STATUS_KILL            (uint8)((uint8)0x00u << PWM1_STATUS_KILL_SHIFT )
    #define PWM1_STATUS_FIFOFULL        (uint8)((uint8)0x01u << PWM1_STATUS_FIFOFULL_SHIFT)
    #define PWM1_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << PWM1_STATUS_FIFONEMPTY_SHIFT)
    #define PWM1_STATUS_TC              (uint8)((uint8)0x01u << PWM1_STATUS_TC_SHIFT)
    #define PWM1_STATUS_CMP2            (uint8)((uint8)0x01u << PWM1_STATUS_CMP2_SHIFT)
    #define PWM1_STATUS_CMP1            (uint8)((uint8)0x01u << PWM1_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define PWM1_STATUS_KILL_INT_EN_MASK            (PWM1_STATUS_KILL)
    #define PWM1_STATUS_FIFOFULL_INT_EN_MASK        (PWM1_STATUS_FIFOFULL)
    #define PWM1_STATUS_FIFONEMPTY_INT_EN_MASK      (PWM1_STATUS_FIFONEMPTY)
    #define PWM1_STATUS_TC_INT_EN_MASK              (PWM1_STATUS_TC)
    #define PWM1_STATUS_CMP2_INT_EN_MASK            (PWM1_STATUS_CMP2)
    #define PWM1_STATUS_CMP1_INT_EN_MASK            (PWM1_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define PWM1_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define PWM1_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define PWM1_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define PWM1_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define PWM1_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* PWM1_UsingFixedFunction */

#endif  /* CY_PWM_PWM1_H */


/* [] END OF FILE */
