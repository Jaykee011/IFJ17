/*
 * IFJ 2017 project
 *	File			scanner.h
 *	Description		Header file for scanner.c
 *	Author			Michal Zilvar (xzilva02)
 *	Last update		10:00, 06-10-2017
 */

#ifndef SCANNERH
#define SCANNERH

#define LEX_WAITING		0
#define LEX_DIGIT		1
#define LEX_FLOAT		2
#define LEX_KEYWORD		3
#define LEX_NOT			4
#define LEX_EQUAL		5
#define LEX_GREATER		6
#define LEX_SMALLER		7
#define LEX_STRING		8
#define LEX_STRINGE		9
#define LEX_EFLOATC		10
#define LEX_EFLOATF		11
#define LEX_EFLOAT		12
#define LEX_FLOATF		13
#define LEX_COMMENT		14

#include <stdio.h>
#include <ctype.h>
#include "define.h"
/*#include "string.h"*/

void setFile(FILE *f);
int scannerFailed();
/*int getToken(String *s);*/
int getToken(char *s);

#endif