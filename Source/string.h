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

int stringInit(string* s);
void stringFree(string *s);
void stringClear(string *s);
int stringAddData(string *s1, char c);
int stringCopyToString(string *s1, string *s2);
int stringCmpString(string *s1, string *s2);
int stringCmpConstString(string *s1, char *s2);
char *stringGetString(string *s);
int stringGetSize(string *s);
void stringToChar(char **c, string *s);

#endif // #ifndef STRINGH