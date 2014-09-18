#define rs pin_d2
#define en pin_d3
#define d4 pin_d4
#define d5 pin_d5
#define d6 pin_d6
#define d7 pin_d7
// #define rs pin_d0
// #define en pin_d1
// #define d4 pin_d2
// #define d5 pin_d3
// #define d6 pin_d4
// #define d7 pin_d5

void lcd_cmd (byte cmd) {
	
	output_bit (d4,bit_test(cmd,0));
	output_bit (d5,bit_test(cmd,1));
	output_bit (d6,bit_test(cmd,2));
	output_bit (d7,bit_test(cmd,3));
	output_high (en);
	output_low (en);

}

void lcd_envia_byte(boolean endereco,byte dado){
	
	output_low(rs);
	output_bit(rs,endereco);
	delay_us(100);
	output_low(en);
	lcd_cmd(dado>>4);
	lcd_cmd(dado&0x0f);
	delay_ms(1);
	
}

void lcd_pos_xy(byte x, byte y) {
	byte endereco;
	if (y!=1)
		endereco = 0xc0;
	else
		endereco = 0x80;
	endereco += x-1;         
	lcd_envia_byte(0,endereco);
}
 
void lcd (char c) {
	switch(c)  {
		case '\f' : lcd_envia_byte(0,1);
								delay_ms(2);
								break;
		case '\n' : 
		case '\r' : lcd_pos_xy(1,2);
								break;
		case '\b' : lcd_envia_byte(0,0x10);
								break;
		default    : lcd_envia_byte(1,c);
								break;
	}
}  

void lcd_init() {

	byte conta;

	output_low(rs);
	output_low(en);
	output_low(d4);
	output_low(d5);
	output_low(d6);
	output_low(d7);

	delay_ms(15);

	for (conta=1;conta<=3;conta++) {
		lcd_cmd(0x03);
		delay_ms(5);
	}
	lcd_cmd(0x02);          //escreve comando para interface de 4 vias de dados
	lcd_envia_byte(0,0x28); //escreve comando informando lcd de 2 linhas (5x7) em 4bits
	lcd_envia_byte(0,0x0c); //escreve comando para limpar todo o display
	lcd_envia_byte(0,0x01); //escreve comando para ligar o display **com cursor piscante
	lcd_envia_byte(0,0x06); //escreve comando para incrementar automaticamente � direita
	delay_ms(5);
} 
 

