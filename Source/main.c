#include "define.h"
#include "scanner.h"

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
	int result = 5;

	while(result != T_EOF) {

		clearString(str);
		result = getToken(str);
		printf("%d (%s)\n", result, str);
		if(result == LEX_ERR) break;
	}
	fclose(f);

	return 0;
}