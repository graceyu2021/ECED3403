/*
file: decodefunctions.c
programmer: Grace Yu
b00: B00902046
class: ECED3403
professor: Dr. Larry Hughes
assignment: A1
submission date: May 23rd, 2024

This is the decode functions file of my program.
*/

#include "MAINHEADER.H"

srcconarray_union srcconarray = {
	.word = {{0}, {0, 1, 2, 4, 8, 16, 32, -1}}
};

psw_struct psw = {
	.prev = PREV_INITIAL,
	.curr = CURR_INITIAL
};

int fetch0(int* ictrl) {
	int instructionaddress = srcconarray.word[REGISTER][R7];

	srcconarray.word[REGISTER][R7] += BYTE; // increment by 2 because of byte memory
	*ictrl = READ;

	return instructionaddress;
}

int imcontroller(int instructionaddress, int ictrl, int imbr) {
	if (ictrl = READ) {
		imbr = imem.word_mem[(instructionaddress + BYTE) / BYTE];
		ictrl = DONEREAD;
	}
	return imbr;
}

void fetch1(int instructionaddress, int* ictrl) {
	int imbr = 0;

	instructionbit = imcontroller(instructionaddress, *ictrl, imbr);
}

void printdecode(int nota2, int instructionaddress, char mnemarray[][6], int instructionmnem) {
	if ((opcode >= ADD && opcode <= SXT) || (opcode >= MOVL && opcode <= MOVH))
		printf("%04X: %-5s ", instructionaddress, mnemarray[instructionmnem]);
	else {
		printf("%04X: %04X  \n", instructionaddress, instructionbit);
		return;
	}

	if (SOURCECONSTANTCHECK_PRINT(instructionmnem)) // print register/constant check bit?
		printf("RC: %d ", reg_const_operands.sourceconstantcheck);

	if (WORDBYTE_PRINT(instructionmnem)) // print word/byte?
		printf("WB: %d ", reg_const_operands.wordbyte);

	if (SOURCECONSTANT_PRINT(instructionmnem)){ // print source/constant
		if (SOURCECONSTANT_SELECT(reg_const_operands.sourceconstantcheck, instructionmnem)) // print source
			printf("SRC: R%d ", reg_const_operands.sourceconstant);
		else // print constant
			printf("CON: %d ", srcconarray.word[CONSTANT][reg_const_operands.sourceconstant]);
	}
	if (BYTEVALUE_PRINT(instructionmnem)) // print bytes
		printf("BYTE: %02X ", movx_operands.bytevalue);

	if (opcode >= MOV && opcode <= MOVH)
		printf("DST: R%d\n", movx_operands.destination);
	else 
		printf("DST: R%d\n", reg_const_operands.destination);
}

void opcode_set(int enum_initial, int enum_offset) {
	opcode = enum_initial + enum_offset;
}

void movx_operands_set(int instructionbit) {
	movx_operands.destination = DESTINATION_BITS(instructionbit);
	movx_operands.bytevalue = BYTEVALUE_BITS(instructionbit);
}

void reg_const_operands_set(int instructionbit) {
	reg_const_operands.sourceconstantcheck = SOURCECONSTANTCHECK_BITS(instructionbit);
	reg_const_operands.wordbyte = WORDBYTE_BITS(instructionbit);
	reg_const_operands.sourceconstant = SOURCECONSTANT_BITS(instructionbit);
	reg_const_operands.destination = DESTINATION_BITS(instructionbit);
}

