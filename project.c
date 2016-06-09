#include <AT898252.h>

// https://www.8051projects.net/lcd-interfacing/initialization.php
// https://www.8051projects.net/lcd-interfacing/lcd-4-bit-programming.php
#define LCD P1
#define LCD_EN P1_6
#define LCD_RS P1_7
#define IN P0

int tmp = 0;

void delayus(int time) {
	int cycles = time / 2;
	for (tmp = 0; tmp < cycles; tmp++);
}

void LCD_cmd (unsigned char cmd) { 
 	LCD = ((cmd >> 4) & 0x0F)|LCD_EN;
 	LCD = ((cmd >> 4) & 0x0F);

 	LCD = (cmd & 0x0F)|LCD_EN;
 	LCD = (cmd & 0x0F);

	delayus(40);
}
     
void LCD_data (unsigned char dat) { 
	LCD = (((dat >> 4) & 0x0F)|LCD_EN|LCD_RS);
 	LCD = (((dat >> 4) & 0x0F)|LCD_RS);
   
 	LCD = ((dat & 0x0F)|LCD_EN|LCD_RS);
 	LCD = ((dat & 0x0F)|LCD_RS);
     
 	delayus(40);
}

void LCD_sendstring(unsigned char *var)
{
  while(*var)              //till string ends
    LCD_data(*var++);  //send characters one by one
}

void LCD_reset() {
	LCD = 0xFF;
	delayus(20);
	LCD = 0x83;
	LCD = 0x03;
	delayus(10);
	LCD = 0x83;
	LCD = 0x03;
	delayus(1);
	LCD = 0x83;
	LCD = 0x03;
	delayus(1);
	LCD = 0x82;
	LCD = 0x02;
	delayus(1);
}

void LCD_init() {
	LCD_reset();
	LCD_cmd(0x28);       // 4-bit mode - 2 line - 5x7 font. 
	LCD_cmd(0x0C);       // Display no cursor - no blink.
	LCD_cmd(0x06);       // Automatic Increment - No Display shift.
	LCD_cmd(0x80);       // Address DDRAM with 0 offset 80h.
}

void main () {
	LCD_init();
	LCD_sendstring("DEINEMUDDADADADADDADA");
}