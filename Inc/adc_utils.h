/**
  ******************************************************************************
  * @file    adc_utils.h
  * @author  Bartosz Rychlicki
  * @author  AGH Eko-Energia

  * @Title   Universal driver for ADC peripheral (Built for F1, F2, F3 and F4 families, but can be implemented for all families)

  * @brief   This file contains common defines, flags and macros that are used to extract ADC configuration from ADC registers
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 AGH Eko-Energy.
  * All rights reserved.
  *
  ******************************************************************************
  */

#ifndef ADC_UTILS_H_
#define ADC_UTILS_H_

#pragma once

/* Includes ------------------------------------------------------------------------------------------------ */
#include "stm32_family.h"
#include "main.h"


/* Macros -------------------------------------------------------------------------------------------------- */
#define ADC_MAX_RANKS_NUMBER (16)           //< Maximum number of ADC ranks
#define ADC_SQR3_2_OFFSET    (6)            //< Offset in reading SQR2 reg of ADC - used in shifting bits while reading ranks 7 - 12, cause for loop would trigger end of bound error
#define ADC_SQR3_1_OFFSET    (12)           //< Offset in reading SQR1 reg of ADC - used in shifting bits while reading ranks 13 - 16, cause for loop would trigger end of bound error
#define ADC_SQR_DATA_RES     (5)            //< Resolution of channel number in regs - generally 5 bits of data
#define ADC_SQR_DATA_Msk     (0x1F)         //< Mask for channel number in regs - generally 4:0 bits of data so 11111

#define ADC_POLLING_TIMEOUT  (500)			//< Macro stores max ADC polling timeout, to prevent endless blocking by polling

#define STM32_VCC            (3.3f)			//< STM32 supply voltage for reading ADC's pins' voltages
#define STM32_GND            (0.0f)         //< STM32 ground voltage for providing reference while reading ADC's pins' voltages


/* Exported variables --------------------------------------------------------------------------------------- */
extern ADC_HandleTypeDef hadc1;



/* Functions' prototypes ------------------------------------------------------------------------------------ */

/*
 * @ brief Function that returns binary type value of ADC Mode according to table presented below
 *
 * Binary Value | Description
   0000         | Independent mode.
   0001         | Combined regular simultaneous + injected simultaneous mode
   0010         | Combined regular simultaneous + alternate trigger mode
   0011         | Combined injected simultaneous + fast interleaved mode
   0100         | Combined injected simultaneous + slow Interleaved mode
   0101         | Injected simultaneous mode only
   0110         | Regular simultaneous mode only
   0111         | Fast interleaved mode only
   1000         | Slow interleaved mode only
   1001         | Alternate trigger mode only
 */
uint8_t ADC_GetMode(ADC_HandleTypeDef* hadc);

/*
 * @ brief Function that returns binary type value of ADC Continuous conversion mode
 * @ note  Continuous conversion mode on ADC indicated if ADC re-launch after sampling sequence of all channels
     or if HAL_ADC_Start should be called every time another  sampling sequence should be launched

 * @ return value:
   - DISABLED - 0
   - ENABLED  - 1
 */
uint8_t ADC_Continuous(ADC_HandleTypeDef* hadc);

/*
 * @ brief Function that returns binary type value of ADC Discontinuous conversion mode
 * @ note  Discontinuous conversion mode on ADC indicated if ADC sample one after one channel, or all channels continuously,
 	 	    - if DISABLED - then ADC read channel after channel with single HAL_ADC_Start call
 	 	    - if ENABLED  - then HAL_ADC_Start should be call to change cchannel to be scanned (for example channel0 -> channel1)

 * @ return value:
   - DISABLED - 0
   - ENABLED  - 1
 */
uint8_t ADC_Discontinuous(ADC_HandleTypeDef* hadc);


#endif /* ADC_UTILS_H_ */
