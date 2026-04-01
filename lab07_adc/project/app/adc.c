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
 * -- $Id: adc.c 5610 2023-02-03 09:22:02Z frtt $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include <reg_stm32f4xx.h>

/* user includes */
#include "adc.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define PERIPH_GPIOF_ENABLE (0x00000020)
#define PERIPH_ADC3_ENABLE  (0x00000400)

/* Configuring pin for ADC: PF.6 */
#define GPIOF_MODER_ANALOG (0x3 << 12)


/* -- Macros used by student code
 * ------------------------------------------------------------------------- */

/// STUDENTS: To be programmed

#define MOVING_AVERAGE_SIZE 16
uint16_t moving_average[MOVING_AVERAGE_SIZE] = {};


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
    ADCCOM->CCR = 0;        // TSVREF = '0'    -> Temp sensor disabled
                            // VBATE = '0'     -> VBAT disabled
                            // ADCPRE = '00'   -> APB2 / 2 -> 21 MHz
                            // DMA = '00'      -> DMA disabled
                            // DELAY = '0000'  -> Delay 5 cycles
                            // MULTI = '00000' -> ADC independent mode

    /* Configure ADC3 */
    
    /// STUDENTS: To be programmed
	  //ADC3->CR1 = 0b11 << 24; // RES[1;0] = 11 (CR1 Bit 24+25)
	  ADC3->CR1 = 0b0;
	  ADC3->SMPR1 = 0x0; // Sample time 000 = 3 cycles
	  ADC3->SMPR2 = 0x0; // Sample time 000 = 3 cycles
		ADC3->SQR1 = 0x0;
		ADC3->SQR2 = 0x0;
		ADC3->SQR3 = 0x4;
	  ADC3->CR2 = 0b1; // Enable = 1


    /// END: To be programmed
}


/*
 *  See header file
 */
uint16_t adc_get_value(adc_resolution_t resolution)
{
    uint16_t adc_value;

    /// STUDENTS: To be programmed
	  ADC3->CR1 = resolution;

		ADC3->CR2 |= (0x1 << 30);
		while(!(ADC3->SR & 0x2)) { }

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
		for (int i = 1; i < MOVING_AVERAGE_SIZE; i++) {
			moving_average[i-1] = moving_average[i];
			filtered_value += moving_average[i];
		}
		moving_average[MOVING_AVERAGE_SIZE-1] = adc_value;
		filtered_value += adc_value;
		filtered_value /= MOVING_AVERAGE_SIZE;



    /// END: To be programmed

    return filtered_value;
}
