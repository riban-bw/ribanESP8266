/**
 * 	Provides access to general purpose interface (GPI)
 *  Copyright riban 2015 GPL V2. Derived from gpio example Copyright (C) 2014 -2016  Espressif System
 *  Brian Walton (brian@riban.co.uk)
 */

#pragma once

#include "c_types.h"

//---Constants representing register addresses

/* GPI_OUTPUT Register providing parallel access all GPI output pins
*	Access: R/W
* 	[Mask]		[Purpose]
* 	0x0000FFFF	Current GPI value (pins 0 - 15)
* 	0xFFFF0000	BT select ???
*/
static const uint32_t GPI_OUTPUT = 0x60000300;

/*	GPI_SET Register allows setting of individual GPI pins
*	Access: WO
*	[Mask]		[Purpose]
*	0x0000FFFF	Setting a bit will assert the corresponding output (pins 0 - 15) but not affect other pins
*/
static const uint32_t GPI_SET = 0x60000304;

/*	GPI_CLEAR Register allows clearing of individual GPI pins
*	Access: WO
*	[Mask]		[Purpose]
*	0x0000FFFF	Setting a bit will clear the corresponding output (pins 0 - 15) but not affect other pins
*/
static const uint32_t GPI_CLEAR = 0x60000308;

/*	GPI_ENABLE_INPUT_OUTPUT Register providing parallel access to all GPI input / output configuration
*	Access: R/W
*	[Mask]		[Purpose]
*	0x0000FFFF	Current GPI configuration for each pin (pins 0 - 15) 0=Input, 1=Output
*	0x003F0000	SDIO select ????
*/
static const uint32_t GPI_ENABLE_INPUT_OUTPUT = 0x6000030C;

/*	GPI_ENABLE_OUTPUT Register allows configuration of each pin as an output
*	Access: WO
*	[Mask]		[Purpose]
*	0x0000FFFF	Setting a bit will configure the corresponding pin as output (pins 0 - 15) but not affect other pins
*/
static const uint32_t GPI_ENABLE_OUTPUT = 0x60000310;

/*	GPI_ENABLE_INPUT Register allows configuration of each pin as an input
*	Access: WO
*	[Mask]		[Purpose]
*	0x0000FFFF	Setting a bit will configure the corresponding pin as input (pins 0 - 15) but not affect other pins
*/
static const uint32_t GPI_ENABLE_INPUT= 0x60000314;

/*	GPI_INPUT	Register providing current GPI input values
*	Access: RO
*	[Mask]		[Purpose]
*	0x0000FFFF	Current GPI input value (pins 0 - 15)
*	0xFFFF0000	Strapping values - I think these may be related to the "pads" or pull-up resistors
*/
static const uint32_t GPI_INPUT = 0x60000318;

/*	GPIINTR_STATUS	Register providing current GPI interrupt status
*	[Mask]		[Purpose]
*	0x0000FFFF	Current interrupt status (pins 0 - 15)
*/
static const uint32_t GPIINTR_STATUS = 0x6000031c;

/*	GPIINTR_SET Register providing set of GPI interrupts
*	[Mask]		[Purpose]
*	0x0000FFFF	Set GPI interrupt (pins 0 - 15) but not affect other pins
*	I don't know what use this would be!
*/
static const uint32_t GPIINTR_SET = 0x60000320;

/*	GPIINTR_ACK Register providing reset or acknowledge of GPI interrupts
*	[Mask]		[Purpose]
*	0x0000FFFF	Acknowledge GPI interrupt (pins 0 - 15) but not affect other pins
*/
static const uint32_t GPIINTR_ACK = 0x60000324;

//	There is one configuration register per input / output pin.
//	Macro providing input / output configuration register for each pin
#define GPICONFIG_REG(i)	(0x60000328 + i * 4)
//	Bit masks or flags for each configuration element
static const uint32_t GPICONFIG_PWM				= 0x00000001; //Set for PWM, clear for GPI
static const uint32_t GPICONFIG_OPENDRAIN		= 0x00000004; //Set for open drain output, clear for normal output
static const uint32_t GPICONFIG_INT_TYPE_MASK	= 0x00000380; //See GPI_INT_TYPE for interrupt types
static const uint32_t GPICONFIG_WAKE_ON_INT		= 0x00000400; //Set to enable wake-up on interrupt (only when interrupt type is GPI_INTR_LOLEVEL or GPI_INTR_HILEVEL)
static const uint32_t GPICONFIG_CONFIG_MASK		= 0x00001800; //Unknown configuration bits

