
/**
  ******************************************************************************
  * @file      adc_driver.c
  * @author    Bartosz Rychlicki
  * @Title     Universal driver for ADC peripheral
  * @brief     This file contains common functions' bodies with ADC Driver's functions implementation
  ******************************************************************************
  * @attention Error codes are called when exact incorrect use of function is made
  *
  * Copyright (c) 2025 AGH Eko-Energy.
  * All rights reserved.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#include "adc_driver.h"



/* Functions' bodies ------------------------------------------------------------------------------------ */

/*
 * @brief Function executes initialize ADC for Continuous flag = DISABLED, Discontinuous flag = DISABLED, No DMA support, ADC in independent mode
 */
static HAL_StatusTypeDef ADC_Init_NoDMA_Independent_Discontinuous(ADC_HandleTypeDef* hadc){

	// checking if user gave null pointers
	if(NULL == hadc){
		return HAL_ERROR;
	}


	// ADC currently started

	return HAL_OK;
}


/*
 * @brief Function executes read channel command for Continuous flag = DISABLED, Discontinuous flag = DISABLED, No DMA support, ADC in independent mode
 */
static HAL_StatusTypeDef ADC_ReadChannel_NoDMA_Independent_Discontinuous(ADC_HandleTypeDef* hadc, ADC_ChannelsConfigTypeDefs* cadc, uint8_t rank, uint16_t* retval){

	// declaration of variable, which is in charge of storing sampled value from ADC pin
	uint16_t binaryType = 0;


	// starting ADC every polling launch
	if(HAL_ADC_Start(hadc) != HAL_OK){
		return HAL_ERROR;
	}

	// iterating to correct rank number and providing for polling through all channels, to reset reading sequence
	for(uint8_t i = 0; i < cadc->numberOfSelectedChannels; ++i){


		// declaration of temporary variable to store sampled value from ADC
		uint16_t tempSampledValue = 0;


		// launching polling to read sampled value
		if(HAL_ADC_PollForConversion(hadc, ADC_POLLING_TIMEOUT) != HAL_OK){
			return HAL_ERROR;
		}


		// using scopes for providing optimized memory usage, and reading sampled value
		tempSampledValue = (uint16_t)HAL_ADC_GetValue(hadc);


		{
			// checking if current iteration is associated with reading selected channel
			if(i == rank){

				// assigning sampled value if current iteration is associated with reading selected channel's value
				binaryType = tempSampledValue;
			}
		}

	}

	// assigning extracted value to retval
	*retval = binaryType;



	return HAL_OK;
}


/*
 * @brief Function read ADC register associated with L data - number of channels in use
 */
static HAL_StatusTypeDef ADC_ChannelsNumberConfig(ADC_HandleTypeDef* hadc, ADC_ChannelsConfigTypeDefs* cadc){

	// Implementation for F1 family
    #if defined(STM32F1_FAMILY)

        // reading number of channels in used from SQR1
	    cadc->numberOfSelectedChannels = ((hadc->Instance->SQR1 >> ADC_SQR1_L_Pos) & 0xFF) + 1;

	    // if read number of selected channels in not 0, then return OK status
	    if(cadc->numberOfSelectedChannels != 0){
	    	return HAL_OK;
	    }

    #endif

	return HAL_ERROR;
}

/*
 * @brief Function read ADC register associated with ranks configuration - SQR1, SQR2, SQR3 and in some families SQR4
 */
static HAL_StatusTypeDef ADC_ChannelsConfig(ADC_HandleTypeDef* hadc, ADC_ChannelsConfigTypeDefs* cadc){

	// checking if correct number of channels has been set
    if(0 == cadc->numberOfSelectedChannels){
    	return HAL_ERROR;
    }

    // Executing operation for F1 family
    #if defined(STM32F1_FAMILY)

	    for(uint8_t i = 0; i < cadc->numberOfSelectedChannels; ++i){

	    	// Execution of read operation of SQR3 register
	    	if(i < ADC_SQR3_2_OFFSET){

                 cadc->ranks[i] = (uint8_t)(hadc->Instance->SQR3 >> (i * ADC_SQR_DATA_RES)) & ADC_SQR_DATA_Msk;

	    	}
	    	// Execution of read operation of SQR2 register
	    	else if(i < ADC_SQR3_1_OFFSET){

	    		 /* Subtracting ADC_SQR3_2_OFFSET because at this point i is no lower than 7, in bit shifting we need to start from 0*/
	    		 cadc->ranks[i] = (uint8_t)(hadc->Instance->SQR2 >> ((i - ADC_SQR3_2_OFFSET)  * ADC_SQR_DATA_RES)) & ADC_SQR_DATA_Msk;

	    	}
	    	// Execution of read operation of SQR1 register
	    	else{

	    		 /* Subtracting ADC_SQR3_1_OFFSET because at this point i is no lower than 13, in bit shifting we need to start from 0*/
	    		 cadc->ranks[i] = (uint8_t)(hadc->Instance->SQR1 >> ((i - ADC_SQR3_1_OFFSET)  * ADC_SQR_DATA_RES)) & ADC_SQR_DATA_Msk;

	    	}

	    }

    #endif

    // returning OK status
	return HAL_OK;
}

