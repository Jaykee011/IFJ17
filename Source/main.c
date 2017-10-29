#include "includes.h"

int main(int argc, char** argv) {
	if(argc != 2) {
		printf("./IFJ17 input.file\n");
		return 1;
	}

	printf("Reading from file: %s\n\n", argv[1]);

	FILE *f = fopen(argv[1], "r");
	setFile(f);
	
	char str[100];
	clearString(str);
	int result = getToken(str);

	while(result != T_EOF) {
		printf("%d\n", result);

		clearString(str);
		result = getToken(str);
		if(result == LEX_ERR) break;
	}
	printf("%d\n", result);
	fclose(f);

	return 0;
}