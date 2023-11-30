#include "LCD.h"
void setup()
{
	lcd_init();
	lcd_clear();
	lcd_setCursor(1,0);
	lcd_print("Hello World");
}

void loop()
{
	while(1);
}
