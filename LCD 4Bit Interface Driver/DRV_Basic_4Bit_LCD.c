#include<avr/io.h>
#include<util/delay.h>

void LCD_cmd(char val);


void main(void)
{
	char buffer[16];
	// -- Define Output Pins -- //
	DDRD |= 0xF0;
	DDRB |= (1 << PINB0) | (1 << PINB1);
	PORTB &= ~(1 << PINB1);		//	E = '0'
	// -- Define Output Pins -- //
	
	// -- Send Initialization commands -- //
	LCD_cmd(0x02);
	LCD_cmd(0x28);
	LCD_cmd(0x0C);
	// -- Send Initialization commands -- //
	LCD_cmd(0x01);
	LCD_Set_Cursor(0,0);
	LCD_data('A');
	_delay_ms(5000);
	LCD_cmd(0x01);
	LCD_Print("Hello World");
	LCD_Set_Cursor(1,0);
	sprintf(buffer,"Data = %d",115);
	LCD_Print(buffer);
}

void LCD_cmd(char val)
{
	PORTB &= ~(1 << PINB0);	// RS = '0'
	PORTD |= (val & 0xF0);
	PORTB |= (1 << PINB1);	// E = '1'
	_delay_ms(5);
	PORTB &= ~(1 << PINB1);	// E = '0'
	_delay_ms(1);
	PORTD &= (0x0F);
	PORTD |= ((val & 0x0F) << 4);
	PORTB |= (1 << PINB1);	// E = '1'
	_delay_ms(5);
	PORTB &= ~(1 << PINB1);	// E = '0'
	_delay_ms(1);
	PORTD &= 0x0F;
}

void LCD_data(char val)
{
	PORTB |= (1 << PINB0);	// RS = '1'
	PORTD |= (val & 0xF0);
	PORTB |= (1 << PINB1);	// E = '1'
	_delay_ms(5);
	PORTB &= ~(1 << PINB1);	// E = '0'
	_delay_ms(1);
	PORTD &= (0x0F);
	PORTD |= ((val & 0x0F) << 4);
	PORTB |= (1 << PINB1);	// E = '1'
	_delay_ms(5);
	PORTB &= ~(1 << PINB1);	// E = '0'
	_delay_ms(1);
	PORTD &= 0x0F;
}

void LCD_Print(char* str)
{
	int i=0;
	while(str[i] != 0)
	{
		LCD_data(str[i]);
		i++;
	}
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