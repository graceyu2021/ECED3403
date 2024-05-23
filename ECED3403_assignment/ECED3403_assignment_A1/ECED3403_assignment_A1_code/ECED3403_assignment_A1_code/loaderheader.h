/*
file: loaderheader.h
programmer: Grace Yu
b00: B00902046
class: ECED3403
professor: Dr. Larry Hughes
assignment: A1
submission date: May 23rd, 2024

This is the header file of my program.
*/

#ifndef LOADERHEADER_H
#define LOADERHEADER_H

#include <stdio.h>

#define FILE_NAME_MAX 100
#define STYPE_MAX 3

enum srecord { // enumerator of s-record types
	S0,
	S1,
	S2,
	S9
};

char print_menu();

void prompt_file(char* filestr, int filelength, FILE* filename);

#endif //LOADERHEADER_H
