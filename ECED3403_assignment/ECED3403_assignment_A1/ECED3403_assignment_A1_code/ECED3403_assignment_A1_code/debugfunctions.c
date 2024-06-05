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

	if (content <= 0xFFFF && content >= 0x0000) {
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
}

void breakpoint_set() {
	printf("Current breakpoint: %04x\n", breakpoint);
	printf("Set breakpoint address\n");
	scanf("%x", &breakpoint);

	return;
}

void reg_display() {
	int i = 0;
	printf("R0: %04x\n", srcconarray[REGISTER][i++]);
	printf("R1: %04x\n", srcconarray[REGISTER][i++]);
	printf("R2: %04x\n", srcconarray[REGISTER][i++]);
	printf("R3: %04x\n", srcconarray[REGISTER][i++]);
	printf("R4 (BP): %04x\n", srcconarray[REGISTER][i++]);
	printf("R5 (LR): %04x\n", srcconarray[REGISTER][i++]);
	printf("R6 (SP): %04x\n", srcconarray[REGISTER][i++]);
	printf("R7 (PC): %04x\n", srcconarray[REGISTER][i++]);

	return;
}

void reg_set() {
	int regno, value;

	printf("Enter regno (0..7) and value (0000..FFFFF)\n");
	scanf(" %d %x", &regno, &value);

	srcconarray[REGISTER][regno] = (unsigned short) value;

	return;
}