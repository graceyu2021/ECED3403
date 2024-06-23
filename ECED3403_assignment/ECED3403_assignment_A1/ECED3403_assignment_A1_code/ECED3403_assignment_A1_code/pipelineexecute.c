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

void add_to_bis_ops(int dest_num, int srccon_num, int srcconcheck, unsigned int* dest_value, unsigned int* srccon_value, int* wordbyte) {
	*wordbyte = reg_const_operands.wordbyte;
	*dest_value = (&wordbyte == WORD_CHECK) ? // adding word or byte?
		srcconarray.word[REGISTER][dest_num] : srcconarray.byte[REGISTER][dest_num][LOW];
	//printf("word: %04x hibyte %04x lobyte %04x",
		//srcconarray.word[REGISTER][dest_num], srcconarray.byte[REGISTER][dest_num][HIGH], srcconarray.byte[REGISTER][dest_num][LOW]);
	//printf("dest_value %04x", *dest_value);
	//printf("wordbyte %d\n", *wordbyte);

	*srccon_value = (wordbyte == WORD_CHECK) ?
		srcconarray.word[srcconcheck][srccon_num] : srcconarray.byte[srcconcheck][srccon_num][LOW];
}

void mov_to_sxt_ops(int dest_num, int srccon_num, unsigned int* dest_value, unsigned int* srccon_value, int* wordbyte) {
	*wordbyte = reg_const_operands.wordbyte;
	*dest_value = (*wordbyte == WORD_CHECK) ? // adding word or byte?
		srcconarray.word[REGISTER][dest_num] : srcconarray.byte[REGISTER][dest_num][LOW];

	*srccon_value = (*wordbyte == WORD_CHECK) ?
		srcconarray.word[REGISTER][srccon_num] : srcconarray.byte[REGISTER][srccon_num][LOW];
}

int movl_to_movh_ops(unsigned int bytevalue, int* wordbyte) {
	*wordbyte = reg_const_operands.wordbyte;
	return movx_operands.bytevalue;
}

void psw_update(unsigned int v, unsigned int n, unsigned int z, unsigned int c) {
	psw.v = v;
	psw.n = n;
	psw.z = z,
	psw.c = c;
}

void psw_arithmetic_update(unsigned short temp_result, unsigned short dest_value, unsigned short srccon_value, int wordbyte) {
	int carry_update[BIT_ON_OFF][BIT_ON_OFF][BIT_ON_OFF] = { {{0, 0}, {1, 0}}, {{1, 0}, {1, 1}} };
	int overflow_update[BIT_ON_OFF][BIT_ON_OFF][BIT_ON_OFF] = { {{0, 1}, {0, 0}}, {{0, 0}, {1, 0}} };

	int dest_msb = (wordbyte == WORD_CHECK) ?
		MASK_SHIFT_MSB_WORD(dest_value) : MASK_SHIFT_MSB_BYTE(dest_value);

	int src_msb = (wordbyte == WORD_CHECK) ?
		MASK_SHIFT_MSB_WORD(srccon_value) : MASK_SHIFT_MSB_BYTE(srccon_value);
		
	int result_msb = (wordbyte == WORD_CHECK) ?
		MASK_SHIFT_MSB_WORD(temp_result) : MASK_SHIFT_MSB_BYTE(temp_result);

	psw_update(overflow_update[src_msb][dest_msb][result_msb], carry_update[src_msb][dest_msb][result_msb], result_msb, (temp_result == ZERO));
}

void add_to_subc_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int wordbyte) {
	unsigned int temp, 
		temp_srccon_value = srccon_value;
	
	if (opcode == ADD || opcode == SUB)
		psw.c = CLEAR; // set carry to 0 so following arithmetic is not affected by previous carry

	if (opcode == SUB || opcode == SUBC) // is this subtraction?
		temp_srccon_value = ~(temp_srccon_value) + ONE; // set source/constant to negative

	temp = dest_value + (temp_srccon_value + psw.c); // temp = destination + source/constant + carry

	if (wordbyte == WORD_CHECK) // result is word
		srcconarray.word[REGISTER][dest_num] = temp;
	else
		srcconarray.byte[REGISTER][dest_num][LOW] = temp; // result is byte

	psw_arithmetic_update(temp, dest_value, srccon_value, wordbyte);
}

unsigned short bcd(unsigned short dest_nib, unsigned short src_nib) {
	int carry_update[BIT_ON_OFF][BIT_ON_OFF][BIT_ON_OFF] = { {{0, 0}, {1, 0}}, {{1, 0}, {1, 1}} };
	int overflow_update[BIT_ON_OFF][BIT_ON_OFF][BIT_ON_OFF] = { {{0, 1}, {0, 0}}, {{0, 0}, {1, 0}} };

	unsigned short res_nib = dest_nib + src_nib + psw.c;
	if (res_nib > NUMERICAL_NIB) // if res_nib is A to F
		res_nib -= NIBtoNUMERICAL; // res_nib -= 10 to make res_nib numerical

	// update overflow and carry based on these bits
	psw.c = carry_update[src_nib][dest_nib][res_nib]; 
	psw.v = overflow_update[src_nib][dest_nib][res_nib];

	return res_nib;
}

