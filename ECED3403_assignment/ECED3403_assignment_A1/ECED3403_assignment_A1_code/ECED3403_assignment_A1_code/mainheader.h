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
#include <signal.h>

#define DEBUG 1

#define SREC_MAX 100//67 // defined macros for records
#define BP 4
#define LR 5
#define SP 6
#define PC 7

int clock;
int instructionbit;
int breakpoint;
int increment;
int bubble;
volatile sig_atomic_t ctrl_c_fnd; /* T|F - indicates whether ^C detected */

#endif //MAINHEADER_H