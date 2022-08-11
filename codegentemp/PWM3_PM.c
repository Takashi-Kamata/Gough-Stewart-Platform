/*******************************************************************************
* File Name: PWM3_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWM3.h"

static PWM3_backupStruct PWM3_backup;


/*******************************************************************************
* Function Name: PWM3_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM3_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM3_SaveConfig(void) 
{

    #if(!PWM3_UsingFixedFunction)
        #if(!PWM3_PWMModeIsCenterAligned)
            PWM3_backup.PWMPeriod = PWM3_ReadPeriod();
        #endif /* (!PWM3_PWMModeIsCenterAligned) */
        PWM3_backup.PWMUdb = PWM3_ReadCounter();
        #if (PWM3_UseStatus)
            PWM3_backup.InterruptMaskValue = PWM3_STATUS_MASK;
        #endif /* (PWM3_UseStatus) */

        #if(PWM3_DeadBandMode == PWM3__B_PWM__DBM_256_CLOCKS || \
            PWM3_DeadBandMode == PWM3__B_PWM__DBM_2_4_CLOCKS)
            PWM3_backup.PWMdeadBandValue = PWM3_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM3_KillModeMinTime)
             PWM3_backup.PWMKillCounterPeriod = PWM3_ReadKillTime();
        #endif /* (PWM3_KillModeMinTime) */

        #if(PWM3_UseControl)
            PWM3_backup.PWMControlRegister = PWM3_ReadControlRegister();
        #endif /* (PWM3_UseControl) */
    #endif  /* (!PWM3_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM3_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM3_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM3_RestoreConfig(void) 
{
        #if(!PWM3_UsingFixedFunction)
            #if(!PWM3_PWMModeIsCenterAligned)
                PWM3_WritePeriod(PWM3_backup.PWMPeriod);
            #endif /* (!PWM3_PWMModeIsCenterAligned) */

            PWM3_WriteCounter(PWM3_backup.PWMUdb);

            #if (PWM3_UseStatus)
                PWM3_STATUS_MASK = PWM3_backup.InterruptMaskValue;
            #endif /* (PWM3_UseStatus) */

            #if(PWM3_DeadBandMode == PWM3__B_PWM__DBM_256_CLOCKS || \
                PWM3_DeadBandMode == PWM3__B_PWM__DBM_2_4_CLOCKS)
                PWM3_WriteDeadTime(PWM3_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM3_KillModeMinTime)
                PWM3_WriteKillTime(PWM3_backup.PWMKillCounterPeriod);
            #endif /* (PWM3_KillModeMinTime) */

            #if(PWM3_UseControl)
                PWM3_WriteControlRegister(PWM3_backup.PWMControlRegister);
            #endif /* (PWM3_UseControl) */
        #endif  /* (!PWM3_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM3_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM3_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM3_Sleep(void) 
{
    #if(PWM3_UseControl)
        if(PWM3_CTRL_ENABLE == (PWM3_CONTROL & PWM3_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM3_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM3_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM3_UseControl) */

    /* Stop component */
    PWM3_Stop();

    /* Save registers configuration */
    PWM3_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM3_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM3_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM3_Wakeup(void) 
{
     /* Restore registers values */
    PWM3_RestoreConfig();

    if(PWM3_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM3_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
