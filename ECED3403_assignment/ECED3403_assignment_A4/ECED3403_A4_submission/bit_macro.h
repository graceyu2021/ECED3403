/*
file:bit_macro.h
programmer: Grace Yu
b00: B00902046
class: ECED3403
professor: Dr. Larry Hughes

This is the bit macro header file of my program.
*/

#ifndef BIT_MACRO_H
#define BIT_MACRO_H

// opcode verify macros
#define OFF_PRINT(a) ((a >= BL && a <= BRA) || a == LDR || a == STR)
#define ADDRESSING_PRINT(a) (a == LD || a == ST)
#define BYTEVALUE_PRINT(a) (a >= MOVL && a <= MOVH)
#define SRCCONCHECK_PRINT(a) (a >= ADD && a <= BIS)
#define WORDBYTE_PRINT(a) ((a >= ADD && a <= MOV) || a == SRA || a == RRC || a == LD || a == ST || a == LDR || a == STR)
#define SRCCON_PRINT(a) ((a >= ADD && a <= SWAP) || a == LD || a == ST || a == LDR || a == STR)
#define SRCCON_CHECK(a) (rec_const.srccon == CONSTANT && (a >= ADD && a <= BIS))
#define DST_PRINT(a) ((a >= ADD && a <= SXT) || (a >= LD && a <= STR))
#define FLAG_PRINT(a) (a == SETCC || a == CLRCC)

// macros to mask instructionbit to identify instruction groups
#define LDRtoSTR_BITS(a) ((a & 0x8000) == 0x8000)
#define BL_BITS(a) ((a & 0x6000) == 0)
#define BEQtoBRA_BITS(a) ((a & 0x6000) == 0x2000)
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
#define MSB_BITS(a) (a & 0x2000)
#define BL_MSB_BITS(a) (a & 0x1000)
#define BRANCH_MSB_BITS(a) (a & 0x0200)
#define SXT_OFF_BITS 0xFF80
#define SXT_BL_OFF_BITS 0xC000
#define SXT_BRANCH_OFF_BITS 0xF800
#define OFF_BITS(a) ((a & 0x3F80) >> 7)
#define BL_OFF_BITS(a) ((a & 0x1FFF) << 1)
#define BRANCH_OFF_BITS(a) ((a & 0x03FF) << 1)
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

#define MASK_SHIFT_MSB_WORD(a) ((a & 0x8000) >> 15)
#define MASK_SHIFT_MSB_BYTE(a) ((a & 0x0080) >> 7)

#define MASK_MSB_WORD(a) (a & 0x8000)
#define MASK_MSB_BYTE(a) (a & 0x0080)
#define MASK_LSB(a) (a & 0x0001)
#define MASK_BYTE(a) (a & 0x00FF)

#define WORD_OFF(a) (a & 0xFFFE)

#define CARRY_TO_MSB_WORD(a) (a << 15)
#define CARRY_TO_MSB_BYTE(a) (a << 7)

#define BIT_SHIFT(a) (1 << a);

#define NIB_0(a) (a & 0x000F)
#define NIB_1(a) ((a & 0x00F0) >> 4)
#define NIB_2(a) ((a & 0x0F00) >> 8)
#define NIB_3(a) ((a & 0xF000) >> 12)

#define NIB_0_SET(a, b) (a | b)
#define NIB_1_SET(a, b) (a | (b << 4))
#define NIB_2_SET(a, b) (a | (b << 8))
#define NIB_3_SET(a, b) (a | (b << 12))

#define RECORD_BOUND(a, b) ((a) >= (BYTE * 4) && (a) < (2 * b + BYTE))

#endif