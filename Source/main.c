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
	int pushBack = 0;

	while(result != T_EOF) {

		result = getToken(str, &pushBack);
		printf("%d (%s)\n", result, stringGetString(str));
		if(result == LEX_ERR) break;
	}
	//stringClear(str);

	instruction("DEFVAR GF@TEST");
	instruction("DEFVAR GF@TEST2");
	instruction("MOVE GF@TEST int@42");
	instruction("MOVE GF@TEST2 GF@TEST");
	instruction("WRITE string@Hodnota\\032promenne\\032TEST");
	instruction("WRITE GF@TEST");
	instruction("WRITE string@Hodnota\\032promenne\\032TEST2");
	instruction("WRITE GF@TEST2");
	instruction("WRITE string@\\012");

	instructionTest(5, "Ahoj", " ", "koťe", " :D\n", "ASFU\n");

	/*char *var = "allahu";
	char *symb = "akbar";
	instructionTest(3, "INT2FLOAT", var, symb);*/

	error(FINE);
	return FINE;
}