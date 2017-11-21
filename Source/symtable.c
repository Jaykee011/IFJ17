/*
 * IFJ 2017 project
 *	File			symtable.c
 *	Description		Source file for binary search tree
 *	Author			Martin Studeny (xstude23)
 */
#include "symtable.h"

void treeInit(nodePtr *tree) {
    *tree = NULL;
}

void treeDispose(nodePtr *tree) {
	if( (*tree) != NULL) {
		treeDispose(&(*tree)->lPtr);
		treeDispose(&(*tree)->rPtr);
	}
	*tree = NULL;
}

nodePtr nodeSearch(nodePtr tree, char* K) {
	if(tree != NULL) {
		if(strcmp(tree->key, K) == 0) {
			//*content = tree->symbol;
			return tree;
		}
		else if(strcmp(tree->key, K) > 0)
			return nodeSearch(tree->lPtr, K/*,content*/);
		else
			return nodeSearch(tree->rPtr, K/*,content*/);
	}
	else
		return NULL;
}

void nodeInsert(nodePtr* tree, loadPtr load, char* K) {
	if(*tree == NULL) {
		*tree = saveMalloc(sizeof(struct node));
		strcpy((*tree)->key, K);
		(*tree)->lPtr = NULL;
		(*tree)->rPtr = NULL;
		(*tree)->symbol = load;
	}
	else {
		if(strcmp((*tree)->key, K) == 0) {
			(*tree)->symbol = load;
			return;
		}
		else {
			if(strcmp((*tree)->key, K) > 0)
				nodeInsert(&(*tree)->lPtr, load, K);
			else
				nodeInsert(&(*tree)->rPtr, load, K);
		}
	}
}

void ReplaceByRightmost(nodePtr treeReplaced, nodePtr *tree) {
	if((*tree)->rPtr != NULL) {
		ReplaceByRightmost(treeReplaced, &(*tree)->rPtr);
		return;
	}
	else {
		treeReplaced->symbol = (*tree)->symbol;
		strcpy(treeReplaced->key,(*tree)->key);
		*tree = (*tree)->lPtr;
	}
}

void nodeDelete(nodePtr *tree, char *K) {
	if(*tree == NULL)
		return;
	if(strcmp((*tree)->key, K) == 0) {
		if((*tree)->lPtr != NULL && (*tree)->rPtr != NULL) {
			ReplaceByRightmost(*tree, &(*tree)->lPtr);
			return;
		}
		if((*tree)->lPtr != NULL && (*tree)->rPtr == NULL) {
			*tree = (*tree)->lPtr;
			return;
		}
		if((*tree)->lPtr == NULL && (*tree)->rPtr != NULL) {
			*tree = (*tree)->rPtr;
			return;
		}
		if((*tree)->lPtr == NULL && (*tree)->rPtr == NULL) {
			*tree = NULL;
			return;
		}
	}
	else {
		if(strcmp((*tree)->key, K) > 0)
			nodeDelete(&(*tree)->lPtr, K);
		else
			nodeDelete(&(*tree)->rPtr, K);
	}
}

// void generateKey(char symbolName[64], int metaType) {
// 	// TODO - udelat ze vseho lower case
// 	// variable
// 	if(metaType == 1) {
// 		strcat(symbolName, "v");
// 	}
// 	// function
// 	else if(metaType == 2) {
// 		strcat(symbolName, "f");
// 	}
// }



int insert_variable(nodePtr *Strom, char *name) {
	nodePtr uzel;

	loadPtr Content_of_Insert2 = saveMalloc(sizeof(struct load));
	//val tmptmp = malloc(sizeof(struct value));
	//strcpy(tmp, name);
	//generateKey(tmp,1);

	uzel = nodeSearch(*Strom, name);
	if(uzel != NULL) {
		fprintf(stderr, "Chyba: Promenna jiz existuje\n");
		return FAIL;
	}
	else {
		//TODO: if value == NULL, pak se jedna o definici
		// pokud se jedna o inicializaci, musi uz byt definovana
		Content_of_Insert2->defined = 1;
		Content_of_Insert2->metaType = VARIABLE;
		stringInit(&(Content_of_Insert2->value.s));
		nodeInsert(Strom, Content_of_Insert2, name);
	}
	return 0;
}

void insert_type(nodePtr Strom, char *name, int type) {
	nodePtr uzel;
	uzel = nodeSearch(Strom, name);

	if (uzel->symbol->metaType == FUNCTION){
		if (uzel->symbol->function.declared && uzel->symbol->defined && uzel->symbol->type != type){
			//FIXME: errorHandle
			exit(-1);
		}
	}

	uzel->symbol->type = type;
}

