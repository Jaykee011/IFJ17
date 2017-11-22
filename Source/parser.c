/*
 * IFJ 2017 project
 *	File			parser.c
 *	Description		Source file for syntax analysis
 *	Author			Radek Wildmann (xwildm00), Jakub Zapletal (xzaple36)
 */
#include "includes.h"

int token = 0;
int tokenSize;
String *attribute = NULL;
String *variableName = NULL;
String *functionName = NULL;
String *inFunction = NULL;
int type = 0; // 1 => int; 2 => double; 3 => string;
int expressionType = 0; // 1 => int; 2 => double; 3 => string;
tokenparam precedenceBuffer[100];

nodePtr *currentSymtable; // lokalni tabulka symbolu 
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
	int firstType;
	int secondType;
	bool intOp = true;
	stringInit(&(result.s));

	if (((tokenparam *)lValue)->token == PRLB && ((tokenparam *)rValue)->token == PRRB){
		if (((tokenparam *)mValue)->data.i)
			expressionType = INTEGER;
		else if (((tokenparam *)mValue)->data.d)
			expressionType = DOUBLE;
		else{
			expressionType = STRING;
		}
		return ((tokenparam *)mValue)->data;
	}
	else if (mValue == NULL){
		if (((tokenparam *)lValue)->data.i)
			expressionType = INTEGER;
		else if (((tokenparam *)lValue)->data.d)
			expressionType = DOUBLE;
		else{
			expressionType = STRING;
		}
		return ((tokenparam *)lValue)->data;
	}
	else{
		if (((tokenparam *)lValue)->data.i)
			firstType = INTEGER;
		else if (((tokenparam *)lValue)->data.d)
			firstType = DOUBLE;
		else{
			firstType = STRING;
		}

		if (((tokenparam *)rValue)->data.i)
			secondType = INTEGER;
		else if (((tokenparam *)rValue)->data.d)
			secondType = DOUBLE;
		else{
			secondType = STRING;
		}

		if ((firstType == STRING && secondType != STRING) || (firstType != STRING && secondType == STRING)){
			error(TYPE_ERR);
		}

		if (firstType == DOUBLE || secondType == DOUBLE){
			//FIXME: conversion
			if (firstType == INTEGER){
				((tokenparam *)lValue)->data.d = ((tokenparam *)lValue)->data.i;
				firstType = DOUBLE;
			}
			else if (secondType == INTEGER){
				((tokenparam *)rValue)->data.d = ((tokenparam *)rValue)->data.i;
				secondType = DOUBLE;
			}
			intOp = false;
		}

		//FIXME: TOTO CELE NEMA DELAT KOMPILATOR, MUSI SE TO NAHRADIT GENEROVANIM INSTRUKCI
		switch(((tokenparam *)mValue)->token){
			case PRPLUS:
				if (firstType == STRING && secondType == STRING){
					//TODO: generate concat
					expressionType = STRING;
				}
				else if (intOp){
					result.i = (((tokenparam *)lValue)->data.i + ((tokenparam *)rValue)->data.i) ;
					expressionType = INTEGER;
				}
				else{
					result.d = (((tokenparam *)lValue)->data.d + ((tokenparam *)rValue)->data.d) ;
					expressionType = DOUBLE;
				}
				break;
			case PRMINUS:
				//FIXME: posefit datove typy
				if (intOp){
					result.i = (((tokenparam *)lValue)->data.i - ((tokenparam *)rValue)->data.i) ;
					expressionType = INTEGER;
				}
				else{
					result.d = (((tokenparam *)lValue)->data.d - ((tokenparam *)rValue)->data.d) ;
					expressionType = DOUBLE;
				}
				break;
			case PRTIMES:
				if (intOp){
					result.i = (((tokenparam *)lValue)->data.i * ((tokenparam *)rValue)->data.i) ;
					expressionType = INTEGER;
				}
				else{
					result.d = (((tokenparam *)lValue)->data.d * ((tokenparam *)rValue)->data.d) ;
					expressionType = DOUBLE;
				}
				break;
			case PRDIV:
				if (intOp){
					result.d = (double)(((tokenparam *)lValue)->data.i / ((tokenparam *)rValue)->data.i) ;
				}
				else{
					result.d = (((tokenparam *)lValue)->data.d / ((tokenparam *)rValue)->data.d) ;
				}
				expressionType = DOUBLE;
				break;
			case PRIDIV:
				if (firstType == INTEGER && secondType == INTEGER){
					result.i = (((tokenparam *)lValue)->data.i / ((tokenparam *)rValue)->data.i) ;
				}
				else{
					error(TYPE_ERR);
				}
				expressionType = INTEGER;
				break;
			case PRLT:
				if (firstType == STRING && secondType == STRING){
					//TODO: strcmp
					expressionType = BOOLEAN;
				}
				else if (intOp){ 
					result.b = (((tokenparam *)lValue)->data.i < ((tokenparam *)rValue)->data.i);
				}
				else{
					result.b = (((tokenparam *)lValue)->data.d < ((tokenparam *)rValue)->data.d);
				}
				break;
			case PRGT:
				if (firstType == STRING && secondType == STRING){
					//TODO: strcmp
					expressionType = BOOLEAN;
				}
				else if (intOp){ 
					result.b = (((tokenparam *)lValue)->data.i > ((tokenparam *)rValue)->data.i);
				}
				else{
					result.b = (((tokenparam *)lValue)->data.d > ((tokenparam *)rValue)->data.d);
				}
				break;
			case PRELT:
				if (firstType == STRING && secondType == STRING){
					//TODO: strcmp
					expressionType = BOOLEAN;
				}
				else if (intOp){ 
					result.b = (((tokenparam *)lValue)->data.i <= ((tokenparam *)rValue)->data.i);
				}
				else{
					result.b = (((tokenparam *)lValue)->data.d <= ((tokenparam *)rValue)->data.d);
				}
				break;
			case PREGT:
				if (firstType == STRING && secondType == STRING){
					//TODO: strcmp
					expressionType = BOOLEAN;
				}
				else if (intOp){ 
					result.b = (((tokenparam *)lValue)->data.i >= ((tokenparam *)rValue)->data.i);
				}
				else{
					result.b = (((tokenparam *)lValue)->data.d >= ((tokenparam *)rValue)->data.d);
				}
				break;
			case PREQUAL:
				if (firstType == STRING && secondType == STRING){
					//TODO: strcmp
					expressionType = BOOLEAN;
				}
				else if (intOp){ 
					result.b = (((tokenparam *)lValue)->data.i == ((tokenparam *)rValue)->data.i);
				}
				else{
					result.b = (((tokenparam *)lValue)->data.d == ((tokenparam *)rValue)->data.d);
				}
				break;
			case PRNEQ:
				if (firstType == STRING && secondType == STRING){
					//TODO: strcmp
					expressionType = BOOLEAN;
				}
				else if (intOp){ 
					result.b = (((tokenparam *)lValue)->data.i != ((tokenparam *)rValue)->data.i);
				}
				else{
					result.b = (((tokenparam *)lValue)->data.d != ((tokenparam *)rValue)->data.d);
				}
				break;
			default:
				error(SYN_ERR);
				break;
		}
		return result;
	}
}

