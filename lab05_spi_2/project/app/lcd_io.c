/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur   -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences) -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : CT2 lab - SPI Display
 * -- Description : Contains the implementations of the functions
 * --               to write and read frames from and to the TFT-LCD
 * --               display EAeDIPTFT43-A.
 * --
 * -- $Id: lcd_io.c 4720 2019-03-04 10:11:31Z akdi $
 * ------------------------------------------------------------------
 */
#include "lcd_io.h"
#include "hal_spi.h"
#include "hal_sbuf.h"

#define ACK_CHAR         (uint8_t)0x06
#define DC1_CHAR         (uint8_t)0x11
#define DC2_CHAR         (uint8_t)0x12
#define ESC_CHAR         (uint8_t)0x1B
#define ONE_CHAR         (uint8_t)0x01

#define NOTHING_RECEIVED (uint8_t)0
enum { SUCCESS = 0, ERRORCODE = 1 };

/* ------------------------------------------------------------------
 * -- Function prototypes
 * ------------------------------------------------------------------
 */
static uint8_t send_read_display_buffer_request(void);


/* ------------------------------------------------------------------
 * -- Function implementations
 * ------------------------------------------------------------------
 */

/*
 * according to description in header file
 */
void init_display_interface(void)
{
    hal_spi_init();
    hal_sbuf_init();
}


/*
 * according to description in header file
 */
uint8_t read_display_buffer(uint8_t *readBuffer)
{
    /// STUDENTS: To be programmed
    int buffer_request_response;
	  uint8_t dc1_byte;
		uint8_t length_byte;
	  uint8_t bcc;
	
		if ((uint8_t*)0 == readBuffer) return 0;
	
    if (hal_sbuf_get_state() == 0) {
        return 0;
    }

    if (send_read_display_buffer_request()) {
        return 0;
    }

    dc1_byte = hal_spi_read_write(0x00);
    if (dc1_byte != DC1_CHAR) {
        return 0;
    }

    length_byte = hal_spi_read_write(0x00);
    for (int i = 0; i < length_byte; i++) {
        readBuffer[i] = hal_spi_read_write(0x00);
    }
    bcc = hal_spi_read_write(0x00);

    return length_byte;

    /// END: To be programmed
}


/*
 * according to description in header file
 */
uint8_t write_cmd_to_display(const uint8_t *cmdBuffer, uint8_t length)
{
    /// STUDENTS: To be programmed
	  int data_length = length + 1;
		uint8_t rec_byte;

    int bcc = DC1_CHAR + data_length + ESC_CHAR;

    hal_spi_read_write(DC1_CHAR);
    hal_spi_read_write(data_length);
    hal_spi_read_write(ESC_CHAR);
    for (int i = 0; i < length; i++) {
        bcc += cmdBuffer[i];
        hal_spi_read_write(cmdBuffer[i]);
    }
    hal_spi_read_write(bcc % 256);

    rec_byte = hal_spi_read_write(0x00);
    if (rec_byte == ACK_CHAR) {
        return SUCCESS;
    }
    return ERRORCODE;
    /// END: To be programmed
}


/*
 * Assemble and send a packet to trigger the reading of the display buffer
 * Uses the sequence "<DC2>, 0x01, 0x53, checksum" according to datasheet
 * Check if the ACK was sent by the lcd and return this
 */
static uint8_t send_read_display_buffer_request(void)
{
    /// STUDENTS: To be programmed
		#define S_CHAR 0x53
    hal_spi_read_write(DC2_CHAR);
    hal_spi_read_write(0x01);
    hal_spi_read_write(S_CHAR);
    hal_spi_read_write(DC2_CHAR + 0x01 + S_CHAR);
    uint8_t rec_byte = hal_spi_read_write(0x00);
    if (rec_byte != ACK_CHAR) {
        return 1;
    }
    return 0;
    /// END: To be programmed

}

