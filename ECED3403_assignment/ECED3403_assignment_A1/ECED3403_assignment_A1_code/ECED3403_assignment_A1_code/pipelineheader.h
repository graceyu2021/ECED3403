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


#define SOURCECONSTANT_BITS 0x0038
#define SOURCECONSTANT_SHIFT 3
#define SOURCECONSTANTCHECK_BITS 0x0080
#define SOURCECONSTANTCHECK_SHIFT 7
#define WORDBYTE_BITS 0x0040
#define WORDBYTE_SHIFT 6
#define BYTEVALUE_BITS 0x07F8
#define BYTEVALUE_SHIFT 3

#define LDRtoSTR_BITS 0x8000
#define BLtoBRA_BITS 0x4000
#define MOVLtoMOVH_BITS 0x2000
#define LDtoST_BITS 0x1000
#define MOVtoCLRCC_BITS 0x0C00
#define MOVtoSWAP_BITS 0x0100

typedef enum instructiontype { // enumerator of s-record types
	BL, BEQBZ, BNEBNZ, BCBHS, BNCBLO, BN, BGE, BLT, BRA, 
	ADD, ADDC, SUB, SUBC, DADD, CMP, XOR, AND, OR, BIT,
	BIC, BIS, MOV, SWAP, SRA, RRC, SWPB, SXT, SETPRI, SVC, 
	SETCC, CLRCC, CEX, LD, ST, MOVL, MOVLZ, MOVLS, MOVH, LDR, STR
} instructiontype;


int fetch0(int* programcounter, int* ictrl);

int fetch1(int instructionaddress, int* instructionbit);

int savesourceconstant(int instructionbit);

int savesourceconstantcheck(int instructionbit);

int savewordbyte(int instructionbit);

int savebytevalue(int instructionbit);

void decode(int instructionaddress, int instructionbit, int instructionmnem, int* sourceconstantcheck, int* wordbyte, int* sourceconstant, int* bytevalue, int* destination);

void pipeline();


#endif //PIPELINEHEADER_H