/*	onewire.h
*	Defines one-wire driver (e.g. for DS18B20 temperature sensor) for ESP8266 micro-controller
*	Copyright	riban 2015 GPL V2.
*	Brian Walton (brian@riban.co.uk)
*/

#include "onewire.h"
#include "c_types.h"
#include "gpi.h"
#include "esp.h"

void ICACHE_FLASH_ATTR onewireInit(uint8_t nPin, ONEWIRE_POWER_MODE nPowerMode)
{
	switch(nPowerMode)
	{
		//Set parasite power mode flag
		case ONEWIRE_POWER_AUTO:
			g_nOnewireAutoPowerFlags = g_nOnewireAutoPowerFlags | BIT(nPin);
			break;
		case ONEWIRE_POWER_NORMAL:
			g_nOnewireAutoPowerFlags = g_nOnewireAutoPowerFlags & ~BIT(nPin);
			g_nOnewirePowerFlags = g_nOnewirePowerFlags & ~BIT(nPin);
			break;
		case ONEWIRE_POWER_PARASITE:
			g_nOnewireAutoPowerFlags = g_nOnewireAutoPowerFlags & ~BIT(nPin);
			g_nOnewirePowerFlags = g_nOnewirePowerFlags | BIT(nPin);
			break;
	}
	gpiSetMode(nPin, GPI_MODE_INPUT_PULLUP);
	gpiWrite(nPin, true);
}

bool onewireReset(uint8_t nPin)
{
	//Pull bus low for 480us minimum
	gpiSetMode(nPin, GPI_MODE_OUTPUT);
	gpiWrite(nPin, false);
	usleep(480);
	gpiSetMode(nPin, GPI_MODE_INPUT_PULLUP);
	//Wait 15 - 60us
	uint8_t nWait = 30;
	while(!gpiRead(nPin))
	{
		if(0 == nWait--)
			return true; //Timed out waiting for data pin to go high
		usleep(2);
	}

	//Detect low pulses (60 - 240us wide) from devices during following 480us
	for(nWait = 0; nWait < 16; ++nWait)
	{
		if(!gpiRead(nPin))
			return false; //detected presence pulse
		usleep(30); //presence pulses should be 60us minimum so check at twice that rate
	}
	return true; //if we are here we have not detected any devices on the bus
}

bool ICACHE_FLASH_ATTR onewireIsParasite()
{
	//!@todo implement onewireIsParasite
	//Send Skip ROM command [0xCC]
	//Send Read Power Supply command [0xB4]
	//Check if bus is pulled low during read period
	return false; //!@todo Return true if bus pulled low during read period
}

void ICACHE_FLASH_ATTR onewireWriteBit(uint8_t nPin, bool bValue)
{
	gpiSetMode(nPin, GPI_MODE_OUTPUT);
	gpiWrite(nPin, true);
	if(bValue)
		usleep(10);
	else
		usleep(65);
	gpiSetMode(nPin, GPI_MODE_INPUT_PULLUP);
	usleep(5);
}

bool ICACHE_FLASH_ATTR onewireReadBit(uint8_t nPin)
{
	gpiSetMode(nPin, GPI_MODE_OUTPUT);
	gpiWrite(nPin, true);
	usleep(5);
	gpiSetMode(nPin, GPI_MODE_INPUT_PULLUP);
	usleep(10);
	bool bValue = gpiRead(nPin);
	usleep(60);
	return bValue;
}

void ICACHE_FLASH_ATTR onewireWriteByte(uint8_t nPin, uint8_t nValue)
{
	uint8_t nBit;
	for(nBit = 0; nBit < 8; ++nBit)
	{
		onewireWriteBit(nPin, BIT(nBit) == (nValue & BIT(nBit)));
	}
}

uint8_t ICACHE_FLASH_ATTR onewireReadByte(uint8_t nPin)
{
	//!@todo implement onewireReadByte
	uint8_t nBit;
	uint8_t nValue = 0;
	for(nBit = 0; nBit < 8; ++nBit)
		nValue &= (onewireReadBit(nPin) << nBit);
	return nValue;
}

void onewireCrcReset()
{
	g_nOneWireCrc = 0;
}

bool onewireCrcAddBit(bool bBit)
{
	g_nOneWireCrc = g_nOneWireCrc >> 1;
	//!@todo Set bit 7 of g_nOneWireCrc to g_nOneWireCrc[bit 0] XOR bBit
	//!@todo Set bit 3 of g_nOneWireCrc to g_nOneWireCrc[bit 3] XOR bBit
	//!@todo Set bit 2 of g_nOneWireCrc to g_nOneWireCrc[bit 2] XOR bBit
}
