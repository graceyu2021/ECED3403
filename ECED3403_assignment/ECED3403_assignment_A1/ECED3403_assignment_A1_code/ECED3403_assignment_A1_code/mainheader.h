/*
file: loaderheader.h
programmer: Grace Yu
b00: B00902046
class: ECED3403
professor: Dr. Larry Hughes
assignment: A1
submission date: May 23rd, 2024

This is the header file of my program.
*/

#ifndef MAINHEADER_H
#define MAINHEADER_H

#include "LOADERHEADER.H"
#include "PIPELINEHEADER.H"
#include "DEBUGHEADER.H"

#include <stdio.h>

#define SREC_MAX 100//67 // defined macros for records

// declared array for file name, imem, and dmem global
memory imem; // instruction memory
memory dmem; // data memory

typedef union srcconarray {
	unsigned char byte[SRCCON][SRCCONOPTIONS][BYTE];
	unsigned short word[SRCCON][SRCCONOPTIONS];
}srcconarray_union;

extern srcconarray_union srcconarray;

int clock;
int instructionbit;
int breakpoint;
int increment;

#endif //MAINHEADER_H