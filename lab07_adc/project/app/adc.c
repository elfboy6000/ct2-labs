/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module adc.
 * --
 * --
 * -- $Id: adc.c 5605 2023-01-05 15:52:42Z frtt $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include <reg_stm32f4xx.h>

/* user includes */
#include "adc.h"

/* -- Macros
 * ------------------------------------------------------------------------- */

#define PERIPH_GPIOF_ENABLE (0x00000020)
#define PERIPH_ADC3_ENABLE (0x00000400)

/* Configuring pin for ADC: PF.6 */
#define GPIOF_MODER_ANALOG (0x3 << 12)

/* -- Macros used by student code
 * ------------------------------------------------------------------------- */

/// STUDENTS: To be programmed

#define ADC3_ENABLE 1
#define ADC3_CONTINUOUS_CONVERSION (1 << 1)
#define ADC3_SINGLE_CONVERSION (0 << 1)
#define ADC3_CHANNEL 4
#define ADC3_START_CONVERSION (1 << 30)
#define ADC3_END_CONVERSION 2
#define ADC3_SAMPLING_TIME (6 << (ADC3_CHANNEL * 3))
#define ADC3_CLEAR_RESOLUTION ~(3 << 24)

#define FILTER_LENGTH 16

/// END: To be programmed

/* Public function definitions
 * ------------------------------------------------------------------------- */

/*
 *  See header file
 */
void adc_init(void)
{
    /* Enable peripheral clocks */
    RCC->AHB1ENR |= PERIPH_GPIOF_ENABLE;
    RCC->APB2ENR |= PERIPH_ADC3_ENABLE;

    /* Configure PF.6 as input */
    GPIOF->MODER |= GPIOF_MODER_ANALOG;

    /* ADC common init */
    ADCCOM->CCR = 0; // TSVREF = '0'    -> Temp sensor disabled
                     // VBATE = '0'     -> VBAT disabled
                     // ADCPRE = '00'   -> APB2 / 2 -> 21 MHz
                     // DMA = '00'      -> DMA disabled
                     // DELAY = '0000'  -> Delay 5 cycles
                     // MULTI = '00000' -> ADC independent mode

    /* Configure ADC3 */

    /// STUDENTS: To be programmed
    ADC3->CR1 = 0;
    ADC3->CR2 |= ADC3_SINGLE_CONVERSION | ADC3_ENABLE; // Enable cont. conversion

    ADC3->SMPR1 = 0;
    ADC3->SMPR2 = 0;

    ADC3->SQR1 = 0;            // Set sequence length to 1
    ADC3->SQR2 = 0;            //
    ADC3->SQR3 = ADC3_CHANNEL; // Set input channel.

    /// END: To be programmed
}

/*
 *  See header file
 */
uint16_t adc_get_value(adc_resolution_t resolution)
{
    uint16_t adc_value;

    /// STUDENTS: To be programmed
    ADC3->CR1 &= ADC3_CLEAR_RESOLUTION; // Clear resolution
    ADC3->CR1 |= resolution;
    ADC3->CR2 |= ADC3_START_CONVERSION;

    while (!(ADC3->SR & ADC3_END_CONVERSION))
    {
    }

    adc_value = ADC3->DR;

    /// END: To be programmed

    return adc_value;
}

/*
 *  See header file
 */
uint16_t adc_filter_value(uint16_t adc_value)
{
    uint16_t filtered_value = 0;

    /// STUDENTS: To be programmed
    static uint16_t values[FILTER_LENGTH] = {0};
    static uint8_t counter = 0;
    values[counter] = adc_value;
    counter = (counter + 1) % FILTER_LENGTH;

    for (uint8_t i = 0; i < FILTER_LENGTH; i++)
    {
        filtered_value += values[i];
    }
    filtered_value /= FILTER_LENGTH;

    /// END: To be programmed

    return filtered_value;
}