HAL_StatusTypeDef ADC_Init(ADC_HandleTypeDef* hadc, ADC_ChannelsConfigTypeDefs* cadc){

	// checking if user passed null pointer to ADC handle or cadc structure
	if(NULL == hadc || NULL == cadc){
		return HAL_ERROR;
	}


	// Initializing ADC
	if(HAL_ADC_Init(hadc) != HAL_OK){
		return HAL_ERROR;
	}

	// Initializing channels configuration operation
	if(ADC_Get_ChannelsConfiguration(hadc, cadc) != HAL_OK){
		return HAL_ERROR;
	}

	// start ADC's conversion in independent/no DMA/discontinuous mode
	if(ADC_Init_NoDMA_Independent_Discontinuous(hadc) != HAL_OK){
		return HAL_ERROR;
	}

	// Enabling clock for ADC1
	__HAL_RCC_ADC1_CLK_ENABLE();


	return HAL_OK;
}


HAL_StatusTypeDef ADC_ReadChannel(ADC_HandleTypeDef* hadc, ADC_ChannelsConfigTypeDefs* cadc, uint8_t* channel, uint16_t* retval){

	if(hadc == NULL || retval == NULL){
		return HAL_ERROR;
	}

	// rank for given channel
	uint8_t rank = 0;

	// getting rank for given channel
	if(ADC_Get_ChannelRank(hadc, cadc, channel, &rank) != HAL_OK){
		return HAL_ERROR;
	}

	// Because driver supports independent conversion mode without DMA support for ADC in independent mode only function beloww is called
	if(ADC_ReadChannel_NoDMA_Independent_Discontinuous(hadc, cadc, rank, retval) != HAL_OK){
		return HAL_ERROR;
	}

	return HAL_OK;
}

HAL_StatusTypeDef ADC_Get_PinVoltage(ADC_HandleTypeDef* hadc, ADC_ChannelsConfigTypeDefs* cadc, uint8_t* channel, float* retval){

	// checking if user gave null pointers
	if(NULL == hadc || NULL == retval){
		return HAL_ERROR;
	}

	// declaration of variables which will store end value of pin's voltage and sampled value from ADC's channel
	float pinVoltage = 0;
	uint16_t binaryType = 0;

	// reading sampled value from channel
	if(ADC_ReadChannel(hadc, cadc, channel, &binaryType) != HAL_OK){
		return HAL_ERROR;
	}

	// calculating voltage on pin, assigned to channel
	pinVoltage = (float)binaryType / (float)ADC_RESOLUTION * STM32_VCC;

	// assigning voltage value to retval
	*retval = pinVoltage;

	return HAL_OK;
}


HAL_StatusTypeDef ADC_Get_ChannelRank(ADC_HandleTypeDef* hadc, ADC_ChannelsConfigTypeDefs* cadc, uint8_t* channel, uint8_t* rank){

	// checking if user gave NULL pointers
	if(NULL == channel || NULL == rank){
		return HAL_ERROR;
	}

	// iterating through table of channels in cadc
	for(uint8_t i = 0; i < cadc->numberOfSelectedChannels; ++i){
		if(*channel == cadc->ranks[i]){

			// overwriting parameter's value - rank - with i value, rank to which channel is assigned
			*rank = i;

			return HAL_OK;
		}
	}

	return HAL_ERROR;
}


HAL_StatusTypeDef ADC_Get_ChannelsConfiguration(ADC_HandleTypeDef* hadc, ADC_ChannelsConfigTypeDefs* cadc){

	// reading number of channels to be used
	if(ADC_ChannelsNumberConfig(hadc, cadc) != HAL_OK){
		return HAL_ERROR;
	}

	// reading channels numbers assigned to ADC's ranks
	if(ADC_ChannelsConfig(hadc, cadc) != HAL_OK){
		return HAL_ERROR;
	}

	return HAL_OK;
}
