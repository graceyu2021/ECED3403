/*
file: debugheader.h
programmer: Grace Yu
b00: B00902046
class: ECED3403
professor: Dr. Larry Hughes
assignment: lab3
submission date: May 23rd, 2024

This is the debug header file of my program.
*/

#ifndef DEBUGHEADER_H
#define DEBUGHEADER_H

#include <stdio.h>

#define REG_NO 8
#define REG_NAME_MAX 10
#define INVALID_HEX(a) (a < 0x0000 || a > 0xFFFF)
#define HIGH 1
#define LOW 0

void mem_change();

void breakpoint_set();

void reg_display();

void reg_set();

void psw_display();

#endif //DEBUGHEADER_H