#define en pin_e1
#define rs pin_e0
#define data output_d

void lcd_cmd(byte cmd) {

	data(cmd);
	delay_us(10);
	output_high(en);
	delay_us(10);
	output_low(en);

}

void lcd_envia_byte(boolean nivel, byte dado) {

	output_low(rs);
	output_bit(rs, nivel);
	delay_us(100);
	output_low(en);
	lcd_cmd(dado);

}

void lcd_pos_xy(byte x, byte y) {

	byte endereco;

	if (y != 1)
		endereco = 0xc0;
	else
		endereco = 0x80;

	endereco += x - 1;
	lcd_envia_byte(0, endereco);

}

void lcd(char c) {

	switch (c) {

	case '\f':
		lcd_envia_byte(0, 1);
		delay_ms(2);
		break;

	case '\n':

	case '\r':
		lcd_pos_xy(1, 2);
		break;

	case '\b':
		lcd_envia_byte(0, 0x10);
		break;

	default:
		lcd_envia_byte(1, c);
		break;
	}

}

void lcd_init() {

	byte conta;

	data(0x00);
	output_low(rs);
	output_low(en);
	delay_ms(15);

	for (conta = 1; conta <= 3; conta++) {

		lcd_cmd(0x30);
		delay_ms(5);

	}

	lcd_cmd(0x38);
	lcd_cmd(0x01);
	delay_ms(1);
	lcd_cmd(0x0c);
	lcd_cmd(0x06);

}
