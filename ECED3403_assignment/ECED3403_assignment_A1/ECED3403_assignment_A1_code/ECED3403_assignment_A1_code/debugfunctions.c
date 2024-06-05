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

void breakpoint_set() {
	printf("Current breakpoint: %04x\n", breakpoint);
	printf("Set breakpoint address\n");
	scanf("%x", &breakpoint);
}