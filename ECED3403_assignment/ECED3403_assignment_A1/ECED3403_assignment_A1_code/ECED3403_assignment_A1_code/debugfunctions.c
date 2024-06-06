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

	if (content < 0x0000 || content > 0xFFFF) {
		return;
	}

	switch (memtype) {
	case('I'): // instruction memory
	case('i'):
		printf("before: %04x", imem.word_mem[address]);
		imem.word_mem[address] = (unsigned short)content;
		printf("before: %04x", imem.word_mem[address]);
		break;

	case('D'): // data memory
	case('d'):
		printf("before: %04x", dmem.word_mem[address]);
		dmem.word_mem[address] = (unsigned short)content;
		printf("before: %04x", dmem.word_mem[address]);
		break;
	default:
		break;
	}
}

void breakpoint_set() {
	printf("Current breakpoint: %04x\n", breakpoint);
	printf("Set breakpoint address\n");
	scanf("%x", &breakpoint);

	return;
}

void reg_display() {
	int i = 0;
	char regprintarray[REG_NO][REG_NAME_MAX] = { "R0: ", "R1: ", "R2: ", "R3: ", "R4 (BP): ", "R5 (LR): ", "R6 (SP): ", "R7 (PC): " };

	for (i; i < REG_NO; i++) {
		printf("%s %04x\n", regprintarray[i], srcconarray[REGISTER][i]);
	}

	return;
}

void reg_set() {
	int regno, value;

	printf("Enter regno (0..7) and value (0000..FFFFF)\n");
	scanf(" %d %x", &regno, &value);

	srcconarray[REGISTER][regno] = (unsigned short) value;

	return;
}