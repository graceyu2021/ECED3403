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


#include <stdio.h>

int main() {
	char option = " ";
	// print menu
	printf("L - load\nM - display mem\nX - exit\nOption: ");

	while (1) {
		if (option == "L" || option == "l") {
			// ask user for new file to load
		}
		else if (option == "M" || option == "m") {
			// print
		}
		else if (option == "X" || option == "x") {
			return 0;
		}
	}
	return 0;
}