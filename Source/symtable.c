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

void generateKey(char symbolName[64], int metaType) {
	// TODO - udelat ze vseho lower case
	// variable
	if(metaType == 1) {
		strcat(symbolName, "v");
	}
	// function
	else if(metaType == 2) {
		strcat(symbolName, "f");
	}
}



int insert_variable(nodePtr *Strom, char *name) {
	nodePtr uzel;
	char tmp[1000];

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
		Content_of_Insert2->metaType = 1;
		nodeInsert(Strom, Content_of_Insert2, name);
	}
	return 0;
}

void insert_variable_type(nodePtr Strom, char *name, int type) {
	nodePtr variable;
	variable = nodeSearch(Strom, name);
	variable->symbol->type = type;
}

int insert_value(nodePtr Strom, char *name, int type, void* pointer) {
	nodePtr uzel;
	char tmp[1000]; // TODO limit je 1000 znaku
	strcpy(tmp, name);
	generateKey(tmp,1);
	uzel = nodeSearch(Strom, tmp);

	if(uzel == NULL) {
		fprintf(stderr, "Chyba: Promenna neexistuje\n");
		return FAIL;
	}
	if(type == 1) {
		uzel->symbol->value.i = *((int*)pointer);
	}
	else if(type == 2) {
		uzel->symbol->value.d = *((double*)pointer);
	}
	else if(type == 3) {
		//(struct String*)pointer;
		uzel->symbol->value.s.data = ((char*)pointer);
	}
	else 
		return FAIL;
	return 0;
}

// vlozit param do funkci

int insert_function(nodePtr *Strom, bool declared, char *name) {
	nodePtr uzel;
	char tmp[1000];
	//strcpy(tmp, name);
	// generateKey(tmp, 2);
	uzel = nodeSearch(*Strom, name);

	loadPtr Content_of_Insert2 = saveMalloc(sizeof(struct load));

	if(declared == true) {
		// pokud chceme deklarovat
		if(uzel != NULL) {
			// pokud jiz uzel ve strome je - chyba
			fprintf(stderr, "Chyba: Promenna jiz existje\n");
			return FAIL;
		}
		// else if(uzel->symbol->function.declared == 1) {
		// 	fprintf(stderr, "Chyba: Funkce jiz byla deklarvana\n");
		// 	return FAIL;
		// }
		else {
			Content_of_Insert2->metaType = 2;
			Content_of_Insert2->defined = false;
			Content_of_Insert2->function.declared = true;
			Content_of_Insert2->function.hasReturn = false;

			nodeInsert(Strom, Content_of_Insert2, name);
			return 0;			
		}
	}
	else {
		// definice
		if(uzel == NULL) {
			fprintf(stderr, "Chyba: Promenna neexistuje\n");
			return FAIL;
		}
		else if(uzel->symbol->defined == 1) {
			fprintf(stderr, "Chyba: Snaha o dvoji definici\n");
			return FAIL;
		}
		if(uzel->symbol->function.declared == 1)
			uzel->symbol->defined = 1;
	}
	return 0;
}

int insert_type_into_f(nodePtr Strom, char *name, int type) {
	nodePtr uzel;
	char tmp[1000];
	strcpy(tmp, name);
	generateKey(tmp,2);
	uzel = nodeSearch(Strom, tmp);	
	uzel->symbol->type = type;
	return 0;
}

int set_hasReturn(nodePtr Strom, char *name) {
	nodePtr uzel;
	char tmp[1000];
	strcpy(tmp, name);
	generateKey(tmp,2);
	uzel = nodeSearch(Strom, tmp);	
	uzel->symbol->function.hasReturn = true;
	return 0;
}

int insert_param(nodePtr Strom, char *name, parStruct *par) {
	nodePtr uzel;
	char tmp[1000];
	strcpy(tmp, name);
	generateKey(tmp,2);
	uzel = nodeSearch(Strom, tmp);	
	
	if(uzel->symbol->function.parameters == NULL) {
		struct parameters *Par1 = saveMalloc(sizeof(*Par1));
		(*Par1).type = (*par).type;
		strcpy((*Par1).name, (*par).id);
		uzel->symbol->function.parameters = &(*Par1);
	}	
	else {
		param tmp = uzel->symbol->function.parameters;
		printf("%s\n", tmp->name);
		// &(*Par1) = uzel->symbol->function.parameters;
		//printf("%s\n", (*Par1).name);
		while(tmp->next != NULL) {		
			tmp = tmp->next;
		}
		struct parameters *Par1 = saveMalloc(sizeof(*Par1));
		(*Par1).type = (*par).type;
		strcpy((*Par1).name, (*par).id);	
		tmp->next = &(*Par1);		
	}
	return 0;
}

val getValue(nodePtr Strom, char *name){
	return nodeSearch(Strom, name)->symbol->value;
}