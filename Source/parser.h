/*
 * IFJ 2017 project
 *	File			parser.h
 *	Description		Source file for syntax analysis
 *	Author			Radek Wildmann (xwildm00), Jakub Zapletal (xzaple36)
 */
#ifndef PRECEDENCEH 	// obaleni pro vicenasobny preklad
#define PRECEDENCEH


#include "includes.h"

#define	SERR		-2 //Stack error

#define	EVALS		40	//<

//Precedence table constants
#define	L			1 //<
#define	R			2 //>
#define	EQ  		3 //=
#define	E   		0 //blank

//Defines for precedence analysis (numbers correspond to precedence table)
#define PRID	 0
#define	PRLB	 1
#define PRRB	 2
#define	PRPLUS	 3
#define	PRMINUS	 4
#define	PRTIMES	 5
#define	PRDIV	 6
#define	PRIDIV   7
#define	PRLT	 8
#define	PRGT	 9
#define	PRELT	 10
#define	PREGT	 11
#define	PREQUAL	 12
#define	PRNEQ	 13
#define	PREND	 14

//Defines E,<,>
#define	NONT	15
#define	LB	    16
#define	RB	    17

//Maximum size of precedence analysis stack
#define STACK_SIZE	100

typedef struct TokenParam {
    char token;
	String *data;
	int type;
	bool identifier;
} tokenparam;

//stack struct
typedef	struct precedenceStack {
	tokenparam arr[STACK_SIZE];
	int top;
} tStack;

int stackInit(tStack *stack); // stack inicialization
int push(tStack *stack, tokenparam token); // pushes new terminal to stack    
int pop(tStack *stack, tokenparam *token); // pops top token
void replaceY(tStack *stack, char a); // <y za A
void changeA(tStack *stack, char a); // a za a<
int top(tStack *stack);
char topTerm(tStack *stack); // returns terminal closes to the top of the stack
void precedence_analysis(bool); //precedence analysis of expression
int precedenceTokenConversion(char token, tokenparam *converted); //converts token code to code which can be used as an index in precedence table
void testTokens(bool);


bool parse();
void functionsState();      
void functionState();
void scopeState();
void paramsState(bool);
void paramState(bool);
void nparamState(bool);
void fcommandsState();
void fcommandState();
void scommandState();
void scommandsState();
void commandsState(int);
void commandState();
void vardefState();
void definitState();
void initState();
void fcallState();
void cparamsState();
void ncparamState(param *);
void cparamState(param *);
void inputState();
void printState();
void nexprState();
void branchState();
void loopState();
void typeState();
void termState();

#endif