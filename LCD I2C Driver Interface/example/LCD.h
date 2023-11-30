#ifndef LCD_H
#define LCD_H
#endif

#define LCD_cmd_En   0x0C
#define LCD_cmd_nEn  0x08
#define LCD_data_En  0x0D
#define LCD_data_nEn 0x09
#define LCD_ADD      0x3F

void lcd_cmd(char val);
void lcd_data(char val);
void lcd_print(char STR[16]);
void lcd_init();
void lcd_clear();
void lcd_setCursor(int line,int Cursor);
