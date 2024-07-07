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

// shorten variable names from add to bid
void add_to_bis_ops(int dest_num, int srccon_num, int srcconcheck, unsigned int* dest_value, unsigned int* srccon_value, int* wordbyte) {
	*wordbyte = operand.wordbyte;

	// adding word or byte?
	*dest_value = (*wordbyte == WORD_CHECK) ? srcconarray.word[REGISTER][dest_num] : srcconarray.byte[REGISTER][dest_num][LOW];
	*srccon_value = (*wordbyte == WORD_CHECK) ? srcconarray.word[srcconcheck][srccon_num] : srcconarray.byte[srcconcheck][srccon_num][LOW];
}

// shorten variable names from mov to sxt
void mov_to_sxt_ops(int dest_num, int srccon_num, unsigned int* dest_value, unsigned int* srccon_value, int* wordbyte) {
	*wordbyte = operand.wordbyte;

	// adding word or byte?
	*dest_value = (*wordbyte == WORD_CHECK) ? srcconarray.word[REGISTER][dest_num] : srcconarray.byte[REGISTER][dest_num][LOW];
	*srccon_value = (*wordbyte == WORD_CHECK) ? srcconarray.word[REGISTER][srccon_num] : srcconarray.byte[REGISTER][srccon_num][LOW];
}

// shorten variable names from movl to movh
void movl_to_movh_ops(int* bytevalue) {
	*bytevalue = operand.bytevalue;
}

// determine which bit to mask
unsigned short mask_shift_msb(unsigned short value, int wordbyte) {
	return (wordbyte == WORD_CHECK) ? // is it a word?
		MASK_SHIFT_MSB_WORD(value) : MASK_SHIFT_MSB_BYTE(value); // mask either bit or word
}

// determine whether new dest is a word or byte
void set_srcconarray(unsigned short dest_value, int dest_num, int wordbyte) {
	if (wordbyte == WORD_CHECK) // result is word
		srcconarray.word[REGISTER][dest_num] = dest_value;
	else						// result is byte
		srcconarray.byte[REGISTER][dest_num][LOW] = dest_value;
}

// update psw
void psw_update(unsigned short v, unsigned short n, unsigned short z, unsigned short c) {
	psw.v = v;
	psw.n = n;
	psw.z = z,
	psw.c = c;
}

// precursor to psw_update for arithmetic operations
void psw_arithmetic_update(unsigned short temp_result, unsigned short dest_value, unsigned short srccon_value, int wordbyte) {
	short carry_update[BIT_ON_OFF][BIT_ON_OFF][BIT_ON_OFF] = { {{0, 0}, {1, 0}}, {{1, 0}, {1, 1}} };
	short overflow_update[BIT_ON_OFF][BIT_ON_OFF][BIT_ON_OFF] = { {{0, 1}, {0, 0}}, {{0, 0}, {1, 0}} };

	unsigned short dest_msb = mask_shift_msb(dest_value, wordbyte); // mask msbs
	unsigned short src_msb = mask_shift_msb(srccon_value, wordbyte); ;
	unsigned short result_msb = mask_shift_msb(temp_result, wordbyte);

	if (wordbyte != WORD_CHECK) // if result is a byte, need to remove extra 1 in bit 8
		temp_result = MASK_BYTE(temp_result);

	if (opcode == SUB || opcode == SUBC) // if a subtraction was performed, ~the msb of source 
		src_msb = src_msb^ ONE; // invert src_msb

	psw_update(overflow_update[src_msb][dest_msb][result_msb], result_msb, (temp_result == ZERO), carry_update[src_msb][dest_msb][result_msb]);
}

void add_to_subc_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int wordbyte) {
	unsigned short temp, temp_srccon_value = srccon_value;
	
	if (opcode == ADD || opcode == SUB)
		psw.c = CLEAR; // set carry to 0 so following arithmetic is not affected by previous carry

	if (opcode == SUB) // is this sub?
		temp_srccon_value = ~(temp_srccon_value) + ONE; // set source/constant to negative
	else if (opcode == SUBC) // is this subc?
		temp_srccon_value = ~(temp_srccon_value); // in subc instruction, no + ONE

	temp = dest_value + (temp_srccon_value + psw.c); // temp = destination + source/constant + carry

	set_srcconarray(temp, dest_num, wordbyte);
	psw_arithmetic_update(temp, dest_value, srccon_value, wordbyte);
}

