// #include <AT898252.h>

#include <LCD.c>

void main() {
	initlcd();
	while (1) {
		charlcd('=');
		charlcd('+');
	}
	
}