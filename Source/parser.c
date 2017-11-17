/*
 * IFJ 2017 project
 *	File			parser.c
 *	Description		Source file for syntax analysis
 *	Author			Radek Wildmann (xwildm00), Jakub Zapletal (xzaple36)
 */
#include "includes.h"

int token = 0;
String attribute;
char id[64] = "";
char functId[64] = "";
int type = 0; // 1 => int; 2 => double; 3 => string; 

//precedence table
//COL	- INPUT CHAR
//ROW	- TOP TERMINAL ON STACK
int precTable[15][15] = {
	//		 ID		(	)	+	-	*	/	\	<	>  <=  >=  ==  !=	$
	/* ID */{ E,	E,	R,	R,	R,	R,	R,	R,	R,	R,	R,	R,	R,	R,	R },
	/*  ( */{ L,	L,	EQ,	L,	L,	L,	L,	L,	L,	L,	L,	L,	L,	L,	E },
	/*  ) */{ E,	E,	R,	R,	R,	R,	R,	R,	R,	R,	R,	R,	R,	R,	R },
	/*  + */{ L,	L,	R,	R,	R,	L,	L,	L,	R,	R,	R,	R,	R,	R,	R },
	/*  - */{ L,	L,	R,	R,	R,	L,	L,	L,	R,	R,	R,	R,	R,	R,	R },
	/*  * */{ L,	L,	R,	R,	R,	R,	R,	R,	R,	R,	R,	R,	R,	R,	R },
	/*  / */{ L,	L,	R,	R,	R,	R,	R,	R,	R,	R,	R,	R,	R,	R,	R },
	/*  \ */{ L,	L,	R,	R,	R,	L,	L,	R,	R,	R,	R,	R,	R,	R,	R },
	/*  < */{ L,	L,	R,	L,	L,	L,	L,	L,	E,	E,	E,	E,	E,	E,	R },
	/*  > */{ L,	L,	R,	L,	L,	L,	L,	L,	E,	E,	E,	E,	E,	E,	R },
	/* <= */{ L,	L,	R,	L,	L,	L,	L,	L,	E,	E,	E,	E,	E,	E,	R },
	/* >= */{ L,	L,	R,	L,	L,	L,	L,	L,	E,	E,	E,	E,	E,	E,	R },
	/*  = */{ L,	L,	R,	L,	L,	L,	L,	L,	E,	E,	E,	E,	E,	E,	R },
	/* <> */{ L,	L,	R,	L,	L,	L,	L,	L,	E,	E,	E,	E,	E,	E,	R },
	/*  $ */{ L,	L,	E,	L,	L,	L,	L,	L,	L,	L,	L,	L,	L,	L,	E }
};

int stackInit(tStack *stack) { // stack inicialization

	if (!stack)
		return INTERN_ERR;
	stack->top = -1;
	return FINE;
}

int push(tStack *stack, tokenparam token) { // pushes new terminal to stack
	
	if (stack->top == STACK_SIZE) // if stack is full
		return INTERN_ERR;
	stack->top++;
	stack->arr[stack->top].token = token.token;
	return FINE;
}
int pop(tStack *stack, tokenparam *token) { // pops top token
	
	if (stack->top == -1)
		return INTERN_ERR;
	*token = stack->arr[stack->top];
	stack->top--;
	return FINE;
}

void replaceY(tStack *stack, char a){ // <y za A
	tokenparam buffer[100];
	int i;
	tokenparam NONTtoken;
	NONTtoken.token = NONT;
	for (i = 0; top(stack) != LB; i++){
		pop(stack, &buffer[i]);
	}
	pop(stack, &buffer[i]);
	push(stack, NONTtoken);
	// i = i - 1; // dekrementation after for cycle
}

void changeA(tStack *stack, char a){ // a za a<
	tokenparam buffer[100];
	int i;
	tokenparam LBtoken;
	LBtoken.token = LB;
	for (i = 0; top(stack) != a; i++){
		pop (stack, &buffer[i]);
	}
	push(stack, LBtoken);
	i = i - 1; // dekrementation after for cycle
	for (; i >= 0; i--){
		push(stack, buffer[i]);
	}
}

int top(tStack *stack){
	if (stack->top == -1)
		return INTERN_ERR;
	return stack->arr[stack->top].token;
}