// binary coded decimal addition
unsigned short bcd(unsigned short dest_nib, unsigned short src_nib) {
	unsigned short res_nib = dest_nib + src_nib + psw.c;
	psw.c = (res_nib > NUMERICAL_NIB) ? SET : CLEAR; // set psw if greater than 10, clear otherwise

	if (res_nib > NUMERICAL_NIB)   // if res_nib is A to F
		res_nib -= NIBtoNUMERICAL; // res_nib -= 10 to make res_nib numerical

	return res_nib;
}

// DECIMAL ADD
void dadd_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int wordbyte) {
	psw.c = CLEAR; // clear carry for first nib
	nibble_word res = {.word = CLEAR };

	// decimal add the nibbles
	res.word = NIB_0_SET(res.word, (bcd(NIB_0(dest_value), NIB_0(srccon_value))));
	res.word = NIB_1_SET(res.word, (bcd(NIB_1(dest_value), NIB_1(srccon_value))));
	if (wordbyte == WORD_CHECK) { // if WORD, need to bcd the high byte
		res.word = NIB_2_SET(res.word, (bcd(NIB_2(dest_value), NIB_2(srccon_value))));
		res.word = NIB_3_SET(res.word, (bcd(NIB_3(dest_value), NIB_3(srccon_value))));
	}

	if (wordbyte == WORD_CHECK)  // result is word
		srcconarray.word[REGISTER][dest_num] = res.word;
	else // result is byte
		srcconarray.byte[REGISTER][dest_num][LOW] = res.byte[LOW]; 

	// set temporary variable to resultt to use in psw update function
	unsigned char res_temp = (wordbyte == WORD_CHECK) ? res.word : res.byte[LOW];

	psw_update(CLEAR, CLEAR, (res_temp == ZERO), psw.c); // psw.c was updated in bcd function
}

// COMPARE: dst - src/con
void cmp_execute(unsigned short dest_value, unsigned short srccon_value) {
	int temp = dest_value - srccon_value;
	psw_update(psw.v, (temp < ZERO), (temp == ZERO), (temp >= ZERO));
}

// EXCLUSIVE: dst <- dst ^ src/con
void xor_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int wordbyte) {
	dest_value ^= srccon_value;
	unsigned short dest_msb = mask_shift_msb(dest_value, wordbyte); // mask msb
	
	set_srcconarray(dest_value, dest_num, wordbyte); // set srcconarray with new destination value
	psw_update(CLEAR, dest_msb, (dest_value == ZERO), psw.c);
}

// AND: dst <- dst & src/con
void and_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int wordbyte) {
	dest_value &= srccon_value;
	unsigned short dest_msb = mask_shift_msb(dest_value, wordbyte); // mask msb

	set_srcconarray(dest_value, dest_num, wordbyte);
	psw_update(psw.v, dest_msb, (dest_value == ZERO), psw.c);
}

// OR: dst <- dst | src/con
void or_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int wordbyte) {
	dest_value |= srccon_value;
	unsigned short dest_msb = mask_shift_msb(dest_value, wordbyte); // mask msb

	set_srcconarray(dest_value, dest_num, wordbyte);
	psw_update(psw.v, dest_msb, (dest_value == ZERO), psw.c);
}

// BIT TEST: dst <- dst & (1 << src/co)
void bit_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num) {
	dest_value &= BIT_SHIFT(srccon_value);

	srcconarray.word[REGISTER][dest_num] = dest_value;
	psw_update(psw.v, psw.n, (dest_value == ZERO), psw.c);
}

// BIT CLEAR: dst << dst & ~(1 << src/con) 
void bic_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int wordbyte) {
	dest_value &= ~BIT_SHIFT(srccon_value);
	unsigned short dest_msb = mask_shift_msb(dest_value, wordbyte); // mask msb

	set_srcconarray(dest_value, dest_num, wordbyte);
	psw_update(psw.v, dest_msb, (dest_value == ZERO), psw.c);
}

// BIT SET: dst <- dst | (1 << src/con)
void bis_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int wordbyte) {
	dest_value |= BIT_SHIFT(srccon_value);
	unsigned short dest_msb = mask_shift_msb(dest_value, wordbyte); // mask msb

	set_srcconarray(dest_value, dest_num, wordbyte);
	psw_update(psw.v, dest_msb, (dest_value == ZERO), psw.c);
}

