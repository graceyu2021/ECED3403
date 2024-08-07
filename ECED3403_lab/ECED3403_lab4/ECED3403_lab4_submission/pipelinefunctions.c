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
		ictrl = DONE;
	}
	return imbr;
}

void fetch1(int instructionaddress, int* ictrl) {
	int tempinstructionbit = instructionbit, imbr = 0;
	instructionbit = imcontroller(instructionaddress, *ictrl, imbr);

#ifndef DEBUG
	printf("\t\tF1: %04X\t\t\tE0: %04X\n", instructionbit, tempinstructionbit);
#endif
}

void printdecode(int nota2, int instructionaddress, char mnemarray[][6], int instructionmnem) {
	printf("D0 at clock %d: ", clock);

	if ((opcode >= ADD && opcode <= SXT) || (opcode >= LD && opcode <= STR) || opcode == SETCC || opcode == CLRCC)
		printf("%04X: %-5s ", instructionaddress, mnemarray[instructionmnem]);
	else {
		printf("%04X: %04X  \n", instructionaddress, instructionbit);
		return;
	}

	if (OFF_PRINT(instructionmnem))
		printf("OFF: %07X ", operand.off); // print offset

	else if (ADDRESSING_PRINT(instructionmnem))
		printf("PRPO: %d DEC %d INC %d ", operand.prpo, operand.dec, operand.inc); // print addressing

	else if (BYTEVALUE_PRINT(instructionmnem))
		printf("BYTE: %02X ", operand.bytevalue); // print byte

	else if (SRCCONCHECK_PRINT(instructionmnem))
		printf("RC: %d ", operand.srcconcheck); // print register/constant check bit

	if (WORDBYTE_PRINT(instructionmnem))
		printf("WB: %d ", operand.wordbyte); // print word/byte
	
	if (SRCCON_PRINT(instructionmnem)) {
		if (SRCCONCHECK_PRINT(instructionmnem))
			printf("CON: %d ", srcconarray.word[CONSTANT][operand.srccon]); // print constant
		else // else srcconcheck = 0
			printf("SRC: R%d ", operand.srccon); // print source
	}

	if (DST_PRINT(instructionmnem))
		printf("DST: R%d", operand.dst); // print destination
	else if (FLAG_PRINT(instructionmnem))
		printf("V: %d SLP %d N: %d Z: %d C: %d", operand.v, operand.slp, operand.n, operand.z, operand.c);

	printf("\n");
}

void opcode_set(int enum_initial, int enum_offset) {
	opcode = enum_initial + enum_offset;
}

void ldrtstr_operands_set() {
	operand.off = OFF_BITS(instructionbit);
	operand.wordbyte = WORDBYTE_BITS(instructionbit);
	operand.wordbyte = WORDBYTE_BITS(instructionbit);
	operand.dst = DST_BITS(instructionbit);
}

void movx_operands_set() {
	operand.dst = DST_BITS(instructionbit);
	operand.bytevalue = BYTEVALUE_BITS(instructionbit);
}

void ldst_operands_set() {
	operand.prpo = PRPO_BITS(instructionbit);
	operand.dec = DEC_BITS(instructionbit);
	operand.inc = INC_BITS(instructionbit);
	operand.wordbyte = WORDBYTE_BITS(instructionbit);
	operand.dst = DST_BITS(instructionbit);
}

void cc_operands_set() {
	operand.v = V_BITS(instructionbit);
	operand.slp = SLP_BITS(instructionbit);
	operand.n = N_BITS(instructionbit);
	operand.z = Z_BITS(instructionbit);
	operand.c = C_BITS(instructionbit);
}

void reg_const_operands_set() {
	operand.srcconcheck = SRCCONCHECK_BITS(instructionbit);
	operand.wordbyte = WORDBYTE_BITS(instructionbit);
	operand.srccon = SRCCON_BITS(instructionbit);
	operand.dst = DST_BITS(instructionbit);
}

