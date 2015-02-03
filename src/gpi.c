/*
 *  Copyright (C) riban GPL V2
 *  Brian Walton brian@riban.co.uk
 *  Derived from code by Espressif System
 */

#include "freertos/portmacro.h"
#include "esp.h"
#include "gpi.h"

void ICACHE_FLASH_ATTR gpiSetMode(uint8_t nPin, GPI_MODE_TYPE nMode)
{
	if(16 == nPin)
	{
		//GPI pin 16 is handled differently to GPI 0 - 15
		switch(nMode)
		{
			case GPI_MODE_INPUT:
				clearRegBits(IOMUX_CONFIG_REG_16, IOMUX_CONFIG_16_MASK);
				setRegBits(IOMUX_CONFIG_REG_16, IOMUX_CONFIG_16_GPI);
				portENTER_CRITICAL();
				clearRegBits(GPI16_CONF, 0x00000001); //mux configuration for out enable
				clearRegBits(GPI16_ENABLE, 0x00000001); //out enable
				portEXIT_CRITICAL();
				break;
			case GPI_MODE_OUTPUT:
				//!@todo GPI16 output not working. (Error may be in gpiWrite)
				clearRegBits(IOMUX_CONFIG_REG_16, IOMUX_CONFIG_16_MASK);
				setRegBits(IOMUX_CONFIG_REG_16, IOMUX_CONFIG_16_GPI);
				portENTER_CRITICAL();
				clearRegBits(GPI16_CONF, 0x00000001); //mux configuration for out enable
				setRegBits(GPI16_ENABLE, 0x00000001); //out enable
				portEXIT_CRITICAL();
				break;
		}
    	return;
	}


    writeReg(IOMUX_CONFIG_REG(nPin), 0); //Reset i/o pin: Input, pull-up/down disabled, function 0 (pin specific)
	portENTER_CRITICAL();
	clearRegBits(GPICONFIG_REG(nPin), GPICONFIG_PWM | GPICONFIG_OPENDRAIN); //Disable PWM and open-drain features - these are set later if specified
	portEXIT_CRITICAL();

	//Configure pin as GPI
    switch(nPin)
    {
        case 0:
        case 2:
        case 4:
        case 5:
        	//Pins 0,2,4&5 GPI function is 0
        	//Do nothing because we have already configured correctly as function 0
//            setRegBits(IOMUX_CONFIG_REG(nPin), IOMUX_CONFIG_0_GPI);
            break;
        default:
        	//Pins 1,3,6,7,8,9,10,11,12,13,14,15&16 GPI function is 3
            setRegBits(IOMUX_CONFIG_REG(nPin), IOMUX_CONFIG_1_GPI);
    }

    switch(nMode)
    {
        case GPI_MODE_INPUT:
        	writeReg(GPI_ENABLE_INPUT, BIT(nPin));
            break;
        case GPI_MODE_INPUT_PULLUP:
        	writeReg(GPI_ENABLE_INPUT, BIT(nPin));
        	setRegBits(IOMUX_CONFIG_REG(nPin), IOMUX_ENABLE_PULLUP);
            break;
        case GPI_MODE_INPUT_PULLDOWN:
        	//!@todo pull-down not working - Espressif github issue #16
        	writeReg(GPI_ENABLE_INPUT, BIT(nPin));
        	setRegBits(IOMUX_CONFIG_REG(nPin), IOMUX_ENABLE_PULLDOWN);
            break;
        case GPI_MODE_OUTPUT:
        	//!@todo Can we avoid using the Espressif code to set i/o and write directly to configuration registers? Previous attempt failed.
//        	setRegBits(IOMUX_CONFIG_REG(nPin), IOMUX_ENABLE_OUTPUT);
        	writeReg(GPI_ENABLE_OUTPUT, BIT(nPin));
            break;
        case GPI_MODE_OUTPUT_OD:
        	writeReg(GPI_ENABLE_OUTPUT, BIT(nPin));
        	portENTER_CRITICAL();
        	setRegBits(GPICONFIG_REG(nPin), GPICONFIG_OPENDRAIN);
        	portEXIT_CRITICAL();
            break;
        case GPI_MODE_OUTPUT_PWM:
            //!@todo Check PWM output works
        	writeReg(GPI_ENABLE_OUTPUT, BIT(nPin));
        	portENTER_CRITICAL();
        	setRegBits(GPICONFIG_REG(nPin), GPICONFIG_PWM);
        	portEXIT_CRITICAL();
            break;
    }
}

void ICACHE_FLASH_ATTR gpiSetInterrupt(uint8_t nPin, GPI_INT_TYPE nType)
{
	if(nPin > 15)
		return;
    portENTER_CRITICAL();
    clearRegBits(GPICONFIG_REG(nPin), GPICONFIG_INT_TYPE_MASK);
    setRegBits(GPICONFIG_REG(nPin), nType);
    portEXIT_CRITICAL();
    enableInterrupts(INTR_TYPE_GPI);
}

bool ICACHE_FLASH_ATTR gpiRead(uint8_t nPin)
{
	if(16 == nPin)
		return (readReg(GPI16_IN) == 0x00000001);
	else
		return (gpiReadAll() & BIT(nPin)) == BIT(nPin);
}

uint32_t ICACHE_FLASH_ATTR gpiReadAll()
{
    return readReg(GPI_INPUT);
}

void ICACHE_FLASH_ATTR gpiWrite(uint32_t nPin, bool bValue)
{
    if(bValue)
    {
    	if(16 == nPin)
    		setRegBits(GPI16_OUT, BIT(1)); //!@todo Setting GPI16 not working (error may be in setMode)
    	else
    		writeReg(GPI_SET, BIT(nPin));
    }
    else
    {
    	if(16 == nPin)
    		clearRegBits(GPI16_OUT, BIT(1));
    	else
    		writeReg(GPI_CLEAR, BIT(nPin));
    }
}

void ICACHE_FLASH_ATTR gpiSetInterruptHandler(void *pFunction)
{
    _xt_isr_attach(ETS_GPIO_INUM, pFunction);
}

void ICACHE_FLASH_ATTR gpiAckIntr(uint8_t nPin)
{
	setRegBits(GPIINTR_ACK, BIT(nPin));
}

uint32_t ICACHE_FLASH_ATTR gpiGetTriggeredInterrupts()
{
	return readReg(GPIINTR_STATUS);
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

void ICACHE_FLASH_ATTR gpiSetPwmPeriod(uint8_t nPin, uint8_t nPeriod)
{
	//!@todo gpiSetPwmPeriod needs some revision - partially works
	writeReg(GPIPWM_CONF, nPin | (nPeriod << 8) | GPIPWM_ENABLE);
}

void ICACHE_FLASH_ATTR gpiEnablePwm()
{
	setRegBits(GPIPWM_CONF, GPIPWM_ENABLE);
}

void ICACHE_FLASH_ATTR gpiDisablePwm()
{
	clearRegBits(GPIPWM_CONF, GPIPWM_ENABLE);
}