int stackInit(tStack *stack) { // stack inicialization

	if (!stack)
		error(INTERN_ERR);
	stack->top = -1;
	return FINE;
}

int push(tStack *stack, tokenparam token) { // pushes new terminal to stack
	
	if (stack->top == STACK_SIZE) // if stack is full
		error(INTERN_ERR);
	stack->top++;
	stack->arr[stack->top].token = token.token;
	stack->arr[stack->top].data = token.data;
	return FINE;
}
int pop(tStack *stack, tokenparam *token) { // pops top token
	
	if (stack->top == -1)
		error(INTERN_ERR);
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
		result = evalExpr(&buffer[2],&buffer[1], &buffer[0]); //TODO: comment

	NONTtoken.data = result;
	pop(stack, &buffer[i]);
	push(stack, NONTtoken);
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
		error(INTERN_ERR);
	return stack->arr[stack->top].token;
}

char topTerm(tStack *stack) { // returns terminal closes to the top of the stack

	if (stack->top == -1)
		error(INTERN_ERR);
	for (int i = stack->top; i >= 0; i--)
		if (stack->arr[i].token != 15 && stack->arr[i].token != 16 && stack->arr[i].token != 17)
		{
			return stack->arr[i].token;
		}
	error(INTERN_ERR);
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

void testTokens(bool boolExpected){ // imput token string control
	bool compareTokenExists = 0; // compare token exists
	bool previousNonId = 0; // previous non-ID
	int leftBracktCounter = 0; // left brackt counter
	bool loop = true;
	for(int i = 0; loop; i++){
		token = getToken(attribute,&tokenSize);
		if (token == LEX_ERR){
			error(LEX_ERR);
		}
		precedenceBuffer[i].token = token;
		switch(precedenceBuffer[i].token){
			case T_ID:
				precedenceBuffer[i].data = getValue(*currentSymtable, attribute->data);
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
					error(SYN_ERR); 
				}
				break;
			case T_LT:
			case T_GT:
			case T_GTE:
			case T_LTE:
			case T_EQ:
			case T_NEQ:
				if (!boolExpected){
					error(SYN_ERR);
				}	
				if(compareTokenExists == 0){
					compareTokenExists = 1;
					previousNonId = 0;
				}
				else{
					error(SYN_ERR); 
				}
				break;
			default:
				pushbackAttr(tokenSize);
				loop=false;
				break;
		}
	}
}

