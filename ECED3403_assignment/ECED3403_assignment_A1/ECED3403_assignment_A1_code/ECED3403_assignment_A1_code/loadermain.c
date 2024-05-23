/*
file: loadermain.c
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

int main() {	
	FILE* filename = NULL;
	int filelength = 0;
	char option = ' ', discard = ' ';
	char filestr[FILE_NAME_MAX], stype[STYPE_MAX];

	// print user and send user input to variable option
	option = print_menu();

	while (option != 'X' && option != 'x') {
		
		switch (option) {
			case '?': // lists the options (below)
				option = print_menu();
				break;

			case 'L': // load an .xme file
			case 'l':
				// ask user for name of file to load and check if valid
				prompt_file(filestr, filelength, filename);
				printf("grace is coool");

				if (filename == NULL) { // could not locate file
					printf("Can't open >%s<\n", filestr);
				}
				else { // successfully located file
					// read type of s-record
					//fgets(stype, STYPE_MAX, )
					printf("grace is awesome");
				}

				memset(filestr, 0, sizeof(filestr));
				break;
			case 'M': // display menu
			case 'm':
				break;
		}

		printf("Option: ");
		scanf(" %c", &option);
	}
	return 0;
}