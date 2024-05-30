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

	//printf("\n\ninstruction bit: %04x instructionaddress: %x\n", *instructionbit, instructionaddress);
}

int savesourceconstant(int instructionbit) {
	int sourceconstant;
	unsigned int constantarray[8] = { 0, 1, 2, 4, 8, 16, 32, -1 };

	instructionbit = (instructionbit & SOURCECONSTANT_BITS) >> SOURCECONSTANT_SHIFT;
	sourceconstant = constantarray[instructionbit];

	return sourceconstant;
}

int savesourceconstantcheck(int instructionbit) {
	int sourceconstantcheck;

	sourceconstantcheck = (instructionbit & SOURCECONSTANTCHECK_BITS) >> SOURCECONSTANTCHECK_SHIFT;

	return sourceconstantcheck;
}

int savewordbyte (int instructionbit) {
	int wordbyte;

	wordbyte = (instructionbit & WORDBYTE_BITS) >> WORDBYTE_SHIFT;
	return wordbyte;
}

int savebytevalue(int instructionbit) {
	int bytevalue;

	bytevalue = (instructionbit & BYTEVALUE_BITS) >> BYTEVALUE_SHIFT;
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

	if (instructionmnem >= ADD && instructionmnem <= BIS) {
		printf("RC: %d ", *sourceconstantcheck);
	}
	if ((instructionmnem >= ADD && MOV <= BIS) || (instructionmnem >= SRA && instructionmnem <= RRC)) {
		printf("WB: %d ", *wordbyte);
	}
	if (instructionmnem >= ADD && instructionmnem <= SWAP) {
		if ((*sourceconstantcheck == 0) || instructionmnem == MOV || instructionmnem == SWAP) { // print source
			printf("SRC: R%d ", *sourceconstant);
		}
		else { // print constant
			printf("CON: %d ", *sourceconstant);
		}
	}
	if (instructionmnem >= MOVL && instructionmnem <= MOVH) {
		printf("BYTE: %02x ", *bytevalue);
	}

	*destination = instructionbit & 0x0007;
	printf("DST: R%d\n", *destination);
}

void decode(int instructionaddress, int instructionbit, int instructionmnem, int* sourceconstantcheck, int* wordbyte, int* sourceconstant, int* bytevalue, int* destination) {

	int arrayplace = 0, nota2 = FALSE;
	char mnemarray[40][6] = {"BL", "BEQBZ", "BNEBNZ", "BCBHS", "BNCBLO", "BN", "BGE", "BLT", "BRA",
	"ADD", "ADDC", "SUB", "SUBC", "DADD", "CMP", "XOR", "AND", "OR", "BIT",
	"BIC", "BIS", "MOV", "SWAP", "SRA", "RRC", "SWPB", "SXT", "SETPRI", "SVC",
	"SETCC", "CLRCC", "CEX", "LD", "ST", "MOVL", "MOVLZ", "MOVLS", "MOVH", "LDR", "STR" };

	if ((instructionbit & LDRtoSTR_BITS) == 1) { // LDR to STR
		//arrayplace = (instructionbit & 0x4000) >> 14;
		//instructionmnem = LDR + arrayplace; // adjust enum to place of first command to appear, move
		nota2 = TRUE;
	}
	else if ((instructionbit & BLtoBRA_BITS) == 0) { // BL to BRA
		//arrayplace = (instructionbit & 0x4C00) >> 10;
		//instructionmnem = BL + arrayplace; // adjust enum to place of first command to appear, move
		nota2 = TRUE;
	}
	else if ((instructionbit & MOVLtoMOVH_BITS) == 0x2000) { // MOVL to MOVH
		arrayplace = (instructionbit & 0x1800) >> 11;
		instructionmnem = MOVL + arrayplace;
		*bytevalue = savebytevalue(instructionbit);
	}
	else if ((instructionbit & LDtoST_BITS) == LDtoST_BITS) { // LD to ST
		//arrayplace = (instructionbit & 0x0400) >> 10;
		//instructionmnem = LD + arrayplace;
		nota2 = TRUE;
	}
	else if ((instructionbit & MOVtoCLRCC_BITS) == MOVtoCLRCC_BITS) { // MOV to CLRCC
		if ((instructionbit & 0x0180) == 0x0180) { //SETPRI to CLRCC

			nota2 = TRUE;
		}
		else { // MOV to SXT
			if ((instructionbit & MOVtoSWAP_BITS) == 0) { // MOV to SWAP
				arrayplace = (instructionbit & 0x0080) >> 7;
				instructionmnem = MOV + arrayplace; // adjust enum to place of first command to appear, move

				*sourceconstant = savesourceconstant(instructionbit);
			}
			else { // SRA to SXT
				arrayplace = (instructionbit & 0x0038) >> 3;
				instructionmnem = SRA + arrayplace;
			}
			*wordbyte = savewordbyte(instructionbit);
		}
	}
	else { // ADD to BIS
		arrayplace = (instructionbit & 0x0F00) >> 8;
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
}
