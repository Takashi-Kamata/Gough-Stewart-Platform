/*******************************************************************************
* File Name: M3_ADC.h  
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

#if !defined(CY_PINS_M3_ADC_H) /* Pins M3_ADC_H */
#define CY_PINS_M3_ADC_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "M3_ADC_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 M3_ADC__PORT == 15 && ((M3_ADC__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    M3_ADC_Write(uint8 value);
void    M3_ADC_SetDriveMode(uint8 mode);
uint8   M3_ADC_ReadDataReg(void);
uint8   M3_ADC_Read(void);
void    M3_ADC_SetInterruptMode(uint16 position, uint16 mode);
uint8   M3_ADC_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the M3_ADC_SetDriveMode() function.
     *  @{
     */
        #define M3_ADC_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define M3_ADC_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define M3_ADC_DM_RES_UP          PIN_DM_RES_UP
        #define M3_ADC_DM_RES_DWN         PIN_DM_RES_DWN
        #define M3_ADC_DM_OD_LO           PIN_DM_OD_LO
        #define M3_ADC_DM_OD_HI           PIN_DM_OD_HI
        #define M3_ADC_DM_STRONG          PIN_DM_STRONG
        #define M3_ADC_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define M3_ADC_MASK               M3_ADC__MASK
#define M3_ADC_SHIFT              M3_ADC__SHIFT
#define M3_ADC_WIDTH              1u

/* Interrupt constants */
#if defined(M3_ADC__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in M3_ADC_SetInterruptMode() function.
     *  @{
     */
        #define M3_ADC_INTR_NONE      (uint16)(0x0000u)
        #define M3_ADC_INTR_RISING    (uint16)(0x0001u)
        #define M3_ADC_INTR_FALLING   (uint16)(0x0002u)
        #define M3_ADC_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define M3_ADC_INTR_MASK      (0x01u) 
#endif /* (M3_ADC__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define M3_ADC_PS                     (* (reg8 *) M3_ADC__PS)
/* Data Register */
#define M3_ADC_DR                     (* (reg8 *) M3_ADC__DR)
/* Port Number */
#define M3_ADC_PRT_NUM                (* (reg8 *) M3_ADC__PRT) 
/* Connect to Analog Globals */                                                  
#define M3_ADC_AG                     (* (reg8 *) M3_ADC__AG)                       
/* Analog MUX bux enable */
#define M3_ADC_AMUX                   (* (reg8 *) M3_ADC__AMUX) 
/* Bidirectional Enable */                                                        
#define M3_ADC_BIE                    (* (reg8 *) M3_ADC__BIE)
/* Bit-mask for Aliased Register Access */
#define M3_ADC_BIT_MASK               (* (reg8 *) M3_ADC__BIT_MASK)
/* Bypass Enable */
#define M3_ADC_BYP                    (* (reg8 *) M3_ADC__BYP)
/* Port wide control signals */                                                   
#define M3_ADC_CTL                    (* (reg8 *) M3_ADC__CTL)
/* Drive Modes */
#define M3_ADC_DM0                    (* (reg8 *) M3_ADC__DM0) 
#define M3_ADC_DM1                    (* (reg8 *) M3_ADC__DM1)
#define M3_ADC_DM2                    (* (reg8 *) M3_ADC__DM2) 
/* Input Buffer Disable Override */
#define M3_ADC_INP_DIS                (* (reg8 *) M3_ADC__INP_DIS)
/* LCD Common or Segment Drive */
#define M3_ADC_LCD_COM_SEG            (* (reg8 *) M3_ADC__LCD_COM_SEG)
/* Enable Segment LCD */
#define M3_ADC_LCD_EN                 (* (reg8 *) M3_ADC__LCD_EN)
/* Slew Rate Control */
#define M3_ADC_SLW                    (* (reg8 *) M3_ADC__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define M3_ADC_PRTDSI__CAPS_SEL       (* (reg8 *) M3_ADC__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define M3_ADC_PRTDSI__DBL_SYNC_IN    (* (reg8 *) M3_ADC__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define M3_ADC_PRTDSI__OE_SEL0        (* (reg8 *) M3_ADC__PRTDSI__OE_SEL0) 
#define M3_ADC_PRTDSI__OE_SEL1        (* (reg8 *) M3_ADC__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define M3_ADC_PRTDSI__OUT_SEL0       (* (reg8 *) M3_ADC__PRTDSI__OUT_SEL0) 
#define M3_ADC_PRTDSI__OUT_SEL1       (* (reg8 *) M3_ADC__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define M3_ADC_PRTDSI__SYNC_OUT       (* (reg8 *) M3_ADC__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(M3_ADC__SIO_CFG)
    #define M3_ADC_SIO_HYST_EN        (* (reg8 *) M3_ADC__SIO_HYST_EN)
    #define M3_ADC_SIO_REG_HIFREQ     (* (reg8 *) M3_ADC__SIO_REG_HIFREQ)
    #define M3_ADC_SIO_CFG            (* (reg8 *) M3_ADC__SIO_CFG)
    #define M3_ADC_SIO_DIFF           (* (reg8 *) M3_ADC__SIO_DIFF)
#endif /* (M3_ADC__SIO_CFG) */

/* Interrupt Registers */
#if defined(M3_ADC__INTSTAT)
    #define M3_ADC_INTSTAT            (* (reg8 *) M3_ADC__INTSTAT)
    #define M3_ADC_SNAP               (* (reg8 *) M3_ADC__SNAP)
    
	#define M3_ADC_0_INTTYPE_REG 		(* (reg8 *) M3_ADC__0__INTTYPE)
#endif /* (M3_ADC__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_M3_ADC_H */


/* [] END OF FILE */