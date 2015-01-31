/**
 * 	Provides access to general purpose interface (GPI)
 *  Copyright riban 2015 GPL V2. Derived from gpio example Copyright (C) 2014 -2016  Espressif System
 *  Brian Walton (brian@riban.co.uk)
 */

#pragma once

#include "c_types.h"

//---Constants representing register addresses

/* GPI_OUTPUT Register providing access all GPI pins
 * 	[Mask]		[Purpose]
 * 	0x0000FFFF	Current GPI value (pins 0 - 15)
 * 	0xFFFF0000	BT select (Bluetooth?)
 */
static const uint32_t GPI_OUTPUT = 0x60000300;

/*	GPI_SET Register providing set individual GPI pins
 *	[Mask]		[Purpose]
 *	0x0000FFFF	Setting a bit will assert the corresponding output (pins 0 - 15)
 */
static const uint32_t GPI_SET = 0x60000304;

/*	GPI_CLEAR Register providing clear individual GPI pins
 *	[Mask]		[Purpose]
 *	0x0000FFFF	Setting a bit will clear the corresponding output (pins 0 - 15)
 */
static const uint32_t GPI_CLEAR = 0x60000308;

/*	GPI_ENABLE_OUTPUT Register providing access to all GPI input / output configuration
 *	[Mask]		[Purpose]
 *	0x0000FFFF	Current GPI configuration for each pin (pins 0 - 15) 0=Input, 1=Output
 */
static const uint32_t GPI_ENABLE_OUTPUT = 0x6000030C;

/*	GPI_ENABLE Register providing enable of GPI outputs
 *	[Mask]		[Purpose]
 *	0x0000FFFF	Setting a bit will configure the corresponding pin as output (pins 0 - 15)
 */
static const uint32_t GPI_CONFIGURE_OUTPUT = 0x60000300;

/*	GPI_DISABLE Register providing disable of GPI outputs
 *	[Mask]		[Purpose]
 *	0x0000FFFF	Setting a bit will configure the corresponding pin as input (pins 0 - 15)
 */
static const uint32_t GPI_CONFIGURE_INPUT= 0x6000030C;

/*	GPI_INPUT	Register providing current GPI input values
*	[Mask]		[Purpose]
*	0x0000FFFF	Current GPI input value (pins 0 - 15)
*	0xFFFF0000	Strapping values - I think these may be related to the "pads" or pull-up resistors
*/
static const uint32_t GPI_INPUT = 0x60000318;

/*	GPI_INT_STATUS	Register providing current GPI interrupt status
*	[Mask]		[Purpose]
*	0x0000FFFF	Current interrupt status (pins 0 - 15)
*/
static const uint32_t GPI_INT_STATUS = 0x60000324;

//Input / output configuration register for each pin
#define GPICONFIG_REG(i)	(0x60000328 + i * 4)

/*	There is one configuration register per input / output pin.
*	Each input / output pin has a register with a common set of configuration parameters:
*		[Mask]	[Purpose]
*		0x0001	0=GPI, 1=PWM
*		0x0004	0=Normal, 1=Open drain
*		0x0380	Interrupt type
*		0x0400	Wake-up enable (only when interrupt type is LOW or HIGH)
*		0x1800	Config ???
*/
static const uint32_t GPICONFIG_PWM				= 0x00000001;
static const uint32_t GPICONFIG_OPENDRAIN		= 0x00000004;
static const uint32_t GPICONFIG_INT_TYPE_MASK	= 0x00000380;
static const uint32_t GPICONFIG_WAKE_ON_INT		= 0x00000400;
static const uint32_t GPICONFIG_CONFIG_MASK		= 0x00001800;

//---Enumerations of configuration options---

typedef enum
{
    GPI_INTR_DISABLE	= 0,			//Disable interrupts
    GPI_INTR_POSEDGE	= 0x00000080,	//Enable interrupt on rising edge
    GPI_INTR_NEGEDGE	= 0x00000100,	//Enable interrupt on falling edge
    GPI_INTR_ANYEGDE	= 0x00000180,	//Enable interrupt on rising and falling edges
    GPI_INTR_LOLEVEL	= 0x00000200,	//Enable interrupt on low level
    GPI_INTR_HILEVEL	= 0x00000280,	//Enable interrupt on high level
} GPI_INT_TYPE;

