#include <AT898252.h>

#include <utils.c>

char solution = 0xFF;

char rightside[5];
char leftside[5];
int tmp = 0;
int gen_op = 0; // <- boolean, next char is op
int gen_in = 0; // amount input

int calc_current;
char calc_last_op;
// TODO: split up in multiple c files

int calc(char *dat, char replacement) {
	// oh shit waddup
	calc_current = *dat;
	dat++;
	calc_last_op = *dat;
	for (tmp = 0; tmp < 3; tmp++) {
		switch (calc_last_op) {
			case '+':
				calc_current += char_to_int(*dat, replacement);
				break;
			case '-':
				calc_current -= char_to_int(*dat, replacement);
				break;
		}
		dat++;
		calc_last_op = *dat;
		dat++;
	}
	return calc_current;
}

char gen_opc() {
	switch (rand_u4() % 2) {
		case 0:
			return '+';
		case 1:
			return '-';
	}
	return '?';
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

int rightside_val;
char generate_with_solution() {
	while (1) {
		generate();
		cursorhome();
		for (tmp = 0; tmp < 5; tmp++) {
			charlcd(leftside[tmp]);
		}
		charlcd('=');
		for (tmp = 0; tmp < 5; tmp++) {
			charlcd(rightside[tmp]);
		}
		rightside_val = calc(&rightside, 0);
		cursorpos(0x40);
		display_num(rightside_val);
		charlcd(' ');
		for (tmp = 0; tmp < 9; tmp++) {
			if (calc(&leftside, tmp) == rightside_val) {
				display_num(tmp);
				return tmp;
			}
		}
	}
}

void main() {
	srand(P2&0xF);
	P2 = P2&0xF >> 4;
	
	P2 = 0xF0;
	initlcd();
	P2 = 0x00;
	
	// display_num(-112);
	// halt();
	cursorpos(0x44);
	display_num(generate_with_solution());
	cursorhome();
	for (tmp = 0; tmp < 5; tmp++) {
		charlcd(leftside[tmp]);
	}
	charlcd('=');
	for (tmp = 0; tmp < 5; tmp++) {
		charlcd(rightside[tmp]);
	}
	
	halt();
}