int decode(int instructionaddress) {

	int arrayplace = 0, nota2 = FALSE;
	char mnemarray[MNEMARRAY_MAX][MNEMARRAY_WORDMAX] = {"BL", "BEQBZ", "BNEBNZ", "BCBHS", "BNCBLO", "BN", "BGE", "BLT", "BRA",
	"ADD", "ADDC", "SUB", "SUBC", "DADD", "CMP", "XOR", "AND", "OR", "BIT",
	"BIC", "BIS", "MOV", "SWAP", "SRA", "RRC", "SWPB", "SXT", "SETPRI", "SVC",
	"SETCC", "CLRCC", "CEX", "LD", "ST", "MOVL", "MOVLZ", "MOVLS", "MOVH", "LDR", "STR" };

	if (LDRtoSTR_BITS(instructionbit)) { // LDR to STR
		opcode_set(LDR, LDRtoSTR_ARRAY(instructionbit));
	}
	else if (BLtoBRA_BITS(instructionbit)) { // BL to BRA
		opcode_set(BL, BLtoBRA_ARRAY(instructionbit));
	}
	else if (MOVLtoMOVH_BITS(instructionbit)){ // MOVL to MOVH
		movx_operands_set(instructionbit);
		opcode_set(MOVL, MOVLtoMOVH_ARRAY(instructionbit));
	}
	else if (LDtoST_BITS(instructionbit)) { // LD to ST
		opcode_set(LD, LDtoST_ARRAY(instructionbit));
	}
	else if (SETPRItoCLRCC_BITS(instructionbit)) { // SETPRI to CLRCC
		opcode_set(SETPRI, ZERO); // will elaborate in when these opcodes are to be implemented
	}
	else if (ADDtoSXT_BITS(instructionbit)) { // ADD to SXT
		reg_const_operands_set(instructionbit); // set operand values for ADD to SXT

		if (MOVtoSWAP_BITS(instructionbit)) { // MOV to SWAP
			opcode_set(MOV, MOVtoSWAP_ARRAY(instructionbit));
		}
		else if (SRAtoRRC_BITS(instructionbit)) { // SRA to RRC
			opcode_set(SRA, SRAtoRRC_ARRAY(instructionbit));
		}
		else if (SWPBtoSXT(instructionbit)) { // SWPB to SXT
			opcode_set(SWPB, SWPBtoSXT_ARRAY(instructionbit));
		}
		else {// ADD to BIS
			opcode_set(ADD, ADDtoBIS_ARRAY(instructionbit));
		}
	}
	else { // CEX
		opcode = CEX;
	}

	if (clock != CLOCK_INITIALIZE)
		printdecode(nota2, instructionaddress, mnemarray, opcode);
}

// convert psw bits to a hexadecimal
unsigned short psw_bit_to_word() {
	unsigned short psw_word;
	psw_word |= psw.prev << PREV_SHIFT;
	psw_word |= psw.flt << FLT_SHIFT;
	psw_word |= psw.curr << CURR_SHIFT;
	psw_word |= psw.v << V_SHIFT;
	psw_word |= psw.slp << SLP_SHIFT;
	psw_word |= psw.n << N_SHIFT;
	psw_word |= psw.z << Z_SHIFT;
	psw_word |= psw.c;

	return psw_word;
}

void pipeline() {
	int instructionaddress = 0, instructionmnem = 0, ictrl = 0;

	unsigned short psw_word = psw_bit_to_word();

	if (clock != CLOCK_INITIALIZE)
		printf("Start: PC: %04X PSW: %04X Brkpt: %04X Clk: %d\n", srcconarray.word[REGISTER][R7], psw_word, breakpoint, clock);
	else
		printf("Start: PC: %04X PSW: %04X Brkpt: %04X Clk: 0\n", srcconarray.word[REGISTER][R7] + NOP_PC_OFFSET, psw_word, breakpoint, clock);

	while (srcconarray.word[REGISTER][R7] != breakpoint && instructionbit != ZERO) { // 0x0000
		// check clock tick
		if (clock % 2 == ZERO) { // even number
			instructionaddress = fetch0(&ictrl);
			instructionmnem = decode(instructionaddress, instructionbit);
		}
		else { // odd number
			fetch1(instructionaddress, &ictrl);;
			execute();
		}

		clock++; // increment clock

		// breaks if increment is set AND if clock is not equal to zero
		if (increment == TRUE && clock % 2 == ZERO && clock != ZERO)
			break;
	}

	printf("End: PC: %04X Clk: %d\n\n", srcconarray.word[REGISTER][R7], clock);
}
