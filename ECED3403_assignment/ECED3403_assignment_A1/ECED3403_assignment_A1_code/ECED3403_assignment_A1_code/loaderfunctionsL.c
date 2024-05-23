/*
file: loaderfunctionsL.c
programmer: Grace Yu
b00: B00902046
class: ECED3403
professor: Dr. Larry Hughes
assignment: A1
submission date: May 23rd, 2024

This is the main file of my program
*/

#include "LOADERHEADER.H"
#include <stdio.h>

// declared array for file name, imem, and dmem global
union memory imem; // instruction memory
union memory dmem; // data memory
int recname[SREC_MAX]; // asm file name
int recnamecount; // size of recname

// print menu to screen
char print_menu() {
	char option = ' ';

	// print menu
	printf("? - lists the option (below)\nL - load\nM - display mem\nX - exit\nOption: ");
	scanf("%c", &option);

	return option;
}

enum srecordtype str_to_int(char* stype) { // enum of s record types
	if (strcmp(stype, "S0") == 0) {
		return S0;
	}
	else if (strcmp(stype, "S1") == 0) {
		return S1;
	}
	else if (strcmp(stype, "S2") == 0) {
		return S2;
	}
	else if (strcmp(stype, "S9") == 0) {
		return S9;
	}
}

int read_s_record(const char* srecord, srecordtype* srectype, int reccount) {
	char stype[BYTE + 1]; // + 1 for '\0'

	sscanf(srecord, "%2s", stype);
	*srectype = str_to_int(stype);
	reccount += BYTE; // += BYTE (2) because record indicator is from 1 byte
	return reccount;
}

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

int load_file(FILE* file) {
	char srecord[SREC_MAX + 1];
	int reclength = 0, address = 0, startaddress = 0, tempbytes = 0;
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
			startaddress = address;
		}

		while (reccount >= (BYTE * 4) && reccount < (2 * (reclength + BYTE) - BYTE)) { // BYTE * 4 to skip first 8 bytes, reclength - BYTE because of checksum
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
			}
		}

		if (compare_checksum(srecord, reccount, checksumcount) != TRUE) {
			printf("Source filename : ");
			int i = 0;

			for (i = 0; i < recnamecount; i++) {
				printf("%c", (char)recname[i]);
			}

			printf("Invalid checksum: > %s", srecord);
		}

	}
	return startaddress;
}

void file_found_print(int startaddress) {
	int i = 0;

	printf("Source filename: ");

	for (i = 0; i < recnamecount; i++) {
		printf("%c", (char)recname[i]);
	}

	printf("\nFile read - no errors detected. Starting address: %.4x\n", startaddress);
}

// locate file based on user inputted file name
void prompt_file() {
	FILE* file = NULL;
	char filename[FILE_NAME_MAX];
	int filelength = 0, startaddress = 0;

	// prompt user for filename and scan
	printf("Enter .XME file to load\n");
	scanf("%s", filename);
	file = fopen(filename, "r");

	if (file == NULL) { // could not locate file
		printf("Can't open >%s<\n", filename);
	}
	else { // successfully located file
		startaddress = load_file(file);
		file_found_print(startaddress);
	}
}