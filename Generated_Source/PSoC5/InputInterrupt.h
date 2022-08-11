/*******************************************************************************
* File Name: InputInterrupt.h
* Version 1.71
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_InputInterrupt_H)
#define CY_ISR_InputInterrupt_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void InputInterrupt_Start(void);
void InputInterrupt_StartEx(cyisraddress address);
void InputInterrupt_Stop(void);

CY_ISR_PROTO(InputInterrupt_Interrupt);

void InputInterrupt_SetVector(cyisraddress address);
cyisraddress InputInterrupt_GetVector(void);

void InputInterrupt_SetPriority(uint8 priority);
uint8 InputInterrupt_GetPriority(void);

void InputInterrupt_Enable(void);
uint8 InputInterrupt_GetState(void);
void InputInterrupt_Disable(void);

void InputInterrupt_SetPending(void);
void InputInterrupt_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the InputInterrupt ISR. */
#define InputInterrupt_INTC_VECTOR            ((reg32 *) InputInterrupt__INTC_VECT)

/* Address of the InputInterrupt ISR priority. */
#define InputInterrupt_INTC_PRIOR             ((reg8 *) InputInterrupt__INTC_PRIOR_REG)

/* Priority of the InputInterrupt interrupt. */
#define InputInterrupt_INTC_PRIOR_NUMBER      InputInterrupt__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable InputInterrupt interrupt. */
#define InputInterrupt_INTC_SET_EN            ((reg32 *) InputInterrupt__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the InputInterrupt interrupt. */
#define InputInterrupt_INTC_CLR_EN            ((reg32 *) InputInterrupt__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the InputInterrupt interrupt state to pending. */
#define InputInterrupt_INTC_SET_PD            ((reg32 *) InputInterrupt__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the InputInterrupt interrupt. */
#define InputInterrupt_INTC_CLR_PD            ((reg32 *) InputInterrupt__INTC_CLR_PD_REG)


#endif /* CY_ISR_InputInterrupt_H */


/* [] END OF FILE */
