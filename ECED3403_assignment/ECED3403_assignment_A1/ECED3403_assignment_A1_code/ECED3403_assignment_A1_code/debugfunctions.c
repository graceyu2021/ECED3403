/*
file: debugfunctions.c
programmer: Grace Yu
b00: B00902046
class: ECED3403
professor: Dr. Larry Hughes
assignment: lab3
submission date: May 23rd, 2024

This is the debug functions file of my program.
*/

#include "MAINHEADER.H"

void mem_change() {
	int address, content;
	char memtype;

	// change instruction or data memory? and scan
	printf("Change instruction or data memory?\nI - instruction memory\nD - data memory\n");
	scanf(" %c", &memtype);

	// prompt user for instruction address to change and new contents
	printf("Enter address and contents (word) \n");
	scanf(" %x %x", &address, &content);

	address = address / BYTE;

	if (INVALID_HEX(address) || INVALID_HEX(content)) {
		return;
	}

	switch (memtype) {
	case('I'): // instruction memory
	case('i'):
		imem.word_mem[address] = (unsigned short)content;
		break;

	case('D'): // data memory
	case('d'):
		dmem.word_mem[address] = (unsigned short)content;
		break;
	}
}

void breakpoint_set() {
	printf("Current breakpoint: %04x\n", breakpoint);
	printf("Set breakpoint address\n");
	scanf("%x", &breakpoint);
}

void reg_display() {
	int i = 0;
	char regprintarray[REG_NO][REG_NAME_MAX] = { "R0: ", "R1: ", "R2: ", "R3: ", "R4 (BP): ", "R5 (LR): ", "R6 (SP): ", "R7 (PC): " };

	for (i; i < REG_NO; i++) {
		printf("%s %04x\n", regprintarray[i], srcconarray.word[REGISTER][i]);
	}
}

void reg_set() {
	int regno, value;

	printf("Enter regno (0..7) and value (0000..FFFFF)\n");
	scanf(" %d %x", &regno, &value);

	srcconarray.word[REGISTER][regno] = (unsigned short) value;
}