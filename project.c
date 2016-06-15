#include <AT898252.h>

#include <stdlib.h>
#include <LCD.c>

//int rand_cache = 0xFFFF;
//int rand_bits_left = 0;
//char rand_u4() {}

char solution = 0;

char rechendata[11];
int tmp = 0;
int gen_op = 0; // <- boolean
int gen_eq = 0;
int gen_in = 0;

void generate() {
	for (tmp = 0; tmp < 11; tmp++) {
		P2 = tmp << 4;
		if (gen_op == 0) {
			if (gen_in == 0 && rand() & 0x1 == 1) {
				rechendata[tmp] = '_';
				gen_in++;
			} else {
				rechendata[tmp] = '0' + rand() % 9;
			}
			gen_op = 1;
		} else {
			if (tmp > 7 && gen_eq == 0) {
					rechendata[tmp] = '=';
					gen_eq++;
			} else {
				switch (rand() % 3) {
					case 0:
						rechendata[tmp] = '+';
						break;
					case 1:
						rechendata[tmp] = '-';
						break;
					case 2:
						if (gen_eq == 1) {
							rechendata[tmp] = 0;
							return;
						}
						rechendata[tmp] = '=';
						gen_eq++;
						break;
				}
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