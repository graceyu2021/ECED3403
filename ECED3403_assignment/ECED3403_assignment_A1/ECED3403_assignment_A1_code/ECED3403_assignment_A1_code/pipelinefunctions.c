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


int savesourceconstant(int instructionbit){
	int sourceconstant;
	unsigned int constantarray[8] = { 0, 1, 2, 4, 8, 16, 32, -1 };

	instructionbit = (instructionbit & 0x0038) >> 3;
	sourceconstant = constantarray[instructionbit];

	return sourceconstant;
}

int savesourceconstantcheck(int instructionbit) {
	int sourceconstantcheck;

	sourceconstantcheck = (instructionbit & 0x0080) >> 7;

	return sourceconstantcheck;
}

int savewordbyte (int instructionbit) {
	int wordbyte;

	wordbyte = (instructionbit & 0x0040) >> 6;

	return wordbyte;
}

int savebytevalue(int instructionbit) {
	int bytevalue;

	bytevalue = (instructionbit & 0x07F8) >> 3;
}

void decode(int instructionaddress, int instructionbit, int instructionmnem, int* sourceconstantcheck, int* wordbyte, int* sourceconstant, int* bytevalue, int* destination) {
	
	//printf("\n\ninstruction bit: %04x instructionaddress: %x\n", instructionbit, instructionaddress);

	int tempbit = 0;
	char mnemarray[40][6] = {"BL", "BEQBZ", "BNEBNZ", "BCBHS", "BNCBLO", "BN", "BGE", "BLT", "BRA",
	"ADD", "ADDC", "SUB", "SUBC", "DADD", "CMP", "XOR", "AND", "OR", "BIT",
	"BIC", "BIS", "MOV", "SWAP", "SRA", "RRC", "SWPB", "SXT", "SETPRI", "SVC",
	"SETCC", "CLRCC", "CEX", "LD", "ST", "MOVL", "MOVLZ", "MOVLS", "MOVH", "LDR", "STR" };

	tempbit = instructionbit & 0x2000; // & 0b 0010 0000 0000 0000

	switch (tempbit) {
	case 0x0000: // add to sxt

		tempbit = instructionbit & 0x0C00; // & 0b 0000 1100 0000 0000
		switch (tempbit) {
		case (0x0C00): // mov to sxt
			tempbit = instructionbit & 0x0100; // & 0b 0000 0001 0000 0000

			switch (tempbit) {
			case (0x0000): // mov to swap
				tempbit = instructionbit & 0x0080; // 0b 0000 0000 1000 0000

				switch (tempbit) {
				case (0x0000): // swap
					instructionmnem = SWAP;
					break;

				default: // mov
					instructionmnem = MOV;
					*wordbyte = savewordbyte(instructionbit);
					break;
				}

				*sourceconstant = savesourceconstant(instructionbit);
				
				break;
			default: // sra to sxt
				tempbit = instructionbit & 0x0038; // & 0b0000 0000 0011 1000
				switch (tempbit) {
				case 0x0000: // SRA
					instructionmnem = SRA;
					*wordbyte = savewordbyte(instructionbit);
					break;
				case 0x0008: // RRC
					instructionmnem = RRC;
					*wordbyte = savewordbyte(instructionbit);
					break;
				case 0x0018: //SWPB
					instructionmnem = SWPB;
					break;
				case 0x0020: // SXT
					instructionmnem = SXT;
				}
				break;
			}
			break;
		default: // add to bis
			tempbit = instructionbit & 0x0F00; // & 0b0000 1111 0000 0000
			switch (tempbit) {
			case 0x0000: // ADD
				instructionmnem = ADD;
				break;
			case 0x0100: // ADDC
				instructionmnem = ADDC;
				break;
			case 0x0200: // SUB
				instructionmnem = SUB;
				break;
			case 0x0300: // SUBC
				instructionmnem = SUBC;
				break;
			case 0x0400: // DADD
				instructionmnem = DADD;
				break;
			case 0X0500: // CMP
				instructionmnem = CMP;
				break;
			case 0X0600: // XOR
				instructionmnem = XOR;
				break;
			case 0x0700: // AND
				instructionmnem = AND;
				break;
			case 0x0800: // OR
				instructionmnem = OR;
				break;
			case 0x0900: // BIT
				instructionmnem = BIT;
				break;
			case 0x0A00: // BIS
				instructionmnem = BIS;
				break;
			}

			*wordbyte = savewordbyte(instructionbit);
			*sourceconstantcheck = savesourceconstantcheck(instructionbit);
			*sourceconstant = savesourceconstant(instructionbit);
			break;
		}
		break;

	default: // 0b0000, movl to movh
		tempbit = instructionbit & 0x1800; // & 0b0001 1000 0000 0000
		switch (tempbit) {
		case 0x0000: // movl
			instructionmnem = MOVL;
			break;
		case 0x0800: // movlz
			instructionmnem = MOVLZ;
			break;
		case 0x0100: //MOVLS
			instructionmnem = MOVLS;
			break;
		default: // MOVH
			instructionmnem = MOVH;
		}

		*bytevalue = savebytevalue(instructionbit);
		break;
	}
;
	printf("\n%04x: %s ", (instructionaddress - 2), mnemarray[instructionmnem]);

	if (instructionmnem >= ADD && instructionmnem <= BIS) {
		printf("RC: %d ", *sourceconstantcheck);
	}
	if ((instructionmnem >= ADD && MOV <= BIS) || (instructionmnem >= SRA && instructionmnem <= RRC)) {
		printf("WB: %d ", *wordbyte);
	}
	if (instructionmnem >= ADD && instructionmnem <= SWAP) {
		if ((*sourceconstantcheck == 0) || instructionmnem == MOV || instructionmnem == SWAP) { // print source
			printf("SRC: R%d ", *sourceconstantcheck);
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