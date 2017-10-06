/* 	autor: Radek Wildmann
*	login: xwildm00
* 	
*	Vytvoreno jako soucast projektu do predmetu IFJ v roce 2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// #include <string.h> // pro testy
#include "ial.h"
#include "string.h"

int IntToStr(char **String, int i){ // int na string
	return sprintf(*String, "%d", i);
}

int DoubleToStr(char **String, double d){ // double na string
	return sprintf(*String, "%g", d);
}

int length(char *s){ // funkce pro vrácení délky řetězce --- TO-DO 
	return strlen(s);
}