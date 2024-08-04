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

#define WORD_CHECK 0
#define BYTE_CHECK 1

#define BIT_ON_OFF 2

#define OFF 2

#define ZERO 0

#define NOP 0x4C40 // NOP mov r0, r0

#define SRCCON 2
#define SRCCONOPTIONS 8

#define DIV2REMAINDER 2

#define NOP_PC_OFFSET 2

#define MNEMARRAY_MAX 40
#define MNEMARRAY_WORDMAX 7

#define SET_HIGH_BYTE 0xFF
#define UNSET_HIGH_BYTE 0x00

#define SET 1
#define CLEAR 0

#define ONE 1
#define SUB_ONE -1

#define BYTEINDEX 1
#define WORDINDEX 2
#define SUB_BYTEINDEX -1
#define SUB_WORDINDEX -2

#define PRE 1
#define POST 0

#define SHIFT_RIGHT 1

#define MSBYTE_SET 0xFF
#define MSBYTE_CLEAR 0x00

#define NUMERICAL_NIB 9
#define NIBtoNUMERICAL 10
#define PREV_INITIAL 3
#define CURR_INITIAL 7

#define PREV_SHIFT 13
#define FLT_SHIFT 8
#define CURR_SHIFT 5
#define V_SHIFT 4
#define SLP_SHIFT 3
#define N_SHIFT 2
#define Z_SHIFT 1

int opcode;
unsigned short dmar, dmbr;
int dctrl;

typedef enum instructiontype { // enumerator of s-record types
	BL, BEQBZ, BNEBNZ, BCBHS, BNCBLO, BN, BGE, BLT, BRA,
	ADD, ADDC, SUB, SUBC, DADD, CMP, XOR, AND, OR, BIT,
	BIC, BIS, MOV, SWAP, SRA, RRC, SWPB, SXT, SETPRI, SVC,
	SETCC, CLRCC, CEX, LD, ST, MOVL, MOVLZ, MOVLS, MOVH, LDR, STR
} instructiontype;

typedef enum registers { // register enum values
	R0, R1, R2, R3, R4, R5, R6, R7
} registers;

typedef enum idmar { // imar and dmar enum values
	DONE, READ, WRITE
} dmar_enum;

typedef enum cex_condition {
	EQ, NE, CSHS, CCLO, MI, PL, VS, VC, 
	HI, LS, GE, LT, GT, LE, TR, FL
} cex_condition;

void sigint_hdlr();

int fetch0(int* ictrl);

void fetch1(int instructionaddress, int* ictrl);

void printdecode(int nota2, int instructionaddress, char mnemarray, int instructionmnem);

void printdecode(int nota2, int instructionaddress, char mnemarray[][6]);

void decode(int instructionaddress);

// determine whether new dest is a word or byte
void set_srcconarray(unsigned short dest_value, int dest_num, int wordbyte);

void execute0();

void execute1();

void pipeline();

#endif //PIPELINEHEADER_H