/**
  ******************************************************************************
  * @file      adc_ut.c
  * @author    Bartosz Rychlicki
  * @author    AGH Eko-Energia
  * @Title     Tests Suites for ADC Driver
  * @brief     File contains tests suites for common modes of ADC and specific data flow with mixed configuration og ADC flags
  ******************************************************************************
  * @attention Error codes are called when exact incorrect use of function is made
  *
  * Copyright (c) 2026 AGH Eko-Energy.
  * All rights reserved.
  *
  ******************************************************************************
  */

#include "adc_driver.h"
#include "CUnit/Basic.h"
#include "adc.h"
#include "stdio.h"
#include "adc_utils.h"
#include "main.h"


#if defined(STM32F1_FAMILY)

    extern ADC_HandleTypeDef hadc1;

#elif defined(STM32F3_FAMILY)
    extern ADC_HandleTypeDef hadc2;
#endif

ADC_ChannelsConfigTypeDefs ut_cadc;



/*
                      ##### General ADC Driver functionalities Tests #####
  ==============================================================================
*/

void TEST_ADCDriver_ChannelsConfiguration(void){

	uint8_t channel0Index = 0, channel1Index = 1, channel4Index = 4;
	uint8_t rank1, rank2, rank3;

	#if defined(STM32F1_FAMILY)
		ADC_Init(&hadc1, &ut_cadc);

		HAL_StatusTypeDef status = ADC_Get_ChannelsConfiguration(&hadc1, &ut_cadc);

		CU_ASSERT_EQUAL(ut_cadc.ranks[0], 0);
		CU_ASSERT_EQUAL(ut_cadc.ranks[1], 1);
		CU_ASSERT_EQUAL(ut_cadc.ranks[2], 4);

		CU_ASSERT_EQUAL(status, HAL_OK);

		status = ADC_Get_ChannelRank(&hadc1, &ut_cadc, channel0Index, &rank1);

		CU_ASSERT_EQUAL(status, HAL_OK);
		CU_ASSERT_EQUAL(rank1, 0);

		status = ADC_Get_ChannelRank(&hadc1, &ut_cadc, channel1Index, &rank2);

		CU_ASSERT_EQUAL(status, HAL_OK);
		CU_ASSERT_EQUAL(rank2, 1);

		status = ADC_Get_ChannelRank(&hadc1, &ut_cadc, channel4Index, &rank3);

		CU_ASSERT_EQUAL(status, HAL_OK);
		CU_ASSERT_EQUAL(rank3, 2);

	#elif defined (STM32F3_FAMILY)
		ADC_Init(&hadc2, &ut_cadc);

		HAL_StatusTypeDef status = ADC_Get_ChannelsConfiguration(&hadc2, &ut_cadc);

		CU_ASSERT_EQUAL(ut_cadc.ranks[0], 0);
		CU_ASSERT_EQUAL(ut_cadc.ranks[1], 1);
		CU_ASSERT_EQUAL(ut_cadc.ranks[2], 4);

		CU_ASSERT_EQUAL(status, HAL_OK);

		status = ADC_Get_ChannelRank(&hadc2, &ut_cadc, channel0Index, &rank1);

		CU_ASSERT_EQUAL(status, HAL_OK);
		CU_ASSERT_EQUAL(rank1, 0);

		status = ADC_Get_ChannelRank(&hadc2, &ut_cadc, channel1Index, &rank2);

		CU_ASSERT_EQUAL(status, HAL_OK);
		CU_ASSERT_EQUAL(rank2, 1);

		status = ADC_Get_ChannelRank(&hadc2, &ut_cadc, channel4Index, &rank3);

		CU_ASSERT_EQUAL(status, HAL_OK);
		CU_ASSERT_EQUAL(rank3, 2);

	#endif
}
/*
                      ##### ADC Driver's No DMA Support Conversion tests - ADC in independent mode#####
  ==============================================================================

*/
void TEST_ADC_Init_NoDMA_Independent_Continuous(void){

	// Tests associated with Init

	#if defined(STM32F1_FAMILY)


		HAL_StatusTypeDef status = ADC_Init(&hadc1, &ut_cadc);

		CU_ASSERT_EQUAL(HAL_OK, status);

		CU_ASSERT_EQUAL(ADC_Resolution(&hadc1), 4095);

		CU_ASSERT_EQUAL(ADC_Continuous(&hadc1), DISABLE);

		CU_ASSERT_EQUAL(ADC_Discontinuous(&hadc1), ENABLE);

		CU_ASSERT_PTR_NULL(hadc1.DMA_Handle);

		CU_ASSERT_EQUAL(ADC_GetMode(&hadc1), ADC_MODE_INDEPENDENT);

	#elif defined(STM32F3_FAMILY)

		HAL_StatusTypeDef status = ADC_Init(&hadc2, &ut_cadc);

		CU_ASSERT_EQUAL(HAL_OK, status);

		CU_ASSERT_EQUAL(ADC_Resolution(&hadc2), 4095);

		CU_ASSERT_EQUAL(ADC_Continuous(&hadc2), DISABLE);

		CU_ASSERT_EQUAL(ADC_Discontinuous(&hadc2), ENABLE);

		CU_ASSERT_PTR_NULL(hadc2.DMA_Handle);

		CU_ASSERT_EQUAL(ADC_GetMode(&hadc2), ADC_MODE_INDEPENDENT);


	#endif

}

