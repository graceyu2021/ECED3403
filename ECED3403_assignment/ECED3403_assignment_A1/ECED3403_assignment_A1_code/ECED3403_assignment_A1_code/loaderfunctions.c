/*
file: loaderfunctions.c
programmer: Grace Yu
b00: B00902046
class: ECED3403
professor: Dr. Larry Hughes
assignment: A1
submission date: May 23rd, 2024

This is the main file of my program.
*/

#include "LOADERHEADER.H"
#include <stdio.h>

// print menu to screen
char print_menu() {
	char option = ' ';

	// print menu
	printf("? - lists the option (below)\nL - load\nM - display mem\nX - exit\nOption: ");
	scanf("%c", &option);

	return option;
}



int load_file(FILE* file) {
	char srecord[SREC_MAX], 
		stype[BYTE + 1]; // + 1 for '\0'
	int reclength = 0, reccount = 0, address = 0, startaddress = 0;

	// obtain complete record from file
	fgets(srecord, BYTE, file);

	// read first two chars of line to obtain type of s-record
	sscanf(srecord, "%2c", &stype);
	enum srecordtype srectype = str_to_int(stype);
	reccount += BYTE; // += BYTE (2) because record indicator is from 1 byte

	// read record length
	sscanf(srecord + reccount, "%2x", reclength); // + reccount to offset the previous bytes
	reccount += BYTE;

	// read address field
	sscanf(srecord + reccount, "%2x", address);
	reccount += BYTE * 2; // += BYTE *2 (4) because address is from 2 bytes
	if (srectype == S9) { // S9 address records the starting address
		startaddress = address;
	}

	// read name or data/instruction byte(s)
	while (reccount < reclength && reccount < SREC_MAX) {
		switch (srectype) {
		case S0:
			break;
		case S1:
			break;

		case S2:
			break;

		case S9:
			break;
		}

	}

}

void file_found_print(const char* filename, int startaddress) {
	int filelength;

	// change filename to .asm instead of .xme
	filelength = strlen(filename);
	strcpy(filename + filelength - 3, "asm");

	printf("Source filename: %s\n", filename);
	printf("File read - no errors detected. Starting address: %.4x\n", startaddress);
}

// locate file based on user inputted file name
void prompt_file() {
	FILE* file = NULL;
	char filename[FILE_NAME_MAX];
	int filelength = 0, startaddress = 0;

	// prompt user for filename and scan
	printf("Enter .XME file to load\n");
	scanf("%s", filename);

	// set last char to '\0'
	//filelength = strlen(filestr);
	//filestr[filelength] = '\0';
	file = fopen(filename, "r");

	if (file == NULL) { // could not locate file
		printf("Can't open >%s<\n", filename);
	}
	else { // successfully located file
		startaddress = load_file(file);
		file_found_print(filename, startaddress);
	}
}

enum srecordtype str_to_int(char* stype) {
	if (strcmp(stype, "S0")) {
		return S0;
	}
	else if (strcmp(stype, "S1")) {
		return S1;
	}
	else if (strcmp(stype, "S2")) {
		return S2;
	}
	else if (strcmp(stype, "S9")) {
		return S9;
	}
}
