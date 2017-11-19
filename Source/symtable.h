/*
 * IFJ 2017 project
 *  File            symtable.h
 *  Description     header file for binary search tree
 *  Author          Martin Studeny (xstude23)
 */
#ifndef SYMTABLEH   // obaleni pro vicenasobny preklad
#define SYMTABLEH 

#include "includes.h"

typedef struct parameters *param;

typedef struct defParam {
    char id[64];
    int type;
} parStruct;

typedef struct value {
    int i;
    double d;
    String s;
} val;

struct parameters{
    char name[1000];
    int type; // see description in struct load
    param next;
};

typedef struct {
    param parameters; // linear list of function parametes
    //nodePtr functTable;  // function local symbol table
    bool hasReturn;
    bool declared;
} functInfo;

typedef struct load {
    bool defined; // TODO: -automatiky na 1
    int type; // 1 => int; 2 => double; 3 => string; 
    int metaType; // 1 => variable; 2 => function;
    val value;
    functInfo function;
} *loadPtr;

typedef struct node {
    char key[64];
    struct node *lPtr;
    struct node *rPtr;
    loadPtr symbol;
} *nodePtr;

void treeInit(nodePtr*);
void nodeInsert(nodePtr*, loadPtr, char*);
void nodeDelete(nodePtr*, char*);
void treeDispose (nodePtr*);
nodePtr nodeSearch(nodePtr, char*);
void generateKey(char *symbolName, int metaType);
int insert_variable(char *name, int type);
int insert_value(char *name, int type, void* pointer);
int insert_function(bool declared, char *name);
int insert_type_into_f(char *name, int type);
int set_hasReturn(char *name);
int insert_param(char *name, parStruct *par);


int insert_variable_int(char *name, int hodnota);
int insert_variable_double(char *name, double hodnota);
int insert_variable_string(char *name, char* hodnota);


#endif