#include <stdio.h>
#include <stdlib.h>

#include 'tester.h'
#include 'define.h'


int *loadRefTokens (char *filename){
    FILE *stream;
	char *line = NULL;
	size_t len = 0;
    ssize_t read;
    int tokens[200];
    
    int i = 0;
 
	stream = fopen(filename, "r");
	if (stream == NULL)
		exit(EXIT_FAILURE);
 
	while ((read = getline(&line, &len, stream)) != -1) {
        tokens[i] = atoi(line);
        i++;
    }
 
	free(line);
    fclose(stream);
    
    return tokens;
}

int *loadTokens() {
    int token;
    int tokenBuffer[200];

    int i = 0;

    while (token = getToken(){
        tokenBuffer[i] = token;
        if (token == EOF) break;
    }

    return tokenBuffer;
}

int checkTokens (int *output, int *ref) {
    int i = 0;
    while (output[i] && ref[i] && ref[i] != EOF){
        if (output[i] != ref[i]) return FAIL;
    }
    return SUCC;
}


int lexicaltest(){

    int referenceTokens[200];
    int tokenBuffer[200];

    referenceTokens = loadRefTokens("Tests/lref1");   
    setFile("../Tests/linput1");
    tokenBuffer = loadTokens();
    if (checkTokens(tokenBuffer, referenceTokens)) return FAIL;

    referenceTokens = loadTokens("Tests/lref2");   
    setFile("../Tests/linput2");
    tokenBuffer = loadTokens();
    if (checkTokens(tokenBuffer, referenceTokens)) return FAIL;

    referenceTokens = loadTokens("Tests/lref3");   
    setFile("../Tests/linput3");
    tokenBuffer = loadTokens();
    if (checkTokens(tokenBuffer, referenceTokens)) return FAIL;

    referenceTokens = loadTokens("Tests/lref4");   
    setFile("../Tests/linput4");
    tokenBuffer = loadTokens();
    if (checkTokens(tokenBuffer, referenceTokens)) return FAIL;

    return SUCC;
}

int syntactictest(){
    return SUCC;
}

int semantictest(){
    return SUCC;
}

int translationtest(){
    return SUCC;
}

int main (int argc, int *argv[]) {

    int result;

    switch (argv[1]) {
        case LEXTEST:
            result = lexicaltest();
            break;
        case SYNTEST:
            result = syntactictest();
            break;
        case SEMTEST:
            result = semantictest();
            break;
        case TRANSLTEST:
            result = translationtest();
            break;
    }

    return result;
}