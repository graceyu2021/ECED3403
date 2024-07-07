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

#define BYTE 2
#define LABEL_MAX 64

#define TRUE 1
#define FALSE 0
#define HEXRANGE 16

#define CLOCK_INITIALIZE 0

#define BYTEMEMSIZE (1 << 16) // 65,546 //defined macros for byte/word memory size
#define WORDMEMSIZE (1 << 15) // 32,768

#define NULLCHAR 1

#define RECORD_BOUND(a, b) ((a) >= (BYTE * 4) && (a) < (2 * b + BYTE))

// union for byte/word memory size

typedef union memory {
	unsigned char byte_mem[BYTEMEMSIZE];
	unsigned short word_mem[WORDMEMSIZE];
}memory;

// global arrays for imem and dmem
//extern memory imem;
//extern memory dmem;

typedef enum srecord { // enumerator of s-record types
	S0,
	S1,
	S2,
	S9,
	INVALID_S
} srecordtype;

char print_menu();

enum srecordtype str_to_int(char snum);

int read_s_record(const char* srecord, srecordtype* srectype, int reccount);

int read_record_len(const char* srecord, int* reclength, int reccount, unsigned int* checksumcount);

int read_address(const char* srecord, int* address, int reccount, unsigned int* checksumcount);

int compare_checksum(const char* srecord, int reccount, unsigned int checksumcount);

int load_file(FILE* file);

void file_origin_print();

void prompt_file();

void idmem_print(memory array[], int* lowbound, int* lowboundtemp);

void display_mem();

void single_step();

#endif //LOADERHEADER_H