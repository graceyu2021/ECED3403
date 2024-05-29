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

	printf("\n\ninstruction bit: %04x instructionaddress: %d\n", *instructionbit, instructionaddress);
}


int savesourceconstant(int instructionbit){
	int sourceconstant;

	instructionbit = instructionbit >> 4;
	sourceconstant = instructionbit;

	return sourceconstant;
}

int savesourceconstantcheck(int instructionbit) {
	int sourceconstantcheck;

	instructionbit = instructionbit >> 6;
	sourceconstantcheck = instructionbit;

	return sourceconstantcheck;
}

void decode(int instructionaddress, int instructionbit, int instructionmnem, int* sourceconstantcheck, int* wordbyte, int* sourceconstant, int* destination) {
	int tempbit = 0;
	char mnemarray[40][6] = {"BL", "BEQBZ", "BNEBNZ", "BCBHS", "BNCBLO", "BN", "BGE", "BLT", "BRA",
	"ADD", "ADDC", "SUB", "SUBC", "DADD", "CMP", "XOR", "AND", "OR", "BIT",
	"BIC", "BIS", "MOV", "SWAP", "SRA", "RRC", "SWPB", "SXT", "SETPRI", "SVC",
	"SETCC", "CLRCC", "CEX", "LD", "ST", "MOVL", "MOVLZ", "MOVLS", "MOVH", "LDR", "STR" };

	tempbit = instructionbit & 0x4000; // & 0b 0100 0000 0000 0000

	printf("instruction!!! %x", instructionbit);

	switch (tempbit) {
	case 0x4000: // add to sxt

		tempbit = instructionbit & 0x0C00; // & 0b 0000 1100 0000 0000
		switch (tempbit) {
		case (0x0C00): // mov to sxt
			tempbit = instructionbit & 0x0100; // 0b 0000 0001 0000 0000

			switch (tempbit) {
			case (0x0000): // mov to swap
				tempbit = instructionbit & 0x0080; // 0b 0000 0000 1000 0000

				switch (tempbit) {
				case (0x0000): // swap
					instructionmnem = SWAP;
					break;
				default: // mov
					instructionmnem = MOV;

					break;
				}

				sourceconstant = savesourceconstant(instructionbit);
				
				break;
			default: // sra to sxt

				break;
			}
			break;
		default: // add to bis

			sourceconstant = savesourceconstant(instructionbit);
			break;
		}
		break;

	default: // 0b0000, movl to movh
		tempbit = instructionbit & 0x0038; // & 0b0000 0000 0011 1000
		switch (tempbit) {
		case 0x0000: // SRA
			instructionmnem = SRA;
			break;
		case 0x0008: // RRC
			instructionmnem = RRC;
			break;
		case 0x0018: //SWPB
			instructionmnem = SWPB;
			break;
		case 0x0020: // SXT
			instructionmnem = SXT;
		}
	
		break;
	}
;
	printf("\n%04x: %s ", instructionaddress, mnemarray[instructionmnem]);

	*destination = instructionbit & 0x0007;
	printf("DST: R%d\n", *destination);
}


void pipeline() {
	int programcounter = 0, clock = 0;
	int instructionbit = NOP, // NOP mov r0, r0
		instructionaddress = 0, instructionmnem = 0, sourceconstantcheck = 0, wordbyte = 0, sourceconstant = 0, destination = 0, ictrl = 0;
	char instructionprint[INSTRUCTIONPRINTMAX];

	programcounter = startaddress;

	

	while (instructionbit != ZERO) { // 0000
		// check clock tick
		if (clock % 2 == 0) { // even number
			instructionaddress = fetch0(&programcounter, &ictrl);
			decode(instructionaddress, instructionbit, instructionmnem, &sourceconstantcheck, &wordbyte, &sourceconstant, &destination);

		}
		else { // odd number
			fetch1(instructionaddress, &instructionbit);
			// execute function
		}

		// print
		//printf("% 04x: %s, RC: %d SRC: %d DST: R%d\n", (programcounter - 2), instructionprint, sourceconstantcheck, sourceconstant, destination);

		clock++; // increment clock

	
	}
}