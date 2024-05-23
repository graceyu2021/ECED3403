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

// locate file based on user inputted file name
void prompt_file(char* filestr, int filelength, FILE* filename) {
	// prompt user for filename and scan
	printf("Enter .XME file to load\n");
	scanf("%s", filestr);

	// set last char to '\0'
	filelength = strlen(filestr);
	filestr[filelength] = '\0';
	filename = fopen(filestr, "r");
	printf("filestr: %s", filestr);
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
