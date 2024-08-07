/*
file: loaderheader.h
programmer: Grace Yu
b00: B00902046
class: ECED3403
professor: Dr. Larry Hughes

This is the header file of my program.
*/

#ifndef MAINHEADER_H
#define MAINHEADER_H

#include "LOADERHEADER.H"
#include "PIPELINEHEADER.H"
#include "DEBUGHEADER.H"
#include "PIPELINEEXECUTE_HEADER.H"
#include "BIT_MACRO.H"
#include "STRUCT_HEADER.H"

#include <stdio.h>

#define DEBUG 1

#define SREC_MAX 100//67 // defined macros for records

int clock;
int instructionbit;
int breakpoint;
int increment;

#endif //MAINHEADER_H