/* 	autor: Radek Wildmann
*	login: xwildm00
* 	
*	Vytvoreno jako soucast projektu do predmetu IFJ v roce 2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ial.h"
#include "string.h"

int IntToStr(char **String, int i){ // int na string
	return sprintf(*String, "%d", i);
}

int DoubleToStr(char **String, double d){ // double na string
	return sprintf(*String, "%g", d);
}

int length(char *s){ // funkce pro vrácení délky řetězce
	return strlen(s);
}

char *SubStr(char* s, int i, int j){ // funkce pro vrácení podřetězce v řtězci s --- TO-DO

}

int Asc(char* s, int i){ // funkce pro vrácení ordinární hodnoty (ASCII) na pozici i v řetězci s --- TO-DO

}

char *Chr(int i){ // funkce pro vrácení jednoznakového řetězce, jehož ASCII je zadán parametrem i --- TO-DO

}