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

#define SREC_MAX 67 // defined macros for records
#define BYTE 2
#define LABEL_MAX 64

#define BYTEMEMSIZE (1 << 16) // 65,546 //defined macros for byte/word memory size
#define WORDMEMSIZE (1 << 15) // 32,768

// union for byte/word memory size
typedef union memory {
	unsigned char byte_mem[BYTEMEMSIZE];
	unsigned short word_mem[WORDMEMSIZE];
}memory;

typedef enum srecord { // enumerator of s-record types
	S0,
	S1,
	S2,
	S9
} srecordtype;

char print_menu();

enum srecordtype str_to_int(char* stype);

int read_s_record(const char* srecord, srecordtype* srectype, int reccount);

int read_record_len(const char* srecord, int* reclength, int reccount, int* checksumcount);

int read_address(const char* srecord, int* address, int reccount, int* checksumcount);

int load_file(FILE* file);

void file_found_print(const char* filename, int startaddress);

void prompt_file();

#endif //LOADERHEADER_H
