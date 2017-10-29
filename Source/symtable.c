/*
 * IFJ 2017 project
 *	File			symtable.c
 *	Description		Source file for binary search tree
 *	Author			Jakub Zapletal (xzaple36)
 */
#include "includes.h"

void init(nodePtr *rootPtr){
    *rootPtr = NULL;
}

nodePtr nodeInsert(nodePtr* tree, loadPtr load, char* key){

}

void nodeDelete(nodePtr* tree, char* key){

}

void treeDispose (nodePtr* rootPtr){
    if((*rootPtr) == NULL) return;
    
    treeDispose(&(*rootPtr)->LPtr);
    treeDispose(&(*rootPtr)->RPtr);

    free(*rootPtr);
    *rootPtr = NULL;
}

nodePtr nodeSearch(nodePtr tree, char* key){
    
    return node;
}