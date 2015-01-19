/**
 * 	Provides access to general purpose interface (GPI)
 *  Copyright riban 2015 GPL V2. Derived from gpio example Copyright (C) 2014 -2016  Espressif System
 *  Brian Walton (brian@riban.co.uk)
 */

#pragma once

#include "esp8266/esp8266.h"
#include "esp8266/gpio_register.h"

//Bitwise flags for each pin
#define GPIO_PINMASK_0              (BIT(0))  /* Pin 0 selected */
#define GPIO_PINMASK_1              (BIT(1))  /* Pin 1 selected */
#define GPIO_PINMASK_2              (BIT(2))  /* Pin 2 selected */
#define GPIO_PINMASK_3              (BIT(3))  /* Pin 3 selected */
#define GPIO_PINMASK_4              (BIT(4))  /* Pin 4 selected */
#define GPIO_PINMASK_5              (BIT(5))  /* Pin 5 selected */
#define GPIO_PINMASK_6              (BIT(6))  /* Pin 6 selected */
#define GPIO_PINMASK_7              (BIT(7))  /* Pin 7 selected */
#define GPIO_PINMASK_8              (BIT(8))  /* Pin 8 selected */
#define GPIO_PINMASK_9              (BIT(9))  /* Pin 9 selected */
#define GPIO_PINMASK_10             (BIT(10)) /* Pin 10 selected */
#define GPIO_PINMASK_11             (BIT(11)) /* Pin 11 selected */
#define GPIO_PINMASK_12             (BIT(12)) /* Pin 12 selected */
#define GPIO_PINMASK_13             (BIT(13)) /* Pin 13 selected */
#define GPIO_PINMASK_14             (BIT(14)) /* Pin 14 selected */
#define GPIO_PINMASK_15             (BIT(15)) /* Pin 15 selected */
#define GPIO_PINMASK_All            (0xFFFF)  /* All pins selected */
//IMultiplex register for each pin
#define GPIO_PIN_REG_0          PERIPHS_IO_MUX_GPIO0_U
#define GPIO_PIN_REG_1          PERIPHS_IO_MUX_U0TXD_U
#define GPIO_PIN_REG_2          PERIPHS_IO_MUX_GPIO2_U
#define GPIO_PIN_REG_3          PERIPHS_IO_MUX_U0RXD_U
#define GPIO_PIN_REG_4          PERIPHS_IO_MUX_GPIO4_U
#define GPIO_PIN_REG_5          PERIPHS_IO_MUX_GPIO5_U
#define GPIO_PIN_REG_6          PERIPHS_IO_MUX_SD_CLK_U
#define GPIO_PIN_REG_7          PERIPHS_IO_MUX_SD_DATA0_U
#define GPIO_PIN_REG_8          PERIPHS_IO_MUX_SD_DATA1_U
#define GPIO_PIN_REG_9          PERIPHS_IO_MUX_SD_DATA2_U
#define GPIO_PIN_REG_10         PERIPHS_IO_MUX_SD_DATA3_U
#define GPIO_PIN_REG_11         PERIPHS_IO_MUX_SD_CMD_U
#define GPIO_PIN_REG_12         PERIPHS_IO_MUX_MTDI_U
#define GPIO_PIN_REG_13         PERIPHS_IO_MUX_MTCK_U
#define GPIO_PIN_REG_14         PERIPHS_IO_MUX_MTMS_U
#define GPIO_PIN_REG_15         PERIPHS_IO_MUX_MTDO_U
//Value register for each pin
#define GPIO_PIN_REG(i) \
    (i==0) ? GPIO_PIN_REG_0:  \
    (i==1) ? GPIO_PIN_REG_1:  \
    (i==2) ? GPIO_PIN_REG_2:  \
    (i==3) ? GPIO_PIN_REG_3:  \
    (i==4) ? GPIO_PIN_REG_4:  \
    (i==5) ? GPIO_PIN_REG_5:  \
    (i==6) ? GPIO_PIN_REG_6:  \
    (i==7) ? GPIO_PIN_REG_7:  \
    (i==8) ? GPIO_PIN_REG_8:  \
    (i==9) ? GPIO_PIN_REG_9:  \
    (i==10)? GPIO_PIN_REG_10: \
    (i==11)? GPIO_PIN_REG_11: \
    (i==12)? GPIO_PIN_REG_12: \
    (i==13)? GPIO_PIN_REG_13: \
    (i==14)? GPIO_PIN_REG_14: \
    GPIO_PIN_REG_15
//Address of each pin???
#define GPIO_PIN_ADDR(i)        (GPIO_PIN0_ADDRESS + i*4)
//Does reg_id index a pin
#define GPIO_ID_IS_PIN_REGISTER(reg_id) \
    ((reg_id >= GPIO_ID_PIN0) && (reg_id <= GPIO_ID_PIN(GPIO_PIN_COUNT-1)))
//Calculate the pin id which happens to be same as pin number because GPIO_ID_PIN0 = 0
#define GPIO_REGID_TO_PINIDX(reg_id) ((reg_id) - GPIO_ID_PIN0)

