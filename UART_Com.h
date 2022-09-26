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
#include <project.h>

#define RxBufferSize	1024     //size of the Rx circular buffer
#define EOM_CR        0x0D    //message separator char (\r)
#define EOM_LF        0x0A    //message separator char (\n)

// each Rx command consists of: <char command><string value><CR>
union TRxBuffer {
    char   RxStr[20];   // received Rx packet (to hold individual command message)
    struct {            // anonimous structure
        char cmd;       // 1-byte command
        char valstr[19];// command value           
    };
} RB;

//=====================================
//        Function Prototypes 
//=====================================
   
CY_ISR_PROTO(RxIsr);          // process Rx interrupt

uint8	IsCharReady(void);		// Returns TRUE when a char is ready
uint8	GetRxStr(void);			// Extract command string from buffer, non-blocking 

/* [] END OF FILE */
