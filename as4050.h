/*
 * as4050.h
 *
 *  Created on: 30/03/2012
 *      Author: cuki
 */

#ifndef AS4050_H_
#define AS4050_H_

#define clockwise 0x0000
#define couter_clockwise 0x8000
#define direction_mask 0x8000
#define zero_mask 0x7FE0
#define index1 0x0000
#define index3 0x0010
#define index_mask 0x0010
#define quatrature_mode	0x0001
#define step_direction_mode 0x0002
#define brushless_dc_mode	0x0003
#define mode_mask 0x0003
#define _10_bit_resolution 0x0000
#define _9_bit_resolution 0x0004
#define _8_bit_resolution 0x0008
#define _7_bit_resolution 0x000C
#define resolution_mask	0x000C
#define DO_PIN PIN_C5
#define DI_PIN PIN_C4
#define CLK_PIN PIN_C3
#ifndef SLAVE_SELECT
#define SLAVE_SELECT PIN_A5
#endif

long setup;

void write_encoder(long data) {

	register int aux = 0;
	register int cont = 0;

	output_high(DO_PIN);
	output_low(CLK_PIN);
	delay_us(10);
	output_high(SLAVE_SELECT);
	spi_xfer(data);
	output_low(CLK_PIN);
	for (cont = 0; cont < 16; ++cont) {
		output_high(DO_PIN);
		delay_us(2);
		output_high(CLK_PIN);
		delay_us(3);
		output_low(CLK_PIN);
		delay_us(2);
		output_low(DO_PIN);
	}
	output_low(SLAVE_SELECT);
}

void setup_encoder(long new_setup, int zero_mark) {

	if (new_setup & direction_mask) {
		setup |= direction_mask;
	}
	if (new_setup & index_mask) {
		setup |= index_mask;
	}
	if (new_setup & mode_mask) {
		switch (new_setup & mode_mask) {
		case quatrature_mode:
			setup |= quatrature_mode;
			break;
		case step_direction_mode:
			setup |= step_direction_mode;
			break;
		case brushless_dc_mode:
			setup |= brushless_dc_mode;
			break;
		default:
			break;
		}
	}
	if (new_setup & resolution_mask)
		switch (new_setup & resolution_mask) {
		case _9_bit_resolution:
			setup |= _9_bit_resolution;
			break;
		case _8_bit_resolution:
			setup |= _8_bit_resolution;
			break;
		case _7_bit_resolution:
			setup |= _7_bit_resolution;
			break;
		default:
			break;
		}
	if (zero_mark && zero_mark < 1024) {
		setup |= (long) zero_mark << 5;
	}

	write_encoder(setup);
}

#endif /* AS4050_H_ */
