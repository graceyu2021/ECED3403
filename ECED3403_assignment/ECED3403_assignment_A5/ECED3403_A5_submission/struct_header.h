/*
file: struct_header.h
programmer: Grace Yu
b00: B00902046
class: ECED3403
professor: Dr. Larry Hughes

This is the struct header file of my program.
*/

#ifndef STRUCT_HEADER_H
#define STRUCT_HEADER_H

// declared array for file name, imem, and dmem global
memory imem; // instruction memory
memory dmem; // data memory

typedef union srcconarray {
	unsigned char byte[SRCCON][SRCCONOPTIONS][BYTE];
	unsigned short word[SRCCON][SRCCONOPTIONS];
}srcconarray_union;

extern srcconarray_union srcconarray;

// structure of operands
typedef struct operands_struct {
	unsigned short off, bytevalue, prpo, dec, inc, wordbyte, srcconcheck, srccon, 
		dst, v, slp, n, z, c, cond, ctrue, cfalse, cstate;
}operands_struct;

operands_struct operand;

typedef struct psw_struct {
	unsigned short prev, flt, curr, v, slp, n, z, c; // arithmetic overflow, negative, zero, carry
}psw_struct;

extern psw_struct psw;

typedef struct nibble_struct {
	unsigned short n0, n1, n2, n3; // 4 nibbles in a word
}nibble_struct;

typedef union nibble_word_union {
	unsigned short word;
	unsigned char byte[BYTE];
	nibble_struct nibble;
}nibble_word;

#endif //MAINHEADER_H