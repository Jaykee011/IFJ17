#include "includes.h"

nodePtr symtable;

int main(int argc, char** argv) {

	/* STROM */

	treeInit(&symtable);

	/* /STROM */
	
	openOutput();
	
	parse();

	treeDispose(&symtable);

	error(FINE);
	return FINE;
}	