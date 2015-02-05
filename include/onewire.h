/*
*	onewire.h
*	Declares one-wire driver (e.g. for DS18B20 temperature sensor) for ESP8266 micro-controller
*	Copyright	riban 2015 GPL V2.
*	Brian Walton (brian@riban.co.uk)
*
*	Interfacing based on Maxim datasheet DS18B20 REV: 042208
*	Maxim one wire interface allows several devices to be connected a single wire bus.
*	All communication occurs over the bus which can also power devices in "parasite power" mode.
*	A ground reference wire is required and an optional power supply wire which means the interconnects have 2 or three wires.
*	Each device has a 64-bit unique serial code which is used to address individual devices.
*	There is a single bus master which this library provides and several slave devices.
*	The bus master controls timing of bus access using a write, wait, listen, read process.
*	Slave devices use a listen, read, wait, write process.
*	Data are transferred least significant bit first
*
*	Holding bus low for 480us or longer resets all components on the bus
*
*	Note: This library provides access to the DS18B20 temperature sensor. Other one-wire devices are available but may not be (fully) supported by the current code.
*	The DS18B20 specification states minimum supply voltage of 3V so the bus may be run directly from the ESP8266 without interfacing components.
*	The internal pull-up resistor is used to bias the data line, removing the need for external components (other than the sensors).
*
*	The DS18B20 presents its temperature as 9, 10, 11 or 12-bit sign-extended two's compliment value in degrees Celsius.
*	This means the unused, most significant bits are padded with zeros if the value is positive or ones if the value is negative.
*	For lower resolution values (9, 10 or 11-bit), the least significant bits of the temperature value are undefined.
*
*	DS18B20 serial number
*	[Mask]				[Purpose]
*	0x0000000000000011	8-bit family code [0x28]
*	0x0011111111111100	48-bit serial number
*	0x1100000000000000	8-bit CRC
*
*	DS18B20 memory layout
*	[Byte]	[Purpose]
*	0		Temperature LSB
*	1		Temperature MSB
*	2		High temperature alarm value - non-volatile (may be used as general purpose NVRAM byte if not using alarm feature)
*	3		Low temperature alarm value - non-volatile (may be used as general purpose NVRAM byte if not using alarm feature)
*	4		Configuration register (see below) - non-volatile
*	5		Reserved - do not overwrite
*	6		Reserved - do not overwrite
*	7		Reserved - do not overwrite
*	8		CRC of bytes 0 - 7
*
*	DS18B20 configuration register
*	[Mask]	[Purpose]
*	0x60	Resolution:	0x00 = 9-bit  (93.74ms conversion)
*						0x20 = 10-bit (187.5ms conversion)
*						0x40 = 11-bit (375ms conversion)
*						0x60 = 12-bit (750ms conversion) - power-up default
*	Do not write to any other bits of configuration register
*
*/
#pragma once

#include "c_types.h"

typedef enum
{
    ONEWIRE_POWER_AUTO		= 0,	//Auto-detect parasite power mode
    ONEWIRE_POWER_PARASITE,			//Use parasite power mode
    ONEWIRE_POWER_NORMAL			//Use normal power mode
} ONEWIRE_POWER_MODE;

static const uint8_t ONEWIRE_CMD_SEARCH_ROM		= 0xF0; //Scan bus for all devices
static const uint8_t ONEWIRE_CMD_READ_ROM		= 0x33; //Read device if only one on bus
static const uint8_t ONEWIRE_CMD_MATCH_ROM		= 0x55; //Specify which device to address on bus
static const uint8_t ONEWIRE_CMD_SKIP_ROM		= 0xCC; //Broadcast function to all devices on bus
static const uint8_t ONEWIRE_CMD_ALARM_SEARCH	= 0xEC; //Scan bus for all devices in alarm state

static uint32_t g_nOnewirePowerFlags = 0; //Bitwise flag for one-wire power mode for each pin
static uint16_t g_nOnewireAutoPowerFlags = 0; //Bitwise flag for one-wire auto power mode detection

/**	@brief	Initialise a one wire interface
*	@param	nPin The pin used for the one wire interface
*	@param	nPowerMode Power mode for bus (see ONEWIRE_POWER_MODE)
*	@note	Configures the specified pin as an input with pull-up resistor
*	@note	If using parasite power mode, no bus activity is permitted during temperature conversion in DS18B20 devices
*	@todo	It is possible to automatically detect parasite power mode devices
*/
void onewireInit(uint8_t nPin, ONEWIRE_POWER_MODE nPowerMode);

/**	@brief	One wire reset function.
*	@param	nPin The pin used for the one wire interface
*	@return	<i>bool</i> True on failure
*	@note	Waits up to 250uS for the bus to go high. If not then it is broken or shorted and we return a 0;
*	@note	This is the first step in the three step process to access devices
*/
bool onewireReset(uint8_t nPin);

/**	@brief	Checks for parasite power mode devices
*	@return	<i>bool</i> True if any devices detected using parasite power mode
*/
bool onewireIsParasite();

/**	@brief	Write a single bit
*	@param	nPin The pin used for the one wire interface
*	@param	bValue The value of the bit to write
*/
void onewireWriteBit(uint8_t nPin, bool bValue);

/**	@brief	Read a single bit
*	@param	nPin The pin used for the one wire interface
*	@return	<i>bool</i> Value of bit read
*/
bool onewireReadBit(uint8_t nPin);

/**	@brief	Write a byte
*	@param	nPin The pin used for the one wire interface
*	@param	nValue The value to write
*/
void onewireWriteByte(uint8_t nPin, uint8_t nValue);

/**	@brief	Read a byte
*	@param	nPin The pin used for the one wire interface
*	@return	<i>uint8_t</i> Value read
*/
uint8_t onewireReadByte(uint8_t nPin);

uint8_t g_nOneWireCrc;

/**	@brief	Resets the CRC checksum generator
*/
void onewireCrcReset();

/*	@brief	Adds a bit to the CRC checksum generator
*	@param	bBit Bit to add
*	@return	<i>bool</i> True if current CRC generation is valid (zero)
*	@note	Add each bit of data (LSB first) then expected CRC to check validation
*/
bool onewireCrcAddBit(bool bBit);