typedef enum
{
    GPI_MODE_INPUT			= 0,	//Input
    GPI_MODE_INPUT_PULLUP,			//Input with internal pull-up resistor enabled
    GPI_MODE_INPUT_PULLDOWN,		//Input with internal pull-down resistor enabled
    GPI_MODE_OUTPUT_OD,				//Open drain output
    GPI_MODE_OUTPUT,				//Push-pull output
    GPI_MODE_OUTPUT_PWM,			//Delta-Sigma PWM DAC output
} GPI_MODE_TYPE;


//---Function definitions---

/** @brief  Configure the mode of a GPI pin
*   @param  nPin Index of the GPI pin
*   @param  nMode Mode to set (see GPI_MODE_TYPE)
*   @todo	GPI_MODE_INPUT_PULLDOWN does not work. Reported to Espressif on github issue #16
*/
void gpiSetMode(uint8_t nPin, GPI_MODE_TYPE nMode);

/** @brief  Set the interrupt type for a GPI pin
*   @param  nPin GPI pin to configure
*   @param  nType Interrupt type (see GPI_INT_TYPE)
*   @todo   Should gpiSetInterrupt be renamed to avoid ambiguity with configuring the overall interrupt mechanism and handlers?
*/
void gpiSetInterrupt(uint8_t nPin, GPI_INT_TYPE nType);

/** @brief  Read a GPI input value
*   @param  nPin Index of the GPI pin
*   @return <i>bool</i> True if GPI pin is high
*/
bool gpiRead(uint8_t nPin);

/** @brief  Get the value of all GPI input pins
*   @return <i>uint32</i> Value of all GPI inputs presented as bitwise flags
*/
uint32_t gpiReadAll();

/** @brief  Set a GPI output value
*   @param  nPin Index of the GPI pin
*   @param  bValue Value to set GPI pin to
*   @note   Set pin to output mode before calling this function
*/
void gpiWrite(uint8_t nPin, bool bValue);

/** @brief  Enable internal pull-up resistor
*   @param  nPin GPI pin
*   @todo   Do we require the ability to change the pull-up resistor configuration separate to configuring the pin mode?
*/
void gpiEnablePullup(uint8_t nPin);

/** @brief  Disable internal pull-up resistor
*   @param  nPin GPI pin
*   @todo   Do we require the ability to change the pull-up resistor configuration separate to configuring the pin mode?
*/
void gpiDisablePullup(uint8_t nPin);

/** @brief  Register an event handler for GPI interrupts
*   @param  pFunction Pointer to the event handler (callback) function
*   @note   Each interrupt must be acknowledged by calling gpiAckInt. No further events will be generated until this is done.
*   @note   Handler accepts one argument: uint32_t which is a bitwise mask of pending GPI interrupts (one bit per pin)
*/
void gpiSetInterruptHandler(void *pFunction);

/** @brief  Acknowledge interrupt
*	@param	nPin The pin whose interrupt to acknowledge
*	@note	Must call this function after a GPI interrupt before further GPI interrupts will occur
*/
void gpiAckInt(uint8_t nPin);

/** @brief  Gets pending GPI interrupts
*   @return <i>uint32_t</i> Bitwise mask of pending GPI interrupts
*/
uint32_t gpiGetPendingInterrupts();

/** @brief  Enable wake-up on a GPI interrupt
*   @param  nPin Index of GPI pin to trigger wake-up
*   @note   Only high level and low level interrupts can be used for wake-up
*/
void gpiEnableWakeOnInt(uint8_t nPin);

/** @brief  Disable wake-up on a GPI interrupt
*   @param  nPin Index of GPI pin
*/
void gpiDisableWakeOnInt(uint8_t nPin);

/**	@brief	Set the PWM output value
*	@param  nPin Index of GPI pin
*   @param  nFreq PWM frequency
*   @param  nWidth PWM width
*/
void gpiSetPwm(uint8_t nPin, uint16_t nFreq, uint8_t nWidth);


/**	@brief	Set the PWM period
*	@param	nPeriod Period of PWM
*/
void gpiSetPwmPeriod(uint8_t nPeriod);
