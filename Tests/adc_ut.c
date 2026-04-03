/*
 * adc_driver_ut.c
 *
 *  Created on: Mar 19, 2026
 *      Author: brych
 */

#include "adc_driver.h"
#include "CUnit/Basic.h"
#include "adc.h"
#include "stdio.h"
#include "adc_utils.h"

extern ADC_HandleTypeDef hadc1;
ADC_ChannelsConfigTypeDefs ut_cadc;
uint8_t channel0 = 0, channel1 = 1;

/*
==============================================================================
                        ##### Tests for ADC Drive features #####
==============================================================================

 */


/*
                      ##### General ADC Driver functionalities Tests #####
  ==============================================================================
*/

void TEST_ADCDriver_Initialization(void){

	HAL_StatusTypeDef status = ADC_Init(&hadc1, &ut_cadc);

	CU_ASSERT_EQUAL(status, HAL_OK);
}


void TEST_ADCDriver_ChannelsConfiguration(void){

	uint8_t rank1, rank2;


	HAL_StatusTypeDef status = ADC_Get_ChannelsConfiguration(&hadc1, &ut_cadc);

	CU_ASSERT_EQUAL(ut_cadc.ranks[0], 0);
	CU_ASSERT_EQUAL(ut_cadc.ranks[1], 1);

	CU_ASSERT_EQUAL(status, HAL_OK);

	status = ADC_Get_ChannelRank(&hadc1, &ut_cadc, &channel0, &rank1);

	CU_ASSERT_EQUAL(rank1, 0);

	status = ADC_Get_ChannelRank(&hadc1, &ut_cadc, &channel1, &rank2);

	CU_ASSERT_EQUAL(rank2, 1);

}

/*
                      ##### Non-Continuous, Independent, No DMA #####
  ==============================================================================
 */

void TEST_ADC_Init_Independent_NoContinous_NoDiscontinuous_NoDMA(void){

	// checking ADC mode
	CU_ASSERT_EQUAL(ADC_GetMode(&hadc1), ADC_MODE_INDEPENDENT);

	// checking Conversion type - if ADC needs to be launched after scanning
	CU_ASSERT_EQUAL(ADC_Continuous(&hadc1), DISABLE);

	// checking if scanning while calling HAL_ADC is associated with single channel
	CU_ASSERT_EQUAL(ADC_Discontinuous(&hadc1), DISABLE);

	// checking if DMA instance was assigned
	CU_ASSERT_PTR_NULL(hadc1.DMA_Handle);


}

void TEST_ADC_ReadChannel_Independent_NoContinous_NoDiscontinuous_NoDMA_Channel0(void){


	// declaration of variable to store sampled value in ut
	uint16_t testBinaryValue = 0;

	// assigning return status by function: ADC_ReadChannel
	HAL_StatusTypeDef status = ADC_ReadChannel(&hadc1, &ut_cadc, &channel0, &testBinaryValue);

	// checking of status returned by ADC_ReadChannel is correct
	CU_ASSERT_EQUAL(status, HAL_OK);

	// checking if sampled value is near 5, because selected channel is wired to GND but fluctuances of sampled values can wrongly indicate that sampled value is not 0
	CU_ASSERT_TRUE(testBinaryValue >= 4000);

}

void TEST_ADC_ReadChannel_Independent_NoContinous_NoDiscontinuous_NoDMA_Channel1(void){


	// declaration of variable to store sampled value in ut
	uint16_t testBinaryValue = 0;

	// assigning return status by function: ADC_ReadChannel
	HAL_StatusTypeDef status = ADC_ReadChannel(&hadc1, &ut_cadc, &channel1, &testBinaryValue);

	// checking of status returned by ADC_ReadChannel is correct
	CU_ASSERT_EQUAL(status, HAL_OK);

	// checking if sampled value is near 5, because selected channel is wired to GND but fluctuances of sampled values can wrongly indicate that sampled value is not 0
	CU_ASSERT_TRUE(testBinaryValue <= 100 );


}

