/*
 * IFJ 2017 project
 *	File			symtable.c
 *	Description		Source file for binary search tree
 *	Author			Martin Studeny (xstude23)
 */
#include "symtable.h"

extern nodePtr Strom;

struct load insert;
loadPtr Content_of_Insert = &insert;

int stringCpy(String *s, char* c) {
	if(s == NULL)
		if(stringInit(&s))
			return INTERN_ERR;

	for(int i = 0; c[i] != '\0'; i++) {
		if(stringAddData(s, c[i]))
			return INTERN_ERR;
	}

	return FINE; // TODO: magie
}

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



int insert_variable(char *name, int type) {
	nodePtr uzel;
	char tmp[1000];

	loadPtr Content_of_Insert2 = saveMalloc(sizeof(struct load));
	//val tmptmp = malloc(sizeof(struct value));
	strcpy(tmp, name);
	generateKey(tmp,1);

	uzel = nodeSearch(Strom, tmp);
	if(uzel != NULL) {
		fprintf(stderr, "Chyba: Promenna jiz existuje\n");
		return FAIL;
	}
	else {
		// TODO if value == NULL, pak se jedna o definici
		// pokud se jedna o inicializaci, musi uz byt definovana
		Content_of_Insert2->defined = 1;
		Content_of_Insert2->type = type;
		Content_of_Insert2->metaType = 1;
		nodeInsert(&Strom, Content_of_Insert2, tmp);
	}
	return 0;
}

