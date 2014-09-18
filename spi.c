#include <18F452.h>

#fuses XT, NOWDT, NOPROTECT, BROWNOUT, PUT, NOLVP
#use delay(clock = 8MHz)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, ERRORS)

#define MASTER_SS  PIN_B2
#define MASTER_SCK PIN_B3
#define MASTER_SDI PIN_B4
#define MASTER_SDO PIN_B5

#define SPI_MODE_0_0 0x4000
#define SPI_MODE_0_1 0x0000
#define SPI_MODE_1_0 0x0010
#define SPI_MODE_1_1 0x4010

#define BUFFER_SIZE  80

int buffer[BUFFER_SIZE];
int next_in = 0;
int next_out = 0;

#define spi_kbhit (next_in != next_out)

#int_ssp
void ssp_isr(void) {

	buffer[next_in] = spi_read();
	next_in++;
	if (next_in >= BUFFER_SIZE)
		next_in = 0;
}

int spi_bgetc(void) {

	int c;

	while (!spi_kbhit)
		;
	c = buffer[next_out];
	next_out++;
	if (next_out >= BUFFER_SIZE)
		next_out = 0;

	return (c);
}

void master_spi_write(char data) {

	int i;

	disable_interrupts(GLOBAL);
	output_low(MASTER_SS);
	for (i = 0; i < 8; i++) {
		if (bit_test(data, 7))
			output_high(MASTER_SDO);
		else
			output_low(MASTER_SDO);
		delay_us(5);
		output_high(MASTER_SCK);
		delay_us(5);
		output_low(MASTER_SCK);
		data <<= 1;
	}
	output_high(MASTER_SS);
	enable_interrupts(GLOBAL);
}

void main() {

	char c;

	setup_spi(SPI_SLAVE | SPI_MODE_0_0);
	output_high(MASTER_SS);
	output_low(MASTER_SCK);
	output_low(MASTER_SDO);
	output_float(MASTER_SDI);
	clear_interrupt(INT_SSP);
	enable_interrupts(INT_SSP);
	enable_interrupts(GLOBAL);

	while (1) {
		if (kbhit()) {
			c = getc();
			master_spi_write(c);
		}
		if (spi_kbhit) {
			c = spi_bgetc();
			putc(c);
		}
	}
}
