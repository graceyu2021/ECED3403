/*
file: pipelineexecute_header.h
programmer: Grace Yu
b00: B00902046
class: ECED3403
professor: Dr. Larry Hughes

This is the pipeline execute header file of my program.
*/

#ifndef PIPELINEEXECUTE_HEADER
#define PIPELINEEXECUTE_HEADER

#include <stdio.h>

void bl_execute();

void beq_to_bra_execute(int condition);

// shorten variable names from add to bid
void add_to_bis_ops(int dest_num, int srccon_num, int srcconcheck, unsigned short* dest_value, unsigned short* srccon_value, int* wordbyte);

// shorten variable names from mov to sxt
void mov_to_sxt_ops(int dest_num, int srccon_num, unsigned short* dest_value, unsigned short* srccon_value, int* wordbyte);

// shorten variable names from movl to movh
void movl_to_movh_ops(unsigned short* bytevalue);

// determine which bit to mask
unsigned short mask_shift_msb(unsigned short value, int wordbyte);

// determine whether new dest is a word or byte
void set_srcconarray(unsigned short dest_value, int dest_num, int wordbyte);

// update psw
void psw_update(unsigned short v, unsigned short n, unsigned short z, unsigned short c);

// precursor to psw_update for arithmetic operations
void psw_arithmetic_update(unsigned short temp_result, unsigned short dest_value, unsigned short srccon_value, int wordbyte);

// BEQ to BRA execute
void beq_to_bra_execute(int condition);

// ADD to SUBC execute
void add_to_subc_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int wordbyte);

// binary coded decimal addition
unsigned short bcd(unsigned short dest_nib, unsigned short src_nib);

// DECIMAL ADD
void dadd_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int wordbyte);

// COMPARE: dst - src/con
void cmp_execute(unsigned short dest_value, unsigned short srccon_value);

// EXCLUSIVE: dst <- dst ^ src/con
void xor_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int wordbyte);

// AND: dst <- dst & src/con
void and_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int wordbyte);

// OR: dst <- dst | src/con
void or_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int wordbyte);

// BIT TEST: dst <- dst & (1 << src/co)
void bit_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num);

// BIT CLEAR: dst << dst & ~(1 << src/con) 
void bic_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int wordbyte);

// BIT SET: dst <- dst | (1 << src/con)
void bis_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int wordbyte);

// MOVE: dst <- src
void mov_execute(unsigned short srccon_value, int dest_num);

// SWAP src and dest
void swap_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int srccon_num);

// SHIFT DST: dst >> 1 while maintainig sign
void sra_execute(unsigned short dest_value, int dest_num, int wordbyte);

// ROTATE DST WITH CARRY: dst >> 1, dst(msb) == 1 
void rrc_execute(unsigned short dest_value, int dest_num, int wordbyte);

// SWAP DST BYTES
void swpb_execute(int dest_num);

// SIGN EXTEND BYTES TO WORD
void sxt_execute(unsigned short dest_value, int dest_num);

// SET PSW bits (1 = SET)
void setcc_execute();

// CLEAR PSW bits (1 = CLEAR)
void clrcc_execute();

// condition execution
void cex_execute();

// get ld or st's index
unsigned short ldst_index();

// perform ld and st's e0 instructions
void ldst_execute(unsigned short srcdst, unsigned short srcdst_value, int RW);

// MOV LOW BYTES INTO DST
void movl_execute(unsigned short bytevalue);

// MOV LOW BYTES INTO DST AND SET HIGH BYTE T0 0X00
void movlz_execute(unsigned short bytevalue);

// MOV LOW BYTES INTO DST AND SET HIGH BYTE T0 0XFF
void movls_execute(unsigned short bytevalue);

// MOV HIGH BYTE INTO DST
void movh_execute(unsigned short bytevalue);

// ldrstr_execute(operand.srccon, srcconarray.word[REGISTER][operand.srccon], READ);
void ldrstr_execute(unsigned short srcdst, unsigned short srcdst_value, int RW);

// dmcontroller function to perform memory modifications
void dmcontroller();

#endif