void dadd_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int wordbyte) {
	psw.c = CLEAR; // clear carry for first nib
	nibble_word res = { .word = CLEAR }, dest = { .word = dest_value }, src = { .word = srccon_value }; // initialize three unions for nibbles

	// decimal add the nibbles
	res.nibble.n0 = bcd(dest.nibble.n0, src.nibble.n0);
	res.nibble.n1 = bcd(dest.nibble.n1, src.nibble.n1);
	if (wordbyte == WORD_CHECK) { // if WORD, need to bcd the high byte
		res.nibble.n2 = bcd(dest.nibble.n2, src.nibble.n2);
		res.nibble.n3 = bcd(dest.nibble.n3, src.nibble.n3);
	}

	if (wordbyte == WORD_CHECK) // result is word
		srcconarray.word[REGISTER][dest_num] = res.word;
	else // result is byte
		srcconarray.byte[REGISTER][dest_num][LOW] = res.byte[LOW]; 

	// set temporary variables to destination msb and destination value
	int res_msb = (wordbyte == WORD_CHECK) ? res.nibble.n3: res.nibble.n1;

	psw_update(psw.v, psw.c, res_msb, (res.word == ZERO)); // pdate psw
}

// COMPARE: dst - src/con
void cmp_execute(unsigned short dest_value, unsigned short srccon_value) {
	int temp = dest_value - srccon_value;
	psw_update(CLEAR, (temp < ZERO), (temp == ZERO), CLEAR);
}

// EXCLUSIVE: dst <- dst ^ src/con
void xor_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num) {
	dest_value ^= srccon_value;
	psw_update(CLEAR, (dest_value < ZERO), (dest_value == ZERO), CLEAR);
	srcconarray.word[REGISTER][dest_num] = dest_value;
}

// AND: dst <- dst & src/con
void and_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num) {
	dest_value &= srccon_value;
	psw_update(CLEAR, (dest_value < ZERO), (dest_value == ZERO), CLEAR);
	srcconarray.word[REGISTER][dest_num] = dest_value;
}

// OR: dst <- dst | src/con
void or_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num) {
	dest_value |= srccon_value;
	psw_update(CLEAR, (dest_value < ZERO), (dest_value == ZERO), CLEAR);
	srcconarray.word[REGISTER][dest_num] = dest_value;
}

// BIT TEST: dst <- dst & (1 << src/co)
void bit_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num) {
	dest_value &= BIT_SHIFT(srccon_value);
	psw_update(CLEAR, CLEAR, (dest_value == ZERO), CLEAR);
	srcconarray.word[REGISTER][dest_num] = dest_value;
}

// BIT CLEAR: dst << dst & ~(1 << src/con) 
void bic_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num) {
	dest_value &= ~BIT_SHIFT(srccon_value);
	psw_update(CLEAR, (dest_value < ZERO), (dest_value == ZERO), CLEAR);
	srcconarray.word[REGISTER][dest_num] = dest_value;
}

// BIT SET: dst <- dst | (1 << src/con)
void bis_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num) {
	dest_value |= BIT_SHIFT(srccon_value);
	psw_update(CLEAR, (dest_value < ZERO), (dest_value == ZERO), CLEAR);
	srcconarray.word[REGISTER][dest_num] = dest_value;
}

// MOVE: dst <- src
void mov_execute(unsigned short srccon_value, int dest_num) {
	srcconarray.word[REGISTER][dest_num] = srccon_value;
}

// SWAP src and dest
void swap_execute(unsigned short dest_value, unsigned short srccon_value) {
	unsigned int temp = dest_value;
	dest_value = srccon_value;
	srccon_value = temp;
}

// SHIFT DST: dst >> 1
void sra_execute(unsigned short dest_value, int dest_num, int wordbyte) {
	int dest_msb = (wordbyte == WORD_CHECK) ?
		MASK_MSB_WORD(dest_value) : MASK_MSB_BYTE(dest_value);
	int dest_lsb = MASK_LSB(dest_value);

	dest_value = (dest_value >> SHIFT_RIGHT) | dest_msb; // shift dest right through carry with sign extension
	psw_update(CLEAR, CLEAR, (dest_value == ZERO), dest_lsb);
	srcconarray.word[REGISTER][dest_num] = dest_value;
}

// ROTATE DST WITH CARRY: dst >> 1, dst(msb) == 1 
void rrc_execute(unsigned short dest_value, int dest_num, int wordbyte) {
	int carry = (wordbyte == WORD_CHECK) ?
		CARRY_TO_MSB_WORD(dest_value) : CARRY_TO_MSB_BYTE(dest_value);
	int dest_lsb = MASK_LSB(dest_value);

	dest_value = (dest_value >> SHIFT_RIGHT) | carry; // shift dest right through carry
	psw_update(CLEAR, CLEAR, (dest_value == ZERO), dest_lsb);
	srcconarray.word[REGISTER][dest_num] = dest_value;
}

