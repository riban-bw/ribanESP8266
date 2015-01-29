/*
 *  Copyright (C) riban GPL V2
 *  Brian Walton brian@riban.co.uk
 *  Derived from code by Espressif System
 */

#include "espressif/esp_common.h"
#include "freertos/portmacro.h"
#include "esp.h"
#include "gpi.h"

void ICACHE_FLASH_ATTR gpiSetMode(uint8 nPin, GPI_MODE_TYPE nMode)
{
    switch(nMode)
    {
		case GPI_MODE_INPUT:
		case GPI_MODE_INPUT_PULLDOWN:
		case GPI_MODE_INPUT_PULLUP:
			//Clear output enable bit - setting GPI to input
	        writeReg(PERIPHS_GPIO_BASEADDR + GPIO_ENABLE_W1TC_ADDRESS, BIT(nPin));
	        break;
		case GPI_MODE_OUTPUT:
		case GPI_MODE_OUTPUT_OD:
		case GPI_MODE_OUTPUT_PWM:
			//Set output enable bit - setting GPI to output
			writeReg(PERIPHS_GPIO_BASEADDR + GPIO_ENABLE_W1TS_ADDRESS, BIT(nPin));
			break;
		default:
			return; //Mode not defined
    }
    //Set pin mode to GPI
    switch(nPin)
    {
        case 0:
        case 2:
        case 4:
        case 5:
        	//Pins 0,2,4&5 GPI function is 0
            selectFunction(GPIO_PIN_REG(nPin), 0);
            break;
        default:
        	//Pins 1,3,6,7,8,9,10,11,12,13,14,15&16 GPI function is 3
            selectFunction(GPIO_PIN_REG(nPin), 3);
    }
    //Disable internal pull-up and pull-down resistors
    clearRegBits(GPIO_PIN_REG(nPin), PERIPHS_IO_MUX_PULLUP);
    clearRegBits(GPIO_PIN_REG(nPin), PERIPHS_IO_MUX_PULLDWN);
    uint32_t nValue;
    switch(nMode)
    {
        case GPI_MODE_INPUT_PULLUP:
            setRegBits(GPIO_PIN_REG(nPin), PERIPHS_IO_MUX_PULLUP);
            break;
        case GPI_MODE_INPUT_PULLDOWN:
        	printf("Enabling pull-down\n");
            setRegBits(GPIO_PIN_REG(nPin), PERIPHS_IO_MUX_PULLDWN);
            break;
        case GPI_MODE_OUTPUT_OD:
            //!@todo Check open drain output works
            portENTER_CRITICAL();
            nValue = readReg(PERIPHS_GPIO_BASEADDR + GPIO_PIN_ADDR(nPin));
            nValue &= (~GPIO_PIN_DRIVER_MASK);
            nValue |= (GPIO_PAD_DRIVER_ENABLE << GPIO_PIN_DRIVER_LSB);
            writeReg(PERIPHS_GPIO_BASEADDR + GPIO_PIN_ADDR(nPin), nValue );
            portEXIT_CRITICAL();
            break;
        case GPI_MODE_OUTPUT_PWM:
            //!@todo Check PWM output works
            portENTER_CRITICAL();
            nValue = readReg(PERIPHS_GPIO_BASEADDR + GPIO_PIN_ADDR(nPin));
            nValue &= (~GPIO_PIN_SOURCE_MASK);
            nValue |= (BIT(GPIO_PIN_SOURCE_LSB));
            writeReg(PERIPHS_GPIO_BASEADDR + GPIO_PIN_ADDR(nPin), GPIO_PIN_REG(nPin));
            writeReg(PERIPHS_GPIO_BASEADDR + GPIO_SIGMA_DELTA_ADDRESS, SIGMA_DELTA_ENABLE);
            portEXIT_CRITICAL();
            break;
    }
}

