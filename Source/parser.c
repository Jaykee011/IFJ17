/*
 * IFJ 2017 project
 *	File			parser.c
 *	Description		Source file for syntax analysis
 *	Author			Radek Wildmann (xwildm00), Jakub Zapletal (xzaple36)
 */
#include "includes.h"

int token = 0;
String *attribute = NULL;
String *variableName = NULL;
char id[64] = "";
char functId[64] = "";
int type = 0; // 1 => int; 2 => double; 3 => string;
tokenparam precedenceBuffer[100];
int tokenSize;

extern nodePtr symtable;

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

struct value evalExpr(void *lValue, void *mValue, void *rValue){
	
	struct value result;
	result.i = 0;
	result.d = 0.0;
	result.s = NULL;
	stringInit(&(result.s));

	if (mValue == NULL){
		//TODO: comment
		return ((tokenparam *)lValue)->data;
	}
	else{
		//TODO: zavorka
		switch(((tokenparam *)mValue)->token){
			case PRPLUS:
				//FIXME: posefit datove typy
				result.i = (((tokenparam *)lValue)->data.i + ((tokenparam *)rValue)->data.i) ;
				return result;
				break;
			default:
				//TODO: errorHandle
				break;
		}
	}
}

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
	stack->arr[stack->top].data = token.data;
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
	struct value result;
	if (i == 1)
		result = evalExpr(&buffer[0],NULL, NULL); //TODO: comment
	else
		result = evalExpr(&buffer[0],&buffer[1], &buffer[2]); //TODO: comment

	NONTtoken.data = result;
	pop(stack, &buffer[i]);
	push(stack, NONTtoken);//TODO: dat hodnotu z evalexpr
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
	case L_INT:
		converted->token = PRID;
		return FINE;
	case L_FLOAT:
		converted->token = PRID;
		return FINE;
	case L_STRING:
		converted->data.s = attribute;
		converted->token = PRID;
		return FINE;
	
	case T_LB:
		converted->token = PRLB;
		return FINE;

	case T_RB:
		converted->token = PRRB;
		return FINE;

	case T_ADD:
		converted->token = PRPLUS;
		return FINE;

	case T_SUB:
		converted->token = PRMINUS;
		return FINE;

	case T_TIMES:
		converted->token = PRTIMES;
		return FINE;

	case T_DIV:
		converted->token = PRDIV;
		return FINE;

	case T_IDIV:
		converted->token = PRIDIV;
		return FINE;

	case T_LT:
		converted->token = PRLT;
		return FINE;

	case T_GT:
		converted->token = PRGT;
		return FINE;

	case T_GTE:
		converted->token = PREGT;
		return FINE;

	case T_LTE:
		converted->token = PRELT;
		return FINE;

	case T_EQ:
		converted->token = PREQUAL;
		return FINE;

	case T_NEQ:
		converted->token = PRNEQ;
		return FINE;

	default:
		converted->token = PREND;
		return FINE;
	}
}

void testTokens(){ // imput token string control
	bool compareTokenExists = 0; // compare token exists
	bool previousNonId = 0; // previous non-ID
	int leftBracktCounter = 0; // left brackt counter
	for(int i = 0;(token = getToken(attribute,&tokenSize)) != T_EOL; i++){
		precedenceBuffer[i].token = token;
		switch(precedenceBuffer[i].token){
			case T_ID:
				precedenceBuffer[i].data = getValue(symtable, attribute->data);
				previousNonId =	0;
				break;		
			case L_INT:
				precedenceBuffer[i].data.i = stringToInt(attribute);
				previousNonId =	0;
				break;		
			case L_FLOAT:
				precedenceBuffer[i].data.d = stringToDouble(attribute);
				previousNonId =	0;
				break;		
			case L_STRING:
				stringInit(&precedenceBuffer[i].data.s);
				concatToString(precedenceBuffer[i].data.s, attribute->data);
				previousNonId =	0;
				break;					
			case T_LB: // (
				leftBracktCounter++;
				break;
			case T_RB: // )
				leftBracktCounter--;
				break;				
			case T_ADD: // +
			case T_SUB: // -
			case T_TIMES: // *
			case T_DIV: // /
			case T_IDIV: 
				if (previousNonId == 0){ // no previous non-ID
					previousNonId = 1;
				}
				else if (previousNonId == 1){
					printf("Dvě non-ID za sebou\n");
					exit(SYN_ERR); 
				}
				break;
			case T_LT:
			case T_GT:
			case T_GTE:
			case T_LTE:
			case T_EQ:
			case T_NEQ:	
				if(compareTokenExists == 0){
					compareTokenExists = 1;
					previousNonId = 0;
				}
				else{
					printf("Více porovnání, než může být\n");
					exit(SYN_ERR); 
				}
				break;
			default:
				exit(SYN_ERR);
				break;
		}
	}
}

