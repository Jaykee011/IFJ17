/*
 * IFJ 2017 project
 *	File			instruction.c
 *	Description		Handles output, instructions and output file
 *	Author			Michal Zilvar (xzilva02)
 *	Last update		16:57, 29-10-2017
 */

#ifndef INSTRUCTIONC
#define INSTRUCTIONC

#include "includes.h"

FILE *s_outputFile;

int openOutput(char *s) {
	s_outputFile = fopen(s, "w");
	if(!s_outputFile) return FAIL;

	char *x = ".IFJcode17\n";
	fwrite(x, sizeof(char), strlen(x), s_outputFile);
	return FINE;
}

void instruction(char *s) {
	fwrite(s, sizeof(char), strlen(s), s_outputFile);
	fwrite("\n", sizeof(char), 1, s_outputFile);
}

void closeOutput() {
	fclose(s_outputFile);
}

#endif