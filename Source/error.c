/*
 * IFJ 2017 project
 *	File			error.c
 *	Description		Error handle, free memory and exit program
 *	Author			Michal Zilvar (xzilva02)
 *	Last update		12:17, 29-10-2017
 */

#ifndef ERRORC
#define ERRORC

#include "includes.h"

void error(int e) {
	//clearAllStrings
	//clearTrees
	exit(e);
}

#endif