struct value precedence_analysis(bool boolExpected){
	testTokens(boolExpected);
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
		if (token == LEX_ERR)
			error(LEX_ERR);
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
	
	if (token == LEX_ERR)
			error(LEX_ERR);

	if (token != t){
		error(SYN_ERR);
	}
}

bool parse(){
	stringInit(&attribute);
	stringInit(&variableName);
	stringInit(&functionName);
	stringInit(&inFunction);
	getNEOLToken(attribute, &tokenSize);
	switch(token){
		case T_DECLARE:
		case T_FUNCTION:
			functionsState();
		case T_SCOPE:
			scopeState();
			break;
		default:
			error(SYN_ERR);
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
				if (token != T_SCOPE){
					error(SYN_ERR);
				}
				loop = false;
				break;
		}
	}
}

void functionState(){
	switch(token){
		case T_DECLARE:
			getNCheckToken(attribute, T_FUNCTION);
			getNCheckToken(functionName, T_ID);

			insert_function(&symtable, true, functionName->data);

			getNCheckToken(attribute, T_LB);
			paramsState(true);
			getNCheckToken(attribute, T_AS);
			getNEOLToken(attribute, &tokenSize);
			typeState();

			insert_type(symtable, functionName->data, type);

			getNCheckToken(attribute, T_EOL);
			getNEOLToken(attribute, &tokenSize);
			break;
		case T_FUNCTION:
			getNCheckToken(functionName, T_ID);
			stringCpy(inFunction, functionName->data);
			
			insert_function(&symtable, false, functionName->data);

			getNCheckToken(attribute, T_LB);
			
			paramsState(false);
			if (validateDefinitionParameters(symtable, functionName->data)){
				error(TYPE_ERR);
			}

			getNCheckToken(attribute, T_AS);
			getNEOLToken(attribute, &tokenSize);
			typeState();

			insert_type(symtable, functionName->data, type);

			currentSymtable = &nodeSearch(symtable, functionName->data)->symbol->function.functTable;
			fcommandsState();
			currentSymtable = &symtable;


			getNCheckToken(attribute, T_FUNCTION);
			getNCheckToken(attribute, T_EOL);
			setFunctionDefined(symtable, inFunction->data);
			getNEOLToken(attribute, &tokenSize);
			break;
		default:
			error(SYN_ERR);
			break;
	}
}

