# ribanESP8266
Library providing access to ESP8266 micro-controller hardware

This library provides global C functions to access the hardware within the ESP8266 micro-controller.
It is based on the Espressif IOT RTOS distribution but removes dependency on multiple levels of pre-processor
defines which some integrated design environments do not handle well.

This library provides simple access to each element of the hardware:

* GPI (general purpose interface) - these are the digital input and output pins
 * Digital Inputs
  * Enable / disable internal pull-up resistor
  * Enable / disable internal pull-down resistor
  * Configure interrupts
 * Digital Output
  * Open drain or push-pull
  * Sigma-delta PWM
* Analogue input
* Serial interfaces
 * UART
 * SDIO
 * SPI
 * I2C
* EEPROM access
* Sleep modes
 * Off
 * Deep sleep
 * Sleep
 * Wakeup
 * On

Note: Not all functions are currently implemented. Check wiki [Roadmap](https://github.com/riban-bw/ribanESP8266/wiki/Roadmap) for details.