// SWAP DST BYTES
void swpb_execute(int dest_num) {
	unsigned int temp = srcconarray.byte[REGISTER][dest_num][HIGH];
	srcconarray.byte[REGISTER][dest_num][HIGH] = srcconarray.byte[REGISTER][dest_num][LOW];
	srcconarray.byte[REGISTER][dest_num][LOW] = temp;
}

 void sxt_execute(unsigned short dest_value, int dest_num) {
	 int dest_msb = MASK_SHIFT_MSB_BYTE(dest_value);
	 if (dest_msb == SET) // dest_msb = 1, negative
		 srcconarray.byte[REGISTER][dest_num][HIGH] = MSBYTE_SET; // sign extend low byte to be negative, 0xFF
	 else // dest_msb = 0, positive
		 srcconarray.byte[REGISTER][dest_num][HIGH] = MSBYTE_CLEAR; // sign extend high byte to be positive, 0x00
}

// MOV LOW BYTES INTO DST
void movl_execute(unsigned short bytevalue) {
	srcconarray.byte[REGISTER][movx_operands.destination][LOW] = bytevalue;
}

void movlz_execute(unsigned short bytevalue) {
	srcconarray.word[REGISTER][movx_operands.destination] &= UNSET_HIGH_BYTE;
	srcconarray.byte[REGISTER][movx_operands.destination][LOW] = bytevalue;
}

void movls_execute(unsigned short bytevalue) {
	srcconarray.word[REGISTER][movx_operands.destination] |= SET_HIGH_BYTE;
	srcconarray.byte[REGISTER][movx_operands.destination][LOW] = bytevalue;
}

void movh_execute(unsigned short bytevalue) {
	srcconarray.byte[REGISTER][movx_operands.destination][HIGH] = bytevalue;
}

void execute() {
	// temporary return just until full instruciton set is implemented
	if (((opcode >= ADD && opcode <= SXT) || (opcode >= MOVL && opcode <= MOVH)) != TRUE) { // not an instruction to implement in a2
		return;
	}
	
	unsigned int dest_value = 0, srccon_value;
	int wordbyte, bytevalue;

	if (opcode >= ADD && opcode <= BIS)
		add_to_bis_ops(reg_const_operands.destination, reg_const_operands.sourceconstant, reg_const_operands.sourceconstantcheck, &dest_value, &srccon_value, &wordbyte);

	else if (opcode >= MOV && opcode <= SXT)
		mov_to_sxt_ops(reg_const_operands.destination, reg_const_operands.sourceconstant, &dest_value, &srccon_value, &wordbyte);
	
	else if (opcode >= MOVL && opcode <= MOVH)
		bytevalue = movl_to_movh_ops(movx_operands.bytevalue, &wordbyte);

	switch (opcode) {
	case(ADD):
	case(SUB):
	case(ADDC):
	case(SUBC):
		add_to_subc_execute(dest_value, srccon_value, reg_const_operands.destination, wordbyte);
		break;
	case(DADD):
		dadd_execute(dest_value, srccon_value, reg_const_operands.destination, wordbyte);
		break;
	case(CMP):
		cmp_execute(dest_value, srccon_value);
		break;
	case(XOR):
		xor_execute(dest_value, srccon_value, reg_const_operands.destination);
		break;
	case(AND):
		and_execute(dest_value, srccon_value, reg_const_operands.destination);
		break;
	case(OR):
		or_execute(dest_value, srccon_value, reg_const_operands.destination);
		break;
	case(BIT):
		bit_execute(dest_value, srccon_value, reg_const_operands.destination);
		break;
	case(BIC):
		bic_execute(dest_value, srccon_value, reg_const_operands.destination);
		break;
	case(BIS):
		bis_execute(dest_value, srccon_value, reg_const_operands.destination);
		break;
	case(MOV):
		mov_execute(srccon_value, reg_const_operands.destination, wordbyte);
		break;
	case(SWAP):
		swap_execute(dest_value, srccon_value);
		break;
	case(SRA):
		sra_execute(dest_value, reg_const_operands.destination, wordbyte);
		break;
	case(RRC):
		rrc_execute(dest_value, reg_const_operands.destination, wordbyte);
		break;
	case(SWPB):
		swpb_execute(reg_const_operands.destination);
		break;
	case(SXT):
		sxt_execute(dest_value, reg_const_operands.destination);
		break;
	case(MOVL):
		movl_execute(bytevalue);
		break;
	case(MOVLZ):
		movlz_execute(bytevalue);
		break;
	case(MOVLS):
		movls_execute(bytevalue);
		break;
	case(MOVH):
		movh_execute(bytevalue);
		break;
	default:
		break;
	}
}
