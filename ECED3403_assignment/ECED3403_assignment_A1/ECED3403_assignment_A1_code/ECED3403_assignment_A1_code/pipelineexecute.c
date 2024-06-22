/*
file: decodefunctions.c
programmer: Grace Yu
b00: B00902046
class: ECED3403
professor: Dr. Larry Hughes
assignment: A1
submission date: May 23rd, 2024

This is the pipeline execute function file of my program.
*/

#include "MAINHEADER.H"

add_to_bis_ops(int dest_num, int srccon_num, int srcconcheck, int* dest_value, int* srccon_value, int* wordbyte) {
	*wordbyte = reg_const_operands.wordbyte;
	*dest_value = (wordbyte == WORD_CHECK) ? // adding word or byte?
		srcconarray.word[REGISTER][dest_num] : srcconarray.byte[REGISTER][dest_num][LOW];

	*srccon_value = (wordbyte == WORD_CHECK) ?
		srcconarray.word[srcconcheck][srccon_num] : srcconarray.byte[srcconcheck][srccon_num][LOW];
}

void psw_arithmetic_update(int temp_result, int dest_value, int srccon_value, int wordbyte) {
	int carry_update[BIT_ON_OFF][BIT_ON_OFF][BIT_ON_OFF] = { {{0, 0}, {1, 0}}, {{1, 0}, {1, 1}} };
	int overflow_update[BIT_ON_OFF][BIT_ON_OFF][BIT_ON_OFF] = { {{0, 1}, {0, 0}}, {{0, 0}, {1, 0}} };

	int dest_msb = (wordbyte == WORD_CHECK) ?
		MASK_MSB_WORD(dest_value) : MASK_MSB_BYTE(dest_value);

	int src_msb = (wordbyte == WORD_CHECK) ?
		MASK_MSB_WORD(srccon_value) : MASK_MSB_BYTE(srccon_value);
		
	int result_msb = (wordbyte == WORD_CHECK) ?
		MASK_MSB_WORD(temp_result) : MASK_MSB_BYTE(temp_result);

	psw.v = overflow_update[src_msb][dest_msb][result_msb];
	psw.c = carry_update[src_msb][dest_msb][result_msb];
	psw.n = (temp_result < ZERO) ? SET : CLEAR;
	psw.z = (temp_result == ZERO) ? SET : CLEAR;
}

void psw_logical_update(int temp_result) {
	psw.v = CLEAR;
	psw.n = CLEAR;
	psw.z = (temp_result == ZERO) ? SET : CLEAR;
	psw.c = CLEAR;
}

void hex_to_bcd() {

}

void add_to_subc_execute(int dest_value, int srccon_value, int wordbyte) {
	int temp;
	int dest_num = reg_const_operands.destination;
	int srccon_num = reg_const_operands.sourceconstant;

	if (opcode == SUB || opcode == SUBC) // is this subtraction?
		srccon_value *= NEGATIVE; // set source/constant to negative

	temp = dest_value + srccon_value + psw.c; // temp = destination + source/constant + carry

	if (wordbyte == WORD_CHECK) // result is word
		srcconarray.word[REGISTER][dest_num] = temp;
	else
		srcconarray.byte[REGISTER][dest_num][LOW] = temp; // result is byte

	psw_arithmetic_update(temp, dest_value, srccon_value, wordbyte);
}

void dadd_execute() {

}

void cmp_execute(int dest_num, int srccon_num, int srcconcheck, int wordbyte) {
	int temp;
	int dest = (wordbyte == WORD_CHECK) ? // adding word or byte?
		srcconarray.word[REGISTER][dest_num] : srcconarray.byte[REGISTER][dest_num][LOW];

	int srccon = (wordbyte == WORD_CHECK) ?
		srcconarray.word[srcconcheck][srccon_num] : srcconarray.byte[srcconcheck][srccon_num][LOW];

	if (reg_const_operands.wordbyte == WORD_CHECK) // cmp word
		psw_logical_update(srcconarray.word[REGISTER][reg_const_operands.destination] -
			srcconarray.word[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant]);
	else // cmp byte
		psw_logical_update(srcconarray.byte[REGISTER][reg_const_operands.destination][LOW] -
			srcconarray.byte[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant][LOW]);
}

void xor_execute(int dest_num, int srccon_num, int srcconcheck, int wordbyte) {
	int temp;
	int dest = (wordbyte == WORD_CHECK) ? // adding word or byte?
		srcconarray.word[REGISTER][dest_num] : srcconarray.byte[REGISTER][dest_num][LOW];

	int srccon = (wordbyte == WORD_CHECK) ?
		srcconarray.word[srcconcheck][srccon_num] : srcconarray.byte[srcconcheck][srccon_num][LOW];

	if (reg_const_operands.wordbyte == WORD_CHECK) { // xor word
		srcconarray.word[REGISTER][reg_const_operands.destination] ^=
			srcconarray.word[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant];
		psw_logical_update(srcconarray.word[REGISTER][reg_const_operands.destination]);
	}
	else { // xor byte
		srcconarray.byte[REGISTER][reg_const_operands.destination][LOW] ^=
			srcconarray.byte[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant][LOW];
		psw_logical_update(srcconarray.byte[REGISTER][reg_const_operands.destination][LOW]);
	}
}

