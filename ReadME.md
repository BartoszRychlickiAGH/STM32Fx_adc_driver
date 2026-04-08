# STM32 Peripheral ADC Driver — PCBCells Project

## Title
**STM32 Peripheral ADC Driver (TDD Implementation)**

## Description
This directory contains source and header files for the ADC driver, designed to optimize the workflow with Analog-to-Digital Converters in the **Perła** racing car projects.

The main purpose of this ADC driver is to provide a clear API and macros that simplify the ADC workflow while ensuring firmware universality across various STM32 cores. The modular structure facilitates locating, analyzing, and debugging software errors at the low-level layer.

> **NOTE:** The driver is built using direct register access and low-level features of the ADC controller embedded in STM32 cores.

---

## Status & Development (TDD)
The project is being developed using the **TDD (Test-Driven Development)** methodology. This ensures high reliability through a suite of diagnostic functions that verify the driver's correctness within the dedicated hardware solution.

* **Current Support:** At this stage of implementation, the driver supports **single-channel readout**.
* **Documentation:** The user manual will be provided and updated continuously with each new functional update.

---

## Project Structure
The project architecture is divided into modules to ensure a transparent flow and separation of logic:

### 📂 Inc (Headers)
* **adc_driver.h**: API function prototypes, macro definitions, and data structures supporting a logical workflow.
* **adc_utils.h**: Declarations of low-level utilities operating directly on ADC registers.
* **adc_conf.h**: Used for parameters that require manual user configuration.

### 📂 Src (Sources)
* **adc_driver.c**: Implementation of core API functions and driver variable definitions.
* **adc_utils.c**: Operational functions extracting and processing data directly from the ADC controller registers.
* **adc_conf.c**: Implementation of specific configuration settings.

### 📂 Tests (Quality Assurance)
* **Inc/adc_ut.h** & **Src/adc_ut.c**: Unit testing modules. Currently, these contain **static tests**. In the final phase of implementation, they will be expanded into **diagnostic functions** to confirm on-target operational correctness.

---

## Application
In `.ioc` file, in section for `ADC`, please make sure that `DISCONTINUOUS` flag is set to `ENABLED`. Also ensure that `Number of DIscontinuous Conversion` is set to `1`.
While working wwith multiple channels, ensures correct `Number of Conversion`, which value should be equal to currently set channels. `.ioc file` whill automatically increase `number of ranks` - make sure, that every rank is assigned to `different channel`. 

To ensure stable operation and prevent `HardFault` errors, please call the **Init** function before working with the ADC. If Dual Mode is required, call the `Init_Multimode` function within the ADC Slave initialization (MX_ADC_Init).

## Manual
*The detailed manual will be expanded as development progresses.*

1.  For a proper workflow, initialize the `ADC_BufferTypeDef` and `ADC_ChannelsTypeDef` objects for each ADC instance.
2.  Call the `ADC_Init` function within each `MX_ADC_Init` function for all ADCs you are working with.
3.  If multimode is required, call the `ADC_InitMultimode` function.

---

## Author
**Bartosz Rychlicki** [bartoszrychl@student.agh.edu.pl](mailto:bartoszrychl@student.agh.edu.pl)

**AGH Eko-Energy** Software Department