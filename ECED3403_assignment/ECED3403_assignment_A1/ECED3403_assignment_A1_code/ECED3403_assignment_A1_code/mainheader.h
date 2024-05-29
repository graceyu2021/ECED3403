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


#include <stdio.h>

#define SREC_MAX 67 // defined macros for records

// declared array for file name, imem, and dmem global
union memory imem; // instruction memory
union memory dmem; // data memory
int recname[SREC_MAX]; // asm file name
int recnamecount; // size of recname
int startaddress; // start address

#endif //MAINHEADER_H