
/*
file: loaderfunctionsM.c
programmer: Grace Yu
b00: B00902046
class: ECED3403
professor: Dr. Larry Hughes
assignment: A1
submission date: May 23rd, 2024

This is the loader functions MS file of my program.
*/

#include "MAINHEADER.H"
#include <stdio.h>

void idmem_print(memory array[], int* lowbound, int* lowboundtemp) {
	int upboundtemp = *lowbound + HEXRANGE;

	// print hexadecimal at memory addresses
	for (*lowboundtemp; *lowboundtemp < upboundtemp; (*lowboundtemp)++) {
		printf("%02x ", array->byte_mem[*lowboundtemp]);
	}

	// print ascii at memory addresses
	for (*lowbound; *lowbound < upboundtemp; (*lowbound)++) {
		char idnumtemp = (char)array->byte_mem[*lowbound]; // convert hexadecimal to ascii

		if ((idnumtemp >= ' ') && (idnumtemp <= '~')) { // range of printable characters
			printf("%c", idnumtemp);
		}
		else { // print '.' if unprintable ASCII character
			printf(".");
		}
	}
}

void mem_display() {
	char memtype, inumtemp, dnumtemp;
	int lowbound = 0, upbound = 0, lowboundtemp = 0, upboundtemp = 0;

	// prompt user to select instruction or data memory
	printf("Display instruction or data memory?\nI - instruction memory\nD - data memory\n");
	scanf(" %c", &memtype);

	printf("Enter lower and upper bound\n");
	scanf(" %x %x", &lowbound, &upbound);
	lowboundtemp = lowbound; // duplicate lowbound, one for each for loop

	while (lowbound < upbound) {

		// print memory location label
		printf("%04X: ", lowbound);

		switch (memtype) { 
		case('I'): // instruction memory
		case('i'):
			idmem_print(imem.byte_mem, &lowbound,&lowboundtemp);
			break;

		case('D'): // data memory
		case('d'):
			idmem_print(dmem.byte_mem, &lowbound, &lowboundtemp);
			break;
		}

		printf("\n");

		// check if enough remaining bytes to print a full row
		if ((upbound - lowbound) <= HEXRANGE) { 
			return; //exit if not enough
		}
	}
}