// MOVE: dst <- src
void mov_execute(unsigned short srccon_value, int dest_num) {
	srcconarray.word[REGISTER][dest_num] = srccon_value;
}

// SWAP src and dest
void swap_execute(unsigned short dest_value, unsigned short srccon_value, int dest_num, int srccon_num) {
	srcconarray.word[REGISTER][dest_num] = srccon_value;
	srcconarray.word[REGISTER][srccon_num] = dest_value;
}

// SHIFT DST: dst >> 1 while maintainig sign
void sra_execute(unsigned short dest_value, int dest_num, int wordbyte) {
	unsigned short dest_msb, dest_lsb = MASK_LSB(dest_value); // mask lsb of dest
	dest_msb = (wordbyte == WORD_CHECK) ? MASK_MSB_WORD(dest_value) : MASK_MSB_BYTE(dest_value); // mask msb of dest

	dest_value = (dest_value >> SHIFT_RIGHT) | dest_msb; // roll dest right while extending sign
	dest_msb = mask_shift_msb(dest_msb, wordbyte); // shift msb to bit 0 to use in psw_update
	
	set_srcconarray(dest_value, dest_num, wordbyte);
	psw_update(CLEAR, dest_msb, (dest_value == ZERO), dest_lsb);
}

// ROTATE DST WITH CARRY: dst >> 1, dst(msb) == 1 
void rrc_execute(unsigned short dest_value, int dest_num, int wordbyte) {
	unsigned short dest_lsb = MASK_LSB(dest_value); // save lsb to determine carry flag in psw

	if (wordbyte == WORD_CHECK) // if word
		dest_value = (dest_value >> SHIFT_RIGHT) | CARRY_TO_MSB_WORD(psw.c); // shift dest right through carry
	else						// if byte
		dest_value = (dest_value >> SHIFT_RIGHT) | CARRY_TO_MSB_BYTE(psw.c); // shift dest right through carry

	unsigned short dest_msb = mask_shift_msb(dest_value, wordbyte); // save msb to determine negative flag in psw
	
	set_srcconarray(dest_value, dest_num, wordbyte);
	psw_update(CLEAR, dest_msb, (dest_value == ZERO), dest_lsb);
}

// SWAP DST BYTES
void swpb_execute(int dest_num) {
	unsigned int temp = srcconarray.byte[REGISTER][dest_num][HIGH];							// set temp to high byte
	srcconarray.byte[REGISTER][dest_num][HIGH] = srcconarray.byte[REGISTER][dest_num][LOW]; // set high byte to low og value
	srcconarray.byte[REGISTER][dest_num][LOW] = temp;										// set low byte to high og value
	temp = srcconarray.word[REGISTER][dest_num]; // temp variable reset to readability in psw_update function call

	psw_update(psw.v, MASK_SHIFT_MSB_WORD(temp), (temp == ZERO), psw.c);
}

