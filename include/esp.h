/*
*	Declares and implements low-level hardware functions for ESP8266 SOC
*	Copyright	riban 2015 GPL V2. Derived from eagle_soc.h Copyright (C) Espressif System
*	Brian Walton (brian@riban.co.uk)
*/

#pragma once

/** @brief  Read from a register
*   @param  nRegister The register to read
*   @return <i>uint32_t</i> Value read from register
*/
uint32_t readReg(uint32_t nRegister);

/** @brief  Write to a value to a register
*   @param  nRegister The register to read
*   @param  nValue The value to write to the register
*/
void writeReg(uint32_t nRegister, uint32_t nValue);

/** @brief  Clear bits within register
*   @param  nRegister The register to modify
*   @param  nMask Bitwise mask of bits to clear
*   @note   Set bits in nMask that should be cleared - other bits are unchanged
*/
void clearRegBits(uint32_t nRegister, uint32_t nMask);

/** @brief  Set bits within register
*   @param  nRegister The register to modify
*   @param  nMask Bitwise mask of bits to set
*   @note   Set bits in nMask that should be set - other bits are unchanged
*/
void setRegBits(uint32_t nRegister, uint32_t nMask);

/**	@brief	Set a range of bits within a register
*	@param	nRegister The register to modify
*	@param	nValue The value of the bits to set, right justified
*	@param	nMask Bitwise mask of the bits to modify, right justified, i.e. presented as LSBs
*	@param	nShift How far left to shift the mask and value
*	@note	This function takes a value, shifts it by a specified number of bits and sets that range within a register
*/
void setRegBitsShifted(uint32_t nRegister, uint32_t nValue, uint32_t nMask, uint8_t nShift);

/**	@brief	Get a range of bits within a register
*	@param	nRegister Register to extract bits from
*	@param	nMask Bitwise mask of the bits to modify, right justified, i.e. presented as LSBs
*	@param	nShift How far left to shift the mask and value
*	@return <i>uint32_t</i> Extracted value shifted to baseband
*	@see	setRegBitsShifted
*/
uint32_t getRegBitsShifted(uint32_t nRegister, uint32_t nMask, uint8_t nShift);

/** @brief  Select an I/O mux register function
*   @param  nRegister I/O mux register
*   @param  nFunction Function to select
*/
void selectFunction(uint32_t nRegister, uint32_t nFunction);

/** @brief	Enable interrupts
*	@param	nType The interrupt type to enable
*/
void enableInterrupts(uint8_t nType);

/** @brief	Disable interrupts
*	@param	nType The interrupt type to disable
*/
void disableInterrupts(uint8_t nType);
