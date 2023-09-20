/*
 * Written By: Ariel Ohayon
 *
 *
 *  Arduino UNO R3	 	LCD (8Bit Mode)
 * ---------------		-------------
 * |		 	 |		|	    	|
 * |	(D9)-PB1 |---->	| E		VCC |---> 5V
 * |	(D8)-PB0 |---->	| RS	    |
 * |		 	 |		|	    	|
 * |	(D7)-PD7 |---->	| D7	GND |---> GND
 * |	.		 | .	|	    	|
 * |	.		 | .	|	    	|
 * |	.		 | .	|	    	|
 * |	(D0)-PD0 |---->	| D0	RW  |---> GND
 * ---------------		-------------
 */



#include<avr/io.h>
#include<util/delay.h>

int main(void)
{
	char buffer[16];
	// Pinout Configurations (output config)
	DDRB |= (1 << PINB0) | (1 << PINB1);
	DDRD = 0xFF;
	PORTD = 0x00;
	PORTB &= ~( (1 << PINB0) | (1 << PINB1) );
	// Pinout Configurations (output config)

	// Send Commands to the LCD Display
	LCD_cmd(0x38);
	LCD_cmd(0x0C);
	LCD_cmd(0x01);
	LCD_Set_Cursor(0,0);
	// Send Commands to the LCD Display
	
	// LCD Print Data
	LCD_Print("Hello World");
	// LCD Print Data
	sprintf(buffer,"Data: %d",164);
	LCD_Set_Cursor(1,0);
	LCD_Print(buffer);
}

void LCD_cmd(char val)
{
	PORTB &= ~(1 << PINB0);	// RS = '0', command mode
	PORTD = val;
	PORTB |= (1 << PINB1);	// E = '1'
	_delay_ms(5);
	PORTB &= ~(1 << PINB1);	// E = '0'
}

void LCD_data(char val)
{
	PORTB |= (1 << PINB0);	// RS = '1', data mode
	PORTD = val;
	PORTB |= (1 << PINB1);	// E = '1'
	_delay_ms(5);
	PORTB &= ~(1 << PINB1);	// E = '0'
}

void LCD_Set_Cursor(int line, int character)
{
	if(line == 1)
	{
		LCD_cmd(0xC0 | character);
	}
	else
	{
		LCD_cmd(0x80 | character);
	}
}

void LCD_Print(char *str)
{
	int i=0;
	while(str[i] != 0)
	{
		LCD_data(str[i]);
		i++;
	}
}