struct value precedence_analysis(){
	testTokens();
	tStack stack;	
	stackInit(&stack);
	tokenparam firstToken, b;
	firstToken.token = PREND;
	push(&stack, firstToken);
	char a = PREND;
	int i = 0;
	do {
		precedenceTokenConversion(precedenceBuffer[i].token, &b); 
		b.data = precedenceBuffer[i].data;
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
				i--;
				break;
			case E:
			default:
				exit(INTERN_ERR);
		}
		i++;
		a = topTerm(&stack);
	} while (((b.token) != PREND) || (a != PREND));
	tokenparam result;
	pop(&stack, &result);
	return result.data;
}

void getNEOLToken(String *s, int *size){
	while(1){
		token = getToken(s,size);
		if (token == T_EOL){
			continue;
		}
		break;
	}
}

void getNCheckToken(String *s, int t){
	if (t == T_EOL)
		token = getToken(s,&tokenSize);
	else
		getNEOLToken(s, &tokenSize);

	if (token != t){
		// FIXME: handleError;
		printf("ERR\n"); exit(-1);
	}
}

bool parse(){
	stringInit(&attribute);
	stringInit(&variableName);
	getNEOLToken(attribute, &tokenSize);
	switch(token){
		case T_DECLARE:
		case T_FUNCTION:
			functionsState();
			getNCheckToken(attribute, T_SCOPE);
		case T_SCOPE:
			scopeState();
			break;
		default:
			// FIXME: handleError;
			printf("ERR\n"); exit(-1);
		}
	getNEOLToken(attribute, &tokenSize);
	
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
			getNCheckToken(attribute, T_FUNCTION);
			getNCheckToken(attribute, T_ID);
			//TODO: declareFunction without params;
			getNCheckToken(attribute, T_LB);
			paramsState();
			getNCheckToken(attribute, T_AS);
			getNEOLToken(attribute, &tokenSize);
			typeState();
			//FIXME: getNCheckToken(attribute, T_EOL);
			break;
		case T_FUNCTION:
			getNCheckToken(attribute, T_ID);
			//TODO: defineFunction without params;
			getNCheckToken(attribute, T_LB);
			paramsState();
			getNCheckToken(attribute, T_AS);
			getNEOLToken(attribute, &tokenSize);
			typeState();
			fcommandsState();
			getNCheckToken(attribute, T_FUNCTION);
			//FIXME: getNCheckToken(attribute, T_EOL);
			//FIXME: 	
			break;
		default:
			//FIXME:
			break;
	}
}

void scopeState(){
	//FIXME: getNCheckToken(attribute, T_EOL);
	scommandsState();
	getNCheckToken(attribute, T_SCOPE);
}

void paramsState(){
	getNEOLToken(attribute, &tokenSize);
	switch(token){
		case T_ID:
			paramState();
			//TODO: add param
			nparamState();
			break;
		case T_RB:
			break;
		default:
			// FIXME: handleError; 		
			printf("ERR\n"); exit(-1);
			break;
	}
}

void paramState(){
	strcpy(id,attribute->data);
	getNCheckToken(attribute, T_AS);
	getNEOLToken(attribute, &tokenSize);
	typeState();
}

void nparamState(){
	bool loop = true;
	while (loop){
		getNEOLToken(attribute, &tokenSize);
		switch(token){
			case T_COMMA:
				getNCheckToken(attribute, T_ID);
				paramState();
				//TODO: add param
				break;
			case T_RB:
				loop = false;
				break;
			default:
				// FIXME: handleError; 		
				printf("ERR\n"); exit(-1);
		}
	}
}

void fcommandsState(){
	bool loop = true;
	while(loop){
		getNEOLToken(attribute, &tokenSize);
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
			default:
				// FIXME: handleError; 		
				printf("ERR\n"); exit(-1);
		}
	}
}

void fcommandState(){
	getNEOLToken(attribute, &tokenSize);
	if (token == T_ID && token != L_INT && token != L_STRING && token != L_FLOAT){
		// FIXME: handleError; 		printf("ERR\n"); exit(-1);
	}
	//FIXME: 
	pushbackAttr(tokenSize);
	//TODO: precedenceAnalysis
	//FIXME: getNCheckToken(attribute, T_EOL);
}

void scommandsState(){
	bool loop = true;
	while(loop){
		getNEOLToken(attribute, &tokenSize);
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
			default:
				// FIXME: handleError; 		
				printf("ERR\n"); exit(-1);
		}
	}
}

void scommandState(){
	vardefState();
}

void vardefState(){
	//TODO: storeVariable
	getNCheckToken(variableName, T_ID);
	insert_variable(&symtable, variableName->data);
	getNCheckToken(attribute, T_AS);
	getNEOLToken(attribute, &tokenSize);
	typeState();
	insert_variable_type(symtable, variableName->data, type);
	definitState();
}