void scopeState(){
	getNCheckToken(attribute, T_EOL);
	scommandsState();
	getNCheckToken(attribute, T_SCOPE);
}

void paramsState(bool declaration){
	getNEOLToken(variableName, &tokenSize);
	switch(token){
		case T_ID:
			paramState(declaration);
			nparamState(declaration);
			break;
		case T_RB:
			break;
		default:
			error(SYN_ERR);
			break;
	}
}

void paramState(bool declaration){
	getNCheckToken(attribute, T_AS);
	getNEOLToken(attribute, &tokenSize);
	typeState();

	insert_param(symtable, functionName->data, variableName->data, type, declaration);
}

void nparamState(bool declaration){
	bool loop = true;
	while (loop){
		getNEOLToken(attribute, &tokenSize);
		switch(token){
			case T_COMMA:
				getNCheckToken(variableName, T_ID);
				paramState(declaration);
				break;
			case T_RB:
				loop = false;
				break;
			default:
				error(SYN_ERR);
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
				error(SYN_ERR);
		}
	}
}

void fcommandState(){
	getNEOLToken(attribute, &tokenSize);
	if (token != T_ID && token != L_INT && token != L_STRING && token != L_FLOAT){
		error(SYN_ERR);
	}
	//TODO: gen return
	pushbackAttr(tokenSize);
	precedence_analysis(false);
	//FIXME: osetrit
	if (expressionType != nodeSearch(symtable, inFunction->data)->symbol->type)
		error(TYPE_ERR);
	getNCheckToken(attribute, T_EOL);
	set_hasReturn(symtable, inFunction->data);
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
				error(SYN_ERR);
		}
	}
}

void scommandState(){
	vardefState();
}

void vardefState(){
	getNCheckToken(variableName, T_ID);
	insert_variable(currentSymtable, variableName->data);
	getNCheckToken(attribute, T_AS);
	getNEOLToken(attribute, &tokenSize);
	typeState();
	insert_type(*currentSymtable, variableName->data, type);
	definitState();
}

void definitState(){
	token = getToken(attribute, &tokenSize);
	if (token == LEX_ERR){
		error(LEX_ERR);
	}
	switch(token){
		case T_EQ:
			initState();
			break;
		case T_EOL:
			break;
		default:
			error(SYN_ERR);
	}
}

void initState(){
	String *storedAttr = NULL;
	stringInit(&storedAttr);
	int storedSize;
	getNEOLToken(storedAttr, &storedSize);
	val precedenceResult;
	switch(token){
		case T_ID:
			getNEOLToken(attribute, &tokenSize);
			if (token == T_LB){
				stringCpy(functionName, storedAttr->data);
				fcallState();
				break;
			}
			else{
				pushbackAttr(tokenSize);
			}
		case L_INT:
		case L_FLOAT:
		case L_STRING:
			pushbackAttr(storedSize);
			precedenceResult = precedence_analysis(false);
			insert_value(*currentSymtable, variableName->data, type, precedenceResult, expressionType);
			break;
		default:
			error(SYN_ERR);
	}
}

void fcallState(){
	validateFunctCall(symtable, *currentSymtable, variableName->data, functionName->data);

	cparamsState();
}

/*//TODO: comment*/
void addParamToList(param *paramList, int t){
	param *head = paramList;

	param newParam = saveMalloc(sizeof(struct parameters));
	newParam->type = t;

	if(*head == NULL) {
		*head = newParam;
	}	
	else {
		param current = *head;
		while(current->next != NULL) {		
			current = current->next;
		}
		current->next = newParam;	
	}
}

void cparamsState(){
	param callParameters = NULL;
	getNEOLToken(attribute, &tokenSize);
	switch(token){
		case T_RB:
			break;
		default:
			cparamState(&callParameters);
			ncparamState(&callParameters);
			break;
	}

	if (validateCallParams(symtable, functionName->data, callParameters)){
		error(TYPE_ERR);
	}
}

