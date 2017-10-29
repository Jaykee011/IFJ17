/*
 * IFJ 2017 project
 *	File			symtable.c
 *	Description		Source file for binary search tree
 *	Author			Martin Studeny (xstude23)
 */
#include "includes.h"

void treeInit(nodePtr *tree) {
    *tree = NULL;
}

void treeDispose(nodePtr *tree) {
	if( (*tree) != NULL) {
		treeDispose(&(*tree)->lPtr);
		treeDispose(&(*tree)->rPtr);
		free(*tree);
	}
	*tree = NULL;
}

nodePtr nodeSearch(nodePtr tree, char* K, loadPtr *content) {
	if(tree != NULL) {
		if(tree->key == K) {
			*content = tree->symbol;
			return tree;
		}
		else if(tree->key > K)
			return nodeSearch(tree->lPtr, K,content);
		else
			return nodeSearch(tree->rPtr, K,content);
	}
	else
		return NULL;
}

nodePtr nodeInsert(nodePtr* tree, loadPtr load, char* K) {
	if(*tree == NULL) {
		*tree = malloc(sizeof(struct node));
		(*tree)->key = K;
		(*tree)->lPtr = NULL;
		(*tree)->rPtr = NULL;
		(*tree)->symbol = load;
		return;
	}
	else {
		if((*tree)->key == K) {
			(*tree)->symbol = load;
			return;
		}
		else {
			if((*tree)->key > K)
				nodeInsert(&(*tree)->lPtr, load, K);
			else
				nodeInsert(&(*tree)->rPtr, load, K);
		}
	}
}

void ReplaceByRightmost(nodePtr treeReplaced, nodePtr *tree) {
	nodePtr tmp = NULL;
	if((*tree)->rPtr != NULL) {
		ReplaceByRightmost(treeReplaced, &(*tree)->rPtr);
		return;
	}
	else {
		treeReplaced->symbol = (*tree)->symbol;
		treeReplaced->key = (*tree)->key;
		tmp = *tree;
		*tree = (*tree)->lPtr;
		free(tmp);
	}
}

void nodeDelete(nodePtr *tree, char *K) {
	static nodePtr tmp = NULL;
	if(*tree == NULL)
		return;
	if((*tree)->key == K) {
		if((*tree)->lPtr != NULL && (*tree)->rPtr != NULL) {
			ReplaceByRightmost(*tree, &(*tree)->lPtr);
			return;
		}
		if((*tree)->lPtr != NULL && (*tree)->rPtr == NULL) {
			tmp = *tree;
			*tree = (*tree)->lPtr;
			free(tmp);
			tmp = NULL;
			return;
		}
		if((*tree)->lPtr == NULL && (*tree)->rPtr != NULL) {
			tmp = *tree;
			*tree = (*tree)->rPtr;
			free(tmp);
			tmp = NULL;
			return;
		}
		if((*tree)->lPtr == NULL && (*tree)->rPtr == NULL) {
			free(*tree);
			*tree = NULL;
			return;
		}
	}
	else {
		if((*tree)->key > K)
			nodeDelete(&(*tree)->lPtr, K);
		else
			nodeDelete(&(*tree)->rPtr, K);
	}
}

void generateKey(char symbolName[64], int metaType) {
	// variable
	if(metaType == 1) {
		strcat(symbolName, "v");
	}
	// function
	else if(metaType == 2) {
		strcat(symbolName, "f");
	}
}
