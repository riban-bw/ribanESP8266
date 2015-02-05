/*
* 	ds18b20.h
*	Declares driver for DS18B20 temperature sensor for ESP8266 micro-controller
*	Copyright	riban 2015 GPL V2.
*	Brian Walton (brian@riban.co.uk)
*	Depends on ribanesp8266 onewire library
*/
#pragma once
#include "c_types.h"

static const uint8_t DS18B20_FUNC_CONVERT		= 0x44; //Begin temperature conversion
static const uint8_t DS18B20_FUNC_WRITE			= 0x4E; //Write to Th, Tl & Config registers
static const uint8_t DS18B20_FUNC_READ			= 0xBE; //Read 9 bytes from memory
static const uint8_t DS18B20_FUNC_COPY			= 0x48; //Copy Th, Tl & Config registers to EEPROM
static const uint8_t DS18B20_FUNC_RECALL		= 0xB8; //Recall Th, Tl & Config registers from EEPROM
static const uint8_t DS18B20_FUNC_POWER			= 0xB4; //Request power mode from all clients
