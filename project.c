#include <AT898252.h>

#include <stdlib.h>
#include <LCD.c>
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

char solution = 0xFF;

char rightside[5];
char leftside[5];
int tmp = 0;
int gen_op = 0; // <- boolean, next char is op
int gen_in = 0; // amount input

int calc_current;
char calc_last_op;
// TODO: split up in multiple c files

char char_to_int(char c) {
	if (c == '_') {
		return 9;
	}
	return c - '0';
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

int calc(char *dat) {
	// oh shit waddup
	calc_current = *dat;
	dat++;
	calc_last_op = *dat;
	for (tmp = 0; tmp < 3; tmp++) {
		switch (calc_last_op) {
			case '+':
				calc_current += char_to_int(*dat);
				break;
			case '-':
				calc_current -= char_to_int(*dat);
				break;
		}
		dat++;
		calc_last_op = *dat;
		dat++;
	}
}

char gen_opc() {
	switch (rand_u4() % 2) {
		case 0:
			return '+';
		case 1:
			return '-';
	}
}

char gen_val() {
	if (gen_in == 0 && (rand_u4() & 0x11 == 2 || tmp > 6)) {
		gen_in++;
		return '_';
	} else {
		return '0' + rand_u4() % 9;
	}
}

void generate() {
	gen_op = 0;
	gen_in = 0;
	for (tmp = 0; tmp < 5; tmp++) {
		if (gen_op == 0) {
			if (gen_in == 0 && tmp > 3) {
				leftside[tmp] = '_';
				gen_in++;
			} else {
				leftside[tmp] = gen_val();
			}
			rightside[tmp] = gen_val();
			gen_op = 1;
		} else {
			leftside[tmp] = gen_opc();
			rightside[tmp] = gen_opc();
			gen_op = 0;
		}
	}
}

void main() {
	srand(P2&0xF);
	P2 = P2&0xF >> 4;
	
	generate();
	P2 = 0xF0;
	initlcd();
	P2 = 0x00;
	P2 = calc(&leftside);
	
	display_num(-112);
	halt();
	
	charlcd(calc(&leftside));
	charlcd(calc(&rightside));
	charlcd(' ');
	for (tmp = 0; tmp < 5; tmp++) {
		charlcd(leftside[tmp]);
	}
	charlcd('=');
	for (tmp = 0; tmp < 5; tmp++) {
		charlcd(rightside[tmp]);
	}
	
	halt();
}