/*
file: loaderfunctionsL.c
programmer: Grace Yu
b00: B00902046
class: ECED3403
professor: Dr. Larry Hughes
assignment: A1
submission date: May 23rd, 2024

This is the loader functions L file of my program
*/

#include "MAINHEADER.H"

#include <stdio.h>

// print menu to screen
char print_menu() {
	char option = ' ';

	// print menu
	printf("? - lists the option (below)\nC - change a memory location (word)\nB - breakpoint\n");
	printf("L - load\nR - display regs\nS - set register values(word)\n");
	printf("M - display mem\nP - display PSW bits\n");
	printf("G - go (continuous or single step [toggle with 'I']\n");
	printf("I - increment PC(toggle continuous or run of single step)\nX - exit\nOption: ");
	scanf(" %c", &option);

	return option;
}

enum srecordtype str_to_int(char snum) { // enum of s record types
	switch (snum) {
	case '0':
		return S0;
	case '1':
		return S1;
	case '2':
		return S2;
	case '9':
		return S9;
	default:
		return INVALID_S;
	}
}

// read what type of s-record it is
int read_s_record(const char* srecord, srecordtype* srectype, int reccount) {
	char stype[BYTE + 1] = { 0 }; // + 1 for '\0'
	int snum;

	//sscanf(srecord, "%2s", stype);
	snum = srecord[1];
	*srectype = str_to_int(snum);
	reccount += BYTE; // += BYTE (2) because record indicator is from 1 byte
	return reccount;
}

// read record length
int read_record_len(const char* srecord, int* reclength, int reccount, unsigned int* checksumcount) {
	sscanf(srecord + reccount, "%2x", reclength); // + reccount to offset the previous bytes
	reccount += BYTE;
	*checksumcount += *reclength;
	return reccount;
}

int read_address(const char* srecord, int* address, int reccount, unsigned int* checksumcount) {
	int addresshi = 0, addresslow = 0;

	sscanf(srecord + reccount, "%2x", &addresshi); // scan address high
	reccount += BYTE; // += BYTE *2 (4) because address is from 2 bytes
	*checksumcount += addresshi; // add address high bytes to checksumcount

	sscanf(srecord + reccount, "%2x", &addresslow); //scan address low
	reccount += BYTE;
	*checksumcount += addresslow; // add address high bytes to checksumcount
	*address = addresslow;
	*address = addresshi << 8 | addresslow; // combine high and low bytes of address

	return reccount;
}

// save new byte to corresponding memory
void memory_save(const char* srecord, int* reccount, int* address, memory array[], unsigned int* checksumcount) {
	int tempbytes;

	sscanf(srecord + *reccount, "%2x", &tempbytes); // + reccount to offset the previous bytes
	array->byte_mem[*address] = tempbytes; // save byte to array
	*reccount += BYTE;
	*checksumcount += tempbytes; // add tempbytes bytes to checksumcount
	(*address)++;
}

//id memory_save_zero_nine
int compare_checksum(const char* srecord, int reccount, unsigned int checksumcount) {
	unsigned int checksum = 0;
	sscanf(srecord + reccount, "%2x", &checksum);

	checksumcount = (~checksumcount) & 0xFF;

	return (checksumcount == checksum);
}

// handle newly loaded file
int load_file(FILE* file) {
	char srecord[SREC_MAX + 1];
	char recname[SREC_MAX]; // asm file name
	int recnamecount = 0, reclength, address, tempbytes, recvalidity = TRUE;
	srecordtype srectype;

	while (fgets(srecord, SREC_MAX + 1, file) != NULL) {// obtain complete record from file. SREC_MAX + 1 because of '/0'
		int reccount = 0, checksum = 0;
		int len = strlen(srecord);
		unsigned int checksumcount = 0;

		// read first two chars of line to obtain type of s-record and increment record count
		reccount = read_s_record(srecord, &srectype, reccount);

		// read record length and increment record count
		reccount = read_record_len(srecord, &reclength, reccount, &checksumcount);

		// read address field
		reccount = read_address(srecord, &address, reccount, &checksumcount);

		if (srectype == S9) { // S9 address records the starting address
			srcconarray.word[REGISTER][R7] = address;
		}

		while (RECORD_BOUND(reccount, reclength) && recvalidity == TRUE) { // BYTE * 4 to skip first 8 bytes, reclength - BYTE because of checksum
			switch (srectype) {
			case S0:
				sscanf(srecord + reccount, "%2x", &tempbytes); // + reccount to offset the previous bytes
				recname[recnamecount] = tempbytes; // save byte to array
				reccount += BYTE;
				checksumcount += tempbytes; // add tempbytes bytes to checksumcount
				recnamecount++;
				break;

			case S1:
				memory_save(&srecord, &reccount, &address, imem.byte_mem, &checksumcount);
				break;

			case S2:
				memory_save(&srecord, &reccount, &address, dmem.byte_mem, &checksumcount);
				break;

			case S9:
				sscanf(srecord + reccount, "%2x", &tempbytes); // + reccount to offset the previous bytes
				reccount += BYTE;
				checksumcount += tempbytes; // add tempbytes bytes to checksumcount
				break;
			case INVALID_S:
				printf("Invalid S-record encountered.\n");
				recvalidity == FALSE;
				break;
			}
		}

		if (compare_checksum(srecord, reccount, checksumcount) == FALSE) { // if invalid checksum encountered
			recvalidity = FALSE;
			break;
		}
	}

	file_origin_print(recname, recnamecount);

	if (recvalidity == TRUE) {; // all records are valid !
		printf("\nFile read - no errors detected. Starting address: %.4x\n", srcconarray.word[REGISTER][R7]);
		srcconarray.word[REGISTER][R7] -= BYTE; // set PC to be one byte less to accomadate for NOP
		instructionbit = NOP; // initialize NOP mov r0, r0
		clock = CLOCK_INITIALIZE; // initialize clock
	}
	else { // invalid record(s) encountered
		int length = strlen(srecord);
		srecord[length - NULLCHAR] = '\0'; // Replace the last character with the null terminator
		printf("Invalid checksum: >%s<\n", srecord);
	}
}

// function to print .asm origin file based on s0 record
void file_origin_print(char* recname, int recnamecount) {
	printf("Source filename: ");
	for (int i = 0; i < recnamecount; i++) {
		printf("%c", (char)recname[i]);
	}
}

// locate file based on user inputted file name
void prompt_file() {
	FILE* file = NULL;
	char filename[FILE_NAME_MAX];
	int filelength = 0;

	// prompt user for filename and scan
	printf("Enter .XME file to load\n");
	scanf("%s", filename);
	file = fopen(filename, "r");

	if (file == NULL) { // could not locate file
		printf("Can't open >%s<\n", filename);
		return;
	}
	
	// load in file
	load_file(file);
}

void single_step() {
	switch (increment) { // change increment based on its current value
	case (TRUE):
		increment = FALSE;
		printf("Single step is disabled\n");
		break;
	case (FALSE):
		increment = TRUE;
		printf("Single step is enabled\n");
	}
}