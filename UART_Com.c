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
#include <UART_Com.h>

uint8	RxBuffer[RxBufferSize];     // Rx circular buffer to hold all incoming command
uint8  *RxReadIndex	 = RxBuffer;    // pointer to position in RxBuffer to write incoming Rx bytes
uint8  *RxWriteIndex = RxBuffer;    // pointer to position in RxBuffer to read and process bytes


char   *RxStrIndex = RB.RxStr;      // pointer to command string buffer (processed messages)
                                    // each Rx command consists of: <byte command><string value><CR>

CY_ISR(RxIsr) //interrupt on Rx byte received
{   
    //move all available characters from Rx queue to RxBuffer
    while(UART_ReadRxStatus() & UART_RX_STS_FIFO_NOTEMPTY)
	{
		*RxWriteIndex++ = UART_ReadRxData();
		if (RxWriteIndex >= RxBuffer + RxBufferSize) RxWriteIndex = RxBuffer;      
	}   
}

uint8 IsCharReady(void) 
{
	return !(RxWriteIndex == RxReadIndex);
}

uint8 GetRxStr(void)
{
    uint8 RxStr_flag = 0;
    static uint8 Ch;//static?
   
	Ch = *RxReadIndex++;       //read next char in buffer
    if (RxReadIndex >= RxBuffer + RxBufferSize) RxReadIndex = RxBuffer;
            
    //if (Ch == EOM_char)
    if ( (Ch == EOM_CR) || (Ch == EOM_LF) ) //any standard terminator
    {
        *RxStrIndex = 0;        //terminate string excluding EOM_char
        RxStrIndex = RB.RxStr;  //reset pointer
        if (strlen(RB.RxStr) > 0)//non-empty message received
        {
            RxStr_flag  = 1;    //set flag to process message
        }   
    }
    else                        //string body char received
    {
        *RxStrIndex++ = Ch;     //build command message   
        //todo: problem if first char is empty space
    }   

    return RxStr_flag;        
}
/* [] END OF FILE */
