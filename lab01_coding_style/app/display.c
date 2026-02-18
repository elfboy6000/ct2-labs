/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module lcd
 * --               Performs all the interactions with the lcd
 * --
 * -- $Id: lcd.c 5144 2020-09-01 06:17:21Z ruan $
 * ------------------------------------------------------------------
 */

/* standard includes */
#include <stdio.h>
#include <reg_ctboard.h>
#include <hal_ct_seg7.h>

/* user includes */
#include "display.h"

/* macros */
#define LCD_ADDR_LINE1      0u
#define LCD_ADDR_LINE2      20u

#define NR_OF_CHAR_PER_LINE 20u
#define MAX_INTENSITY 			65535
#define MIN_INTENSITY 			0

#define LCD_CLEAR           "                    "

/* function declarations */
/// STUDENTS: To be programmed




/// END: To be programmed

/* function definitions */

/*brief Writes 'throw' to the seven-segment display.
*
*param trown dice number to be displayed
*/
void display_write_throw(uint8_t dice_number)
{
   hal_ct_seg7_bin_write(dice_number); 
}



/*
 * \brief  Writes 'value' to the indicated 'position' on the lcd
 * 
 * \param  slot_nr: A number between 1 and 6 indicating at which one of the 
 *         six available slots on the lcd the 'value' shall be printed. The 
 *         values at the other 5 slots remain unchanged.
 * 
 * \param  value: The value to be printed
 */
void display_write_value(uint8_t slot_nr, uint8_t value){
	 if (slot_nr < 1 || slot_nr > 6) return;

    uint8_t addr = (slot_nr - 1) * 3;   // Slots: 0,3,6,9,12,15
    char buf[4];
    snprintf(buf, sizeof(buf), "%3u", value);

    CT_LCD->ASCII[addr + 0] = buf[0];
    CT_LCD->ASCII[addr + 1] = buf[1];
    CT_LCD->ASCII[addr + 2] = buf[2];


}

/*
 * \brief  Writes an explanatory string followed by 'total_value' on the lcd. 
 * 
 * \param  total_value: The value to be printed
 */
void display_write_total(uint8_t total_value){
	 char buf[21];
   snprintf(buf, sizeof(buf), "Total: %12u", total_value);

   // ab Line2 (Adresse 20) schreiben
   for (uint8_t i = 0; i < 20; i++) {
       CT_LCD->ASCII[20 + i] = (buf[i] != '\0') ? buf[i] : ' ';
   }
	

}


/*
 * \brief  Clears the lcd and switches it to light green. 
 */
void display_clear(void){
	hal_ct_seg7_clear_all();

  for (uint8_t i = 0; i < 40; i++) {
      CT_LCD->ASCII[i] = ' ';
  }
		
	CT_LCD->BG.GREEN = MAX_INTENSITY;
	CT_LCD->BG.BLUE = MIN_INTENSITY;
	CT_LCD->BG.RED = MIN_INTENSITY;

}





/// STUDENTS: To be programmed




/// END: To be programmed
