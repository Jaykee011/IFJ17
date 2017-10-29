/*
 * IFJ 2017 project
 *	File			instruction.h
 *	Description		Header file for instruction.c
 *	Author			Michal Zilvar (xzilva02)
 *	Last update		16:54, 29-10-2017
 */

#ifndef INSTRUCTIONH
#define INSTRUCTIONH

int openOutput(char *s);	// Set file to output
void instruction(char *s);	// Add instruction
void closeOutput();			// Close output file

#endif