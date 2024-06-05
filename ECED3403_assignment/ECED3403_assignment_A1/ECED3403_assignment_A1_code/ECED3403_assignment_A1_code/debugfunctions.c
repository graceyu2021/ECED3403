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

	if (content <= 0xFFFF && content >= 0x0000) {
		switch (memtype) {
		case('I'): // instruction memory
		case('i'):
			imem.word_mem[address] = content;
			break;

		case('D'): // data memory
		case('d'):
			dmem.word_mem[address] = content;
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
}

