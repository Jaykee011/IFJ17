#include "includes.h"

nodePtr symtable;

int main(int argc, char** argv) {
	if(argc != 3) {
		printf("./IFJ17 input output\n");
		return 1;
	}

	/* STROM */

	treeInit(&symtable);

	/* /STROM */


	printf("Input file: %s\nOuput file: %s\n", argv[1], argv[2]);

	if(openInput(argv[1]) || openOutput(argv[2])) {
		error(1);
	}



	parse();

	printf("HOTOVO\n");

	treeDispose(&symtable);

	//error(FINE);
	return FINE;
}	