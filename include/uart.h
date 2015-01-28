/*
*	Copyright (C) riban 2015 GPL V2 derived from uart example Espressif System
*	Brian Walton (brian@riban.co.uk)
*/

#pragma once

#define UART_TX_FIFO_SIZE		127

typedef enum {
    UART_WordLength_5b = 0x0,
    UART_WordLength_6b = 0x1,
    UART_WordLength_7b = 0x2,
    UART_WordLength_8b = 0x3
} UART_WordLength;

typedef enum {
    UART_StopBits_1   = 0x1,
    UART_StopBits_1_5 = 0x2,
    UART_StopBits_2   = 0x3,
} UART_StopBits;

typedef enum {
    UART0 = 0x0,
    UART1 = 0x1,
} UART_Port;

typedef enum {
    UART_Parity_None = 0x0,
    UART_Parity_Even = 0x2,
    UART_Parity_Odd  = 0x3
} UART_ParityMode;

typedef enum {
    UART_HardwareFlowControl_None    = 0x0,
    UART_HardwareFlowControl_RTS     = 0x1,
    UART_HardwareFlowControl_CTS     = 0x2,
    UART_HardwareFlowControl_CTS_RTS = 0x3
} UART_HwFlowCtrl;

typedef enum {
    UART_INVERT_NONE	= 0x0,
    UART_INVERT_RX		= BIT(19),
    UART_INVERT_CTS		= BIT(20),
	UART_INVERT_DSR		= BIT(21),
    UART_INVERT_TX		= BIT(22),
    UART_INVERT_RTS		= BIT(23),
	UART_INVERT_DTR		= BIT(24),
	UART_INVERT_ALL		= (0x3f << 19)
} UART_INVERT_FLAGS;

//=======================================

/**	@brief	Wait for the UART Tx FIFO buffer to empty
*	@param	nUart Index of the UART port (0 | 1)
*	@param	nThreshold Quantity of bytes in buffer to fall below before returning
*	@note	Blocks until threshold reached
*	@note	Returns immediately if Tx flow control enabled
*/
void uartWaitTxFifoEmpty(UART_Port nUart, uint8_t nThreshold);

/**	@brief	Reset Tx and Rx FIFO buffers
*	@param	nUart UART number (0 | 1)
*/
void uartResetFifo(UART_Port nUart);

/**	@brief	Clear interrupt statuses
*	@param	nUart UART number (0 | 1)
*	@param	nMask Bitwise mask of interrupts to clear
*/
void uartClearInterruptStatus(UART_Port nUart, uint32_t nMask);

/**	@brief	Enable interrupts
*	@param	nUart UART number (0 | 1)
*	@param	nMask Bitwise mask of interrupts to enable
*/
void uartEnableInterrupts(UART_Port nUart, uint32_t nMask);

/**	@brief	Disable interrupts
*	@param	nUart UART number (0 | 1)
*	@param	nMask Bitwise mask of interrupts to disable
*	@todo	This was not in the original example - is it valid?
*/
void uartDisableInterrupts(UART_Port nUart, uint32_t nMask);

/**	@brief	Set the event handler (callback function) to handle UART interrupt events
*	@param	pFunction Pointer to the event handler function
*   @Note	One event handler function handles _all_ UART interrupt events, e.g. for both UARTs
*   @todo	Should there be a global setInterruptHandler(nType, pFunction) rather than individual ones for each type (GPI, UART, etc)?
*/
void uartSetInterruptHandler(void *pFunction);

/** @brief  Initialise UART with default settings
*	@param	nUart UART number (0 | 1)
*	@param	nBitrate Baud rate (300 - 3686400)
*	@param	pEventHandler Pointer to a function to handle UART interrupt events or NULL for no change
*   @note   Sets parameters 8 bits, no parity, 1 stop bit, no hardware flow control
*   @Note	One event handler function handles _all_ UART interrupt events, e.g. for both UARTs
*/
void uartInit(UART_Port nUart, uint32_t nBitrate, void* pEventHandler);

