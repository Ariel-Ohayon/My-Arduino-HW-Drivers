#include <Wire.h>
#include <Arduino.h>

#define LCD_cmd_En   0x0C
#define LCD_cmd_nEn  0x08
#define LCD_data_En  0x0D
#define LCD_data_nEn 0x09
#define LCD_ADD      0x27

// RS = 0, E = 2, RW = 1, DATA = 4 - 7 (pins between the lcd and the PCF8574)
// MCU (MicroController) <-- PCF8574 <-- LCD (16,2)

void lcd_cmd(char val) //function that do a command on the lcd (clear or setting the cursor)
                       //in all of this process the RS pin on the LCD is OFF ('0')
{
  Wire.begin (); // start I2C Communication
  Wire.beginTransmission (LCD_ADD); //send to the LCD data (according to the address)
                                    //sending the first Most Significant Bits (MSB)
  Wire.write ((val & 0xF0) | LCD_cmd_En); //send 4 MSB to the LCD and enable pin on LCD OFF ('0')
  Wire.write ((val & 0xF0) | LCD_cmd_nEn); //set the enable pin on LCD ON ('1')
  delay (5); //delay 5 mlliseconds
  
  Wire.write (((val & 0x0F) << 4) | LCD_cmd_En); //send 4 LSB to the LCD and enable pin on LCD OFF ('0')
  Wire.write (((val & 0x0F) << 4) | LCD_cmd_nEn); //set the enable pin on LCD ON ('1')
  delay (5); // delay 5 milliseconds
  
  Wire.endTransmission (); //end to send data from I2C Communication for this component
  Wire.end (); //end I2C Communication
}

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

void lcd_print (char STR[16]) // print a string of 16 characters on LCD
{
  int x = 0; //counter
  while (STR[x] != '\0') //check if the character is null and if not continue to print
  {
    lcd_data (STR[x]);//send character to print on display
    x++;
  }
}

void lcd_init () // initializing LCD (16x2) and work with 4 bit mode
{
  lcd_cmd (0x02); // set the addres on 0; 
  lcd_cmd (0x28); // work with 4 bit mode
  lcd_cmd (0x0C); // display on
  lcd_cmd (0x01); // clear display
}

void lcd_setCursor (int line, int Cursor) // function that set the cursor on a specific place on LCD
{
  if (line == 1)
  {
    lcd_cmd (0x80 | Cursor); //set the cursor on line 1 with the specific location on the line
  }
  else
  {
    lcd_cmd (0xC0 | Cursor); //set the cursor on line 2 with the specific location on the line
  }
}

void lcd_clear () // command that clear LCD
{
  lcd_cmd (0x01); //clear display
}
