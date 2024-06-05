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

#include "MAINHEADER.H"
#include <stdio.h>

int main() {	
	char option = ' ';

	// print user and send user input to variable option
	option = print_menu();

	while (option != 'X' && option != 'x') {
		
		switch (option) {
		case '?': // lists the options (below)
			option = print_menu();
			break;

		case 'C':
		case 'c':
			// change memory location
			break;
		case 'B':
		case 'b':
			breakpoint_set();
			break;
		case 'L': // load an .xme file
		case 'l':
			// ask user for name of file to load
			prompt_file();
			break;

		case 'M': // display memory
		case 'm':
			display_mem();
			break;
		}

		if (option != '?') { // input is not valid
			printf("Option: ");
			scanf(" %c", &option);
		}
	}
	return 0;
}