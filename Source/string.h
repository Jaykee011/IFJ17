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

typedef struct stringPointer {
	String *p;					// Pointer to string
	struct stringPointer *next;	// Pointer to next string
} SPointer;

int stringInit(String** s);
void stringFree(String *s);
int stringClear(String *s);
int stringAddData(String *s1, char c);
void makeCharLowerCase(char* s);
char *stringGetString(String *s);
int stringGetSize(String *s);
void stringToChar(char **c, String *s);

int sClearAdd(String *s);	// Add new pointer to a string
void sClearRem(String *s);	// Removes a pointer
void sClear();				// Free all memory

#endif // #ifndef STRINGH