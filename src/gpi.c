/*
 *  Copyright (C) riban GPL V2
 *  Brian Walton brian@riban.co.uk
 *  Derived from code by Espressif System
 */

#include "freertos/portmacro.h"
#include "esp.h"
#include "gpi.h"

void ICACHE_FLASH_ATTR gpiSetMode(uint8 nPin, GPI_MODE_TYPE nMode)
{
    portENTER_CRITICAL();
//    clearRegBits(IOMUX_CONFIG_REG(nPin), IOMUX_ENABLE_OUTPUT | IOMUX_ENABLE_PULLUP | IOMUX_ENABLE_PULLDOWN | GPICONFIG_PWM | GPICONFIG_OPENDRAIN);
    writeReg(IOMUX_CONFIG_REG(nPin), 0); //Reset i/o pin
    clearRegBits(GPICONFIG_REG(nPin), GPICONFIG_PWM | GPICONFIG_OPENDRAIN);

    //Configure pin as GPI
    switch(nPin)
    {
        case 0:
        case 2:
        case 4:
        case 5:
        	//Pins 0,2,4&5 GPI function is 0
//            setRegBits(GPICONFIG_REG(nPin), IOMUX_CONFIG_0_GPI);
            break;
        default:
        	//Pins 1,3,6,7,8,9,10,11,12,13,14,15&16 GPI function is 3
            setRegBits(GPICONFIG_REG(nPin), IOMUX_CONFIG_1_GPI);
    }
    switch(nMode)
    {
        case GPI_MODE_INPUT:
            break;
        case GPI_MODE_INPUT_PULLUP:
        	setRegBits(IOMUX_CONFIG_REG(nPin), IOMUX_ENABLE_PULLUP);
            break;
        case GPI_MODE_INPUT_PULLDOWN:
        	//!@todo pull-down not working
        	setRegBits(IOMUX_CONFIG_REG(nPin), IOMUX_ENABLE_PULLDOWN);
            break;
        case GPI_MODE_OUTPUT:
        	setRegBits(GPICONFIG_REG(nPin), IOMUX_ENABLE_OUTPUT);
            break;
        case GPI_MODE_OUTPUT_OD:
        	setRegBits(GPICONFIG_REG(nPin), IOMUX_ENABLE_OUTPUT | GPICONFIG_OPENDRAIN);
            break;
        case GPI_MODE_OUTPUT_PWM:
            //!@todo Check PWM output works
        	setRegBits(GPICONFIG_REG(nPin), IOMUX_ENABLE_OUTPUT | GPICONFIG_PWM);
            break;
    }
    portEXIT_CRITICAL();
}

void ICACHE_FLASH_ATTR gpiSetInterrupt(uint8_t nPin, GPI_INT_TYPE nType)
{
    portENTER_CRITICAL();
    clearRegBits(GPICONFIG_REG(nPin), GPICONFIG_INT_TYPE_MASK);
    setRegBits(GPICONFIG_REG(nPin), nType);
    portEXIT_CRITICAL();
}

bool ICACHE_FLASH_ATTR gpiRead(uint8 nPin)
{
    return (gpiReadAll() & BIT(nPin)) == BIT(nPin);
}

uint32_t ICACHE_FLASH_ATTR gpiReadAll()
{
    return readReg(GPI_INPUT & 0x0000FFFF);
}

void ICACHE_FLASH_ATTR gpiWrite(uint8 nPin, bool bValue)
{
    if(bValue)
        setRegBits(GPI_OUTPUT, BIT(nPin));
    else
        clearRegBits(GPI_OUTPUT, BIT(nPin));
}

void ICACHE_FLASH_ATTR gpiSetInterruptHandler(void *pFunction)
{
    _xt_isr_attach(ETS_GPIO_INUM, pFunction);
}

void ICACHE_FLASH_ATTR gpiAckInt(uint8_t nPin)
{
	//!@todo Check gpiAckInt
	writeReg(GPI_INT_STATUS, nPin);
}

uint32_t ICACHE_FLASH_ATTR gpiGetPendingInterrupts()
{
	return readReg(GPI_INT_STATUS);
}

void ICACHE_FLASH_ATTR gpiEnableWakeOnInt(uint8_t nPin)
{
	portENTER_CRITICAL();
	setRegBits(GPICONFIG_REG(nPin), GPICONFIG_WAKE_ON_INT);
	portEXIT_CRITICAL();
}

void ICACHE_FLASH_ATTR gpiDisableWakeOnInt(uint8_t nPin)
{
	portENTER_CRITICAL();
	clearRegBits(GPICONFIG_REG(nPin), GPICONFIG_WAKE_ON_INT);
	portEXIT_CRITICAL();
}

void ICACHE_FLASH_ATTR gpiEnablePullup(uint8_t nPin)
{
    setRegBits(IOMUX_CONFIG_REG(nPin), IOMUX_ENABLE_PULLUP);
}

void ICACHE_FLASH_ATTR gpiDisablePullup(uint8_t nPin)
{
    clearRegBits(IOMUX_CONFIG_REG(nPin), IOMUX_ENABLE_PULLUP);
}

void ICACHE_FLASH_ATTR gpiSetPwm(uint8_t nPin, uint16_t nFreq, uint8_t nWidth)
{
    //!@todo Implement gpiSetPwm
}

void gpiSetPwmPeriod(uint8_t nPeriod)
{
	//!@todo implement gpiSetPwmPeriod
}
