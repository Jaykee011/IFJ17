/*
 * IFJ 2017 project
 *	File			error.c
 *	Description		Handler for errors and error messages
 *	Author			Michal Zilvar (xzilva02)
 *	Last update		11:45, 06-10-2017
 */

#ifndef SCANNERC
#define SCANNERC

#include "includes.h"

void error(int e) {
	//clear();
	exit(e);
}

#endif