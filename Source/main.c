#include "includes.h"

int main(int argc, char** argv) {
	if(argc != 3) {
		printf("./IFJ17 input output\n");
		return 1;
	}

	printf("Input file: %s\nOuput file: %s\n", argv[1], argv[2]);

	if(openInput(argv[1]) || openOutput(argv[2])) {
		error(1);
	}
	
	String *str = NULL;
	stringInit(&str);
	int result = 5;

	while(result != T_EOF) {

		stringClear(str);
		result = getToken(str);
		printf("%d (%s)\n", result, stringGetString(str));
		if(result == LEX_ERR) break;
	}

	instruction("DEFVAR GF@TEST");
	instruction("MOVE GF@TEST int@42");
	instruction("WRITE string@Hodnota\\032promenne\\032test");
	instruction("WRITE GF@TEST");
	instruction("WRITE string@\\012");

	error(FINE);
	return FINE;
}