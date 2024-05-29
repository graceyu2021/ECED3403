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

#ifndef PIPELINEHEADER_H
#define PIPELINEHEADER_H

#include <stdio.h>

#define REGISTER 0
#define CONSTANT 1

#define WORD 0

#define ZERO 0

#define INSTRUCTIONPRINTMAX 5

#define NOP 0x4C44 // NOP mov r0, r0

#define READ 1

typedef enum instructiontype { // enumerator of s-record types
	BL, BEQBZ, BNEBNZ, BCBHS, BNCBLO, BN, BGE, BLT, BRA, 
	ADD, ADDC, SUB, SUBC, DADD, CMP, XOR, AND, OR, BIT,
	BIC, BIS, MOV, SWAP, SRA, RRC, SWPB, SXT, SETPRI, SVC, 
	SETCC, CLRCC, CEX, LD, ST, MOVL, MOVLZ, MOVLS, MOVH, LDR, STR
} instructiontype;


int fetch0(int* programcounter, int* ictrl);

int fetch1(int instructionaddress, int* instructionbit);

void decode(int instructionaddress, int instructionbit, int instructionmnem, int* sourceconstant, int* wordbyte, int* source, int* destination);

void pipeline();


#endif //PIPELINEHEADER_H