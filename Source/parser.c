//Authors: xzaple36 (Jakub Zapletal), xzilva02 (Michal Zilvar)
#include "includes.h"

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
	for (i = 0; buffer[i].token != LB; i++){
		pop(stack, &buffer[i]);
	}
	push(stack, NONTtoken);
	i = i - 1; // dekrementation after for cycle
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
		if (stack->arr[i].token != 15 || stack->arr[i].token != 16 || stack->arr[i].token != 17)
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
	case LB:
		converted->token = PRLB;
		return FINE;
	case RB:
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

int precedence_analysis(){
	
	tStack stack;	
	stackInit(&stack);
	tokenparam firstToken;
	firstToken.token = PREND;
	push(&stack, firstToken);
	char a = PREND;
	do {
		tokenparam b = precedenceTokenConversion(getToken(), &b);
		switch (precTable[a][b.token]){
			case PREQUAL:
				push(&stack, b);
				break;
			case LB:
				changeA(&stack, a);
				push(&stack, b);
				break;
			case RB:
				replaceY(&stack, a);
			case E:
				return INTERN_ERR;
		}
	} while (((b.token) != PREND) || (a = topTerm(&stack) != PREND))
	printf("úspěch syntaktické analýzy");
}