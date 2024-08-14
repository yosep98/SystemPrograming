#include "lcd.h"

int fd;  // seen by all subroutines

void lcd_toggle_enable(int bits)   {
  // Toggle enable pin on LCD display
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits | ENABLE));
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
  delayMicroseconds(500);
}

void lcd_byte(int bits, int mode)   {

  //Send byte to data pins
  // bits = the data
  // mode = 1 for data, 0 for command
  int bits_high;
  int bits_low;
  // uses the two half byte writes to LCD
  bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
  bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

  // High bits
  wiringPiI2CReadReg8(fd, bits_high);
  lcd_toggle_enable(bits_high);

  // Low bits
  wiringPiI2CReadReg8(fd, bits_low);
  lcd_toggle_enable(bits_low);
}

// this allows use of any size string
void typeln(const char *s)   {
  while ( *s ) lcd_byte(*(s++), LCD_CHR);
}

// float to string
void typeFloat(float myFloat)   {
  char buffer[20];
  sprintf(buffer, "%4.2f",  myFloat);
  typeln(buffer);
}

// int to string
void typeInt(int i)   {
  char array1[20];
  sprintf(array1, "%d",  i);
  typeln(array1);
}

// clr lcd go home loc 0x80
void ClrLcd(void)   {
  lcd_byte(0x01, LCD_CMD);
  lcd_byte(0x02, LCD_CMD);
}

// go to location on LCD
void lcdLoc(int line)   {
  lcd_byte(line, LCD_CMD);
}

// out char to LCD at current position
void typeChar(char val)   {
  lcd_byte(val, LCD_CHR);
}

void lcd_init()   {
  // Initialise display
  lcd_byte(0x33, LCD_CMD); // Initialise
  lcd_byte(0x32, LCD_CMD); // Initialise
  lcd_byte(0x06, LCD_CMD); // Cursor move direction
  lcd_byte(0x0C, LCD_CMD); // 0x0F On, Blink Off
  lcd_byte(0x28, LCD_CMD); // Data length, number of lines, font size
  lcd_byte(0x01, LCD_CMD); // Clear display
  delayMicroseconds(500);
}

//Show "Have a nice day!!" on lcd
void activate_LCD()   {

  if (wiringPiSetup () == -1) exit (1);

  fd = wiringPiI2CSetup(I2C_ADDR);

  lcd_init(); // setup LCD

  char array1[] = "Have a";
  char array2[] = "Nice day!!";
  
  ClrLcd();
  lcdLoc(LINE1);
  typeln(array1);
  lcdLoc(LINE2);
  typeln(array2);
}

//Show "Welcome!! Master!!" on lcd
void welcome_LCD(void){
  if (wiringPiSetup () == -1) exit (1);

  fd = wiringPiI2CSetup(I2C_ADDR);

  lcd_init(); // setup LCD

  char array1[] = "Welcome!!";
  char array2[] = "Master!!";
  
  ClrLcd();
  lcdLoc(LINE1);
  typeln(array1);
  lcdLoc(LINE2);
  typeln(array2);
}

//Show "The box has arrived!!" on lcd
void box_LCD(void){
  if (wiringPiSetup () == -1) exit (1);

  fd = wiringPiI2CSetup(I2C_ADDR);

  lcd_init(); // setup LCD

  char array1[] = "The Box";
  char array2[] = "has arrived!!";
  
  ClrLcd();
  lcdLoc(LINE1);
  typeln(array1);
  lcdLoc(LINE2);
  typeln(array2);
}

//clear lcd text boxes
void clear_LCD(){
  if (wiringPiSetup () == -1) exit (1);

  fd = wiringPiI2CSetup(I2C_ADDR);

  lcd_init();

  ClrLcd();
}