// SIGN EXTEND BYTES TO WORD
 void sxt_execute(unsigned short dest_value, int dest_num) {
	 int dest_msb = MASK_SHIFT_MSB_BYTE(dest_value);
	 if (dest_msb == SET)	// dest_msb = 1, negative
		 srcconarray.byte[REGISTER][dest_num][HIGH] = MSBYTE_SET; // sign extend low byte to be negative, 0xFF
	 else					// dest_msb = 0, positive
		 srcconarray.byte[REGISTER][dest_num][HIGH] = MSBYTE_CLEAR; // sign extend high byte to be positive, 0x00
	 unsigned char temp = srcconarray.word[REGISTER][dest_num];

	 psw_update(psw.v, dest_msb, (temp == ZERO), psw.c);
 }

 void setcc_execute() {
	 psw.v |= operand.v;
	 psw.slp |= operand.slp;
	 psw.n |= operand.n;
	 psw.z |= operand.z;
	 psw.c |= operand.c;
 }

 void clrcc_execute() {
	 psw.v &= ~operand.v;
	 psw.slp &= ~operand.slp;
	 psw.n &= ~operand.n;
	 psw.z &= ~operand.z;
	 psw.c &= ~operand.c;
 }

 unsigned short ldst_index() {
	 printf("inc %x dec %x prepo %x\n", operand.inc, operand.dec, operand.prpo);
	 if (operand.inc == SET)
		 return (operand.wordbyte == WORD_CHECK) ? WORDINDEX : BYTEINDEX;
	 else if (operand.dec == SET)
		 return (operand.wordbyte == WORD_CHECK) ? SUB_WORDINDEX : SUB_BYTEINDEX;
	 return ZERO;
 }

 unsigned short ldst_buffer_prep(unsigned short ldst_address, int RW) {
	 unsigned short eff_address, 
		 index = ldst_index();

	 if (operand.prpo == PRE)
		 ldst_address += index;

	 eff_address = ldst_address;
	 dmar = eff_address;
	 dctrl = RW;

	 if (operand.prpo == POST)
		 ldst_address += index;

	 return ldst_address;
 }

 void ld_execute() {
	 unsigned short new_src, ld_src = srcconarray.word[REGISTER][operand.srccon];
		 //index = ldst_index();

	 printf("before and after index dst %04x:", ld_src);

	 //if (operand.prpo == PRE)
		// srcconarray.word[REGISTER][operand.srccon] += index;

	 new_src = ldst_buffer_prep(ld_src, READ);
	 srcconarray.word[REGISTER][operand.srccon] = new_src;

	 //if (operand.prpo == POST)
		// srcconarray.word[REGISTER][operand.srccon] += index;

	 printf("%04x\n", srcconarray.word[REGISTER][operand.srccon]);
 }

 void st_execute() {
	 unsigned short new_dst, st_dst = srcconarray.word[REGISTER][operand.dst];
		 //index = ldst_index();
	 //printf("index %d ", index);
	 //printf("before and after index dst %04x:", srcconarray.word[REGISTER][operand.dst]);

	// if (operand.prpo == PRE)
		// srcconarray.word[REGISTER][operand.dst] += index;
	 
	 new_dst = ldst_buffer_prep(st_dst, WRITE);
	 srcconarray.word[REGISTER][operand.dst] = new_dst;
	 dmbr = srcconarray.word[REGISTER][operand.srccon];
	 printf("st e0: dmbr %04x dctrl %d\n", dmbr, dctrl);

	 //if (operand.prpo == POST)
		// srcconarray.word[REGISTER][operand.dst] += index;

	 printf("%04x\n", srcconarray.word[REGISTER][operand.dst]);
 }

// MOV LOW BYTES INTO DST
void movl_execute(unsigned short bytevalue) {
	srcconarray.byte[REGISTER][operand.dst][LOW] = bytevalue;
}

// MOV LOW BYTES INTO DST AND SET HIGH BYTE T0 0X00
void movlz_execute(unsigned short bytevalue) {
	srcconarray.byte[REGISTER][operand.dst][HIGH] = UNSET_HIGH_BYTE; // clear high byte
	srcconarray.byte[REGISTER][operand.dst][LOW] = bytevalue;
}

// MOV LOW BYTES INTO DST AND SET HIGH BYTE T0 0XFF
void movls_execute(unsigned short bytevalue) {
	srcconarray.byte[REGISTER][operand.dst][HIGH] = SET_HIGH_BYTE; // set high byte
	srcconarray.byte[REGISTER][operand.dst][LOW] = bytevalue;
}

// MOV HIGH BYTE INTO DST
void movh_execute(unsigned short bytevalue) {
	srcconarray.byte[REGISTER][operand.dst][HIGH] = bytevalue;
}

void ldrstr_buffer_prep(unsigned short address_reg, int RW) {
	unsigned short eff_address;

	eff_address = address_reg;
	dmar = eff_address;
	dctrl = RW;
}

// LOAD MEMORY INTO DST
void ldr_execute() {
	operand.srccon += operand.srccon + operand.off;
	if (operand.wordbyte == WORD_CHECK)
		operand.srccon = WORD_OFF(operand.srccon);
	
	ldrstr_buffer_prep(srcconarray.word[REGISTER][operand.srccon], READ);
}

// STORE SRC INTO MEMORY
void str_execute() {
	operand.srccon += operand.srccon + operand.off;
	if (operand.wordbyte == WORD_CHECK)
		operand.srccon = WORD_OFF(operand.srccon);

	ldrstr_buffer_prep(operand.srccon, WRITE);
	dmbr = srcconarray.word[REGISTER][operand.srccon];
}

