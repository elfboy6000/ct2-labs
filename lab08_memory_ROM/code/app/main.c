/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Application for testing external memory
 * --
 * -- $Id: main.c 5605 2023-01-05 15:52:42Z frtt $
 * ------------------------------------------------------------------
 */

/* standard includes */
#include <stdint.h>


/// STUDENTS: To be programmed

#include <reg_stm32f4xx.h>
#include <reg_ctboard.h>
#include "hal_ct_buttons.h"


#define BASE_ADDRESS   (volatile uint8_t*)0x64000400
#define EEPROM_SIZE   0x100

/// END: To be programmed

int main(void)
{
    /// STUDENTS: To be programmed
		uint8_t value;
		while (1) {
        for (uint16_t i = 0; i < EEPROM_SIZE; ++i) {
            value = *(BASE_ADDRESS + i);      
            if (value != (uint8_t)i) {
                
                CT_LED->BYTE.LED23_16 = i;
                CT_LED->BYTE.LED7_0  = value;

                while (!hal_ct_button_is_pressed(HAL_CT_BUTTON_T0));
            }
        }
    }

    /// END: To be programmed
}