typedef enum
{
    GPI_INTR_DISABLE   = 0,	//Disable interrupts
    GPI_INTR_POSEDGE,       	//Enable interrupt on rising edge
    GPI_INTR_NEGEDGE,   	    //Enable interrupt on falling edge
    GPI_INTR_ANYEGDE,           //Enable interrupt on rising and falling edges
    GPI_INTR_LOLEVEL,   	    //Enable interrupt on low level
    GPI_INTR_HILEVEL,       	//Enable interrupt on high level
} GPI_INT_TYPE;

typedef enum
{
    GPI_MODE_INPUT	    = 0x0,	//Input
    GPI_MODE_INPUT_PULLUP, 	    //Input with internal pull-up resistor enabled
    GPI_MODE_INPUT_PULLDOWN, 	//Input with internal pull-down resistor enabled
    GPI_MODE_OUTPUT_OD,	        //Open drain output
    GPI_MODE_OUTPUT,		    //Push-pull output
    GPI_MODE_OUTPUT_PWM,   	    //Delta-Sigma PWM DAC output
} GPI_MODE_TYPE;

typedef enum
{
    GPI_PULLUP_DIS     = 0,    	//Disable internal pull-up resistor
    GPI_PULLUP_EN                  //Enable internal pull-up resistor
} GPI_PULLUP_TYPE;

typedef struct
{
    uint16		        PinMask;	//Bitwise mask of which GPIO pins are affected
    GPI_MODE_TYPE	    Mode;       //Input / output mode (see GPIOMode_TypeDef)
    GPI_PULLUP_TYPE    Pullup;     //Internal pull-up resistor state (see GPIO_Pullup_IF)
    GPI_INT_TYPE       Interrupt;  //Interupt type (see GPIO_INT_TYPE)
} GPI_Config;

/** @brief  Configure the mode of a GPI pin
*   @param  nPin Index of the GPI pin
*   @param  nMode Mode to set (GPIO_MODE_INPUT | GPIO_MODE_OUTPUT_OD | GPIO_MODE_OUTPUT | GPIO_MODE_OUTPUT_PWM)
*/
void gpiSetMode(uint8_t nPin, GPI_MODE_TYPE nMode);

/** @brief  Set the interrupt type for a GPI pin
*   @param  nPin GPI pin to configure
*   @param  nType Interrupt type (GPI_INTR_DISABLE | GPI_INTR_POSEDGE | GPI_INTR_NEGEDGE | GPI_INTR_ANYEGDE | GPI_INTR_LOLEVEL | GPI_INTR_HILEVEL)
*   @todo   Should gpiSetInterrupt be renamed to avoid ambiguity with configuring the overall interrupt mechanism and handlers?
*/
void gpiSetInterrupt(uint32_t nPin, GPI_INT_TYPE nType);

/** @brief  Read a GPI input value
*   @param  nPin Index of the GPI pin
*   @return <i>bool</i> True if GPI pin is high
*/
bool gpiRead(uint8 nPin);

/** @brief  Get the value of all GPI input pins
*   @return <i>uint32</i> Value of all GPI inputs presented as bitwise flags
*/
uint32_t gpiReadAll();

/** @brief  Set a GPI output value
*   @param  nPin Index of the GPI pin
*   @param  bValue Value to set GPI pin to
*   @note   Set pin to output mode before calling this function
*/
void gpiWrite(uint8 nPin, bool bValue);

/** @brief  Enable internal pull-up resistor
*   @param  nPin GPI pin
*   @todo   Do we require the ability to change the pull-up resistor configuration separate to configuring the pin mode?
*/
void gpiEnablePullup(uint8 nPin);

/** @brief  Disable internal pull-up resistor
*   @param  nPin GPI pin
*   @todo   Do we require the ability to change the pull-up resistor configuration separate to configuring the pin mode?
*/
void gpiDisablePullup(uint8 nPin);

/** @brief  Select GPI bank
*   @param  nPin GPI pin to select bank for
*   @note   Used internally by GPI driver
*   @todo   Should this be more generic - outside GPI driver?
*/
void gpiSelectBank(uint8_t nPin);

/** @brief  Register an event handler for GPI interrupts
*   @param  pFunction Pointer to the event handler (callback) function
*   @note   Each interrupt must be acknowledged by calling gpiAckInt. No further events will be generated until this is done.
*   @note   Handler accepts one argument: uint32_t which is a bitwise mask of pending GPI interrupts (one bit per pin)
*/
void gpiRegisterIntHandler(void *pFunction);

/** @brief  Acknowledge interrupt
*	@note	Must call this function after a GPI interrupt before further GPI interrupts will occur
*/
void gpiAckInt();

/** @brief  Gets pending GPI interrupts
*   @return <i>uint32_t</i> Bitwise mask of pending GPI interrupts
*/
uint32_t gpiGetPendingInterrupts();

/** @brief  Enable wake on a GPI interrupt
*   @param  nPin Index of GPI pin
*   @param  nState Interrupt state (GPI_INTR_LOLEVEL | GPI_INTR_HILEVEL)
*   @note   Only high level and low level interrupts can be used for wakeup
*/
void gpiEnableWakeOnInt(uint32_t nPin, GPI_INT_TYPE nState);

/** @brief  Disable wake on GPI interrupt
*/
void gpiDisableWakeOnInt();

/**	@brief	Set the PWM output value
*	@param  nPin Index of GPI pin
*   @param  nFreq PWM frequency
*   @param  nWidth PWM width
*/
void gpiSetPwm(uint32_t nPin, uint16_t nFreq, uint8_t nWidth);
