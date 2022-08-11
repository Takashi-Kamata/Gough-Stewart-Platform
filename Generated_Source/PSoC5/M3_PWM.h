/*******************************************************************************
* File Name: M3_PWM.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_M3_PWM_H) /* Pins M3_PWM_H */
#define CY_PINS_M3_PWM_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "M3_PWM_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 M3_PWM__PORT == 15 && ((M3_PWM__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    M3_PWM_Write(uint8 value);
void    M3_PWM_SetDriveMode(uint8 mode);
uint8   M3_PWM_ReadDataReg(void);
uint8   M3_PWM_Read(void);
void    M3_PWM_SetInterruptMode(uint16 position, uint16 mode);
uint8   M3_PWM_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the M3_PWM_SetDriveMode() function.
     *  @{
     */
        #define M3_PWM_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define M3_PWM_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define M3_PWM_DM_RES_UP          PIN_DM_RES_UP
        #define M3_PWM_DM_RES_DWN         PIN_DM_RES_DWN
        #define M3_PWM_DM_OD_LO           PIN_DM_OD_LO
        #define M3_PWM_DM_OD_HI           PIN_DM_OD_HI
        #define M3_PWM_DM_STRONG          PIN_DM_STRONG
        #define M3_PWM_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define M3_PWM_MASK               M3_PWM__MASK
#define M3_PWM_SHIFT              M3_PWM__SHIFT
#define M3_PWM_WIDTH              1u

/* Interrupt constants */
#if defined(M3_PWM__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in M3_PWM_SetInterruptMode() function.
     *  @{
     */
        #define M3_PWM_INTR_NONE      (uint16)(0x0000u)
        #define M3_PWM_INTR_RISING    (uint16)(0x0001u)
        #define M3_PWM_INTR_FALLING   (uint16)(0x0002u)
        #define M3_PWM_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define M3_PWM_INTR_MASK      (0x01u) 
#endif /* (M3_PWM__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define M3_PWM_PS                     (* (reg8 *) M3_PWM__PS)
/* Data Register */
#define M3_PWM_DR                     (* (reg8 *) M3_PWM__DR)
/* Port Number */
#define M3_PWM_PRT_NUM                (* (reg8 *) M3_PWM__PRT) 
/* Connect to Analog Globals */                                                  
#define M3_PWM_AG                     (* (reg8 *) M3_PWM__AG)                       
/* Analog MUX bux enable */
#define M3_PWM_AMUX                   (* (reg8 *) M3_PWM__AMUX) 
/* Bidirectional Enable */                                                        
#define M3_PWM_BIE                    (* (reg8 *) M3_PWM__BIE)
/* Bit-mask for Aliased Register Access */
#define M3_PWM_BIT_MASK               (* (reg8 *) M3_PWM__BIT_MASK)
/* Bypass Enable */
#define M3_PWM_BYP                    (* (reg8 *) M3_PWM__BYP)
/* Port wide control signals */                                                   
#define M3_PWM_CTL                    (* (reg8 *) M3_PWM__CTL)
/* Drive Modes */
#define M3_PWM_DM0                    (* (reg8 *) M3_PWM__DM0) 
#define M3_PWM_DM1                    (* (reg8 *) M3_PWM__DM1)
#define M3_PWM_DM2                    (* (reg8 *) M3_PWM__DM2) 
/* Input Buffer Disable Override */
#define M3_PWM_INP_DIS                (* (reg8 *) M3_PWM__INP_DIS)
/* LCD Common or Segment Drive */
#define M3_PWM_LCD_COM_SEG            (* (reg8 *) M3_PWM__LCD_COM_SEG)
/* Enable Segment LCD */
#define M3_PWM_LCD_EN                 (* (reg8 *) M3_PWM__LCD_EN)
/* Slew Rate Control */
#define M3_PWM_SLW                    (* (reg8 *) M3_PWM__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define M3_PWM_PRTDSI__CAPS_SEL       (* (reg8 *) M3_PWM__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define M3_PWM_PRTDSI__DBL_SYNC_IN    (* (reg8 *) M3_PWM__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define M3_PWM_PRTDSI__OE_SEL0        (* (reg8 *) M3_PWM__PRTDSI__OE_SEL0) 
#define M3_PWM_PRTDSI__OE_SEL1        (* (reg8 *) M3_PWM__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define M3_PWM_PRTDSI__OUT_SEL0       (* (reg8 *) M3_PWM__PRTDSI__OUT_SEL0) 
#define M3_PWM_PRTDSI__OUT_SEL1       (* (reg8 *) M3_PWM__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define M3_PWM_PRTDSI__SYNC_OUT       (* (reg8 *) M3_PWM__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(M3_PWM__SIO_CFG)
    #define M3_PWM_SIO_HYST_EN        (* (reg8 *) M3_PWM__SIO_HYST_EN)
    #define M3_PWM_SIO_REG_HIFREQ     (* (reg8 *) M3_PWM__SIO_REG_HIFREQ)
    #define M3_PWM_SIO_CFG            (* (reg8 *) M3_PWM__SIO_CFG)
    #define M3_PWM_SIO_DIFF           (* (reg8 *) M3_PWM__SIO_DIFF)
#endif /* (M3_PWM__SIO_CFG) */

/* Interrupt Registers */
#if defined(M3_PWM__INTSTAT)
    #define M3_PWM_INTSTAT            (* (reg8 *) M3_PWM__INTSTAT)
    #define M3_PWM_SNAP               (* (reg8 *) M3_PWM__SNAP)
    
	#define M3_PWM_0_INTTYPE_REG 		(* (reg8 *) M3_PWM__0__INTTYPE)
#endif /* (M3_PWM__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_M3_PWM_H */


/* [] END OF FILE */
