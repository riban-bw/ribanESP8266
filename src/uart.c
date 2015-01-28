/*
*	Copyright (C) riban 2015 GPL V2 derived from uart example Espressif System
*	Brian Walton (brian@riban.co.uk)
*/

#include "esp_common.h"

#include "freertos/FreeRTOS.h" //provides _xt_isr_attach
//#include "freertos/task.h" //provides xTaskHanle
//#include "freertos/queue.h" //provides xQueueHandle
#include "esp.h"
#include "uart.h"

enum
{
    UART_EVENT_RX_CHAR,
    UART_EVENT_MAX
};

typedef struct _os_event_
{
    uint32_t event;
    uint32_t param;
} os_event_t;

//xTaskHandle xUartTaskHandle;
//xQueueHandle xQueueUart;

void ICACHE_FLASH_ATTR uartSendChar(UART_Port nUart, char nChar)
{
	uartWaitTxFifoEmpty(nUart, UART_TX_FIFO_SIZE - 1); //!@todo do we need to subtract one from fifo size?
    writeReg(UART_FIFO(nUart) , nChar);
}

/*	Handle UART receive interrupts, dispatching event to tasks if necessary
*	@todo Check out this function
*/
//LOCAL void ICACHE_FLASH_ATTR uart_rx_intr_handler_ssc(void)
//{
//	/* uart0 and uart1 intr combine together, when interrupt occur, see reg 0x3ff20020, bit2, bit0 represents
//     * uart1 and uart0 respectively
//     */
//    os_event_t e;
//    portBASE_TYPE xHigherPriorityTaskWoken;
//
//    uint8 nRxChar;
//    uint8 nUart = 0;
//
//    //Find which UART has triggered interrupt
//    if(UART_RXFIFO_FULL_INT_ST != (readReg(UART_INT_ST(nUart)) & UART_RXFIFO_FULL_INT_ST))
//        return;
//    nRxChar = readReg(UART_FIFO(nUart)) & 0xFF;
//
//    writeReg(UART_INT_CLR(nUart), UART_RXFIFO_FULL_INT_CLR);
//
//    e.event = UART_EVENT_RX_CHAR;
//    e.param = nRxChar;
//
//    xQueueSendFromISR(xQueueUart, (void *)&e, &xHigherPriorityTaskWoken);
//    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
//}

//LOCAL void ICACHE_FLASH_ATTR
//uart_task(void *pvParameters)
//{
//    os_event_t e;
//
//    for (;;) {
//        if (xQueueReceive(xQueueUart, (void *)&e, (portTickType)portMAX_DELAY)) {
//            switch (e.event) {
//                case UART_EVENT_RX_CHAR:
//                    printf("%c", e.param);
//                    break;
//
//                default:
//                    break;
//            }
//        }
//    }
//
//    vTaskDelete(NULL);
//}

void ICACHE_FLASH_ATTR uartSetWordLength(UART_Port nUart, UART_WordLength nLen)
{
    setRegBitsShifted(UART_CONF0(nUart), nLen, UART_BIT_NUM, UART_BIT_NUM_S);
}

void ICACHE_FLASH_ATTR uartSetStopBits(UART_Port nUart, UART_StopBits nBits)
{
	setRegBitsShifted(UART_CONF0(nUart), nBits, UART_STOP_BIT_NUM, UART_STOP_BIT_NUM_S);
}

void ICACHE_FLASH_ATTR uartSetSignalInvert(UART_Port nUart, UART_INVERT_FLAGS nMask)
{
    clearRegBits(UART_CONF0(nUart), UART_INVERT_ALL);
    setRegBits(UART_CONF0(nUart), nMask);
}

void ICACHE_FLASH_ATTR uartSetParity(UART_Port nUart, UART_ParityMode nMode)
{
    clearRegBits(UART_CONF0(nUart), UART_PARITY | UART_PARITY_EN);
    setRegBits(UART_CONF0(nUart), nMode);
}

void ICACHE_FLASH_ATTR uartSetBaudrate(UART_Port nUart, uint32_t nRate)
{
	if(0 == nRate)
	{
		//Auto-baud
		//!@todo Auto-baud does not seem to work
		setRegBits(UART_AUTOBAUD(nUart), UART_AUTOBAUD_EN);
		//!@todo Configure auto-baud glitch filter
	}
	else
	{
		clearRegBits(UART_AUTOBAUD(nUart), UART_AUTOBAUD_EN);
		writeReg(UART_CLKDIV(nUart), UART_CLK_FREQ / nRate);
	}
}

