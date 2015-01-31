/*
*	Declares and implements low-level hardware functions for ESP8266 SOC
*	Copyright	riban 2015 GPL V2. Derived from eagle_soc.h Copyright (C) Espressif System
*	Brian Walton (brian@riban.co.uk)
*/
#pragma once

/*	Input and output pins may be configured for different functions
*	The following constants define the configuration register for each
*/
const static uint32_t IOMUX_CONFIG_REG_0	= 0x60000834; //GPI0, SPICS2, CLK_OUT
const static uint32_t IOMUX_CONFIG_REG_1	= 0x60000818; //UART0 Tx, SPICS1, GPI1, CLK_RTC_BK
const static uint32_t IOMUX_CONFIG_REG_2	= 0x60000838; //GPI2, I2SO_WS, UART1 TXD_BK, UART0 TXD_BK
const static uint32_t IOMUX_CONFIG_REG_3	= 0x60000814; //UART0 Rx, I2SO_DATA, GPI3, CLK_XTAL_BK
const static uint32_t IOMUX_CONFIG_REG_4	= 0x6000083C; //GPI4, CLK XTAL
const static uint32_t IOMUX_CONFIG_REG_5	= 0x60000840; //GPI5, CLK RTC
const static uint32_t IOMUX_CONFIG_REG_6	= 0x6000081C; //SD CLK, SPI CLK, GPI6, UART1 CTS
const static uint32_t IOMUX_CONFIG_REG_7	= 0x60000820; //SD Data 0, SPI MISO, GPI7, UART1 Tx
const static uint32_t IOMUX_CONFIG_REG_8	= 0x60000824; //SD Data 1, SPI MOSI, GPI8, UART1 Rx
const static uint32_t IOMUX_CONFIG_REG_9	= 0x60000828; //SD Data 2, SPI HD, GPI9, HSPI HD
const static uint32_t IOMUX_CONFIG_REG_10	= 0x6000082C; //SD Data 3, SPI WP, GPI10, HSPI WP
const static uint32_t IOMUX_CONFIG_REG_11	= 0x60000830; //SD Cmd, SPI CS0, GPI11, UART1 RTS
const static uint32_t IOMUX_CONFIG_REG_12	= 0x60000804; //MTDI, I2SI Data,HSPI MISO, GPI12, UART0 DTR
const static uint32_t IOMUX_CONFIG_REG_13	= 0x60000808; //MTCK, I2SI_BCK, HSPID_MOSI, GPI13, UART0 CTS
const static uint32_t IOMUX_CONFIG_REG_14	= 0x6000080C; //MTMS, I2SI_WS, HSPI_CLK, GPI14, UART0 DSR
const static uint32_t IOMUX_CONFIG_REG_15	= 0x60000810; //MTDO, I2SO_BCK, HSPI_CS0, GPI15, UART0 RTS

const static uint32_t IOMUX_CONFIG_0_GPI		= 0x00000000; //GPI 0
const static uint32_t IOMUX_CONFIG_0_SPICS2		= 0x00000010; //SPI Chip select 2
const static uint32_t IOMUX_CONFIG_0_CLKOUT		= 0x00000100; //Clock output

const static uint32_t IOMUX_CONFIG_1_UART0TX	= 0x00000000; //UART 0tTransmit data
const static uint32_t IOMUX_CONFIG_1_SPICS1		= 0x00000010; //SPI Chip select 1
const static uint32_t IOMUX_CONFIG_1_GPI		= 0x00000030; //GPI 1
const static uint32_t IOMUX_CONFIG_1_CLKRTCBK	= 0x00000100; //!@todo What is CLKRTCBK

const static uint32_t IOMUX_CONFIG_2_GPI		= 0x00000000; //GPI 2
const static uint32_t IOMUX_CONFIG_2_I2SOWS		= 0x00000010; //I2S Output word select
const static uint32_t IOMUX_CONFIG_2_UART1TXBK	= 0x00000020; //UART 1 Tx break condition?
const static uint32_t IOMUX_CONFIG_2_UART0TXBK	= 0x00000100; //UART 0 Tx break condition?

const static uint32_t IOMUX_CONFIG_3_UART0RX	= 0x00000000; //UART 0 Receive data
const static uint32_t IOMUX_CONFIG_3_I2SODATA	= 0x00000010; //I2S output data
const static uint32_t IOMUX_CONFIG_3_GPI		= 0x00000030; //GPI 3
const static uint32_t IOMUX_CONFIG_3_CLKXTALBK	= 0x00000100; //Clock crystal break condition?

