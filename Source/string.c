/* 	autor: Radek Wildmann
*	login: xwildm00
* 	
*	Vytvoreno jako soucast projektu do predmetu IFJ v roce 2017
*/

#include "includes.h"

#define STR_LEN_INC 8 // velikost (byty) na kolik se bude alokovat pocatecni pamet

int stringInit(String** s){ // inicializuje retezec
	if (*s == NULL){ // test, zda-li neni na vstupu NULL

		*s = saveMalloc(sizeof(String)); // Alokujeme dostatek pameti
		if(*s == NULL) return INTERN_ERR; // chyba v alokaci

		(*s)->data = NULL;
		return stringClear(*s);
	}
	return INTERN_ERR; // pro pripad, ze se neprovede ani jeden if
}

void stringFree(String *s){ // uvolni pamet
	if (s != NULL){ // test, zda-li neni na vstupu NULL
		if(s->data != NULL) saveFree(s->data);
		saveFree(s);
	}
}

int stringClear(String *s){ // vymaze obsah -> nastavi vychozi hodnoty
	if(s == NULL) return INTERN_ERR;

	if(s->data != NULL) saveFree(s->data);
	s->data = saveMalloc(STR_LEN_INC * sizeof(char));
	if(s->data == NULL) return INTERN_ERR;

	s->data[0] = '\0'; // pro pripad vypisu prazdneho retezce
	s->size = 0;
	s->capacity = STR_LEN_INC;

	return FINE;
}

int stringAddData(String *s1, char c){ // prida na konec retezce 1 znak
	if (s1 != NULL){ // test, zda-li neni na vstupu NULL

		if (s1->size + 1 >= s1->capacity){ // pokud nam nestaci prostor, musime provest realokaci pameti
			s1->data = saveRealloc(s1->data, (s1->size + STR_LEN_INC) * sizeof(char));
			if (s1->data == NULL) // test, zda-li alokace probehla v poradku
				return INTERN_ERR;
			s1->capacity = s1->size + STR_LEN_INC; // zvetsime o 8 bytu
		}

		s1->data[s1->size] = c; // vlozime novy znak
		s1->size = s1->size + 1; // zvetsime velikost o 1 (1 znak)
		s1->data[s1->size] = '\0'; // a pridame ukoncujici znak
		return FINE;
	}
	return INTERN_ERR; // pro pripad, ze se neprovede ani jeden if
}

void makeStringLowerCase(String *s){ // prevede na male znaky
	if(s != NULL) {
		for(int i = 0; i < s->size; i++)
			(s->data)[i] = tolower((s->data)[i]);
	}
}

int stringCmpConstString(String *s1, char *s2){ // porovna velikost retezce s konstantnim retezcem
	if (s1 != NULL){ // test, zda-li neni na vstupu NULL
		return strcmp(s1->data, s2);
	}
	
	return INTERN_ERR; // pro pripad, ze se neprovede ani jeden if
}

char *stringGetString(String *s){ // vraci textovou cast retezce
	if (s != NULL){ // test, zda-li neni na vstupu NULL
		return s->data;
	}
	return NULL; // pro pripad, ze se neprovede ani jeden if
}

int stringGetSize(String *s){ // vraci delku retezce
	if (s != NULL){ // test, zda-li neni na vstupu NULL
		return s->size;
	}
	return INTERN_ERR; // pro pripad, ze se neprovede ani jeden if
}

void stringToChar(char **c, String *s){ // prekopirovani "naseho" stringu do c charu
	if (s != NULL){ // test, zda-li neni na vstupu NULL
		if (s->size == 0){ // pokud neexistuje zadny retezec
			(*c) = saveMalloc(sizeof(char)); // naalokuje velikost charu
			(*c)[0] = '\0'; // pro pripad vypisu prazdneho retezce
		}
		else{
			(*c) = saveMalloc((sizeof(char)) * (s->size + 1));
			strncpy(*c, s->data, s->size); // pro zamezeni prekopirovani velmi velkeho poctu znaku (hazelo by chyby)
			(*c)[s->size] = '\0'; // vlozime ukoncovaci znak na konec
		}
	}
}