void ICACHE_FLASH_ATTR gpiSetInterrupt(uint32_t nPin, GPI_INT_TYPE nType)
{
    //!@todo Demo sets pin mode to GPI - should we do it?
    uint32 nValue;

    portENTER_CRITICAL();

    nValue = readReg(PERIPHS_GPIO_BASEADDR + GPIO_PIN_ADDR(nPin));
    nValue &= (~GPIO_PIN_INT_TYPE_MASK);
    nValue |= (nType << GPIO_PIN_INT_TYPE_LSB);
    writeReg(PERIPHS_GPIO_BASEADDR + GPIO_PIN_ADDR(nPin), nValue);

    portEXIT_CRITICAL();
}

bool ICACHE_FLASH_ATTR gpiRead(uint8 nPin)
{
    return(gpiReadAll() & (1 << nPin)) == (1 << nPin);
}

uint32_t ICACHE_FLASH_ATTR gpiReadAll()
{
    return readReg(PERIPHS_GPIO_BASEADDR + GPIO_IN_ADDRESS);
}

void ICACHE_FLASH_ATTR gpiWrite(uint8 nPin, bool bValue)
{
    if(bValue)
        writeReg(PERIPHS_GPIO_BASEADDR + GPIO_OUT_W1TS_ADDRESS, (1 << nPin));
    else
        writeReg(PERIPHS_GPIO_BASEADDR + GPIO_OUT_W1TC_ADDRESS, (1 << nPin));
}


void ICACHE_FLASH_ATTR gpiSetInterruptHandler(void *pFunction)
{
	//!@todo Implement gpiRegisterIntHandler
    _xt_isr_attach(ETS_GPIO_INUM, pFunction);
}

void ICACHE_FLASH_ATTR gpiAckInt()
{
    //!@todo Implement gpiAckInt
}

uint32_t ICACHE_FLASH_ATTR gpiGetPendingInterrupts()
{
    //!@todo Implement gpiGetPendingInterrupts
	return 0;
}

void ICACHE_FLASH_ATTR gpiEnableWakeOnInt(uint32_t nPin, GPI_INT_TYPE nState)
{
    uint32_t nReg;
    if((GPI_INTR_LOLEVEL == nState) || (GPI_INTR_HILEVEL == nState))
    {
        portENTER_CRITICAL();
        nReg = readReg(PERIPHS_GPIO_BASEADDR + GPIO_PIN_ADDR(nPin));
        nReg &= (~GPIO_PIN_INT_TYPE_MASK);
        nReg |= (nState << GPIO_PIN_INT_TYPE_LSB);
        nReg |= GPIO_PIN_WAKEUP_ENABLE_SET(GPIO_WAKEUP_ENABLE);
        writeReg(PERIPHS_GPIO_BASEADDR + GPIO_PIN_ADDR(nPin), nReg);
        portEXIT_CRITICAL();
    }
}

void ICACHE_FLASH_ATTR gpiDisableWakeOnInt()
{
    uint8_t nPin;
    uint32_t nReg;

    for(nPin = 0; nPin < GPIO_PIN_COUNT; ++nPin)
    {
        nReg = readReg(PERIPHS_GPIO_BASEADDR + GPIO_PIN_ADDR(nPin));

        if (nReg & GPIO_PIN_WAKEUP_ENABLE_MASK)
        {
            nReg &= (~GPIO_PIN_INT_TYPE_MASK);
            nReg |= (GPI_INTR_DISABLE << GPIO_PIN_INT_TYPE_LSB);
            nReg &= ~(GPIO_PIN_WAKEUP_ENABLE_SET(GPIO_WAKEUP_ENABLE));
            writeReg(PERIPHS_GPIO_BASEADDR + GPIO_PIN_ADDR(nPin), nReg);
        }
    }
}

void ICACHE_FLASH_ATTR gpiEnablePullup(uint8_t nPin)
{
    setRegBits(PERIPHS_IO_MUX_PULLUP, BIT(nPin));
}

void ICACHE_FLASH_ATTR gpiDisablePullup(uint8_t nPin)
{
    clearRegBits(PERIPHS_IO_MUX_PULLUP, BIT(nPin));
}

void ICACHE_FLASH_ATTR gpiSetPwm(uint32_t nPin, uint16_t nFreq, uint8 nWidth)
{
    //!@todo Implement gpiSetPwm
}