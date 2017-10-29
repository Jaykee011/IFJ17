#include "includes.h"

int main(int argc, char** argv) {
	if(argc != 2) {
		printf("./IFJ17 input.file\n");
		return 1;
	}

	printf("Reading from file: %s\n\n", argv[1]);

	FILE *f = fopen(argv[1], "r");
	setFile(f);
	
	String *str = NULL;
	stringInit(&str);
	int result = 5;

	while(result != T_EOF) {

		stringClear(str);
		result = getToken(str);
		printf("%d (%s)\n", result, stringGetString(str));
		if(result == LEX_ERR) break;
	}
	stringFree(str);
	fclose(f);

	return 0;
}