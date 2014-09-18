/*
 * spi_master.c
 *
 *  Created on: 21/03/2012
 *      Author: cuki
 */
#include <18F452.h>

#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=8MHz)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)
#use spi(MASTER, DI=PIN_C4, DO=PIN_C5, CLK=PIN_C3, MODE=0,  BAUD=2000000, BITS=16, DATA_HOLD=1)

#define SLAVE_SELECT PIN_A5
#include "as4050.h"

long leitura;
int ctrl;

void main(void) {

	//setup_spi(SPI_MASTER | SPI_CLK_DIV_16 | SPI_XMIT_L_TO_H);

//	ctrl = read_eeprom(0x00);
//	if (ctrl == 0xFF) {
		setup_encoder(
				couter_clockwise | _7_bit_resolution | step_direction_mode, 64);
		write_eeprom(0x00, 0x00);
		printf("\n\rReboot");
		delay_ms(100);
		sleep();
//	} else
//		write_eeprom(0x00, 0xFF);

	while (true) {

		output_low(SLAVE_SELECT);
		leitura = spi_xfer(0);
		//	leitura = make16(spi_read(0), spi_read(0));
		output_high(SLAVE_SELECT);
		printf("%Lu  -  %d \n\r", leitura >> 6, (int) (leitura & 0x0006) >> 1);
		//		printf("%3.2f  -  %d \n\r", (float) (leitura >> 6) * 360 / 1024,
		//				(int) (leitura & 0x0006) >> 1);
		delay_ms(500);
	}
}
/*
 void main(void) {
 printf("\n\r");
 switch (restart_cause()) {
 case WDT_FROM_SLEEP:
 printf("WDT_FROM_SLEEP\n\r");
 break;
 case WDT_TIMEOUT:
 printf("WDT_TIMEOUT\n\r");
 break;
 case MCLR_FROM_SLEEP:
 printf("MCLR_FROM_SLEEP\n\r");
 break;
 case NORMAL_POWER_UP:
 printf("NORMAL_POWER_UP\n\r");
 break;
 }
 //	while (true) {
 //		output_toggle(pin_a0);
 //		delay_ms(500);
 //	}
 }
 */