void cparamState(param *callParameters){
	termState();
	addParamToList(callParameters, type);
}

void ncparamState(param *callParameters){
	bool loop = true;
	while(loop){
		getNEOLToken(attribute, &tokenSize);
		switch(token){
			case T_COMMA:
				token = getToken(attribute, &tokenSize);
				if (token == LEX_ERR){
					error(LEX_ERR);
				}
				cparamState(callParameters);
				break;
			case T_RB:
				loop = false;
				break;
			default:
				error(SYN_ERR);
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
					error(SYN_ERR);
				}
		}
	}
}

void commandState(){
	switch(token){
		case T_ID:
			stringCpy(variableName, attribute->data);
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
			error(SYN_ERR);
	}
	getNCheckToken(attribute, T_EOL);
}

void inputState(){
	getNCheckToken(attribute, T_ID);
	if (nodeSearch(*currentSymtable, attribute->data) == NULL){
		error(DEF_ERR);
	}
	//TODO: input gen
}

void printState(){
	//TODO: print gen
	getNEOLToken(attribute, &tokenSize);
	if (token != T_ID && token != L_INT && token != L_STRING && token != L_FLOAT){
		error(SYN_ERR);
	}
	pushbackAttr(tokenSize);
	precedence_analysis(false);
	getNCheckToken(attribute, T_SEMICOLON);
	nexprState();
	//TODO: generate print
}

void nexprState(){
	bool loop = true;
	while(loop){
		token = getToken(attribute, &tokenSize);
		if (token == LEX_ERR){
			error(LEX_ERR);
		}
		switch(token){
			case T_ID:
			case L_INT:
			case L_FLOAT:
			case L_STRING:
				pushbackAttr(tokenSize);
				precedence_analysis(false);
				getNCheckToken(attribute, T_SEMICOLON);	
				break;			
			case T_EOL:
				pushbackAttr(tokenSize);
				loop = false;
				break;
			
			default:
				error(SYN_ERR);
				break;
		}
	}
}

void branchState(){
	//TODO: if gen
	getNEOLToken(attribute, &tokenSize);
	if (token != T_ID && token != L_INT && token != L_STRING && token != L_FLOAT){
		error(SYN_ERR);
	}
	pushbackAttr(tokenSize);
	precedence_analysis(true);
	getNCheckToken(attribute, T_THEN);
	getNCheckToken(attribute, T_EOL);
	commandsState(T_ELSE);
	getNCheckToken(attribute, T_EOL);
	commandsState(T_END);
	getNCheckToken(attribute, T_IF);
	getNCheckToken(attribute, T_EOL);
}

void loopState(){
	//TODO: while gen
	getNCheckToken(attribute, T_WHILE);
	getNEOLToken(attribute, &tokenSize);
	if (token != T_ID && token != L_INT && token != L_STRING && token != L_FLOAT){
		error(SYN_ERR);
	}
	pushbackAttr(tokenSize);
	precedence_analysis(true);
	getNCheckToken(attribute, T_EOL);
	commandsState(T_LOOP);
	getNCheckToken(attribute, T_EOL);
}

void typeState(){
	switch(token){
		case T_INTEGER:
			type = INTEGER;
			break;
		case T_DOUBLE:
			type = DOUBLE;
			break;
		case T_STRING:
			type = STRING;
			break;
		default:
			error(SYN_ERR);
	}
}

void termState(){
	//TODO: gen push instructions
	switch(token){
		case L_INT:
			type = INTEGER; 
			break;
		case L_FLOAT:
			type = DOUBLE;
			break;
		case L_STRING:
			type = STRING;
			break;
		case T_ID:
			type = nodeSearch(*currentSymtable, attribute->data)->symbol->type; 
			break;
		default:
			error(SYN_ERR);
	}
}