/** @brief  Send character to UART
*	@param	nUart UART number (0 | 1)
*   @param  nChar Character to send
*   @note	Blocks until space in Tx buffer available
*/
void uartSendChar(UART_Port nUart, char nChar);

/** @brief  Send data to UART from byte buffer (array)
*	@param	nUart UART number (0 | 1)
*   @param  pBuffer Pointer to send buffer
*   @param  nLen Quantity of bytes to send
*/
void uartSend(UART_Port nUart, uint8_t *pBuffer, uint16_t nLen);

/** @brief  Print null terminated string (c-string) to UART
*	@param	nUart UART number (0 | 1)
*   @param  pString Pointer to null terminated string
*/
void uartPrint(UART_Port nUart, const char *pString);

/** @brief  Print null terminated string (c-string) to UART with end of line characters
*	@param	nUart UART number (0 | 1)
*   @param  pString Pointer to null terminated string
*   @note	End of line characters are newline + carriage return
*/
void uartPrintln(UART_Port nUart, const char *pString);

/** @brief	Set UART word length
* 	@param	nUart UART number (0 | 1)
* 	@param	nLen Word length (see UART_WordLength)
*/
void uartSetWordLength(UART_Port nUart, UART_WordLength nLen);

/** @brief	Set UART stop bits
* 	@param	nUart UART number (0 | 1)
* 	@param	nBits Quantity of stop bits (see UART_StopBits)
*/
void uartSetStopBits(UART_Port nUart, UART_StopBits nBits);

/** @brief	Set UART parity
* 	@param	nUart UART number (0 | 1)
* 	@param	nMode Parity mode (see UART_ParityMode)
*/
void uartSetParity(UART_Port nUart, UART_ParityMode nMode);

/**	@brief	Set UART baud rate
*	@param	nUart UART number (0 | 1)
*	@param	nRate Baud rate (0 for automoatic baud detection)
*/
void uartSetBaudrate(UART_Port nUart, uint32_t nRate);

/**	@brief	Set UART flow control
*	@param	nUart UART number (0 | 1)
*	@param	nFlow Flow control method flags (see UART_HwFlowCtrl)
*	@param	nRxThreshold Receive threshold (only used if UART_HardwareFlowControl_RTS bit set in nFlow)
*/
void uartSetFlowCtrl(UART_Port nUart, UART_HwFlowCtrl nFlow, uint8 nRxThreshold);

/**	@brief	Invert signal levels
*	@param	nUart UART number (0 | 1)
*	@param	nMask Bitwise mask of signals to invert (see UART_INVERT_FLAGS)
*/
void uartSetSignalInvert(UART_Port nUart, UART_INVERT_FLAGS nMask) ;

/**	@brief	Set threshold for receive timeout
* 	@param	nUart UART number (0 | 1)
* 	@param	nThreshold Duration of timeout to trigger interrupt
* 	@todo	Document units of threshold
*/
void uartSetRxTimeoutThreshold(UART_Port nUart, uint32_t nThreshold);

/**	@brief	Set threshold for transmit buffer empty interrupt to trigger
* 	@param	nUart UART number (0 | 1)
* 	@param	nThreshold Quantity of bytes in buffer to trigger event
*/
void uartSetTxFifoEmptyThreshold(UART_Port nUart, uint32_t nThreshold);

/**	@brief	Set threshold for receive buffer full interrupt to trigger
* 	@param	nUart UART number (0 | 1)
* 	@param	nThreshold Quantity of bytes in buffer to trigger event
*/
void uartSetRxFifoFullThreshold(UART_Port nUart, uint32_t nThreshold);

/**	@brief	Dummy function to test linking only used functions
*	@todo	Remove uartDeleteMe when linking working correctly
*/
void uartDeleteMe(UART_Port nUart);
