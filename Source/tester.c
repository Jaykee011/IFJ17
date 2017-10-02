#include 'tester.h'
#include 'define.h'

int lexicaltest(){
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