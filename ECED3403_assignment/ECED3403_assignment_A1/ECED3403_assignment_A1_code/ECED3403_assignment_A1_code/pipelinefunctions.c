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

void set_srcconarray(){ // set constant values
	srcconarray[CONSTANT][0] = 0;
	srcconarray[CONSTANT][1] = 1;
	srcconarray[CONSTANT][2] = 2;
	srcconarray[CONSTANT][3] = 4;
	srcconarray[CONSTANT][4] = 8;
	srcconarray[CONSTANT][5] = 16;
	srcconarray[CONSTANT][6] = 32;
	srcconarray[CONSTANT][7] = -1;

	return;
}

int fetch0(int* ictrl) {
	int instructionaddress = srcconarray[REGISTER][R7] - BYTE;

	srcconarray[REGISTER][R7] += BYTE; // increment by 2 because of byte memory
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

int fetch1(int instructionaddress, int* ictrl) {
	int imbr = 0, instructionbit = 0;

	instructionbit = imcontroller(instructionaddress, *ictrl, imbr);

	return instructionbit;
}

void printdecode(int nota2, int instructionaddress, char mnemarray[][6], int instructionmnem, int instructionbit) {

	if (nota2 == FALSE)
		printf("\n%04x: %-5s ", instructionaddress, mnemarray[instructionmnem]);
	else {
		printf("\n%04x: %04x  \n", instructionaddress, instructionbit);
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
			printf("CON: %d ", srcconarray[CONSTANT][reg_const_operands.sourceconstant]);
	}
	if (BYTEVALUE_PRINT(instructionmnem)) // print bytes
		printf("BYTE: %04x ", movx_operands.bytevalue);

	movx_operands.destination = instructionbit & DESTINATION_BITS;
	printf("DST: R%d\n", movx_operands.destination);
}

int decode(int instructionaddress, int instructionbit) {

	int arrayplace = 0, nota2 = FALSE, instructionmnem = 0;
	char mnemarray[MNEMARRAY_MAX][MNEMARRAY_WORDMAX] = {"BL", "BEQBZ", "BNEBNZ", "BCBHS", "BNCBLO", "BN", "BGE", "BLT", "BRA",
	"ADD", "ADDC", "SUB", "SUBC", "DADD", "CMP", "XOR", "AND", "OR", "BIT",
	"BIC", "BIS", "MOV", "SWAP", "SRA", "RRC", "SWPB", "SXT", "SETPRI", "SVC",
	"SETCC", "CLRCC", "CEX", "LD", "ST", "MOVL", "MOVLZ", "MOVLS", "MOVH", "LDR", "STR" };

	if (LDRtoSTR_BITS(instructionbit)) { // LDR to STR
		//arrayplace = (instructionbit & 0x4000) >> 14;
		//instructionmnem = LDR + arrayplace; // adjust enum to place of first command to appear, move
		nota2 = TRUE;
	}
	else if (BLtoBRA_BITS(instructionbit)) { // BL to BRA
		//arrayplace = (instructionbit & 0x4C00) >> 10;
		//instructionmnem = BL + arrayplace; // adjust enum to place of first command to appear, move
		nota2 = TRUE;
	}
	else if (MOVLtoMOVH_BITS(instructionbit)){ // MOVL to MOVH
		arrayplace = MOVLtoMOVH_ARRAY(instructionbit);
		instructionmnem = MOVL + arrayplace;
		movx_operands.bytevalue = BYTEVALUE_BITS(instructionbit);
	}
	else if (LDtoST_BITS(instructionbit)) { // LD to ST
		//arrayplace = (instructionbit & 0x0400) >> 10;
		//instructionmnem = LD + arrayplace;
		nota2 = TRUE;
	}
	else if (MOVtoCLRCC_BITS(instructionbit)) { // MOV to CLRCC
		if (SETPRItoCLRCC_BITS(instructionbit)) //SETPRI to CLRCC
			nota2 = TRUE;
	
		else { // MOV to SXT
			if (MOVtoSWAP_BITS(instructionbit)) { // MOV to SWAP
				arrayplace = MOVtoSWAP_ARRAY(instructionbit);
				instructionmnem = MOV + arrayplace; // adjust enum to place of first command to appear, move

				reg_const_operands.sourceconstant = SOURCECONSTANT_BITS(instructionbit);
			}
			else { // SRA to SXT
				if (SRAtoRRC_BITS(instructionbit)) { // SRA to RRC
					arrayplace = SRAtoRRC_ARRAY(instructionbit);
					instructionmnem = SRA + arrayplace;
				}
				else { // SWPB to SXT
					arrayplace = SWPBtoSXT_ARRAY(instructionbit);
					instructionmnem = SWPB + arrayplace;
				}
			}
			reg_const_operands.wordbyte = WORDBYTE_BITS(instructionbit);
		}
	}
	else { // ADD to BIS
		arrayplace = ADDtoBIS_ARRAY(instructionbit);
		instructionmnem = ADD + arrayplace;

		reg_const_operands.wordbyte = WORDBYTE_BITS(instructionbit);
		reg_const_operands.sourceconstantcheck = SOURCECONSTANTCHECK_BITS(instructionbit);
		reg_const_operands.sourceconstant = SOURCECONSTANT_BITS(instructionbit);
	}
	printdecode(nota2, instructionaddress, mnemarray, instructionmnem, instructionbit);
}

void pipeline() {
	int clock = 0;
	int instructionbit = NOP, // NOP mov r0, r0
		instructionaddress = 0x4C40, instructionmnem = 0, ictrl = 0;

	set_srcconarray();

	printf("Start: PC: %04x PSW: ---- Brkpt: %04x Clk: %d", srcconarray[REGISTER][R7], breakpoint, clock);

	while ((instructionbit != ZERO) && (srcconarray[REGISTER][R7] != breakpoint + BYTE)) { // 0x0000
		// check clock tick
		if (clock % 2 == ZERO) { // even number
			instructionaddress = fetch0(&ictrl);
			instructionmnem = decode(instructionaddress, instructionbit);
		}
		else { // odd number
			instructionbit = fetch1(instructionaddress, &ictrl);
			// execute function
		}

		clock++; // increment clock

		if (increment == TRUE && clock % 2 != ZERO)
			break;
	}
	printf("End: PC: %04x Clk: %d\n\n", srcconarray[REGISTER][R7] - PC_OFFSET, clock);
	//printf("\n%04x: %04x\n\n", instructionaddress + BYTE, imem.word_mem[(instructionaddress + BYTE) / BYTE]);
}