char topTerm(tStack *stack) { // returns terminal closes to the top of the stack

	if (stack->top == -1)
		return INTERN_ERR;
	for (int i = stack->top; i >= 0; i--)
		if (stack->arr[i].token != 15 && stack->arr[i].token != 16 && stack->arr[i].token != 17)
		{
			return stack->arr[i].token;
		}
	return INTERN_ERR;
}



int precedenceTokenConversion(char token, tokenparam *converted) //converts token code to code which can be used as an index in precedence table
{
	switch (token)
	{
	case T_ID:
		converted->token = PRID;
		return FINE;
	
	/*case '1':
		converted->token = PRID;
		return FINE;
	case '2':
		converted->token = PRID;
		return FINE;
	case '3':
		converted->token = PRID;
		return FINE;*/
	
	case T_LB:
		converted->token = PRLB;
		return FINE;

	/*case '(':
		converted->token = PRLB;
		return FINE;*/

	case T_RB:
		converted->token = PRRB;
		return FINE;

	/*case ')':
		converted->token = PRRB;
		return FINE;*/

	case T_ADD:
		converted->token = PRPLUS;
		return FINE;

	/*case '+':
		converted->token = PRPLUS;
		return FINE;*/

	case T_SUB:
		converted->token = PRMINUS;
		return FINE;

	/*case '-':
		converted->token = PRMINUS;
		return FINE;*/

	case T_TIMES:
		converted->token = PRTIMES;
		return FINE;

	/*case '*':
		converted->token = PRTIMES;
		return FINE;*/

	case T_DIV:
		converted->token = PRDIV;
		return FINE;

	/*case '/':
		converted->token = PRDIV;
		return FINE;*/

	case T_IDIV:
		converted->token = PRIDIV;
		return FINE;

	/*case '\\':
		converted->token = PRDIV;
		return FINE;*/

	case T_LT:
		converted->token = PRLT;
		return FINE;

	/*case '<':
		converted->token = PRLT;
		return FINE;*/

	case T_GT:
		converted->token = PRGT;
		return FINE;

	/*case '>':
		converted->token = PRGT;
		return FINE;*/

	case T_GTE:
		converted->token = PREGT;
		return FINE;

	/*case '´':
		converted->token = PREGT;
		return FINE;*/

	case T_LTE:
		converted->token = PRELT;
		return FINE;

	/*case '¨':
		converted->token = PRELT;
		return FINE;*/

	case T_EQ:
		converted->token = PREQUAL;
		return FINE;

	/*case '=':
		converted->token = PREQUAL;
		return FINE;*/

	case T_NEQ:
		converted->token = PRNEQ;
		return FINE;

	/*case '.':
		converted->token = PRNEQ;
		return FINE;*/

	default:
		converted->token = PREND;
		return FINE;
	}
}

//char str[50] = "(1+1)*2/3\\3-8=2.2¨7´8";

/*int myGetToken(int i){
	int a = str[i];
	return a;
}*/

int precedence_analysis(){
	tStack stack;	
	stackInit(&stack);
	tokenparam firstToken, b;
	firstToken.token = PREND;
	push(&stack, firstToken);
	char a = PREND;
	//int i = 0;
	do { 
		precedenceTokenConversion(/*TODO: getToken(), FIXME:*/1, &b);
		switch (precTable[a][b.token]){
			case EQ:
				push(&stack, b);
				break;
			case L:
				changeA(&stack, a);
				push(&stack, b);
				break;
			case R:
				replaceY(&stack, a);
				//i--;
				break;
			case E:
			default:
				return INTERN_ERR;
			}
	//i++;
	a = topTerm(&stack);
} while (((b.token) != PREND) || (a != PREND));
	//printf("boobs\n");
}
/*int main(){
	precedence_analysis();
	printf("úspěch syntaktické analýzy\n");
}*/


void getNCheckToken(String *s, int t){
	// TODO: getToken(s);
	if (token != t){
		// TODO: handleError;
	}
}

bool parse(){
	//TODO: getToken(*attribute);
	switch(token){
		case T_DECLARE:
		case T_FUNCTION:
			functionsState();
			getNCheckToken(&attribute, T_SCOPE);
		case T_SCOPE:
			scopeState();
			break;
		//TODO: default:
			//TODO:  errorHandle
		}
	//TODO: getToken(*attribute);
	
	return (token == T_EOF);
}

