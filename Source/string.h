/* 	autor: Radek Wildmann
*	login: xwildm00
* 	
*	Vytvoreno jako soucast projektu do predmetu IFJ v roce 2017
*/

#ifndef STRINGH 	// obaleni pro vicenasobny preklad
#define STRINGH 	// definujeme hlavickovy soubor pro praci s nekonecnym stringem

typedef struct {
	char* data;		// retezec znaku  
	int size;		// velikost retezce
	int capacity;	// velikost alokovane casti (pouzita pamet)
} String;

int stringInit(String** s);
void stringFree(String *s);
int stringClear(String *s);
int stringCpy(String *s, char* c);
int stringAddData(String *s1, char c);
void makeStringLowerCase(String *s);
int stringCmpConstString(String *s1, char *s2);
char *stringGetString(String *s);
int stringGetSize(String *s);
void stringToChar(char **c, String *s);

#endif // #ifndef STRINGH