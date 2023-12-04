# LCD I2C Driver Interface Library
<p>
  This directory contain the firmware drivers and functions to operate the LCD module with the PCF8574 I2C converter.
</p>

## Functions
<p>
  The library contain six functions:<br>
  1. lcd_cmd(): send command to the LCD module.<br>
  2. lcd_data(): send ascii code for writing to the LCD module.<br>
  3. lcd_print(): print string on the LCD module.<br>
  4. lcd_init(): send initialize commands to the LCD module.<br>
  5. lcd_clear(): send clear command to the LCD module.<br>
  6. lcd_setCursor(): set cursor of the display on the LCD module.<br>
</p>

<p>
  lcd_cmd() function:<br>
  https://github.com/Ariel-Ohayon/My-Arduino-HW-Drivers/blob/be67c498882cc2156bd53f2bd9402d8471afea0d/LCD%20I2C%20Driver%20Interface/example/LCD.cpp#L13-L29
  
  lcd_data() function:<br>
  ```C++
  void lcd_data (char val) //function that print character on the display
                           //the same process like the "lcd_cmd" function but the RS pin on the LCD is ON ('1')
  {
    Wire.begin ();
    Wire.beginTransmission (LCD_ADD);
    Wire.write ((val & 0xF0) | LCD_data_En);
    Wire.write ((val & 0xF0) | LCD_data_nEn);
    delay (5);
  
  
    Wire.write (((val & 0x0F) << 4)| LCD_data_En);
    Wire.write (((val & 0x0F) << 4)| LCD_data_nEn);
    delay (5);
    
    Wire.endTransmission();
    Wire.end ();
  }
  ```
</p>
