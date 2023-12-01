#ifndef LCD_H
#define LCD_H
#endif



void lcd_cmd(char val);
void lcd_data(char val);
void lcd_print(char STR[16]);
void lcd_init();
void lcd_clear();
void lcd_setCursor(int line,int Cursor);