int decode(int instructionaddress) {

#ifndef DEBUG
	printf("%d\t%04X\tF0: %04X\tD0: %04X\n", clock, srcconarray.word[REGISTER][R7], srcconarray.word[REGISTER][R7], instructionbit);
#endif

	int arrayplace = 0, nota2 = FALSE;
	char mnemarray[MNEMARRAY_MAX][MNEMARRAY_WORDMAX] = {"BL", "BEQBZ", "BNEBNZ", "BCBHS", "BNCBLO", "BN", "BGE", "BLT", "BRA",
	"ADD", "ADDC", "SUB", "SUBC", "DADD", "CMP", "XOR", "AND", "OR", "BIT",
	"BIC", "BIS", "MOV", "SWAP", "SRA", "RRC", "SWPB", "SXT", "SETPRI", "SVC",
	"SETCC", "CLRCC", "CEX", "LD", "ST", "MOVL", "MOVLZ", "MOVLS", "MOVH", "LDR", "STR" };

	if (LDRtoSTR_BITS(instructionbit)) { // LDR to STR
		ldrtstr_operands_set();
		opcode_set(LDR, LDRtoSTR_ARRAY(instructionbit));
	}
	else if (BLtoBRA_BITS(instructionbit)) { // BL to BRA
		opcode_set(BL, BLtoBRA_ARRAY(instructionbit));
	}
	else if (MOVLtoMOVH_BITS(instructionbit)){ // MOVL to MOVH
		movx_operands_set();
		opcode_set(MOVL, MOVLtoMOVH_ARRAY(instructionbit));
	}
	else if (LDtoST_BITS(instructionbit)) { // LD to ST
		ldst_operands_set();
		opcode_set(LD, LDtoST_ARRAY(instructionbit));
	}
	else if (SETPRItoCLRCC_BITS(instructionbit)) { // SETPRI to CLRCC
		if (SETPRItoSVC_BITS(instructionbit)) { // SETPRI to SVC
			opcode_set(SETPRI, SETPRItoSVC_ARRAY(instructionbit));
		}
		else { // SETCC to CLRCC
			cc_operands_set();
			opcode_set(SETCC, SETCCtoCLRCC_ARRAY(instructionbit));
		}
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

#ifndef DEBUG
	//printdecode(nota2, instructionaddress, mnemarray, opcode);
#endif
}

// convert psw bits to a hexadecimal
unsigned short psw_bit_to_word() {
	unsigned short psw_word = 0;
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

// function to keep track of pipeline
void pipeline() {
	int instructionaddress = 0, instructionmnem = 0, ictrl = 0;

	unsigned short psw_word = psw_bit_to_word(); // convert bits to word

	// print start status. when program is first loaded in, NOP (MOV R0,R0) is executed. clock is initialized to -2 to
	// offset this NOP, making the actual first instruction fetched at clock 0
	printf("Start: PC: %04X PSW: %04X Brkpt: %04X Clk: %d\n", srcconarray.word[REGISTER][R7], psw_word, breakpoint, clock);

#ifndef DEBUG
	printf("Clock\tPC\tFetch\t\tDecode\t\tExecute\n");
#endif

	while (srcconarray.word[REGISTER][R7] != breakpoint && instructionbit != ZERO) { // 0x0000

		// check clock tick
		if (clock % DIV2REMAINDER == ZERO) { // odd number
			instructionaddress = fetch0(&ictrl); // fetch program counter
			instructionmnem = decode(instructionaddress, instructionbit);
		}
		else { // odd number
			fetch1(instructionaddress, &ictrl); // fet
			execute0();
		}

		clock++; // increment clock

		// breaks if increment is set AND if clock is not equal to zero
		if (increment == TRUE && (clock % DIV2REMAINDER == ZERO))
			break;
	}

	printf("End: PC: %04X Clk: %d\n\n", srcconarray.word[REGISTER][R7], clock);
}
