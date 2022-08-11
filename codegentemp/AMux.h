/*******************************************************************************
* File Name: AMux.h
* Version 1.80
*
*  Description:
*    This file contains the constants and function prototypes for the Analog
*    Multiplexer User Module AMux.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_AMUX_AMux_H)
#define CY_AMUX_AMux_H

#include "cyfitter.h"
#include "cyfitter_cfg.h"

#if ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) || \
         (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4) || \
         (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5))    
    #include "cytypes.h"
#else
    #include "syslib/cy_syslib.h"
#endif /* ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) */


/***************************************
*        Function Prototypes
***************************************/

void AMux_Start(void) ;
#define AMux_Init() AMux_Start()
void AMux_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void AMux_Stop(void); */
/* void AMux_Select(uint8 channel); */
/* void AMux_Connect(uint8 channel); */
/* void AMux_Disconnect(uint8 channel); */
/* void AMux_DisconnectAll(void) */


/***************************************
*         Parameter Constants
***************************************/

#define AMux_CHANNELS  6u
#define AMux_MUXTYPE   1
#define AMux_ATMOSTONE 0

/***************************************
*             API Constants
***************************************/

#define AMux_NULL_CHANNEL 0xFFu
#define AMux_MUX_SINGLE   1
#define AMux_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if AMux_MUXTYPE == AMux_MUX_SINGLE
# if !AMux_ATMOSTONE
#  define AMux_Connect(channel) AMux_Set(channel)
# endif
# define AMux_Disconnect(channel) AMux_Unset(channel)
#else
# if !AMux_ATMOSTONE
void AMux_Connect(uint8 channel) ;
# endif
void AMux_Disconnect(uint8 channel) ;
#endif

#if AMux_ATMOSTONE
# define AMux_Stop() AMux_DisconnectAll()
# define AMux_Select(channel) AMux_FastSelect(channel)
void AMux_DisconnectAll(void) ;
#else
# define AMux_Stop() AMux_Start()
void AMux_Select(uint8 channel) ;
# define AMux_DisconnectAll() AMux_Start()
#endif

#endif /* CY_AMUX_AMux_H */


/* [] END OF FILE */
