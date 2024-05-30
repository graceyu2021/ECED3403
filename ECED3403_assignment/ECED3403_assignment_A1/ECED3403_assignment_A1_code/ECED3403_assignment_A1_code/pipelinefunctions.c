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
//#include "LOADERFUNCTIONSL.C"

int fetch0(int* programcounter, int* ictrl) {
	int instructionaddress = *programcounter;

	*programcounter += BYTE; // increment by 2 because of byte memory
	*ictrl = READ;

	return instructionaddress;
}

int fetch1(int instructionaddress, int* instructionbit) {


	*instructionbit = (imem.byte_mem[instructionaddress + 1] << 8);
	*instructionbit += imem.byte_mem[instructionaddress];
}

int savesourceconstant(int instructionbit) {
	int sourceconstant;
	unsigned int constantarray[8] = { 0, 1, 2, 4, 8, 16, 32, -1 };

	instructionbit = SOURCECONSTANT_BITS(instructionbit);
	sourceconstant = constantarray[instructionbit];

	return sourceconstant;
}

int savesourceconstantcheck(int instructionbit) {
	int sourceconstantcheck;
	sourceconstantcheck = SOURCECONSTANTCHECK_BITS(instructionbit);
	return sourceconstantcheck;
}

int savewordbyte (int instructionbit) {
	int wordbyte;
	wordbyte = WORDBYTE_BITS(instructionbit);
	return wordbyte;
}

int savebytevalue(int instructionbit) {
	int bytevalue;
	bytevalue = BYTEVALUE_BITS(instructionbit);
	return bytevalue;
}

void printdecode(int nota2, int instructionaddress, char mnemarray[][6], int instructionmnem, int instructionbit, int* sourceconstantcheck,
	 int* wordbyte, int* sourceconstant, int* bytevalue, int* destination) {
	if (nota2 == FALSE) {
		printf("\n%04x: %-5s ", (instructionaddress - BYTE), mnemarray[instructionmnem]);
	}
	else {
		printf("\n%04x: %04x  \n", (instructionaddress - BYTE), instructionbit);
		return;
	}

	if (SOURCECONSTANTCHECK_PRINT(instructionmnem)) {
		printf("RC: %d ", *sourceconstantcheck);
	}
	if (WORDBYTE_PRINT(instructionmnem)) {
		printf("WB: %d ", *wordbyte);
	}
	if (SOURCECONSTANT_PRINT(instructionmnem)){
		if (SOURCECONSTANT_SELECT(*sourceconstantcheck, instructionmnem)) { // print source
			printf("SRC: R%d ", *sourceconstant);
		}
		else { // print constant
			printf("CON: %d ", *sourceconstant);
		}
	}
	if (BYTEVALUE_PRINT(instructionmnem)) {
		printf("BYTE: %04x ", *bytevalue);
	}

	*destination = instructionbit & DESTINATION_BITS;
	printf("DST: R%d\n", *destination);
}

void decode(int instructionaddress, int instructionbit, int instructionmnem, int* sourceconstantcheck, int* wordbyte, int* sourceconstant, int* bytevalue, int* destination) {

	int arrayplace = 0, nota2 = FALSE;
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
	else if (MOVLtoMOBH_BITS(instructionbit)){ // MOVL to MOVH
		arrayplace = MOVLtoMOVH_ARRAY(instructionbit);
		instructionmnem = MOVL + arrayplace;
		*bytevalue = savebytevalue(instructionbit);
	}
	else if (LDtoST_BITS(instructionbit)) { // LD to ST
		//arrayplace = (instructionbit & 0x0400) >> 10;
		//instructionmnem = LD + arrayplace;
		nota2 = TRUE;
	}
	else if (MOVtoCLRCC_BITS(instructionbit)) { // MOV to CLRCC
		if (SETPRItoCLRCC_BITS(instructionbit)) { //SETPRI to CLRCC

			nota2 = TRUE;
		}
		else { // MOV to SXT
			if (MOVtoSWAP_BITS(instructionbit)) { // MOV to SWAP
				arrayplace = MOVtoSWAP_ARRAY(instructionbit);
				instructionmnem = MOV + arrayplace; // adjust enum to place of first command to appear, move

				*sourceconstant = savesourceconstant(instructionbit);
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
			*wordbyte = savewordbyte(instructionbit);
		}
	}
	else { // ADD to BIS
		arrayplace = ADDtoBIS_ARRAY(instructionbit);
		instructionmnem = ADD + arrayplace;

		*wordbyte = savewordbyte(instructionbit);
		*sourceconstantcheck = savesourceconstantcheck(instructionbit);
		*sourceconstant = savesourceconstant(instructionbit);
	}

	printdecode(nota2, instructionaddress, mnemarray, instructionmnem, instructionbit, sourceconstantcheck,
		wordbyte, sourceconstant, bytevalue, destination);
}

void pipeline() {
	int programcounter = 0, clock = 0;
	int instructionbit = NOP, // NOP mov r0, r0
		instructionaddress = 0, instructionmnem = 0, sourceconstantcheck = 0, wordbyte = 0, 
		sourceconstant = 0, bytevalue = 0,destination = 0, ictrl = 0;
	char instructionprint[INSTRUCTIONPRINTMAX];

	programcounter = startaddress;
	
	while (instructionbit != ZERO) { // 0000
		// check clock tick
		if (clock % 2 == 0) { // even number
			instructionaddress = fetch0(&programcounter, &ictrl);
			decode(instructionaddress, instructionbit, instructionmnem, &sourceconstantcheck, &wordbyte, &sourceconstant, &bytevalue, &destination);

		}
		else { // odd number
			fetch1(instructionaddress, &instructionbit);
			// execute function
		}

		clock++; // increment clock
	}
	printf("\n%04x: 0000\n\n", instructionaddress);
}