//	GPI16 is different
static const uint32_t GPI16_OUT					= 0x60000768; //Register to set GPI 16 output (bit 0)
static const uint32_t GPI16_ENABLE				= 0x60000774; //Register to enable GPI 16 output (bit 0: 0=input, 1=output)
static const uint32_t GPI16_IN					= 0x6000078C; //Register to read GPI 16 input (bit 0)
static const uint32_t GPI16_CONF				= 0x60000790; //Register to configure GPI 16 (set bit 1 for GPI)

//	PWM Sigma-Delta configuration
/*	GPIPWM_CONF Register providing configuration of PWM
*	[Mask]		[Purpose]
*	0x000000FF	Target pin (0 - 15)
*	0x0000FF00	Pre-scaler
*	0x00010000	Global PWM enable
*/
static const uint32_t GPIPWM_CONF = 0x60000368;
static const uint32_t GPIPWM_ENABLE	= 0x00010000;

//	RTC calibration configuration
/*	GPIRTC_SYNC Register providing configuration of RTC sync
*	[Mask]		[Purpose]
*	0x000003FF	Period
*	0x10000000	Start
*/
static const uint32_t GPIRTC_SYNC			= 0x6000036C;
static const uint32_t GPIRTC_PERIOD_MASK	= 0x000003FF;
static const uint32_t GPIRTC_START			= 0x10000000;


//---Enumerations of configuration options---
typedef enum
{
    GPI_INTR_DISABLE	= 0x00000000,	//Disable interrupts
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
*   @note	GPI16 only supports GPI_MODE_INPUT and GPI_MODE_OUTPUT
*/
void gpiSetMode(uint8_t nPin, GPI_MODE_TYPE nMode);

/** @brief  Set the interrupt type for a GPI pin
*   @param  nPin GPI pin to configure (0 - 15)
*   @param  nType Interrupt type (see GPI_INT_TYPE)
*   @note	This function also enables GPI interrupts globally (see enableInterrupts)
*   @note	Interrupts are not supported on GPI 16
*/
void gpiSetInterrupt(uint8_t nPin, GPI_INT_TYPE nType);

/** @brief  Read a GPI input value
*   @param  nPin Index of the GPI pin
*   @return <i>bool</i> True if GPI pin is high
*/
bool gpiRead(uint8_t nPin);

/** @brief  Get the value of all GPI input pins
*   @return <i>uint32</i> Value of all GPI inputs and strapping bits (?) presented as bitwise flags
*/
uint32_t gpiReadAll();

/** @brief  Set a GPI output value
*   @param  nPin Index of the GPI pin
*   @param  bValue Value to set GPI pin to
*   @note   Set pin to output mode before calling this function
*/
void gpiWrite(uint32_t nPin, bool bValue);

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
*   @note   Handler function has no parameters: void onGpiInt(); Use gpiGetTriggeredInterrupts to identify which interrupts have triggered.
*   @note	Call gpiAckInt to acknowledge interrupts
*/
void gpiSetInterruptHandler(void *pFunction);

/** @brief  Acknowledge interrupt
*	@param	nPin The pin whose interrupt to acknowledge
*	@note	Must call this function after a GPI interrupt before further GPI interrupts will occur
*/
void gpiAckIntr(uint8_t nPin);

/** @brief  Gets triggered GPI interrupts
*   @return <i>uint32_t</i> Bitwise mask of pending GPI interrupts (pins 0 - 15)
*/
uint32_t gpiGetTriggeredInterrupts();

/** @brief  Enable wake-up on a GPI interrupt
*   @param  nPin Index of GPI pin to trigger wake-up
*   @note   Only high level and low level interrupts can be used for wake-up
*/
void gpiEnableWakeOnInt(uint8_t nPin);

/** @brief  Disable wake-up on a GPI interrupt
*   @param  nPin Index of GPI pin
*/
void gpiDisableWakeOnInt(uint8_t nPin);

/**	@brief	Set the PWM period
*	@param	nPin GPI pin
*	@param	nPeriod Period of PWM
*/
void gpiSetPwmPeriod(uint8_t nPin, uint8_t nPeriod);

/**	@brief	Global enable of PWM
*/
void gpiEnablePwm();

/**	@brief	Global disable of PWM
*/
void gpiDisablePwm();