void functionsState(){
	bool loop = true;
	while(loop){
		switch(token){
			case T_DECLARE:
			case T_FUNCTION:
				functionState();
				break;
			default:
				//FIXME: 
				loop = false;
				break;
		}
	}
}

void functionState(){
	switch(token){
		case T_DECLARE:
			getNCheckToken(&attribute, T_FUNCTION);
			getNCheckToken(&attribute, T_ID);
			//TODO: declareFunction without params;
			getNCheckToken(&attribute, T_LB);
			paramsState();
			getNCheckToken(&attribute, T_AS);
			//TODO: getToken(&attribute);
			typeState();
			getNCheckToken(&attribute, T_EOL);
			break;
		case T_FUNCTION:
			getNCheckToken(&attribute, T_ID);
			//TODO: defineFunction without params;
			getNCheckToken(&attribute, T_LB);
			paramsState();
			getNCheckToken(&attribute, T_AS);
			//TODO: getToken(&attribute);
			typeState();
			fcommandsState();
			getNCheckToken(&attribute, T_FUNCTION);
			getNCheckToken(&attribute, T_EOL);
			//FIXME: 	
			break;
		default:
			//FIXME:
			break;
	}
}

void scopeState(){
	getNCheckToken(&attribute, T_EOL);
	scommandsState();
	getNCheckToken(&attribute, T_SCOPE);
}

void paramsState(){
	//TODO: getToken(&attribute);
	switch(token){
		case T_ID:
			paramState();
			//TODO: add param
			nparamState();
			break;
		case T_RB:
			break;
		default:
			//TODO: errorHandle
			break;
	}
}

void paramState(){
	strcpy(id,attribute.data);
	getNCheckToken(&attribute, T_AS);
	//TODO: getToken(&attribute);
	typeState();
}

void nparamState(){
	bool loop = true;
	while (loop){
		//TODO: getToken(&attribute);
		switch(token){
			case T_COMMA:
				getNCheckToken(&attribute, T_ID);
				paramState();
				//TODO: add param
				break;
			case T_RB:
				loop = false;
				break;
			//TODO: default:
				//TODO: errorhandle
		}
	}
}

void fcommandsState(){
	bool loop = true;
	while(loop){
		//TODO: getToken(&attribute);
		switch (token){
			case T_RETURN:
				fcommandState();
				break;
			case T_DIM:
				scommandState();
				break;
			case T_ID:
			case T_INPUT:
			case T_PRINT:
			case T_IF:
			case T_DO:
				commandState();
				break;
			case T_END:
				loop = false;
				break;
			//TODO: default:
				//TODO: errorHandle
		}
	}
}

void fcommandState(){
	//TODO: getToken(&attribute);
	if (token == T_ID && token != L_INT && token != L_STRING && token != L_FLOAT){
		//TODO: errorHandle
	}
	//FIXME: 
	pushbackAttr(attribute.size);
	//TODO: precedenceAnalysis
	getNCheckToken(&attribute, T_EOL);
}

void scommandsState(){
	bool loop = true;
	while(loop){
		//TODO: getToken(&attribute);
		switch (token){
			case T_DIM:
				scommandState();
				break;
			case T_ID:
			case T_INPUT:
			case T_PRINT:
			case T_IF:
			case T_DO:
				commandState();
				break;
			case T_END:
				loop = false;
				break;
			//TODO: default:
				//TODO: errorHandle
		}
	}
}

void scommandState(){
	vardefState();
}

void vardefState(){
	//TODO: storeVariable
	getNCheckToken(&attribute, T_ID);
	getNCheckToken(&attribute, T_AS);
	//TODO: getToken(&attribute);
	typeState();
	definitState();
}

void definitState(){
	//TODO: getToken(&attribute);
	switch(token){
		case T_EQ:
			initState();
			break;
		case T_EOL:
			break;
		//TODO: default:
			//TODO: errorhandle
	}
}

void initState(){
	String storedAttr;
	//TODO: getToken(&storeAttr);
	switch(token){
		case T_ID:
			//TODO: getToken(&attribute);
			if (token == T_LB){
				fcallState();
				break;
			}
			else{
				//FIXME: 
				pushbackAttr(attribute.size);
			}
		case L_INT:
		case L_FLOAT:
		case L_STRING:
			//FIXME: 
			pushbackAttr(storedAttr.size);
			//TODO: precedenceAnalysis
			//TODO: initialize variable
			break;
		//TODO: default:
			//TODO: errorHandle
	}
}