const static uint32_t IOMUX_CONFIG_4_GPI		= 0x00000000; //GPI 4
const static uint32_t IOMUX_CONFIG_4_CLKXTAL	= 0x00000010; //Clock crystal

const static uint32_t IOMUX_CONFIG_5_GPI		= 0x00000000; //GPI 5
const static uint32_t IOMUX_CONFIG_5_CLKRTC		= 0x00000010; //Real time clock

const static uint32_t IOMUX_CONFIG_6_SDCLK		= 0x00000000; //SD clock
const static uint32_t IOMUX_CONFIG_6_SPICLK		= 0x00000010; //SPI clock
const static uint32_t IOMUX_CONFIG_6_GPI		= 0x00000030; //GPI 5
const static uint32_t IOMUX_CONFIG_6_UART1CTS	= 0x00000100; //UART 1 CTS

const static uint32_t IOMUX_CONFIG_7_SDDATA0	= 0x00000000; //SD data 0
const static uint32_t IOMUX_CONFIG_7_SPIMISO	= 0x00000010; //SPI MISO
const static uint32_t IOMUX_CONFIG_7_GPI		= 0x00000030; //GPI 7
const static uint32_t IOMUX_CONFIG_7_UART1TX	= 0x00000100; //UART 1 transmit data

const static uint32_t IOMUX_CONFIG_8_SDDATA1	= 0x00000000; //SD data 1
const static uint32_t IOMUX_CONFIG_8_SPIMOSI	= 0x00000010; //SPI MOSI
const static uint32_t IOMUX_CONFIG_8_GPI		= 0x00000020; //GPI 8
const static uint32_t IOMUX_CONFIG_8_UART1RX	= 0x00000100; //UART 1 recieve data

const static uint32_t IOMUX_CONFIG_9_SDDATA2	= 0x00000000; //SD data 2
const static uint32_t IOMUX_CONFIG_9_SPIHD		= 0x00000010; //!@todo What is SPIHD
const static uint32_t IOMUX_CONFIG_9_GPI		= 0x00000030; //GPI 9
const static uint32_t IOMUX_CONFIG_9_HSPIHD		= 0x00000100; //!@todo What is H SPI HD?

const static uint32_t IOMUX_CONFIG_10_SDDATA3	= 0x00000000; //SD Data 3
const static uint32_t IOMUX_CONFIG_10_SPIWP		= 0x00000010; //!@todo What is SPI WP?
const static uint32_t IOMUX_CONFIG_10_GPI		= 0x00000030; //GPI 10
const static uint32_t IOMUX_CONFIG_10_HSPIWP	= 0x00000100; //!@todo What is H SPI WP?

const static uint32_t IOMUX_CONFIG_11_SDCMD		= 0x00000000; //SD Command
const static uint32_t IOMUX_CONFIG_11_SPICS0	= 0x00000010; //SPI Chip select 0
const static uint32_t IOMUX_CONFIG_11_GPI		= 0x00000030; //GPI 11
const static uint32_t IOMUX_CONFIG_11_UART1RTS	= 0x00000100; //UART 1 RTS

const static uint32_t IOMUX_CONFIG_12_MTDI		= 0x0000000; //MT digital input
const static uint32_t IOMUX_CONFIG_12_I2S1DATA	= 0x0000010; //I2S input data
const static uint32_t IOMUX_CONFIG_12_HSPIMISO	= 0x0000020; //SPI host (slave) MISO
const static uint32_t IOMUX_CONFIG_12_GPI		= 0x0000030; //GPI 12
const static uint32_t IOMUX_CONFIG_12_UART0DTR	= 0x0000100; //UART 0 DTR

const static uint32_t IOMUX_CONFIG_13_MTCK		= 0x0000000; //MT clock
const static uint32_t IOMUX_CONFIG_13_I2SIBCK	= 0x0000010; //I2S input bit clock
const static uint32_t IOMUX_CONFIG_13_HSPIDMOSI	= 0x0000020; //SPI host (slave) MOSI
const static uint32_t IOMUX_CONFIG_13_GPI		= 0x0000030; //GPI 13
const static uint32_t IOMUX_CONFIG_13_UART0CTS	= 0x0000100; //UART 0 CTS