int insert_value(char *name, int type, void* pointer) {
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

int insert_function(bool declared, char *name) {
	nodePtr uzel;
	char tmp[1000];
	strcpy(tmp, name);
	generateKey(tmp, 2);
	uzel = nodeSearch(Strom, tmp);

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
			Content_of_Insert->metaType = 2;
			Content_of_Insert->defined = false;
			Content_of_Insert->function.declared = true;
			Content_of_Insert->function.hasReturn = false;

			nodeInsert(&Strom, Content_of_Insert, tmp);
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

int insert_type_into_f(char *name, int type) {
	nodePtr uzel;
	char tmp[1000];
	strcpy(tmp, name);
	generateKey(tmp,2);
	uzel = nodeSearch(Strom, tmp);	
	uzel->symbol->type = type;
	return 0;
}

int set_hasReturn(char *name) {
	nodePtr uzel;
	char tmp[1000];
	strcpy(tmp, name);
	generateKey(tmp,2);
	uzel = nodeSearch(Strom, tmp);	
	uzel->symbol->function.hasReturn = true;
	return 0;
}

int insert_param(char *name, parStruct *par) {
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

// int insert_variable_int(char *name, int hodnota) {
// 	nodePtr uzel;
// 	char tmp[1000];

	
// 	loadPtr Content_of_Insert2 = saveMalloc(sizeof(struct load));
// 	//val tmptmp = malloc(sizeof(struct value));

// 	strcpy(tmp, name);
// 	generateKey(tmp,1);

// 	uzel = nodeSearch(Strom, tmp);
// 	if(uzel != NULL) {
// 		fprintf(stderr, "Chyba: Promenna jiz existuje\n");
// 		return FAIL;
// 	}
// 	else {
// 		// TODO if value == NULL, pak se jedna o definici
// 		// pokud se jedna o inicializaci, musi uz byt definovana
// 		Content_of_Insert2->value.i = hodnota;
// 		Content_of_Insert2->defined = 1;
// 		Content_of_Insert2->type = 1;
// 		Content_of_Insert2->metaType = 1;
// 		Content_of_Insert2->function.declared = 1;
// 		Content_of_Insert2->function.hasReturn = 0;
// 		nodeInsert(&Strom, Content_of_Insert2, tmp);
// 	}
// 	return 0;
// }

// int insert_variable_double(char *name, double hodnota) {
// 	nodePtr uzel;
// 	char tmp[1000];

// 	loadPtr Content_of_Insert2 = saveMalloc(sizeof(struct load));

// 	strcpy(tmp, name);
// 	generateKey(tmp,1);

// 	uzel = nodeSearch(Strom, tmp);
// 	if(uzel != NULL) {
// 		fprintf(stderr, "Chyba: Promenna jiz existuje\n");
// 		return FAIL;
// 	}
// 	else {
// 		// TODO if value == NULL, pak se jedna o definici
// 		// pokud se jedna o inicializaci, musi uz byt definovana
// 		Content_of_Insert2->value.d = hodnota;
// 		Content_of_Insert2->defined = 1;
// 		Content_of_Insert2->type = 2;
// 		Content_of_Insert2->metaType = 1;
// 		Content_of_Insert2->function.declared = 1;
// 		Content_of_Insert2->function.hasReturn = 0;
// 		nodeInsert(&Strom, Content_of_Insert2, tmp);
// 	}
// 	return 0;	
// }

// int insert_variable_string(char *name, char* hodnota) {
// 	nodePtr uzel;
// 	char tmp[1000];

// 	loadPtr Content_of_Insert2 = saveMalloc(sizeof(struct load));

// 	strcpy(tmp, name);
// 	generateKey(tmp,1);

// 	uzel = nodeSearch(Strom, tmp);
// 	if(uzel != NULL) {
// 		fprintf(stderr, "Chyba: Promenna jiz existuje\n");
// 		return FAIL;
// 	}
// 	else {
// 		printf(">>>>>>1<<<<<<<%s\n", hodnota);
// 		// TODO if value == NULL, pak se jedna o definici
// 		// pokud se jedna o inicializaci, musi uz byt definovana
// 		stringCpy(&Content_of_Insert2->value.s, hodnota);
// 		printf("%s\n", hodnota);
// 		printf(">>>>>>2<<<<<<<%s\n",Content_of_Insert2->value.s.data);
// 		Content_of_Insert2->defined = 1;
// 		Content_of_Insert2->type = 3;
// 		Content_of_Insert2->metaType = 1;
// 		Content_of_Insert2->function.declared = 1;
// 		Content_of_Insert2->function.hasReturn = 0;
// 		nodeInsert(&Strom, Content_of_Insert2, tmp);
// 	}
// 	return 0;	
// }



/*
int insert_function(bool declared, int type, char *name, parStruct *par) {
	nodePtr uzel;
	char tmp[1000];

	// pridame na konec jmena funkce znak f
	// ve strome budou nazvy uz s f/v na konci
	strcpy(tmp, name);
	generateKey(tmp, 2);


	uzel = nodeSearch(Strom, tmp);

	if(uzel != NULL) {
		// uzel uz ve strome existuje
		// jedna se bud o promennou, nebo o funkci, ktera jiz byla dekalrovana nebo definovana
		if(uzel->symbol->metaType == 1) {
			// pokud se jedna o promennou, skonci automaticky s chybou
			fprintf(stderr, "Chyba: Promenna jiz existuje\n");
			return FAIL;
		}
		else {
			// pokud se jedna o dvoji deklaraci nebo o definici
			if(declared == 1) {
				// jedna se o dvoji deklaraci - skonci s chybou
				fprintf(stderr, "Chyba: Pokousite se podruhe deklarovat stejnou funkci\n");
				return FAIL;
			}
			else if(uzel->symbol->defined == 1) {
				fprintf(stderr, "Chyba: Pokousite se znovu definovat jiz definovanou funkci\n");
				return FAIL;
			}
			else {
				// pokud se jedna o definici
				// aktualizuji se data ve strome a funkce uspesne konci
				uzel->symbol->defined = 1;
				return 0;
			}
		}
	}
	else {
		// pokud uzel ve strome jete neni, znamena to, ze se jedna o funcki, ktera se ted muze byt teprve deklrovat, nebo primo i definovat
		if(declared == 1) {
			// pro deklaraci nove funkce
			Content_of_Insert->type = type;
			Content_of_Insert->metaType = 2;
			Content_of_Insert->defined = false;
			Content_of_Insert->function.declared = true;
			Content_of_Insert->function.hasReturn = false;

			struct parameters *Par1 = saveMalloc(sizeof(*Par1));
			struct parameters *Par2 = saveMalloc(sizeof(*Par2));
			struct parameters *Par3 = saveMalloc(sizeof(*Par3));

			(*Par1).type = par[0].type;
			strcpy((*Par1).name, par[0].id);
			(*Par2).type = par[1].type;
			strcpy((*Par2).name, par[1].id);
			(*Par3).type = par[2].type;
			strcpy((*Par3).name, par[2].id);

			(*Par1).next = &(*Par2);
			(*Par2).next = &(*Par3);

			Content_of_Insert->function.parameters = &(*Par1); // ukazatel na prvni rpvek seznamu

			nodeInsert(&Strom, Content_of_Insert, tmp);
			return 0;
		}
		else {
			Content_of_Insert->type = type;
			Content_of_Insert->metaType = 2;
			Content_of_Insert->defined = true;
			Content_of_Insert->function.declared = true;
			Content_of_Insert->function.hasReturn = false;

			struct parameters *Par1 = saveMalloc(sizeof(*Par1));
			struct parameters *Par2 = saveMalloc(sizeof(*Par2));
			struct parameters *Par3 = saveMalloc(sizeof(*Par3));

			(*Par1).type = par[0].type;
			strcpy((*Par1).name, par[0].id);
			(*Par2).type = par[1].type;
			strcpy((*Par2).name, par[1].id);
			(*Par3).type = par[2].type;
			strcpy((*Par3).name, par[2].id);

			(*Par1).next = &(*Par2);
			(*Par2).next = &(*Par3);

			Content_of_Insert->function.parameters = &(*Par1); // ukazatel na prvni rpvek seznamu

			nodeInsert(&Strom, Content_of_Insert, tmp);
			return 0;
		}
	}


	// // pokud je declared 0, jedna se o dekalraci
	// // jinak je to definice
	// if(declared == 1) {
	// 	//printf("%d\n", uzel->key->function->declared);
	// 	// poku uz byla funcke jednou delarovana
	// 	// zkontrolujeme, jestli uz byla deklarovana, pokud byla, tak ukoncit s chybou dvoji deklarace
	// }
	// else {
	// 	// TODO musime zkontrolovat, jestli uz nahodou nebyla definovana jednou
	// }






	// printf("-------------  %s\n", Strom->key);
	// printf("%d\n", declared);
	// printf("%d\n", type);
	// printf("%s\n", name);
	// printf("%d\n", par[1].type);
	// printf("%s\n", par[1].id);
	return 0;
}
*/