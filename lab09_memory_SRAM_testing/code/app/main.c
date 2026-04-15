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

#include <stdint.h>
#include "hal_rcc.h"
#include "hal_fmc.h"
#include "hal_ct_lcd.h"
#include "reg_ctboard.h"
#include "hal_ct_buttons.h"
#include "hal_ct_seg7.h"

#define NR_OF_DATA_LINES           8
#define NR_OF_ADDRESS_LINES       11
#define CHECKER_BOARD           0xAA
#define INVERSE_CHECKER_BOARD   0x55
#define WALKING_ONES_ADDRESS    0x7FF

/* Set-up the macros (#defines) for your test */
/// STUDENTS: To be programmed
# define START_ADDRESS_NE2      ((uint32_t) 0x64000000)



/// END: To be programmed

int main(void)
{
    hal_fmc_sram_init_t init;
    hal_fmc_sram_timing_t timing;
    
    /* add your required automatic (local) variables here */ 
    /// STUDENTS: To be programmed
    uint8_t* const ptr_start_8 = (uint8_t*) START_ADDRESS_NE2;
    uint8_t fail_pattern_data = 0;
    
    const uint16_t checker_board = 0xAA;
    const uint16_t anti_checker = 0x55;
    uint16_t fail_pattern_address = 0;
  
    const uint16_t max_address = 0x07FF;
    /// END: To be programmed

    init.address_mux = DISABLE;                             // setup peripheral
    init.type = HAL_FMC_TYPE_SRAM;
    init.width = HAL_FMC_WIDTH_8B;
    init.write_enable = ENABLE;

    timing.address_setup = 0xFF;                            // all in HCLK
                                                            // cycles
    timing.address_hold = 0xFF;
    timing.data_setup = 0xFF;

    hal_fmc_init_sram(HAL_FMC_SRAM_BANK2, init, timing);    // init external bus
                                                            // bank 2 (NE2)
                                                            // asynch
    
    /* Data Bus Test - Walking ONES test */
    /// STUDENTS: To be programmed    
    for(uint8_t i = 0; i < 8; i++) {
      uint8_t test_pattern = (0x1 << i);
      * ptr_start_8 = test_pattern;
      uint8_t read_value = * ptr_start_8;
      fail_pattern_data |= read_value == test_pattern? 0 : test_pattern; 
    }
    
    CT_LED->BYTE.LED7_0 = fail_pattern_data;
    /// END: To be programmed
    
    /* Address Bus Test 
     * (1)  Write default values
     *
     *      Write the memory at all the power of 2 addresses (including 0x0000)
     *      to the default value of CHECKER_BOARD
     *
     * (2)  Perform tests
     *
     *      Select one power of 2 addresses after the other as test_address 
     *      (starting from the highest all the way down to 0x0000) --> 
     *          - Write the memory at test_address to INVERSE_CHECKER_BOARD
     *          - For all the power of 2 addresses including 0x0000
     *              o Read the memory content
     *              o Verify that the read value is either
     *                  (a) equal to CHECKER_BOARD in case a different address 
     *                      than test_addressed has been read or
     *                  (b) equal to INVERSE_CHECKER_BOARD in case the address
     *                      at test_address has been read
     *              o Errors found shall be indicated on LED31--16
     */
    
    /// STUDENTS: To be programmed
    uint16_t test_address = (uint16_t) 0x01 << NR_OF_ADDRESS_LINES;
    while(test_address) {
      test_address >>= 1;
      uint16_t fail_lights = test_address == 0? 0xF000 : test_address;
      uint8_t* ptr_current_8;
      
      uint16_t init_address = (uint16_t) 0x01 << NR_OF_ADDRESS_LINES;   
      while(init_address) {  
        init_address >>= 1;      
        ptr_current_8 = ptr_start_8 + init_address;
        * ptr_current_8 = checker_board;
      }
       
      ptr_current_8 = ptr_start_8 + test_address;
      * ptr_current_8 = anti_checker;
      
      uint16_t address = (uint16_t) 0x01 << NR_OF_ADDRESS_LINES;   
      while(address) {  
        address >>= 1;  
        ptr_current_8 = ptr_start_8 + address;
        uint16_t read_value = * ptr_current_8;
        if(address == test_address) {
          fail_pattern_address |= read_value == anti_checker? 0 : fail_lights; 
        } else {
          fail_pattern_address |= read_value == checker_board? 0 : fail_lights;
        }
      }
    }
    
    CT_LED->HWORD.LED31_16 = fail_pattern_address;
    /// END: To be programmed
    
    /* Device Test 
     * (1) Fill the whole memory with known increment pattern.
     *          Address     Data
     *          0x000       0x01
     *          0x001       0x02
     *          .....       ....
     *          0x0FE       0xFF
     *          0x0FF       0x00
     *          0x100       0x01
     *          .....       ....
     *
     * (2) First test: Read back each location and check pattern.
     *     In case of error, write address with wrong data to 7-segment and
     *     wait for press on button T0.
     *     Bitwise invert  the pattern in each location for the second test
     *
     * (3) Second test: Read back each location and check for new pattern.
     *     In case of error, write address with wrong data to 7-segment and
     *     wait for press on button T0.
     */
    /// STUDENTS: To be programmed
    // Wait for user input to start test
    while(!hal_ct_button_is_pressed(HAL_CT_BUTTON_T0)) {}
    
    // First Initialization
    uint16_t i = 0;
    uint8_t* ptr_current_8;
    while(i <= max_address) {        
      ptr_current_8 = ptr_start_8 + i;
      * ptr_current_8 = ++i % 0x100;
    }
    
    // First test and second Initialization
    i = 0;
    while(i <= max_address) {
      ptr_current_8 = ptr_start_8 + i;
      uint8_t expected_value = ++i % 0x100;
      if(* ptr_current_8 != expected_value) {
        CT_LCD->BG.BLUE = 0xFFFF;
        CT_LED->BYTE.LED7_0 = expected_value;
        CT_LED->BYTE.LED15_8 = * ptr_current_8;
        CT_SEG7->BIN.HWORD = i; 
        while(!hal_ct_button_is_pressed(HAL_CT_BUTTON_T0)) {}
      }
      * ptr_current_8 = ~expected_value;
    }
    
    // Reset LCD
    CT_LCD->BG.BLUE = 0;
    
    // Second Test
    i = 0;
    while(i <= max_address) {
      ptr_current_8 = ptr_start_8 + i;
      uint8_t expected_value = ~(++i % 0x100);
      if(* ptr_current_8 != expected_value) {
        CT_LCD->BG.GREEN = 0xFFFF;
        CT_LED->BYTE.LED7_0 = expected_value;
        CT_LED->BYTE.LED15_8 = * ptr_current_8;
        CT_SEG7->BIN.HWORD = i; 
        while(!hal_ct_button_is_pressed(HAL_CT_BUTTON_T0)) {}
      }
    }
    /// END: To be programmed
    
    // Write 'End'
    CT_SEG7->RAW.BYTE.DS0 = 0xA1;
    CT_SEG7->RAW.BYTE.DS1 = 0xAB;
    CT_SEG7->RAW.BYTE.DS2 = 0x86;
    CT_SEG7->RAW.BYTE.DS3 = 0xFF;
    
    while(1){
    }

}