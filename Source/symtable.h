/*
 * IFJ 2017 project
 *	File			symtable.h
 *	Description		header file for binary search tree
 *	Author			Martin Studeny (xstude23)
 */
#ifndef SYMTABLEH 	// obaleni pro vicenasobny preklad
#define SYMTABLEH 

#include "includes.h"

typedef struct parameters *param;

typedef struct {
    int i;
    double d;
    char *s;
} val;

struct parameters{
    char *name;
    int type; // see description in struct load
    param next;
};

typedef struct {
    param parameters; // linear list of function parametes
    //nodePtr functTable;  // function local symbol table
    bool hasReturn = false;
    bool declared;
} functInfo;

typedef struct load {
    bool defined;
    int type; // 1 => int; 2 => double; 3 => string; 
    int metaType; // 1 => variable; 2 => function;
    val value;
    functInfo function;
} *loadPtr;

typedef struct node {
    char *key;
    struct node *lPtr;
    struct node *rPtr;
    loadPtr symbol;
} *nodePtr;

void treeInit(nodePtr*);
nodePtr nodeInsert(nodePtr*, loadPtr, char*);
void nodeDelete(nodePtr*, char*);
void treeDispose (nodePtr*);
nodePtr nodeSearch(nodePtr, char*, loadPtr*);
void generateKey(char *symbolName, int metaType);

#endif