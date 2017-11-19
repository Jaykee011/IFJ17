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
	if(pointerAdd(tmp) != FINE) return NULL;

	return tmp;
}

void *saveRealloc(void *s, size_t size) {
	void *tmp = realloc(s, size);
	if(pointerAdd(tmp) != FINE) return NULL;

	return tmp;
}

int pointerAdd(void *s) { // Add new pointer
	if(s == NULL) return FINE;

	if(pClearList == NULL) {
		pClearList = malloc(sizeof(SPointer));
		pClearList->p = NULL;
		pClearList->next = NULL;
	}
	if(pClearList == NULL) return INTERN_ERR;

	SPointer *p;
	for(p = pClearList; p->next != NULL; p = p->next) {
		if(p->p == s) return FINE;
	}

	p->p = s;
	p->next = malloc(sizeof(SPointer));
	if(p->next == NULL) return INTERN_ERR;
	p->next->p = NULL;
	p->next->next = NULL;

	return FINE;
}

void saveFree(void *s) { // Removes a pointer
	if(pClearList == NULL) return; // No allocated

	SPointer *p = pClearList;
	if(p->p == s) { // First in list
		pClearList = p->next;
		free(p->p);
		free(p);
	} else {
		if(p->next == NULL) {
			if(p->p == s) {
				free(p->p);
				free(p);
				pClearList = NULL;
			}
			return;
		}

		for(; p->next != NULL; p = p->next) {
			if(p->next->p == s) {
				SPointer *tmp = p->next;
				p->next = tmp->next;
				free(tmp->p);
				free(tmp);
				break;
			}
		}
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