/* 	autor: Radek Wildmann
*	login: xwildm00
* 	
*	Vytvoreno jako soucast projektu do predmetu IFJ v roce 2017
*/

#ifndef STRINGH 	// obaleni pro vicenasobny preklad
#define STRINGH 	// definujeme hlavickovy soubor pro praci s nekonecnym stringem

#include "includes.h"

typedef struct {
	char* data;		// retezec znaku  
	int size;		// velikost retezce
	int capacity;	// velikost alokovane casti (pouzita pamet)
} String;

int stringInit(String* s);
void stringFree(String *s);
void stringClear(String *s);
int stringAddData(String *s1, char c);
int stringCopyToString(String *s1, String *s2);
int stringCmpString(String *s1, String *s2);
int stringCmpConstString(String *s1, char *s2);
char *stringGetString(String *s);
int stringGetSize(String *s);
void stringToChar(char **c, String *s);

#endif // #ifndef STRINGH