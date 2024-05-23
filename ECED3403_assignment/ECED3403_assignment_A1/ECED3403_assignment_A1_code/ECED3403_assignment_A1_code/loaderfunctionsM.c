/*
file: loaderfunctionsM.c
programmer: Grace Yu
b00: B00902046
class: ECED3403
professor: Dr. Larry Hughes
assignment: A1
submission date: May 23rd, 2024

This is the main file of my program.
*/

#include "LOADERHEADER.H"
#include <stdio.h>

void display_mem() {
	char memtype, inumtemp, dnumtemp;
	int lowbound = 0, upbound = 0, lowboundtemp = 0, upboundtemp = 0;

	// prompt user to select instruction or data memory
	printf("Display instruction or data memory?\nI - instruction memory\nD - data memory\n");
	scanf(" %c", &memtype);

	printf("Enter lower and upper bound\n");
	scanf(" %x %x", &lowbound, &upbound);
	lowboundtemp = lowbound;

	printf("%x %x\n %x\n", lowbound, upbound, upbound - lowbound);
	while (lowbound < upbound) {
		upboundtemp = lowbound + HEXRANGE;

		// print memory location label
		printf("%04x: ", lowbound);

		for (lowboundtemp; lowboundtemp < upboundtemp; lowboundtemp++) {
			switch (memtype) {
			case('I'):
			case('i'):
				printf("%02x ", imem.byte_mem[lowboundtemp]);
				break;

			case('D'):
			case('d'):
				printf("%02x ", dmem.byte_mem[lowboundtemp]);
				break;

			default:
				return;
			}
		}

		for (lowbound; lowbound < upboundtemp; lowbound++) {

			switch (memtype) {
			case('I'):
			case('i'):
				inumtemp = (char)imem.byte_mem[lowbound];

				if (inumtemp > 31 && inumtemp < 127) {
					printf("%c", inumtemp);
				}
				else {
					printf(".");
				}
				break;

			case('D'):
			case('d'):
				dnumtemp = (char)dmem.byte_mem[lowbound];
				if (dnumtemp > 31 && dnumtemp < 127) {
					printf("%c", dnumtemp);
				}
				else {
					printf(".");
				}
				break;

			default:
				return;
			}
		}

		printf("\n");

		if ((upbound - lowbound) <= HEXRANGE) {
			return;
		}
	}
	
}