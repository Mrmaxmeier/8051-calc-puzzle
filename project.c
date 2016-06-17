#include <AT898252.h>

#include <utils.c>

char solution = 0xFF;

char rightside[5];
char leftside[5];
int tmp = 0;
int tmp2 = 0;
int gen_op = 0; // <- boolean, next char is op
int gen_in = 0; // amount input

int calc_current; 
char calc_last_op;
char calc_curr_num;
// TODO: split up in multiple c files

int calc(char *dat, char replacement) {
	// oh shit waddup
	calc_current = char_to_int(*dat);
	dat++;
	calc_last_op = *dat;
	dat++;
	for (tmp = 0; tmp < 3; tmp++) {
		/*charlcd('!');
		charlcd(calc_last_op);
		charlcd(':');*/
		calc_curr_num = char_to_int(*dat, replacement);
		// display_num(calc_curr_num);
		switch (calc_last_op) {
			case '+':
				calc_current += calc_curr_num;
				break;
			case '-':
				calc_current -= calc_curr_num;
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
		return '1' + rand_u4() % 8;
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
unsigned char generate_with_solution() {
	while (1) {
		generate();
		loeschenlcd();
		cursorhome();
		for (tmp = 0; tmp < 5; tmp++) {
			charlcd(leftside[tmp]);
		}
		charlcd('=');
		for (tmp = 0; tmp < 5; tmp++) {
			charlcd(rightside[tmp]);
		}
		cursorpos(0x40);
		for (tmp = 0; tmp < 5; tmp++) {
			charlcd(rightside[tmp]);
		}
		charlcd('=');
		rightside_val = calc(&rightside, 0);
		display_num(rightside_val);
		charlcd(' ');
		// hacky optimization for '+' and '-'
		P2 = 0;
		tmp = calc(&leftside, 0) - rightside_val;
		P2 = 0xFF00;
		if (tmp == 0) {
			return 0;
		} else if (tmp > 9) {
			continue;
			// return 0xFF;
		} else if (tmp < -9) {
			continue;
			// return 0xFF;
		}
		
		for (tmp2 = 1; tmp2 < 9; tmp2++) {
			P2 = tmp << 4;
			if (calc(&leftside, tmp2) == rightside_val) {
				display_num(tmp2);
				return tmp2;
			}
		}
		// return 0xFF;
	}
}

char solution;
void main() {
	srand(P2&0xF);
	P2 = P2&0xF >> 4;
	
	P2 = 0xF0;
	initlcd();
	P2 = 0x00;
	
	// display_num(-112);
	// halt();
	
	while (1) {
	solution = generate_with_solution();
	if (solution == 0xFF) {
		cursorpos(0x40);
		textlcd("NO SOLUTION", 2);
		halt();
	}
	charlcd('>');
	display_num(tmp);
	
	cursorpos(0x40);
	textlcd("PRESS A KEY!   ", 2);
	
	tmp = get_hex_input();
	loeschenlcd();
	if (tmp == solution) {
		textlcd("\\ :)", 1);
	} else {
		textlcd("/ :(", 1);
	}
	
	
	
	
	}
}