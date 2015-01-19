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
static uint32_t ICACHE_FLASH_ATTR readReg(uint32_t nRegister)
{
    return (*((volatile uint32_t *)(nRegister)));
}

/** @brief  Write to a value to a register
*   @param  nRegister The register to read
*   @param  nValue The value to write to the register
*/
static void ICACHE_FLASH_ATTR writeReg(uint32_t nRegister, uint32_t nValue)
{
    (*((volatile uint32_t *)(nRegister))) = nValue;
}

/** @brief  Clear bits within register
*   @param  nRegister The register to modify
*   @param  nMask Bitwise mask of bits to clear
*   @note   Set bits in nMask that should be cleared
*/
static void ICACHE_FLASH_ATTR clearRegBits(uint32_t nRegister, uint32_t nMask)
{
    writeReg(nRegister, readReg(nRegister) & (~nMask));
}

/** @brief  Set bits within register
*   @param  nRegister The register to modify
*   @param  nMask Bitwise mask of bits to set
*   @note   Set bits in nMask that should be set
*/
static void ICACHE_FLASH_ATTR setRegBits(uint32_t nRegister, uint32_t nMask)
{
    writeReg(nRegister, readReg(nRegister) | nMask);
}

/** @brief  Select an I/O mux register function
*   @param  nRegister I/O mux register
*   @param  nFunction Function to select
*/
static void ICACHE_FLASH_ATTR selectFunction(uint32_t nRegister, uint32_t nFunction)
{
    clearRegBits(nRegister, (PERIPHS_IO_MUX_FUNC << PERIPHS_IO_MUX_FUNC_S));
    setRegBits(nRegister, (((nFunction & BIT2) << 2) | (nFunction & 0x3)) << PERIPHS_IO_MUX_FUNC_S);
}
