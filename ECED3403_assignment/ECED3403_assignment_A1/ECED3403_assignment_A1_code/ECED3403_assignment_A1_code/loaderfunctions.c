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


void load_file(FILE* file) {
	char stype[STYPE_MAX];
	int reclength = 0, labelnumcount = 0;


	// read first two chars of line to obtain type of s-record
	sscanf(stype, "%2c", file);
	enum srecord srectype = str_to_int(stype);

	// read record length
	sscanf(labelnumcount, "%x", file);
}

// locate file based on user inputted file name
void prompt_file() {
	FILE* file = NULL;
	char filename[FILE_NAME_MAX];
	int filelength = 0;

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
		printf("hello");
		load_file(file);
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