const static uint32_t IOMUX_CONFIG_14_MTMS		= 0x0000000; //MT MS???
const static uint32_t IOMUX_CONFIG_14_I2SIWS	= 0x0000010; //I2S input word select
const static uint32_t IOMUX_CONFIG_14_HSPICLK	= 0x0000020; //SPI host (slave) clock
const static uint32_t IOMUX_CONFIG_14_GPI		= 0x0000030; //GPI 14
const static uint32_t IOMUX_CONFIG_14_UART0DSR	= 0x0000100; //UART 0 DSR

const static uint32_t IOMUX_CONFIG_15_MTDO		= 0x0000000; //MT digital output
const static uint32_t IOMUX_CONFIG_15_I2SOBCK	= 0x0000010; //I2S output bit clock
const static uint32_t IOMUX_CONFIG_15_HSPI_CS0	= 0x0000020; //SPI host (slave) chip select 0
const static uint32_t IOMUX_CONFIG_15_GPI		= 0x0000030; //GPI 15
const static uint32_t IOMUX_CONFIG_15_UART0RTS	= 0x0000100; //UART 0 RTS

//Input / output multiplexer configuration register from pin (i = 0..15)
#define IOMUX_CONFIG_REG(i) 			\
    (i==0)	?	IOMUX_CONFIG_REG_0:		\
    (i==1)	?	IOMUX_CONFIG_REG_1:		\
    (i==2)	?	IOMUX_CONFIG_REG_2:		\
    (i==3)	?	IOMUX_CONFIG_REG_3:		\
    (i==4)	?	IOMUX_CONFIG_REG_4:		\
    (i==5)	?	IOMUX_CONFIG_REG_5:		\
    (i==6)	?	IOMUX_CONFIG_REG_6:		\
    (i==7)	?	IOMUX_CONFIG_REG_7:		\
    (i==8)	?	IOMUX_CONFIG_REG_8:		\
    (i==9)	?	IOMUX_CONFIG_REG_9:		\
    (i==10)	?	IOMUX_CONFIG_REG_10:	\
    (i==11)	?	IOMUX_CONFIG_REG_11:	\
    (i==12)	?	IOMUX_CONFIG_REG_12:	\
    (i==13)	?	IOMUX_CONFIG_REG_13:	\
    (i==14)	?	IOMUX_CONFIG_REG_14:	\
    			IOMUX_CONFIG_REG_15
    //!@todo GPI4 & GPI5 seem reversed on ESP-12 - been discussed in forum - maybe wrong silk screen on PCB

/*	The input / output multiplexer (mux) registers configure the routing of input and output pins 0 - 15 (not pin 16)
*	Each input / output pin has a register with a common set of configuration parameters:
*	Each pin may be configured as GPI or special (pin specific) functions.
*	Unfortunately, the function number for GPI differs on each pin (either 0 or 3).
*/
const static uint32_t IOMUX_ENABLE_OUTPUT				= 0x00000001;
const static uint32_t IOMUX_ENABLE_OUTPUT_IN_SLEEP 		= 0x00000002;
const static uint32_t IOMUX_ENABLE_PULLDOWN_IN_SLEEP	= 0x00000004;
const static uint32_t IOMUX_ENABLE_PULLUP_IN_SLEEP		= 0x00000008;
const static uint32_t IOMUX_ENABLE_PULLDOWN				= 0x00000040;
const static uint32_t IOMUX_ENABLE_PULLUP				= 0x00000080;
const static uint32_t IOMUX_FUNCTION_MASK				= 0x00000130;

const static uint32_t IOMUX_CONFIG_REG_SPICLK			= 0x60000800;
const static uint32_t IOMUX_SPI0_AT_SYSCLK				= 0x00000100;
const static uint32_t IOMUX_SPI1_AT_SYSCLK				= 0x00000200;

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

/** @brief  Set the function / mode for a input / output pin
*   @param  nPin Input /output pin
*   @param  nFunction Function (or mode) to select for pin
*/
void setPinFunction(uint8_t nPin, uint32_t nFunction);

/** @brief	Enable interrupts
*	@param	nType The interrupt type to enable
*/
void enableInterrupts(uint8_t nType);

/** @brief	Disable interrupts
*	@param	nType The interrupt type to disable
*/
void disableInterrupts(uint8_t nType);