void insert_value(nodePtr Strom, char *name, int type, val data, int valueType) {
	nodePtr uzel;
	uzel = nodeSearch(Strom, name);

	if(uzel == NULL) {
		//FIXME: errorHandle
		exit(FAIL);
	}
	switch(type){
		case INTEGER:
			if (valueType == INTEGER)
				uzel->symbol->value.i = data.i;
			else if (valueType == DOUBLE)
				uzel->symbol->value.i = data.d;
			else{
				//FIXME: errorHandle
			}
			break;
		case DOUBLE:
			if (valueType == INTEGER)
				uzel->symbol->value.d = data.i;
			else if (valueType == DOUBLE)
				uzel->symbol->value.d = data.d;
			else{
				//FIXME: errorHandle
			}
			break;
		case STRING:
			if (valueType != STRING){
				//FIXME: errorHandle
			}
			stringInit(&uzel->symbol->value.s);
			concatToString(uzel->symbol->value.s, data.s->data);
			break;
		default:
			//FIXME: errorHandle
			exit(FAIL);
			break;
	}
	uzel->symbol->initialized = true;
}

// vlozit param do funkci

void insert_function(nodePtr *Strom, bool declaration, char *name) {
	nodePtr uzel;
	uzel = nodeSearch(*Strom, name);


	if(declaration == true) {
		// pokud chceme deklarovat
		if(uzel != NULL) {
			//FIXME: 
			exit(-1);
		}
		else {
			loadPtr Content_of_Insert = saveMalloc(sizeof(struct load));
			Content_of_Insert->metaType = FUNCTION;
			Content_of_Insert->defined = false;
			Content_of_Insert->function.declared = true;
			Content_of_Insert->function.hasReturn = false;

			nodeInsert(Strom, Content_of_Insert, name);
		}
	}
	else {
		if(uzel == NULL) {
			loadPtr Content_of_Insert = saveMalloc(sizeof(struct load));
			Content_of_Insert->metaType = FUNCTION;
			Content_of_Insert->defined = true;
			Content_of_Insert->function.declared = false;
			Content_of_Insert->function.hasReturn = false;

			nodeInsert(Strom, Content_of_Insert, name);
		}
		else if(uzel->symbol->defined == 1) {
			exit(-1);
		}
		if(uzel->symbol->function.declared == 1)
			uzel->symbol->defined = 1;
	}
}

void set_hasReturn(nodePtr Strom, char *name) {
	nodePtr uzel;
	uzel = nodeSearch(Strom, name);	
	uzel->symbol->function.hasReturn = true;
}

void insert_param(nodePtr Strom, char *name, char *parName, int type, bool declaration) {
	nodePtr function = nodeSearch(Strom, name);
	
	param newParam = saveMalloc(sizeof(struct parameters));
	newParam->type = type;
	strcpy(newParam->name, parName); 

	if (declaration){
		param *head = &function->symbol->function.declaredParameters;
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
	else{
		param *head = &function->symbol->function.parameters;
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
}

int validateDefinitionParameters(nodePtr Strom, char *name){
	nodePtr uzel;
	uzel = nodeSearch(Strom, name);	

	if (!uzel->symbol->function.declared)
		return FINE;
	
	param headDec = uzel->symbol->function.declaredParameters;
	param headDef = uzel->symbol->function.parameters;

	if (headDec == NULL && headDef == NULL)
		return FINE;

	if ((headDec == NULL && headDef != NULL) || (headDec != NULL && headDef == NULL))
		return FAIL;

	while (headDec != NULL && headDef != NULL){
		if (headDec->type != headDef->type)
			return FAIL;
		
		headDec = headDec->next;
		headDef = headDef->next;
	}

	if ((headDec == NULL && headDef != NULL) || (headDec != NULL && headDef == NULL))
		return FAIL;

	return FINE;
}

void validateFunctCall(nodePtr Strom, char *varName, char *functName){
	nodePtr function = nodeSearch(Strom, functName);
	nodePtr variable = nodeSearch(Strom, varName);

	if (variable == NULL){
		//FIXME: errorhandle
		exit(-1);
	}
	
	if (function == NULL){
		//FIXME: errorhandle
		exit(-2);
	}

	if (!function->symbol->defined && !function->symbol->function.declared){
		//FIXME: errorhandle
		exit(-3);
	}

	if (function->symbol->type != variable->symbol->type){
		//FIXME: errorhandle
		exit(-4);
	}
}

int validateCallParams(nodePtr Strom, char *name, param callParams){
	nodePtr uzel;
	uzel = nodeSearch(Strom, name);

	param headDef;
	
	if (uzel->symbol->defined)
		headDef = uzel->symbol->function.parameters;
	else 
		headDef = uzel->symbol->function.declaredParameters;
		
	param headCall = callParams;

	if (headDef == NULL && headCall == NULL)
		return FINE;

	if ((headDef == NULL && headCall != NULL) || (headDef != NULL && headCall == NULL))
		return FAIL;

	while (headDef != NULL && headCall != NULL){
		if (headDef->type != headCall->type)
			return FAIL;
		
		headDef = headDef->next;
		headCall = headCall->next;
	}

	if ((headDef == NULL && headDef != NULL) || (headDef != NULL && headDef == NULL))
		return FAIL;

	return FINE;

}

val getValue(nodePtr Strom, char *name){
	return nodeSearch(Strom, name)->symbol->value;
}