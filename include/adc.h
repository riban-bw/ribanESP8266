/**
 * 	Provides access to analogue to digital converter
 *  Copyright riban 2015 GPL V2.
 *  Brian Walton (brian@riban.co.uk)
 */

#pragma once

#include "c_types.h"

#define adcRead system_adc_read

/** @brief  Get the analogue input value
*	@return	<i>uint32_t</i> Analogue value 0 - 1024 (0V - 1V)
*/
uint32_t adcRead();
