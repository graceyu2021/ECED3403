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

#define ZERO 0

#define INSTRUCTIONPRINTMAX 

#define NOP 0x4C44 // NOP mov r0, r0

#define READ 1
#define DONEREAD 0

// macros to mask instructionbit to identify instruction groups
#define LDRtoSTR_BITS(a) ((a & 0x8000) == 0x8000)
#define BLtoBRA_BITS(a) ((a & 0x4000) == 0)
#define MOVLtoMOBH_BITS(a) ((a & 0x2000) == 0x2000)
#define LDtoST_BITS(a) ((a & 0x1000) == 0x1000)
#define MOVtoCLRCC_BITS(a) ((a & 0x0C00) == 0x0C00)
#define SETPRItoCLRCC_BITS(a) ((a & 0x0180) == 0x0180)
#define MOVtoSWAP_BITS(a) ((a & 0x0100) == 0)
#define SRAtoRRC_BITS(a) ((a & 0x0030) == 0)

// macros to mask and shift instructionbit to identify instructionmnem
#define MOVLtoMOVH_ARRAY(a) ((a & 0x1800) >> 11)
#define MOVtoSWAP_ARRAY(a) ((a & 0x0080) >> 7)
#define SRAtoRRC_ARRAY(a) ((a & 0x0038) >> 3)
#define SWPBtoSXT_ARRAY(a) ((a & 0x0020) >> 5)
#define ADDtoBIS_ARRAY(a) ((instructionbit & 0x0F00) >> 8)

// macros to mask and shift instructionbit to identify operand parameters
#define SOURCECONSTANT_BITS(a) ((a & 0x0038) >> 3)
#define SOURCECONSTANTCHECK_BITS(a) ((a & 0x0080) >> 7)
#define WORDBYTE_BITS(a) ((a & 0x0040) >> 6)
#define BYTEVALUE_BITS(a) ((a & 0x07F8) >> 3)

#define SOURCECONSTANTCHECK_PRINT(a) (a >= ADD && instructionmnem <= BIS)
#define WORDBYTE_PRINT(a) ((a >= ADD && a <= MOV) || (a >= SRA && a <= RRC))
#define SOURCECONSTANT_PRINT(a) (a >= ADD && a <= SWAP)
#define SOURCECONSTANT_SELECT(a, b) ((a == 0) || b == MOV || b == SWAP)
#define BYTEVALUE_PRINT(a) (a >= MOVL && a <= MOVH)

#define DESTINATION_BITS 0x0007
#define MNEMARRAY_MAX 40
#define MNEMARRAY_WORDMAX 6

// structure of operands for ADD to SXT
typedef struct reg_const {
	unsigned int sourceconstantcheck, wordbyte, sourceconstant, destination;
}reg_const;

reg_const reg_const_operands; // reg_const global struct

// structure of operands for MOVL to MOVH
typedef struct movx {
	unsigned int bytevalue, destination;
}movx;

movx movx_operands; // movx global struct

typedef enum instructiontype { // enumerator of s-record types
	BL, BEQBZ, BNEBNZ, BCBHS, BNCBLO, BN, BGE, BLT, BRA, 
	ADD, ADDC, SUB, SUBC, DADD, CMP, XOR, AND, OR, BIT,
	BIC, BIS, MOV, SWAP, SRA, RRC, SWPB, SXT, SETPRI, SVC, 
	SETCC, CLRCC, CEX, LD, ST, MOVL, MOVLZ, MOVLS, MOVH, LDR, STR
} instructiontype;

int fetch0(int* programcounter, int* ictrl);

int fetch1(int instructionaddress, int* instructionbit);

void printdecode(nota2, instructionaddress, mnemarray, instructionmnem, instructionbit);

void decode(int instructionaddress, int instructionbit, int instructionmnem);

void pipeline();

#endif //PIPELINEHEADER_H