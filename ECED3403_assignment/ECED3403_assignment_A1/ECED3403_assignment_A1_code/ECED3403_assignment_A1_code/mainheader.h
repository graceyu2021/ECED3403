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

/*typedef union wordbyte {
	unsigned short word;
	unsigned char byte[BYTE];
}wordbyte;
*/

// declared array for file name, imem, and dmem global
memory imem; // instruction memory
memory dmem; // data memory
unsigned short srcconarray[SRCCON][SRCCONOPTIONS]; // source and constant array
int recname[SREC_MAX]; // asm file name
int recnamecount; // size of recname
int startaddress; // start address
int breakpoint;

#endif //MAINHEADER_H