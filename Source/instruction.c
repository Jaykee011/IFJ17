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

/* TODO: */
void preparedInstructions() {
instruction("CREATEFRAME", NULL, NULL, NULL);
instruction("PUSHFRAME", NULL, NULL, NULL);
instruction("POPFRAME", NULL, NULL, NULL);
instruction("RETURN", NULL, NULL, NULL);
instruction("CLEARS", NULL, NULL, NULL);
instruction("ADDS", NULL, NULL, NULL);
instruction("SUBS", NULL, NULL, NULL);
instruction("MULS", NULL, NULL, NULL);
instruction("DIVS", NULL, NULL, NULL);
instruction("LTS", NULL, NULL, NULL);
instruction("GTS", NULL, NULL, NULL);
instruction("EQS", NULL, NULL, NULL);
instruction("BREAK", NULL, NULL, NULL);

instruction("DEFVAR", var, NULL, NULL);
instruction("CALL", label, NULL, NULL);
instruction("PUSHS", symbol, NULL, NULL);
instruction("POPS", var, NULL, NULL);

instruction("LABEL", label, NULL, NULL);
instruction("JUMP", label, NULL, NULL);

instruction("JUMPIFEQS", label, NULL, NULL);
instruction("JUMPIFNEQS", label, NULL, NULL);

instruction("DPRINT", symb, NULL, NULL);
instruction("WRITE", symb, NULL, NULL); // GF@%s

instruction("STRLEN", var, symb, NULL);

instruction("INT2FLOAT", var, symb, NULL);
instruction("FLOAT2INT", var, symb, NULL);
instruction("FLOAT2R2EINT", var, symb, NULL);
instruction("FLOAT2R2OINT", var, symb, NULL);
instruction("INT2CHAR", var, symb, NULL);

instruction("READ", var, type, NULL);
instruction("TYPE", var, symb, NULL);
instruction("MOVE", var, symb, NULL);

instruction("ADD", var, symb, symb);
instruction("SUB", var, symb, symb);
instruction("DIV", var, symb, symb);

instruction("LT", var, symb, symb);
instruction("GT", var, symb, symb);
instruction("EQ", var, symb, symb);

instruction("CONCAT", var, symb, symb);
instruction("GETCHAR", var, symb, symb);
instruction("SETCHAR", var, symb, symb);

instruction("JUMPIFEQ", label, symb, symb);
instruction("JUMPIFNEQ", label, symb, symb);

instruction("STRI2INT", var, symb, symb);
}

#endif