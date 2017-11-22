/*
 * IFJ 2017 project
 *	File			instruction.h
 *	Description		Header file for instruction.c
 *	Author			Michal Zilvar (xzilva02)
 *	Last update		18:00, 19-11-2017
 */

#ifndef INSTRUCTIONH
#define INSTRUCTIONH

void instruction(char *s, String *a1, String *a2, String *a3); // Add instruction
int openOutput(char *s); // Set file to output
void closeOutput(); // Close output file

#endif