void TEST_ADC_ReadChannel_0_NoDMA_Independent_Continuous(void){

     uint8_t  channelIndex = 0;
     uint16_t channelValue = 0;


	#if  defined(STM32F1_FAMILY)

		 HAL_StatusTypeDef status = ADC_ReadChannel(&hadc1, &ut_cadc, channelIndex, &channelValue);

	#elif defined(STM32F3_FAMILY)

		 HAL_StatusTypeDef status = ADC_ReadChannel(&hadc2, &ut_cadc, channelIndex, &channelValue);

	#endif


     CU_ASSERT_EQUAL(HAL_OK, status);

     CU_ASSERT(channelValue >= 4000);
}

void TEST_ADC_ReadChannel_1_NoDMA_Independent_Continuous(void){

     uint8_t  channelIndex = 1;
     uint16_t channelValue = 0;


	#if  defined(STM32F1_FAMILY)

		 HAL_StatusTypeDef status = ADC_ReadChannel(&hadc1, &ut_cadc, channelIndex, &channelValue);

	#elif defined(STM32F3_FAMILY)

		 HAL_StatusTypeDef status = ADC_ReadChannel(&hadc2, &ut_cadc, channelIndex, &channelValue);

	#endif


     CU_ASSERT_EQUAL(HAL_OK, status);

     CU_ASSERT(channelValue > 2800 && channelValue < 3000);
}

void TEST_ADC_ReadChannel_4_NoDMA_Independent_Continuous(void){

     uint8_t  channelIndex = 4;
     uint16_t channelValue = 0;


#if  defined(STM32F1_FAMILY)

     HAL_StatusTypeDef status = ADC_ReadChannel(&hadc1, &ut_cadc, channelIndex, &channelValue);

#elif defined(STM32F3_FAMILY)

     HAL_StatusTypeDef status = ADC_ReadChannel(&hadc2, &ut_cadc, channelIndex, &channelValue);

#endif


     CU_ASSERT_EQUAL(HAL_OK, status);

     CU_ASSERT(channelValue < 20);
}

/*
                      ##### ADC Driver's Unit Tests entry point #####
  ==============================================================================

*/
void ADCDriver_Ut_main(void){

	// initialize CUnit test suit
	CU_pSuite generalSuite; 		 // init tests suite for Continuous = DISABLED, No DMA, Discontinuous = DISBLED, Independent
    CU_pSuite noDMASuite;            // test suite for ADC without DMA, independent

	// Creating tests db
	CU_initialize_registry();

	// Adding tests

	/*
	 * Tests for general ADC Driver functionalities
	 */
	generalSuite = CU_add_suite("ADC Common Init Test Suite", NULL, NULL);
	CU_add_test(generalSuite, "Channels Config Test", TEST_ADCDriver_ChannelsConfiguration);

	/*
	 * Tests for ADC Driver functionalities in No DMA support - independent mode of ADC, Conversion is not Continuous
	 */
     noDMASuite = CU_add_suite("Test suite for ADC in not continuous conversion mode, without DMA usage and in indepenDent mode", NULL, NULL);
     CU_add_test(noDMASuite, "Init test", TEST_ADC_Init_NoDMA_Independent_Continuous);
     CU_add_test(noDMASuite, "Reading channel 0 test", TEST_ADC_ReadChannel_0_NoDMA_Independent_Continuous);
     CU_add_test(noDMASuite, "Reading channel 1 test", TEST_ADC_ReadChannel_1_NoDMA_Independent_Continuous);
     CU_add_test(noDMASuite, "Reading channel 4 test", TEST_ADC_ReadChannel_4_NoDMA_Independent_Continuous);



	// Set verbose mode
	CU_basic_set_mode(CU_BRM_VERBOSE);

	// Running tests
	CU_basic_run_tests();

	// Print results
	CU_basic_show_failures(CU_get_failure_list());

	// Deinitialization of CUnit
	CU_cleanup_registry();


}
