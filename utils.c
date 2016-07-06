#include <stdlib.h>
#include <AT898252.h>
#include "LCD.c"

#define halt() while (1) {}

int rand_cache = 0xFFFF;
int rand_bits_left = 0;
unsigned char rand_u4() {
	if (rand_bits_left < 4) {
		rand_cache = rand();
		rand_bits_left = 16;
	} else {
		rand_cache = rand_cache >> 4;
		rand_bits_left -= 4;
	}
	P2 = (rand_cache & 0xF) << 4;
	return rand_cache & 0xF;
}


char char_to_int(char c, char err_char) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	return err_char;
}

void display_num(char c) {
	if (c < 0) {
		charlcd('-');
		c = -c;
	}
	
	if (c >= 100) {
		charlcd('0' + c / 100);
		while (c > 100) {
			c -= 100;
		}
		charlcd('0' + c / 10);
		while (c > 10) {
			c -= 10;
		}
	} else if (c >= 10) {
		charlcd('0' + c / 10);
		while (c > 10) {
			c -= 10;
		}
	}
	
	charlcd('0' + c);
}

char hex_i;
char get_hex_input() {
	while (P0 == 0xFF) {}
	while (1) {
		hex_i = P0;
		if (hex_i != 0xFF) {
			return hex_i;
		}
	}
}