void and_execute(int dest_num, int srccon_num, int srcconcheck, int wordbyte) {
	int temp;
	int dest = (wordbyte == WORD_CHECK) ? // adding word or byte?
		srcconarray.word[REGISTER][dest_num] : srcconarray.byte[REGISTER][dest_num][LOW];

	int srccon = (wordbyte == WORD_CHECK) ?
		srcconarray.word[srcconcheck][srccon_num] : srcconarray.byte[srcconcheck][srccon_num][LOW];

	if (reg_const_operands.wordbyte == WORD_CHECK) { // and word
		srcconarray.word[REGISTER][reg_const_operands.destination] &=
			srcconarray.word[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant];
		psw_logical_update(srcconarray.word[REGISTER][reg_const_operands.destination]);
	}
	else { // and byte
		srcconarray.byte[REGISTER][reg_const_operands.destination][LOW] &=
			srcconarray.byte[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant][LOW];
		psw_logical_update(srcconarray.byte[REGISTER][reg_const_operands.destination][LOW]);
	}
}

void or_execute(int dest_num, int srccon_num, int srcconcheck, int wordbyte) {
	int temp;
	int dest = (wordbyte == WORD_CHECK) ? // adding word or byte?
		srcconarray.word[REGISTER][dest_num] : srcconarray.byte[REGISTER][dest_num][LOW];

	int srccon = (wordbyte == WORD_CHECK) ?
		srcconarray.word[srcconcheck][srccon_num] : srcconarray.byte[srcconcheck][srccon_num][LOW];

	if (reg_const_operands.wordbyte == WORD_CHECK) { // cmp word
		srcconarray.word[REGISTER][reg_const_operands.destination] |=
			srcconarray.word[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant];
		psw_logical_update(srcconarray.word[REGISTER][reg_const_operands.destination]);
	}
}

void bit_execute(int dest_num, int srccon_num, int srcconcheck, int wordbyte) {
	int temp;
	int dest = (wordbyte == WORD_CHECK) ? // adding word or byte?
		srcconarray.word[REGISTER][dest_num] : srcconarray.byte[REGISTER][dest_num][LOW];

	int srccon = (wordbyte == WORD_CHECK) ?
		srcconarray.word[srcconcheck][srccon_num] : srcconarray.byte[srcconcheck][srccon_num][LOW];

	if (reg_const_operands.wordbyte == WORD_CHECK) { // cmp word
		srcconarray.word[REGISTER][reg_const_operands.destination] &=
			(1 << srcconarray.word[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant]);
		psw_logical_update(srcconarray.word[REGISTER][reg_const_operands.destination]);
	}
}

void bis_execute(int dest_num, int srccon_num, int srcconcheck, int wordbyte) {
	int temp;
	int dest = (wordbyte == WORD_CHECK) ? // adding word or byte?
		srcconarray.word[REGISTER][dest_num] : srcconarray.byte[REGISTER][dest_num][LOW];

	int srccon = (wordbyte == WORD_CHECK) ?
		srcconarray.word[srcconcheck][srccon_num] : srcconarray.byte[srcconcheck][srccon_num][LOW];

	if (reg_const_operands.wordbyte == WORD_CHECK) { // cmp word
		srcconarray.word[REGISTER][reg_const_operands.destination] &=
			~(1 << srcconarray.word[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant]);
		psw_logical_update(srcconarray.word[REGISTER][reg_const_operands.destination]);
	}
}

void mov_execute() {
	srcconarray.word[REGISTER][reg_const_operands.destination] = srcconarray.word[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant];
}

void execute() {
	int dest_value, srccon_value, wordbyte;
	if (opcode >= ADD && opcode <= BIS) {
		add_to_bis_ops(reg_const_operands.destination, reg_const_operands.sourceconstant, reg_const_operands.sourceconstantcheck, &dest_value, &srccon_value, &wordbyte);
	}
	int temp;

	switch (opcode) {
	case(ADD):
	case(SUB):
		psw.c = CLEAR; // set carry to 0 so following arithmetic is not affected by previous carry
	case(ADDC):
	case(SUBC):
		add_to_subc_execute(dest_value, srccon_value, wordbyte);
		break;
	case(DADD):
		//dadd_execute(dest_num, srccon_num, srcconcheck, wordbyte);
		break;
	case(CMP):
		//cmp_execute(dest_num, srccon_num, srcconcheck, wordbyte);
		break;
	case(XOR):
		//xor_execute(dest_num, srccon_num, srcconcheck, wordbyte);
		break;
	case(AND):
		//and_execute(dest_num, srccon_num, srcconcheck, wordbyte);
		break;
	case(OR):
		//or_execute(dest_num, srccon_num, srcconcheck, wordbyte);
		break;
	case(BIT):
		//bit_execute(dest_num, srccon_num, srcconcheck, wordbyte);
		break;
	case(BIS):
		//bis_execute(dest_num, srccon_num, srcconcheck, wordbyte);
		break;
	case(MOV):
		//mov_execute(dest_num, srccon_num, srcconcheck, wordbyte);
		break;
	case(SWAP):

		break;
	case(SRA):

		break;
	case(RRC):

		break;
	case(SWPB):

		break;
	case(SXT):

		break;
	case(MOVL):
		srcconarray.byte[REGISTER][movx_operands.destination][LOW] = movx_operands.bytevalue;
		break;
	case(MOVLZ):
		srcconarray.word[REGISTER][movx_operands.destination] &= UNSET_HIGH_BYTE;
		srcconarray.byte[REGISTER][movx_operands.destination][LOW] = movx_operands.bytevalue;
		break;
	case(MOVLS):
		srcconarray.word[REGISTER][movx_operands.destination] |= SET_HIGH_BYTE;
		srcconarray.byte[REGISTER][movx_operands.destination][LOW] = movx_operands.bytevalue;
		break;
	case(MOVH):
		srcconarray.byte[REGISTER][movx_operands.destination][HIGH] = movx_operands.bytevalue;
		break;
	default:
		break;
	}



}
