#include <AT898252.h>

#include <stdlib.h>
#include <LCD.c>

int rand_cache = 0xFFFF;
int rand_bits_left = 0;
char rand_u4() {
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

char solution = 0;

char rechendata[11];
int tmp = 0;
int gen_op = 0; // <- boolean, next char is op
int gen_eq = 0; // amount equals
int gen_in = 0; // amount input

void gen_opc() {
	if (tmp > 7 && gen_eq == 0) {
		rechendata[tmp] = '=';
		gen_eq++;
	} else {
		switch (rand_u4() % 4) {
			case 0:
				rechendata[tmp] = '+';
				break;
			case 1:
				rechendata[tmp] = '-';
				break;
			case 2:
			case 3:
				if (gen_eq == 1) {
					rechendata[tmp] = 0;
				} else {
					rechendata[tmp] = '=';
					gen_eq++;
				}
				break;
		}
	}
}

void gen_val() {
	if (gen_in == 0 && (rand_u4() & 0x11 == 2 || tmp > 6)) {
		rechendata[tmp] = '_';
		gen_in++;
	} else {
		rechendata[tmp] = '0' + rand_u4() % 9;
	}
}

void generate() {
	gen_op = 0;
	gen_eq = 0;
	gen_in = 0;
	for (tmp = 0; tmp < 11; tmp++) {
		//P2 = tmp << 4;
		if (gen_op == 0) {
			gen_val();
			gen_op = 1;
		} else {
			gen_opc();
			if (rechendata[tmp] == 0) {
				return;
			}
			gen_op = 0;
		}
	}
	rechendata[tmp++] = 0;
}

void check() {
	
}

void main() {
	srand(P2&0xF);
	P2 = P2&0xF >> 4;
	
	generate();
	P2 = 0xF0;
	initlcd();
	P2 = 0x00;
	tmp = 0;
	while (1) {
		if (rechendata[tmp] == 0) {
			while(1){}
		}
		charlcd(rechendata[tmp]);
		tmp++;
	}
	
}