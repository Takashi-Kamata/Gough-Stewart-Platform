/*******************************************************************************
* File Name: ADC_DelSig_1_Bypass_P03.h  
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

#if !defined(CY_PINS_ADC_DelSig_1_Bypass_P03_H) /* Pins ADC_DelSig_1_Bypass_P03_H */
#define CY_PINS_ADC_DelSig_1_Bypass_P03_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "ADC_DelSig_1_Bypass_P03_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 ADC_DelSig_1_Bypass_P03__PORT == 15 && ((ADC_DelSig_1_Bypass_P03__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    ADC_DelSig_1_Bypass_P03_Write(uint8 value);
void    ADC_DelSig_1_Bypass_P03_SetDriveMode(uint8 mode);
uint8   ADC_DelSig_1_Bypass_P03_ReadDataReg(void);
uint8   ADC_DelSig_1_Bypass_P03_Read(void);
void    ADC_DelSig_1_Bypass_P03_SetInterruptMode(uint16 position, uint16 mode);
uint8   ADC_DelSig_1_Bypass_P03_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the ADC_DelSig_1_Bypass_P03_SetDriveMode() function.
     *  @{
     */
        #define ADC_DelSig_1_Bypass_P03_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define ADC_DelSig_1_Bypass_P03_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define ADC_DelSig_1_Bypass_P03_DM_RES_UP          PIN_DM_RES_UP
        #define ADC_DelSig_1_Bypass_P03_DM_RES_DWN         PIN_DM_RES_DWN
        #define ADC_DelSig_1_Bypass_P03_DM_OD_LO           PIN_DM_OD_LO
        #define ADC_DelSig_1_Bypass_P03_DM_OD_HI           PIN_DM_OD_HI
        #define ADC_DelSig_1_Bypass_P03_DM_STRONG          PIN_DM_STRONG
        #define ADC_DelSig_1_Bypass_P03_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define ADC_DelSig_1_Bypass_P03_MASK               ADC_DelSig_1_Bypass_P03__MASK
#define ADC_DelSig_1_Bypass_P03_SHIFT              ADC_DelSig_1_Bypass_P03__SHIFT
#define ADC_DelSig_1_Bypass_P03_WIDTH              1u

/* Interrupt constants */
#if defined(ADC_DelSig_1_Bypass_P03__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in ADC_DelSig_1_Bypass_P03_SetInterruptMode() function.
     *  @{
     */
        #define ADC_DelSig_1_Bypass_P03_INTR_NONE      (uint16)(0x0000u)
        #define ADC_DelSig_1_Bypass_P03_INTR_RISING    (uint16)(0x0001u)
        #define ADC_DelSig_1_Bypass_P03_INTR_FALLING   (uint16)(0x0002u)
        #define ADC_DelSig_1_Bypass_P03_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define ADC_DelSig_1_Bypass_P03_INTR_MASK      (0x01u) 
#endif /* (ADC_DelSig_1_Bypass_P03__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define ADC_DelSig_1_Bypass_P03_PS                     (* (reg8 *) ADC_DelSig_1_Bypass_P03__PS)
/* Data Register */
#define ADC_DelSig_1_Bypass_P03_DR                     (* (reg8 *) ADC_DelSig_1_Bypass_P03__DR)
/* Port Number */
#define ADC_DelSig_1_Bypass_P03_PRT_NUM                (* (reg8 *) ADC_DelSig_1_Bypass_P03__PRT) 
/* Connect to Analog Globals */                                                  
#define ADC_DelSig_1_Bypass_P03_AG                     (* (reg8 *) ADC_DelSig_1_Bypass_P03__AG)                       
/* Analog MUX bux enable */
#define ADC_DelSig_1_Bypass_P03_AMUX                   (* (reg8 *) ADC_DelSig_1_Bypass_P03__AMUX) 
/* Bidirectional Enable */                                                        
#define ADC_DelSig_1_Bypass_P03_BIE                    (* (reg8 *) ADC_DelSig_1_Bypass_P03__BIE)
/* Bit-mask for Aliased Register Access */
#define ADC_DelSig_1_Bypass_P03_BIT_MASK               (* (reg8 *) ADC_DelSig_1_Bypass_P03__BIT_MASK)
/* Bypass Enable */
#define ADC_DelSig_1_Bypass_P03_BYP                    (* (reg8 *) ADC_DelSig_1_Bypass_P03__BYP)
/* Port wide control signals */                                                   
#define ADC_DelSig_1_Bypass_P03_CTL                    (* (reg8 *) ADC_DelSig_1_Bypass_P03__CTL)
/* Drive Modes */
#define ADC_DelSig_1_Bypass_P03_DM0                    (* (reg8 *) ADC_DelSig_1_Bypass_P03__DM0) 
#define ADC_DelSig_1_Bypass_P03_DM1                    (* (reg8 *) ADC_DelSig_1_Bypass_P03__DM1)
#define ADC_DelSig_1_Bypass_P03_DM2                    (* (reg8 *) ADC_DelSig_1_Bypass_P03__DM2) 
/* Input Buffer Disable Override */
#define ADC_DelSig_1_Bypass_P03_INP_DIS                (* (reg8 *) ADC_DelSig_1_Bypass_P03__INP_DIS)
/* LCD Common or Segment Drive */
#define ADC_DelSig_1_Bypass_P03_LCD_COM_SEG            (* (reg8 *) ADC_DelSig_1_Bypass_P03__LCD_COM_SEG)
/* Enable Segment LCD */
#define ADC_DelSig_1_Bypass_P03_LCD_EN                 (* (reg8 *) ADC_DelSig_1_Bypass_P03__LCD_EN)
/* Slew Rate Control */
#define ADC_DelSig_1_Bypass_P03_SLW                    (* (reg8 *) ADC_DelSig_1_Bypass_P03__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define ADC_DelSig_1_Bypass_P03_PRTDSI__CAPS_SEL       (* (reg8 *) ADC_DelSig_1_Bypass_P03__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define ADC_DelSig_1_Bypass_P03_PRTDSI__DBL_SYNC_IN    (* (reg8 *) ADC_DelSig_1_Bypass_P03__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define ADC_DelSig_1_Bypass_P03_PRTDSI__OE_SEL0        (* (reg8 *) ADC_DelSig_1_Bypass_P03__PRTDSI__OE_SEL0) 
#define ADC_DelSig_1_Bypass_P03_PRTDSI__OE_SEL1        (* (reg8 *) ADC_DelSig_1_Bypass_P03__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define ADC_DelSig_1_Bypass_P03_PRTDSI__OUT_SEL0       (* (reg8 *) ADC_DelSig_1_Bypass_P03__PRTDSI__OUT_SEL0) 
#define ADC_DelSig_1_Bypass_P03_PRTDSI__OUT_SEL1       (* (reg8 *) ADC_DelSig_1_Bypass_P03__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define ADC_DelSig_1_Bypass_P03_PRTDSI__SYNC_OUT       (* (reg8 *) ADC_DelSig_1_Bypass_P03__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(ADC_DelSig_1_Bypass_P03__SIO_CFG)
    #define ADC_DelSig_1_Bypass_P03_SIO_HYST_EN        (* (reg8 *) ADC_DelSig_1_Bypass_P03__SIO_HYST_EN)
    #define ADC_DelSig_1_Bypass_P03_SIO_REG_HIFREQ     (* (reg8 *) ADC_DelSig_1_Bypass_P03__SIO_REG_HIFREQ)
    #define ADC_DelSig_1_Bypass_P03_SIO_CFG            (* (reg8 *) ADC_DelSig_1_Bypass_P03__SIO_CFG)
    #define ADC_DelSig_1_Bypass_P03_SIO_DIFF           (* (reg8 *) ADC_DelSig_1_Bypass_P03__SIO_DIFF)
#endif /* (ADC_DelSig_1_Bypass_P03__SIO_CFG) */

/* Interrupt Registers */
#if defined(ADC_DelSig_1_Bypass_P03__INTSTAT)
    #define ADC_DelSig_1_Bypass_P03_INTSTAT            (* (reg8 *) ADC_DelSig_1_Bypass_P03__INTSTAT)
    #define ADC_DelSig_1_Bypass_P03_SNAP               (* (reg8 *) ADC_DelSig_1_Bypass_P03__SNAP)
    
	#define ADC_DelSig_1_Bypass_P03_0_INTTYPE_REG 		(* (reg8 *) ADC_DelSig_1_Bypass_P03__0__INTTYPE)
#endif /* (ADC_DelSig_1_Bypass_P03__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_ADC_DelSig_1_Bypass_P03_H */


/* [] END OF FILE */
