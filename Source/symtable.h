/*
 * IFJ 2017 project
 *	File			symtable.h
 *	Description		header file for binary search tree
 *	Author			Jakub Zapletal (xzaple36)
 */
#ifndef SYMTABLEH 	// obaleni pro vicenasobny preklad
#define SYMTABLEH 

typedef struct {
    int i;
    double d;
    char *s;
} val;

typedef struct parameters{
    char *name;
    int type; // see description in struct load
    param next;
} *param;

typedef struct {
    param parameters; // linear list of function parametes
    //nodePtr functTable;  // function local symbol table
} functInfo;

typedef struct load {
    bool declared;
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
nodePtr nodeSearch(nodePtr, char*);
char *generateKey(char *symbolName, int metaType);

#endif