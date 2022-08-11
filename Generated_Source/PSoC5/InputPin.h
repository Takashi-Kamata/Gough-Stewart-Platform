/*******************************************************************************
* File Name: InputPin.h  
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

#if !defined(CY_PINS_InputPin_H) /* Pins InputPin_H */
#define CY_PINS_InputPin_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "InputPin_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 InputPin__PORT == 15 && ((InputPin__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    InputPin_Write(uint8 value);
void    InputPin_SetDriveMode(uint8 mode);
uint8   InputPin_ReadDataReg(void);
uint8   InputPin_Read(void);
void    InputPin_SetInterruptMode(uint16 position, uint16 mode);
uint8   InputPin_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the InputPin_SetDriveMode() function.
     *  @{
     */
        #define InputPin_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define InputPin_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define InputPin_DM_RES_UP          PIN_DM_RES_UP
        #define InputPin_DM_RES_DWN         PIN_DM_RES_DWN
        #define InputPin_DM_OD_LO           PIN_DM_OD_LO
        #define InputPin_DM_OD_HI           PIN_DM_OD_HI
        #define InputPin_DM_STRONG          PIN_DM_STRONG
        #define InputPin_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define InputPin_MASK               InputPin__MASK
#define InputPin_SHIFT              InputPin__SHIFT
#define InputPin_WIDTH              1u

/* Interrupt constants */
#if defined(InputPin__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in InputPin_SetInterruptMode() function.
     *  @{
     */
        #define InputPin_INTR_NONE      (uint16)(0x0000u)
        #define InputPin_INTR_RISING    (uint16)(0x0001u)
        #define InputPin_INTR_FALLING   (uint16)(0x0002u)
        #define InputPin_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define InputPin_INTR_MASK      (0x01u) 
#endif /* (InputPin__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define InputPin_PS                     (* (reg8 *) InputPin__PS)
/* Data Register */
#define InputPin_DR                     (* (reg8 *) InputPin__DR)
/* Port Number */
#define InputPin_PRT_NUM                (* (reg8 *) InputPin__PRT) 
/* Connect to Analog Globals */                                                  
#define InputPin_AG                     (* (reg8 *) InputPin__AG)                       
/* Analog MUX bux enable */
#define InputPin_AMUX                   (* (reg8 *) InputPin__AMUX) 
/* Bidirectional Enable */                                                        
#define InputPin_BIE                    (* (reg8 *) InputPin__BIE)
/* Bit-mask for Aliased Register Access */
#define InputPin_BIT_MASK               (* (reg8 *) InputPin__BIT_MASK)
/* Bypass Enable */
#define InputPin_BYP                    (* (reg8 *) InputPin__BYP)
/* Port wide control signals */                                                   
#define InputPin_CTL                    (* (reg8 *) InputPin__CTL)
/* Drive Modes */
#define InputPin_DM0                    (* (reg8 *) InputPin__DM0) 
#define InputPin_DM1                    (* (reg8 *) InputPin__DM1)
#define InputPin_DM2                    (* (reg8 *) InputPin__DM2) 
/* Input Buffer Disable Override */
#define InputPin_INP_DIS                (* (reg8 *) InputPin__INP_DIS)
/* LCD Common or Segment Drive */
#define InputPin_LCD_COM_SEG            (* (reg8 *) InputPin__LCD_COM_SEG)
/* Enable Segment LCD */
#define InputPin_LCD_EN                 (* (reg8 *) InputPin__LCD_EN)
/* Slew Rate Control */
#define InputPin_SLW                    (* (reg8 *) InputPin__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define InputPin_PRTDSI__CAPS_SEL       (* (reg8 *) InputPin__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define InputPin_PRTDSI__DBL_SYNC_IN    (* (reg8 *) InputPin__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define InputPin_PRTDSI__OE_SEL0        (* (reg8 *) InputPin__PRTDSI__OE_SEL0) 
#define InputPin_PRTDSI__OE_SEL1        (* (reg8 *) InputPin__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define InputPin_PRTDSI__OUT_SEL0       (* (reg8 *) InputPin__PRTDSI__OUT_SEL0) 
#define InputPin_PRTDSI__OUT_SEL1       (* (reg8 *) InputPin__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define InputPin_PRTDSI__SYNC_OUT       (* (reg8 *) InputPin__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(InputPin__SIO_CFG)
    #define InputPin_SIO_HYST_EN        (* (reg8 *) InputPin__SIO_HYST_EN)
    #define InputPin_SIO_REG_HIFREQ     (* (reg8 *) InputPin__SIO_REG_HIFREQ)
    #define InputPin_SIO_CFG            (* (reg8 *) InputPin__SIO_CFG)
    #define InputPin_SIO_DIFF           (* (reg8 *) InputPin__SIO_DIFF)
#endif /* (InputPin__SIO_CFG) */

/* Interrupt Registers */
#if defined(InputPin__INTSTAT)
    #define InputPin_INTSTAT            (* (reg8 *) InputPin__INTSTAT)
    #define InputPin_SNAP               (* (reg8 *) InputPin__SNAP)
    
	#define InputPin_0_INTTYPE_REG 		(* (reg8 *) InputPin__0__INTTYPE)
#endif /* (InputPin__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_InputPin_H */


/* [] END OF FILE */
