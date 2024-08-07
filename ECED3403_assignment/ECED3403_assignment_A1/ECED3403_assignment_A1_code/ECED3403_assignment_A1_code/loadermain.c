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
	increment = FALSE;

	/* Call signal() - bind sigint_hdlr to SIGINT */
	ctrl_c_fnd = FALSE;
	signal(SIGINT, (_crt_signal_t)sigint_hdlr);

	// print user and send user input to variable option
	option = print_menu();


	while (option != 'X' && option != 'x') {

		switch (option) {
		case '?': // lists the options (below)
			option = print_menu();
			break;

		case 'C':
		case 'c':
			mem_change();
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

		case 'R': // display registers
		case 'r':
			reg_display();
			break;

		case 'S': // set register values
		case 's':
			reg_set();
			break;

		case 'M': // display memory
		case 'm':
			mem_display();
			break;
		case 'P': // display psw bits
		case 'p':
			psw_display();
			break;
		case 'G':
		case 'g':
			// pipeline
			pipeline();
			break;

		case 'I':
		case 'i':
			single_step();
		}

		if (option != ' ?') { // input is not ?, reset
			printf("Option: ");
			scanf(" %c", &option);
		}
	}
	return 0;
}