void fcallState(){
	cparamsState();
}

void cparamsState(){
	//TODO: param control
	//TODO: getToken(&attribute);
	switch(token){
		case T_RB:
			break;
		default:
			cparamState();
			ncparamState();
			break;
	}
}

void cparamState(){
	termState();
}

void ncparamState(){
	bool loop = true;
	while(loop){
		//TODO: getToken(&attribute);
		switch(token){
			case T_COMMA:
				cparamState();
				ncparamState();
				break;
			case T_RB:
				loop = false;
				break;
			//TODO: default:
				//TODO: errorhandle
		}
	}
}

void commandsState(int finalizingToken){
	bool loop = true;
	while(loop){
		//TODO: getToken(&attribute);
		switch(token){
			case T_ID:
			case T_INPUT:
			case T_PRINT:
			case T_IF:
			case T_DO:
				commandState();
				break;
			default:
				if (token == finalizingToken){
					loop = false;
					break;
				}
				else{
					//TODO: errorhandle 
				}
		}
	}
}

void commandState(){
	switch(token){
		case T_ID:
			getNCheckToken(&attribute, T_EQ);
			initState();
			break;
		case T_INPUT:
			inputState();
			break;
		case T_PRINT:
			printState();
			break;
		case T_IF:
			branchState();
			break;
		case T_DO:
			loopState();
			break;
		//TODO: default:
			//TODO: errorHandle
	}
	getNCheckToken(&attribute, T_EOL);
}

void inputState(){
	getNCheckToken(&attribute, T_ID);
	//TODO: input gen
}

void printState(){
	//TODO: print gen
	if (token == T_ID && token != L_INT && token != L_STRING && token != L_FLOAT){
		//TODO: errorHandle
	}
	//FIXME: 
	pushbackAttr(attribute.size);
	//TODO: precedenceAnalysis
	getNCheckToken(&attribute, T_SEMICOLON);
	nexprState();
}

void nexprState(){
	bool loop = true;
	while(loop){
		//TODO: getToken(&attribute);
		switch(token){
			case T_ID:
			case L_INT:
			case L_FLOAT:
			case L_STRING:
				//FIXME: 
				pushbackAttr(attribute.size);
				//TODO: precedenceAnalysis
				getNCheckToken(&attribute, T_SEMICOLON);	
				break;			
			case T_EOL:
				pushbackAttr(attribute.size);
				loop = false;
				break;
			//TODO: default:
				//TODO: errorHandle
		}
	}
}

void branchState(){
	//TODO: if gen
	if (token == T_ID && token != L_INT && token != L_STRING && token != L_FLOAT){
		//TODO: errorHandle
	}
	//FIXME: 
	pushbackAttr(attribute.size);
	//TODO: precedenceAnalysis
	getNCheckToken(&attribute, T_THEN);
	getNCheckToken(&attribute, T_EOL);
	commandsState(T_ELSE);
	getNCheckToken(&attribute, T_EOL);
	commandsState(T_END);
	getNCheckToken(&attribute, T_IF);
}

void loopState(){
	//TODO: while gen
	getNCheckToken(&attribute, T_WHILE);
	if (token == T_ID && token != L_INT && token != L_STRING && token != L_FLOAT){
		//TODO: errorHandle
	}
	//FIXME: 
	pushbackAttr(attribute.size);
	//TODO: precedenceAnalysis
	getNCheckToken(&attribute, T_EOL);
	commandsState(T_LOOP);
}

void typeState(){
	switch(token){
		case T_INTEGER:
			type = 1;
			break;
		case T_DOUBLE:
			type = 2;
			break;
		case T_STRING:
			type = 3;
			break;
		//TODO: default:
			//TODO: errorHandle
	}
}

void termState(){
	switch(token){
		case L_INT:
			//TODO: 
			break;
		case L_FLOAT:
			//TODO: 
			break;
		case L_STRING:
			//TODO: 
			break;
		case T_ID:
			//TODO: 
			break;
		//TODO: default:
			//TODO: errorHandle
	}
}