void TEST_ADC_GetValue_Independent_NoContinous_NoDiscontinuous_NoDMA_Channel0(void){

	// Declaration of target value and retval, where their values will be compared
	float targetValue = 3.3f;
	float testVoltage = 0;

	// declaration of tolerance for measured voltage
	float tolerance = 0.01f;

	// reading status of ADC_ReadPinVoltage
	HAL_StatusTypeDef status = ADC_Get_PinVoltage(&hadc1, &ut_cadc, &channel0, &testVoltage);

	// assertion for returned by ADC_getVoltagePin status
	CU_ASSERT_EQUAL(status, HAL_OK);

	// assertion for GND value for channel wired to GND of PCB
	CU_ASSERT_DOUBLE_EQUAL(targetValue, testVoltage, tolerance);
}

void TEST_ADC_GetValue_Independent_NoContinous_NoDiscontinuous_NoDMA_Channel1(void){

	// Declaration of target value and retval, where their values will be compared
		float targetValue = 0.0f;
		float testVoltage = 0.0f;

		// declaration of tolerance for measured voltage
		float tolerance = 0.01f;


		// reading status of ADC_ReadPinVoltage
		HAL_StatusTypeDef status = ADC_Get_PinVoltage(&hadc1, &ut_cadc, &channel1, &testVoltage);

		// assertion for returned by ADC_getVoltagePin status
		CU_ASSERT_EQUAL(status, HAL_OK);

		// assertion for GND value for channel wired to GND of PCB
		CU_ASSERT_DOUBLE_EQUAL(targetValue, testVoltage, tolerance);

}

/*
                      ##### ADC Driver's Unit Tests entry point #####
  ==============================================================================

 */
void ADCDriver_Ut_main(void){

	// initialize CUnit test suit
	CU_pSuite readChannelSuite; 		 // tests suite for Continuous = DISABLED, No DMA, Discontinuous = DISBLED, Independent
	CU_pSuite getPinVoltageSuite;
	CU_pSuite generalFunctionsTestSuite; // tests suite for general ADC Driver functionalities

	// Creating tests db
	CU_initialize_registry();

	// Adding tests

	/*
	 * Tests for general ADC Driver functionalities
	 */
	generalFunctionsTestSuite = CU_add_suite("ADC general functionalities test suite", NULL, NULL);
	CU_add_test(generalFunctionsTestSuite, "ADC initialization tests", TEST_ADCDriver_Initialization);
	CU_add_test(generalFunctionsTestSuite, "Channels configuration tests", TEST_ADCDriver_ChannelsConfiguration);


	/*
	 * Tests for Continuous = DISABLED, No DMA, Discontinuous = DISBLED, Independent - Read Channel
	 */
	readChannelSuite = CU_add_suite("[Read Channel] ADC DISCONTINUOUS = DISABLED, CONTINUOUS = DISABLED, No DMA and independent mode", NULL, NULL);
	CU_add_test(readChannelSuite, "Channel 0 test", TEST_ADC_ReadChannel_Independent_NoContinous_NoDiscontinuous_NoDMA_Channel0);
	//CU_add_test(readChannelSuite, "Channel 1 test", TEST_ADC_ReadChannel_Independent_NoContinous_NoDiscontinuous_NoDMA_Channel1);


	/*
	 * Tests for Continuous = DISABLED, No DMA, Discontinuous = DISBLED, Independent - Get Pin Voltage
	 */
	getPinVoltageSuite = CU_add_suite("[Get Pin Voltage] ADC DISCONTINUOUS = DISABLED, CONTINUOUS = DISABLED, No DMA and independent mode", NULL, NULL);
	CU_add_test(getPinVoltageSuite, "Channel 0 test", TEST_ADC_GetValue_Independent_NoContinous_NoDiscontinuous_NoDMA_Channel0);
	CU_add_test(getPinVoltageSuite, "Channel 1 test", TEST_ADC_GetValue_Independent_NoContinous_NoDiscontinuous_NoDMA_Channel1);



	// Set verbose mode
	CU_basic_set_mode(CU_BRM_VERBOSE);

	// Running tests
	CU_basic_run_tests();

	// Print results
	CU_basic_show_failures(CU_get_failure_list());

	// Deinitialization of CUnit
	CU_cleanup_registry();


}