// execute function
void execute0() {
	// temporary return just until full instruciton set is implemented
	if (((opcode >= ADD && opcode <= SXT) || (opcode >= SETCC && opcode <= STR)) != TRUE) // not an instruction to implement in a2
		return;

	unsigned int dest_value = 0, srccon_value;
	int wordbyte, bytevalue;

	// initilalize variables to struct contents for readability
	if (opcode >= ADD && opcode <= BIS) // if opcode is from add to bis
		add_to_bis_ops(operand.dst, operand.srccon, operand.srcconcheck, &dest_value, &srccon_value, &wordbyte);

	else if (opcode >= MOV && opcode <= SXT) // if opcode is from mov to sxt
		mov_to_sxt_ops(operand.dst, operand.srccon, &dest_value, &srccon_value, &wordbyte);
	
	else if (opcode >= MOVL && opcode <= MOVH) // if opcode is from movl to movh
		movl_to_movh_ops(&bytevalue);

	// switch case to identify what execute process to go through
	switch (opcode) {
	case(ADD):
	case(SUB):
	case(ADDC):
	case(SUBC): // ADD to SUBC are all performed in one function
		add_to_subc_execute(dest_value, srccon_value, operand.dst, wordbyte);
		break;
	case(DADD):
		dadd_execute(dest_value, srccon_value, operand.dst, wordbyte);
		break;
	case(CMP):
		cmp_execute(dest_value, srccon_value);
		break;
	case(XOR):
		xor_execute(dest_value, srccon_value, operand.dst, wordbyte);
		break;
	case(AND):
		and_execute(dest_value, srccon_value, operand.dst, wordbyte);
		break;
	case(OR):
		or_execute(dest_value, srccon_value, operand.dst, wordbyte);
		break;
	case(BIT):
		bit_execute(dest_value, srccon_value, operand.dst);
		break;
	case(BIC):
		bic_execute(dest_value, srccon_value, operand.dst, wordbyte);
		break;
	case(BIS):
		bis_execute(dest_value, srccon_value, operand.dst, wordbyte);
		break;
	case(MOV):
		mov_execute(srccon_value, operand.dst, wordbyte);
		break;
	case(SWAP):
		swap_execute(dest_value, srccon_value, operand.dst, operand.srccon);
		break;
	case(SRA):
		sra_execute(dest_value, operand.dst, wordbyte);
		break;
	case(RRC):
		rrc_execute(dest_value, operand.dst, wordbyte);
		break;
	case(SWPB):
		swpb_execute(operand.dst);
		break;
	case(SXT):
		sxt_execute(dest_value, operand.dst);
		break;
	case(SETCC):
		setcc_execute();
		break;
	case(CLRCC):
		clrcc_execute();
		break;
	case (CEX):
		break;
	case(LD):
		ld_execute();
		break;
	case(ST):
		st_execute();
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
	case(LDR):
		ldr_execute();
		break;
	case(STR):
		str_execute();
		break;
	default:
		break;
	}

	// called debugging functions for ease of marker
#ifdef DEBUG
	reg_display();
	//psw_display();
	printf("\n");
#endif
}

void dmcontroller() {
	printf("dctrl = %d operand.dst = %d dmbr = %04X dmar = %04x\n", dctrl, operand.dst, dmbr, dmar);
	printf("rah\n");
	if (dctrl == READ) { // LD
		if (operand.wordbyte == WORD_CHECK) { // result is word
			dmbr = dmem.word_mem[dmar / BYTE];
			srcconarray.word[REGISTER][operand.dst] = dmbr;
		}
		else { // result is byte
			 dmbr = dmem.byte_mem[dmar];
			 srcconarray.byte[REGISTER][operand.dst][LOW] = MASK_BYTE(dmbr);
		}
	}
	else if (dctrl == WRITE) {
		if (operand.wordbyte == WORD_CHECK) { // result is word
			dmem.word_mem[operand.dst / BYTE] = dmbr;
			srcconarray.word[REGISTER][operand.dst] = dmbr;
		}
		else // result is byte
			dmem.byte_mem[dmar] = MASK_BYTE(dmbr);
	}

	dctrl = DONE;
}

void execute1() {
	dmcontroller();
}