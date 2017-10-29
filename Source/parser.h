//Authors xzaple36 (Jakub Zapletal), xzilva02 (Michal Zilvar)
#ifndef PRECEDENCEH 	// obaleni pro vicenasobny preklad
#define PRECEDENCEH


#include "define.h"
#include "string.h"
#include "scanner.h"
// #include "semantic.h"
// #include "interpreter.h"
// #include "instruction_list.h"

#define	SERR		-2 //Stack error

#define	EVALS		40	//<

//Precedence table constants
#define	L			1 //<
#define	R			2 //>
#define	EQ  		3 //=
#define	E   		0 //blank

//Defines for precedence analysis (numbers correspond to precedence table)
#define PRID	0
#define	PRLB	1
#define PRRB	2
#define	PRPLUS	3
#define	PRMINUS	4
#define	PRTIMES	5
#define	PRDIV	6
#define	PRIDIV  7
#define	PRLT	8
#define	PRGT	9
#define	PRELT	10
#define	PREGT	11
#define	PREQUAL	12
#define	PRNEQ	13
#define	PREND	14

//Defines E,<,>
#define	NONT	15
#define	LB	    16
#define	RB	    17

//Maximum size of precedence analysis stack
#define STACK_SIZE	100


typedef struct TokenParam {
    char token;
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
int precedence_analysis(); //precedence analysis of expression
int precedenceTokenConversion(char token, tokenparam *converted); //converts token code to code which can be used as an index in precedence table

#endif