void definitState(){
	token = getToken(attribute, &tokenSize);
	switch(token){
		case T_EQ:
			initState();
			break;
		case T_EOL:
			break;
		default:
			// FIXME: handleError; 		
			printf("ERR\n"); exit(-1);
	}
}

void initState(){
	String *storedAttr = NULL;
	stringInit(&storedAttr);
	int storedSize;
	getNEOLToken(storedAttr, &storedSize);
	switch(token){
		case T_ID:
			getNEOLToken(attribute, &tokenSize);
			if (token == T_LB){
				fcallState();
				break;
			}
			else{
				//FIXME: 
				pushbackAttr(tokenSize);
			}
		case L_INT:
		case L_FLOAT:
		case L_STRING:
			//FIXME: 
			pushbackAttr(storedSize);
			insert_value(symtable, variableName->data, type, precedence_analysis());
			//TODO: initialize variable
			break;
		default:
			// FIXME: handleError; 		
			printf("ERR\n"); exit(-1);
	}
}

void fcallState(){
	cparamsState();
}

void cparamsState(){
	//TODO: param control
	getNEOLToken(attribute, &tokenSize);
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
		getNEOLToken(attribute, &tokenSize);
		switch(token){
			case T_COMMA:
				cparamState();
				ncparamState();
				break;
			case T_RB:
				loop = false;
				break;
			default:
				// FIXME: handleError; 		
				printf("ERR\n"); exit(-1);
		}
	}
}

void commandsState(int finalizingToken){
	bool loop = true;
	while(loop){
		getNEOLToken(attribute, &tokenSize);
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
					// FIXME: handleError; 		
					printf("ERR\n"); exit(-1); 
				}
		}
	}
}

void commandState(){
	switch(token){
		case T_ID:
			getNCheckToken(attribute, T_EQ);
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
		default:
			// FIXME: handleError; 		
			printf("ERR\n"); exit(-1);
	}
	//FIXME: getNCheckToken(attribute, T_EOL);
}

void inputState(){
	getNCheckToken(attribute, T_ID);
	//TODO: input gen
}

void printState(){
	//TODO: print gen
	getNEOLToken(attribute, &tokenSize);
	if (token != T_ID && token != L_INT && token != L_STRING && token != L_FLOAT){
		// FIXME: handleError; 		
		printf("ERR\n"); exit(-1);
	}
	//FIXME: 
	//pushbackAttr(tokenSize);
	//TODO: precedenceAnalysis
	getNCheckToken(attribute, T_SEMICOLON);
	nexprState();
}

void nexprState(){
	bool loop = true;
	while(loop){
		token = getToken(attribute, &tokenSize);
		switch(token){
			case T_ID:
			case L_INT:
			case L_FLOAT:
			case L_STRING:
				//FIXME: 
				//pushbackAttr(tokenSize);
				//TODO: precedenceAnalysis
				getNCheckToken(attribute, T_SEMICOLON);	
				break;			
			case T_EOL:
				pushbackAttr(tokenSize);
				loop = false;
				break;
			
			default:
				pushbackAttr(tokenSize);
				loop = false;
				break;
				// FIXME: handleError; 		
				printf("ERR\n"); exit(-1);
		}
	}
}

void branchState(){
	//TODO: if gen
	getNEOLToken(attribute, &tokenSize);
	if (token != T_ID && token != L_INT && token != L_STRING && token != L_FLOAT){
		// FIXME: handleError; 		
		printf("ERR\n"); exit(-1);
	}
	//FIXME: 
	//pushbackAttr(tokenSize);
	//TODO: precedenceAnalysis
	getNCheckToken(attribute, T_THEN);
	//FIXME: getNCheckToken(attribute, T_EOL);
	commandsState(T_ELSE);
	//FIXME: getNCheckToken(attribute, T_EOL);
	commandsState(T_END);
	getNCheckToken(attribute, T_IF);
}

void loopState(){
	//TODO: while gen
	getNCheckToken(attribute, T_WHILE);
	getNEOLToken(attribute, &tokenSize);
	if (token != T_ID && token != L_INT && token != L_STRING && token != L_FLOAT){
		// FIXME: handleError; 		
		printf("ERR\n"); exit(-1);
	}
	//FIXME: 
	//pushbackAttr(tokenSize);
	//TODO: precedenceAnalysis
	//FIXME: getNCheckToken(attribute, T_EOL);
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
		default:
			// FIXME: handleError; 		
			printf("ERR\n"); exit(-1);
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
		default:
			// FIXME: handleError; 		
			printf("ERR\n"); exit(-1);
	}
}
// int main() {
// 	openInput("../Tests/linput1");
// 	//printf("weadcsad\n");
// 	//parse();
// 	precedence_analysis();
// 	printf("úspěch syntaktické analýzy\n");
// }