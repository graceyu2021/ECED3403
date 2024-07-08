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

#define ZERO 0

#define INSTRUCTIONPRINTMAX 

#define NOP 0x4C40 // NOP mov r0, r0

#define SRCCON 2
#define SRCCONOPTIONS 8

#define DIV2REMAINDER 2

#define NOP_PC_OFFSET 2

// macros to mask instructionbit to identify instruction groups
#define LDRtoSTR_BITS(a) ((a & 0x8000) == 0x8000)
#define BLtoBRA_BITS(a) ((a & 0x4000) == 0)
#define MOVLtoMOVH_BITS(a) ((a & 0x2000) == 0x2000)
#define LDtoST_BITS(a) ((a & 0x1000) == 0x1000)
#define SETPRItoCLRCC_BITS(a) ((a & 0x0D80) == 0x0D80)
#define SETPRItoSVC_BITS(a) ((a & 0x0060) == 0)
#define ADDtoSXT_BITS(a) ((a & 0x1000) == 0)
#define MOVtoSWAP_BITS(a) ((a & 0x0D00) == 0x0C00)
#define SRAtoRRC_BITS(a) ((a & 0x0C30) == 0x0C00)
#define SWPBtoSXT(a) ((a & 0x0C00) == 0x0C00)

// macros to mask and shift instructionbit to identify instructionmnem
#define LDRtoSTR_ARRAY(a) ((a & 0x4000) >> 14)
#define BLtoBRA_ARRAY(a) ((a & 0x1C00) >> 10)
#define MOVLtoMOVH_ARRAY(a) ((a & 0x1800) >> 11)
#define LDtoST_ARRAY(a) ((a & 0x0400) >> 10)
#define SETPRItoSVC_ARRAY(a) ((a & 0x0010) >> 4)
#define SETCCtoCLRCC_ARRAY(a) ((a & 0x0040) >> 6)
#define MOVtoSWAP_ARRAY(a) ((a & 0x0080) >> 7)
#define SRAtoRRC_ARRAY(a) ((a & 0x0008) >> 3)
#define SWPBtoSXT_ARRAY(a) ((a & 0x0020) >> 5)
#define ADDtoBIS_ARRAY(a) ((a & 0x0F00) >> 8)

// macros to mask and shift instructionbit to identify operand parameters
#define MSB_BITS(a) ((a & 0x2000) << 1)
#define CLR_OFF_BIT8(a) (a & 0x3F80)
#define OFF_BITS(a) ((a & 0x7F80) >> 7)
#define SRCCON_BITS(a) ((a & 0x0038) >> 3)
#define SRCCONCHECK_BITS(a) ((a & 0x0080) >> 7)
#define WORDBYTE_BITS(a) ((a & 0x0040) >> 6)
#define BYTEVALUE_BITS(a) ((a & 0x07F8) >> 3)
#define DST_BITS(a) (a & 0x0007)
#define PRPO_BITS(a) ((a & 0x0200) >> 9)
#define DEC_BITS(a) ((a & 0x0100) >> 8)
#define INC_BITS(a) ((a & 0x0080) >> 7)
#define V_BITS(a) ((a & 0x0010) >> 4)
#define SLP_BITS(a) ((a & 0x0008) >> 3)
#define N_BITS(a) ((a & 0x0004) >> 2)
#define Z_BITS(a) ((a & 0x0002) >> 1)
#define C_BITS(a) (a & 0x0001)

#define OFF_PRINT(a) (a == LDR || a == STR)
#define ADDRESSING_PRINT(a) (a == LD || a == ST)
#define BYTEVALUE_PRINT(a) (a >= MOVL && a <= MOVH)
#define SRCCONCHECK_PRINT(a) (a >= ADD && a <= BIS)
#define WORDBYTE_PRINT(a) ((a >= ADD && a <= MOV) || a == SRA || a == RRC || a == LD || a == ST || a == LDR || a == STR)
#define SRCCON_PRINT(a) ((a >= ADD && a <= SWAP) || a == LD || a == ST || a == LDR || a == ST)
#define SRCCON_CHECK(a) (rec_const.srccon == CONSTANT && (a >= ADD && a <= BIS))
#define DST_PRINT(a) ((a >= ADD && a <= SXT) || (a >= LD && a <= STR))
#define FLAG_PRINT(a) (a == SETCC || a == CLRCC)

#define MASK_SHIFT_MSB_WORD(a) ((a & 0x8000) >> 15)
#define MASK_SHIFT_MSB_BYTE(a) ((a & 0x0080) >> 7)

#define MASK_MSB_WORD(a) (a & 0x8000)
#define MASK_MSB_BYTE(a) (a & 0x0080)
#define MASK_LSB(a) (a & 0x0001)
#define MASK_BYTE(a) (a & 0x0FF)

#define MNEMARRAY_MAX 40
#define MNEMARRAY_WORDMAX 6

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

#define WORD_OFF(a) (a & 0xFFFE)

#define SHIFT_RIGHT 1

#define CARRY_TO_MSB_WORD(a) (a << 15)
#define CARRY_TO_MSB_BYTE(a) (a << 7)

#define BIT_SHIFT(a) (1 << a);

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

#define NIB_0(a) (a & 0x000F)
#define NIB_1(a) ((a & 0x00F0) >> 4)
#define NIB_2(a) ((a & 0x0F00) >> 8)
#define NIB_3(a) ((a & 0xF000) >> 12)

#define NIB_0_SET(a, b) (a | b)
#define NIB_1_SET(a, b) (a | (b << 4))
#define NIB_2_SET(a, b) (a | (b << 8))
#define NIB_3_SET(a, b) (a | (b << 12))

// structure of operands
typedef struct operands_struct {
	unsigned short off, bytevalue, prpo, dec, inc, wordbyte, srcconcheck, srccon, dst, v, slp, n, z, c;
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