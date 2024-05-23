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

#define FILE_NAME_MAX 100 // defined macros for file

#define SREC_MAX 33 // defined macros for records
#define BYTE 2
#define LABEL_MAX 31

#define BYTEMEMSIZE (1 << 16) // 65,546 //defined macros for byte/word memory size
#define WORDMEMSIZE (1 << 15) // 32,768

// union for byte/word memory size
union memory {
	unsigned char byte_mem[BYTEMEMSIZE];
	unsigned short word_mem[WORDMEMSIZE];
};

// declared array for imem and dmem global
union memory imem; // instruction memory
union memory dmem; // data memory

typedef enum srecord { // enumerator of s-record types
	S0,
	S1,
	S2,
	S9
} srecordtype;

char print_menu();

int read_s_record_type(const char* srecord, int reccount, srecordtype* srectype);

int read_record_len(const char* srecord, int* reclength, int reccount);

int load_file(FILE* file);

void file_found_print(const char* filename, int startaddress);

void prompt_file();

#endif //LOADERHEADER_H
