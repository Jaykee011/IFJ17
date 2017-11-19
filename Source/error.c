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

SPointer *pClearList = NULL;

void error(int e) {
	pointerClear();		// Free all memory
	closeOutput();	// Close output file
	closeInput();	// Close input file
	exit(e);
}

void *saveMalloc(size_t size) {
	void *tmp = malloc(size);
	if(pointerAdd(tmp)) return NULL;

	return tmp;
}

void *saveRealloc(void *s, size_t size) {

	char *temp = saveMalloc(size);
	if(pointerAdd(temp)) return NULL;

	strcpy(temp, s);
	saveFree(s);

	return temp;
}

int pointerAdd(void *s) { // Add new pointer
	if(s == NULL) return FINE;

	if(pClearList == NULL) {
		pClearList = malloc(sizeof(SPointer));
		if(pClearList == NULL) return INTERN_ERR;

		pClearList->p = s;
		pClearList->next = NULL;
		return FINE;
	}

	SPointer *p = pClearList;
	SPointer *prev = NULL;

	for(; p != NULL; p = p->next) {
		if(p->p == s) {
			$(Already inserted!);
			return FINE;
		}
		prev = p;
	}

	prev->next = malloc(sizeof(SPointer));
	if(prev->next == NULL) return INTERN_ERR;
	prev->next->p = s;
	prev->next->next = NULL;

	return FINE;
}

void saveFree(void *s) { // Removes a pointer
	if(pClearList == NULL) return; // No allocated

	SPointer *p = pClearList;
	if(p->p == s) { // First in list
		pClearList = p->next;

		free(p->p);
		free(p);
		return;
	}

	SPointer *prev = NULL;
	for(; p != NULL; p = p->next) {

		if(p->p == s) {
			prev->next = p->next;
			free(p->p);
			free(p);
			break;
		}
		prev = p;
	}
}

void pointerClear() { // Free all memory
	if(pClearList == NULL) return;

	SPointer *tmp;
	for(SPointer *p = pClearList; p != NULL; p = tmp) {
		tmp = p->next;
		free(p->p);
		free(p);
	}

	pClearList = NULL;
}

#endif