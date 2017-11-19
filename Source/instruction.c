/*
 * IFJ 2017 project
 *	File			instruction.c
 *	Description		Handles output, instructions and output file
 *	Author			Michal Zilvar (xzilva02)
 *	Last update		18:00, 19-11-2017
 */

#ifndef INSTRUCTIONC
#define INSTRUCTIONC

#include "includes.h"

FILE *s_outputFile;

/* Open output file where will be the instructions */
int openOutput(char *s) {
	s_outputFile = fopen(s, "w");
	if(!s_outputFile) return FAIL;

	/* Every instruction list starts with this TAG */
	char *x = ".IFJcode17\n";
	fwrite(x, sizeof(char), strlen(x), s_outputFile);
	return FINE;
}

/* Universal function to add an instruction */
void instruction(char *s, char *a1, char *a2, char *a3) {
	
	// There is a possibility of infinite String in there
	String *format;
	stringInit(&format);
	stringClear(format);

	// Add instruction type
	concatToString(format, s);

	// Add first argument
	if(a1 != NULL) {
		stringAddData(format, ' ');
		concatToString(format, a1);
	}

	// Add second argument
	if(a2 != NULL) {
		stringAddData(format, ' ');
		concatToString(format, a2);
	}

	// Add thirth argument
	if(a3 != NULL) {
		stringAddData(format, ' ');
		concatToString(format, a3);
	}

	// Write the formated output to the file and LF
	fwrite(stringGetString(format), sizeof(char), strlen(stringGetString(format)), s_outputFile);
	fwrite("\n", sizeof(char), 1, s_outputFile);

	// Free some temporary memory
	stringFree(format);
}

/* While program exit we need to close all streams */
void closeOutput() {
	fclose(s_outputFile);
}

#endif