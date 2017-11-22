/*
 * IFJ 2017 project
 *  File            symtable.h
 *  Description     header file for binary search tree
 *  Author          Martin Studeny (xstude23)
 */
#ifndef SYMTABLEH   // obaleni pro vicenasobny preklad
#define SYMTABLEH 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <malloc.h>
#include <string.h>
#include "define.h"
#include "macros.h"
#include "string.h"
#include "error.h"

typedef struct parameters *param;
typedef struct node *nodePtr;

typedef struct defParam {
    char id[64];
    int type;
} parStruct;

typedef struct value {
    int i;
    double d;
    String *s;
    bool b;
} val;

struct parameters{
    char name[64];
    int type; // see description in struct load
    param next;
};

typedef struct {
    param parameters; // linear list of function parametes
    param declaredParameters;
    nodePtr functTable;  // function local symbol table
    bool hasReturn;
    bool declared;
} functInfo;

typedef struct load {
    bool defined; // TODO: -automatiky na 1
    bool initialized;
    int type; // 1 => int; 2 => double; 3 => string; 
    int metaType; // 1 => variable; 2 => function;
    val value;
    functInfo function;
} *loadPtr;

struct node {
    char key[64];
    struct node *lPtr;
    struct node *rPtr;
    loadPtr symbol;
};

void treeInit(nodePtr*);
void nodeInsert(nodePtr*, loadPtr, char*);
void nodeDelete(nodePtr*, char*);
void treeDispose (nodePtr*);
nodePtr nodeSearch(nodePtr, char*);
void generateKey(char *symbolName, int metaType);
void insert_variable(nodePtr *, char *name);
void insert_type(nodePtr, char *, int);
void insert_value(nodePtr, char *name, int type, val data, int valueType);
void insert_function(nodePtr *, bool , char *);
void setFunctionDefined(nodePtr, char *);
void set_hasReturn(nodePtr, char *name);
void insert_param(nodePtr, char *name, char *, int, bool);
int validateDefinitionParameters(nodePtr , char *);
void validateFunctCall(nodePtr, nodePtr, char *, char *);
int validateCallParams(nodePtr , char *, param);
val getValue(nodePtr, char *);


#endif