void ICACHE_FLASH_ATTR uartSetFlowCtrl(UART_Port nUart, UART_HwFlowCtrl nFlow, uint8 nRxThreshold)
{
    if (nFlow & UART_HardwareFlowControl_RTS)
    {
        selectFunction(PERIPHS_IO_MUX_MTDO_U, FUNC_U0RTS);
        setRegBitsShifted(UART_CONF1(nUart), nRxThreshold, UART_RX_FLOW_THRHD, UART_RX_FLOW_THRHD_S);
        setRegBits(UART_CONF1(nUart), UART_RX_FLOW_EN);
    }
    else
    {
        clearRegBits(UART_CONF1(nUart), UART_RX_FLOW_EN);
    }

    if (nFlow & UART_HardwareFlowControl_CTS)
    {
        selectFunction(PERIPHS_IO_MUX_MTCK_U, FUNC_UART0_CTS);
        setRegBits(UART_CONF0(nUart), UART_TX_FLOW_EN);
    }
    else
    {
        clearRegBits(UART_CONF0(nUart), UART_TX_FLOW_EN);
    }
}

void ICACHE_FLASH_ATTR uartWaitTxFifoEmpty(UART_Port nUart, uint8_t nThreshold)
{
	if(readReg(UART_CONF0(nUart)) & UART_TX_FLOW_EN)
		return; //do not use if tx flow control enabled
	while (getRegBitsShifted(UART_STATUS(nUart),  UART_TXFIFO_CNT,  UART_TXFIFO_CNT_S) > nThreshold)
		;
}

void ICACHE_FLASH_ATTR uartResetFifo(UART_Port nUart)
{
    setRegBits(UART_CONF0(nUart), UART_RXFIFO_RST | UART_TXFIFO_RST);
    clearRegBits(UART_CONF0(nUart), UART_RXFIFO_RST | UART_TXFIFO_RST);
}

void ICACHE_FLASH_ATTR uartClearInterruptStatus(UART_Port nUart, uint32_t nMask)
{
    writeReg(UART_INT_CLR(nUart), nMask);
}

void ICACHE_FLASH_ATTR uartEnableInterrupts(UART_Port nUart, uint32_t nMask)
{
	setRegBits(UART_INT_ENA(nUart), nMask);
}

void ICACHE_FLASH_ATTR uartDisableInterrupts(UART_Port nUart, uint32_t nMask)
{
	clearRegBits(UART_INT_ENA(nUart), nMask);
}

void ICACHE_FLASH_ATTR uartSetInterruptHandler(void *pFunction)
{
    _xt_isr_attach(ETS_UART_INUM, pFunction); //FreeRTOS function to attach interrupt with callback function
}

//LOCAL void ICACHE_FLASH_ATTR uartRxIntHandler(void *para)
//{
//    /* uart0 and uart1 intr combine togther, when interrupt occur, see reg 0x3ff20020, bit2, bit0 represents
//    * uart1 and uart0 respectively
//    */
//    uint8 nUart = UART0;//UartDev.buff_uart_no;
//    uint8 nFifoLen = 0;
//    uint8 buf_idx = 0;
//
//    uint32_t nIntStatus = readReg(UART_INT_ST(nUart));
//
//    while(nIntStatus != 0x0)
//    {
//        if (UART_FRM_ERR_INT_ST == (nIntStatus & UART_FRM_ERR_INT_ST))
//        {
//            //Frame error
//            writeReg(UART_INT_CLR(nUart), UART_FRM_ERR_INT_CLR); //Clear frame error interrupt status flag
//        }
//        else if (UART_RXFIFO_FULL_INT_ST == (nIntStatus & UART_RXFIFO_FULL_INT_ST))
//        {
//            //Receive buffer full
//            nFifoLen = getRegBitsShifted(UART_STATUS(UART0), UART_RXFIFO_CNT, UART_RXFIFO_CNT_S);
//            buf_idx = 0;
//
//            while (buf_idx < nFifoLen)
//            {
//                uartSendChar(UART0, readReg(UART_FIFO(UART0)) & 0xFF);
//                buf_idx++;
//            }
//
//            writeReg(UART_INT_CLR(UART0), UART_RXFIFO_FULL_INT_CLR);
//        }
//        else if(UART_RXFIFO_TOUT_INT_ST == (nIntStatus & UART_RXFIFO_TOUT_INT_ST))
//        {
//            //Receive timeout
//            nFifoLen = getRegBitsShifted(UART_STATUS(UART0), UART_RXFIFO_CNT, UART_RXFIFO_CNT_S);
//            buf_idx = 0;
//
//            while(buf_idx < nFifoLen)
//            {
//                uartSendChar(UART0, readReg(UART_FIFO(UART0)) & 0xFF);
//                buf_idx++;
//            }
//
//            writeReg(UART_INT_CLR(UART0), UART_RXFIFO_TOUT_INT_CLR);
//        }
//        else if (UART_TXFIFO_EMPTY_INT_ST == (nIntStatus & UART_TXFIFO_EMPTY_INT_ST))
//        {
//            //Transmit buffer empty
//            writeReg(UART_INT_CLR(nUart), UART_TXFIFO_EMPTY_INT_CLR);
//            clearRegBits(UART_INT_ENA(UART0), UART_TXFIFO_EMPTY_INT_ENA);
//        }
//        else
//        {
//            //skip
//        }
//
//        nIntStatus = readReg(UART_INT_ST(nUart)) ;
//    }
//}

