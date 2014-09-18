/*
 * spi_slave.c
 *
 *  Created on: 21/03/2012
 *      Author: cuki
 */

#include <18F452.h>

#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=8MHz)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

#include"lcd_8b.c"

void main(void) {

	int instr = 0;
	int address = 0;

	setup_spi(SPI_SLAVE | SPI_H_TO_L | SPI_CLK_DIV_16);

	lcd_init();
	delay_ms(20);

//	printf(lcd, "\fPronto!");
	while (!spi_data_is_in())
		;
	instr = spi_read();

	while (!spi_data_is_in())
		;
	address = spi_read();

	printf(lcd, "\f%X\n%X", instr, address);
}
