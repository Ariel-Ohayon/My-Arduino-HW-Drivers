/* LCD I2C Library Hello World Example.
 * This example print on the LCD module the string: "Hello World"
 */

#include <LCD_I2C.h>

void setup() {
  // Initialize the LCD:
  lcd_init();

  // Making sure that the display is clean from characters by using clear command:
  lcd_clear();

  // Setting up the cursor of the display to the first character
  lcd_setCursor(1,0);

  // Print to the LCD the string: "Hello World"
  lcd_print("Hello World");

}

void loop(){
}