void ICACHE_FLASH_ATTR uartInit(UART_Port nUart, uint32_t nBitrate, void* pEventHandler)
{
	uartSetFlowCtrl(nUart, UART_HardwareFlowControl_None, 0);
	uartWaitTxFifoEmpty(nUart, 0);
	uartResetFifo(nUart);

    uartSetBaudrate(nUart, nBitrate);
    uartSetWordLength(nUart, UART_WordLength_8b);
    uartSetParity(nUart, UART_Parity_None);
    uartSetStopBits(nUart, UART_StopBits_1);
    uartSetSignalInvert(nUart, UART_INVERT_NONE);

    if(pEventHandler)
    {
    	uartEnableInterrupts(nUart, UART_RXFIFO_TOUT_INT_ENA | UART_FRM_ERR_INT_ENA | UART_RXFIFO_FULL_INT_ENA);
//    	uartEnableInterrupts(nUart, UART_RXFIFO_TOUT_INT_ENA);
        uartSetRxFifoFullThreshold(nUart, 10);
        uartSetRxTimeoutThreshold(nUart, 2);
        uartSetTxFifoEmptyThreshold(nUart, 20);
    	uartSetInterruptHandler(pEventHandler);
    	enableInterrupts(ETS_UART_INUM);
    }
}

void ICACHE_FLASH_ATTR uartSend(UART_Port nUart, uint8_t *pBuffer, uint16_t nLen)
{
  uint16_t i;
  for(i = 0; i < nLen; i++)
  {
    uartSendChar(nUart, pBuffer[i]);
  }
}

void ICACHE_FLASH_ATTR uartPrint(UART_Port nUart, const char *pString)
{
	while(*pString)
	{
		uartSendChar(nUart, *pString++);
	}
}

void ICACHE_FLASH_ATTR uartPrintln(UART_Port nUart, const char *pString)
{
	uartPrint(nUart, pString);
	uartPrint(nUart, "\r\n");
}

void ICACHE_FLASH_ATTR uartSetRxTimeoutThreshold(UART_Port nUart, uint32_t nThreshold)
{
	setRegBitsShifted(UART_CONF1(nUart), nThreshold, UART_RX_TOUT_THRHD, UART_RX_TOUT_THRHD_S);
}

void ICACHE_FLASH_ATTR uartSetTxFifoEmptyThreshold(UART_Port nUart, uint32_t nThreshold)
{
	setRegBitsShifted(UART_CONF1(nUart), nThreshold, UART_TXFIFO_EMPTY_THRHD, UART_TXFIFO_EMPTY_THRHD_S);
}

void ICACHE_FLASH_ATTR uartSetRxFifoFullThreshold(UART_Port nUart, uint32_t nThreshold)
{
	setRegBitsShifted(UART_CONF1(nUart), nThreshold, UART_RXFIFO_FULL_THRHD, UART_RXFIFO_FULL_THRHD_S);
}

void ICACHE_FLASH_ATTR uartDeleteMe(UART_Port nUart)
{
	uartPrintln(nUart, "I am a useless function");
}
