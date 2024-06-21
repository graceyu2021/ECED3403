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

void psw_arithmetic_update(int temp_result) {
	int carry_update[BIT_ON_OFF][BIT_ON_OFF][BIT_ON_OFF] = { {{0, 0}, {1, 0}}, {{1, 0}, {1, 1}} };
	int overflow_update[BIT_ON_OFF][BIT_ON_OFF][BIT_ON_OFF] = { {{0, 1}, {0, 0}}, {{0, 0}, {1, 0}} };
	int dest_msb, src_msb, result_msb;

	psw.n = CLEAR; // initialize negative and zero flags before they are
	psw.z = CLEAR; // potentially changed

	if (reg_const_operands.wordbyte == WORD_CHECK) { // added word
		src_msb = MASK_MSB_WORD(reg_const_operands.sourceconstant);
		dest_msb = MASK_MSB_WORD(reg_const_operands.destination); // mask bit 15
		result_msb = MASK_MSB_WORD(temp_result);

	}
	else { // added byte
		src_msb = MASK_MSB_BYTE(srcconarray.byte[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant][LOW]); // mask bit 7
		dest_msb = MASK_MSB_BYTE(srcconarray.byte[REGISTER][reg_const_operands.destination][LOW]); // mask bit 7
		result_msb = MASK_MSB_BYTE(temp_result);
	}

	psw.v = overflow_update[src_msb][dest_msb][result_msb];
	psw.c = carry_update[src_msb][dest_msb][result_msb];
	if (temp_result < ZERO)
		psw.n = SET;
	if (temp_result = ZERO)
		psw.z = SET;
}

void psw_logical_update(int temp_result) {
	psw.v = CLEAR;
	psw.n = CLEAR;
	psw.z = temp_result;
	psw.c = CLEAR;
}

void hex_to_bcd() {

}

void add_to_subc_execute() {
	int temp;
	if (opcode == SUB || opcode == SUBC)
		srcconarray.word[REGISTER][reg_const_operands.sourceconstant] *= NEGATIVE;

	if (reg_const_operands.wordbyte == WORD_CHECK) { // adding word
		temp = srcconarray.word[REGISTER][reg_const_operands.destination] +
			srcconarray.word[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant] + psw.c; // result = dest + source + carry

		srcconarray.word[REGISTER][reg_const_operands.destination] = temp;
	}
	else { // adding LSB byte
		temp = srcconarray.byte[REGISTER][reg_const_operands.destination][LOW] +
			srcconarray.byte[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant][LOW] + psw.c; // result = dest + source + carry

		srcconarray.byte[REGISTER][reg_const_operands.destination][LOW] = temp;
	}
	psw_arithmetic_update(temp);
}

void dadd_execute() {

}

void cmp_execute() {
	psw_logical_update(srcconarray.word[REGISTER][reg_const_operands.destination] -
		srcconarray.word[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant]);
}

void xor_execute() {
	psw_logical_update(srcconarray.word[REGISTER][reg_const_operands.destination] ^
		srcconarray.word[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant]);
}

void and_execute() {
	psw_logical_update(srcconarray.word[REGISTER][reg_const_operands.destination] &
		srcconarray.word[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant]);
}

void or_execute() {
	psw_logical_update(srcconarray.word[REGISTER][reg_const_operands.destination] |
		srcconarray.word[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant]);
}

void bit_execute() {
	psw_logical_update(srcconarray.word[REGISTER][reg_const_operands.destination] &
		(1 << srcconarray.word[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant]));
}

void bis_execute() {
	psw_logical_update(srcconarray.word[REGISTER][reg_const_operands.destination] &
		(1 << srcconarray.word[reg_const_operands.sourceconstantcheck][reg_const_operands.sourceconstant]));
}

void execute() {
	signed int temp;

	switch (opcode) {
	case(ADD):
	case(SUB):
		psw.c = CLEAR; // set carry to 0 so following arithmetic is not affected by previous carry
	case(ADDC):
	case(SUBC):
		add_to_subc_execute();
		break;
	case(DADD):
		dadd_execute();
		break;
	case(CMP):
		cmp_execute();
		break;
	case(XOR):
		xor_execute();
		break;
	case(AND):
		and_execute();
		break;
	case(OR):
		or_execute();
		break;
	case(BIT):
		bit_execute();
		break;
	case(BIS):
		bis_execute();
		break;
	case(MOV):

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
