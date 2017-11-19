/*
 * IFJ 2017 project
 *	File			string.h
 *	Description		Header file for strings
 *	Author			Michal Zilvar (xzilva02)
 *	Last update		17:08, 19-11-2017
 */

#ifndef STRINGH 
#define STRINGH

#define STR_LEN_INC 8 // velikost (byty) na kolik se bude alokovat pocatecni pamet

typedef struct {
	char* data;
	int size;
	int capacity;
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