/* 	autor: Radek Wildmann
*	login: xwildm00
* 	
*	Vytvoreno jako soucast projektu do predmetu IFJ v roce 2017
*/

#include <string.h>
#include <malloc.h> // knihovna pro praci s dinamycky alokovanou pameti
#include <ctype.h> // prace se znaky
#include "string.h" // hlavickovy soubor
#include "macros.h" // testovani

#include "define.h" // define pro testování ERR

#define STR_LEN_INC 8 // velikost (byty) na kolik se bude alokovat pocatecni pamet

int stringInit(String** s){ // inicializuje retezec
	if (*s == NULL){ // test, zda-li neni na vstupu NULL
																											$(stringInit initialization);
		*s = malloc(sizeof(String)); // Alokujeme dostatek pameti
		if(*s == NULL) return INTERN_ERR; // chyba v alokaci
																											$(stringInit structure allocated);
		(*s)->data = NULL;
		return stringClear(*s);
	}
	return INTERN_ERR; // pro pripad, ze se neprovede ani jeden if
}

void stringFree(String *s){ // uvolni pamet
	if (s != NULL){ // test, zda-li neni na vstupu NULL
		if(s->data != NULL) free(s->data);
		free(s);
	}
}

int stringClear(String *s){ // vymaze obsah -> nastavi vychozi hodnoty
	if(s == NULL) return INTERN_ERR;

	if(s->data != NULL) free(s->data);
	s->data = malloc(STR_LEN_INC * sizeof(char));
	if(s->data == NULL) return INTERN_ERR;

	s->data[0] = '\0'; // pro pripad vypisu prazdneho retezce
	s->size = 0;
	s->capacity = STR_LEN_INC;
																											$(stringClear done);
	return FINE;
}

int stringAddData(String *s1, char c){ // prida na konec retezce 1 znak
	if (s1 != NULL){ // test, zda-li neni na vstupu NULL

																											$$("stringAddData: +%c | %s\n", c, s1->data);
		if (s1->size + 1 >= s1->capacity){ // pokud nam nestaci prostor, musime provest realokaci pameti
			s1->data = realloc(s1->data, (s1->size + STR_LEN_INC) * sizeof(char));
			if (s1->data == NULL) // test, zda-li alokace probehla v poradku
				return INTERN_ERR;
			s1->capacity = s1->size + STR_LEN_INC; // zvetsime o 8 bytu
		}
		else{ // pokud prostor dostacuje
			s1->data[s1->size] = c; // vlozime novy znak
			s1->size = s1->size + 1; // zvetsime velikost o 1 (1 znak)
			s1->data[s1->size] = '\0'; // a pridame ukoncujici znak
			return FINE;
		}
	}
																											$(stringAddData INTERN_ERR);
	return INTERN_ERR; // pro pripad, ze se neprovede ani jeden if
}

/*int stringCopyToString(String *s1, String *s2){ // prekopiruje retezec s1 do s2
	if (s1 != NULL && s2 != NULL){ // test, zda-li neni na vstupu NULL
		int tempSize = s1->size; // uchovani velikosti kopirovaneho retezce
		if (tempSize >= s2->capacity){ // pokud nam nestaci prostor, musime provest realokaci pameti
			s2->data = realloc(s2->data, tempSize + 1);
			if (s2->data == NULL) // test, zda-li alokace probehla v poradku
				return INTERN_ERR;
			s2->capacity = tempSize + 1;
		}
		else{ // pokud prostor dostacuje
			strcpy(s2->data, s1->data); 
			s2->size = s2->size + tempSize; // zvetsi velikost o velikost kopirovaneho
			return FINE;
		}
	}
	return INTERN_ERR; // pro pripad, ze se neprovede ani jeden if
}*/

/*void makeStringLowerCase(String* s){ // prevede na male znaky
	if(s != NULL){ // test, zda-li neni na vstupu null
		for(int i = 0; i < stringGetSize(s); i++){ // musime zmenit vsechny znaky
			(s->data)[i] = tolower((s->data)[i]); // ulozime tentyz znak z vystupu tolower
		}
	}
}*/

void makeCharLowerCase(char* s){ // prevede na male znaky
	for(int i = 0; s[i] != '\0'; i++){ // musime zmenit vsechny znaky
		s[i] = tolower(s[i]); // ulozime tentyz znak z vystupu tolower
	}
}

/*int stringCmpString(String *s1, String *s2){ // porovna velikosti dvou retezcu
	if (s1 != NULL && s2 != NULL){ // test, zda-li neni na vstupu NULL
		if(s1->data == NULL && s2->data == NULL) return FINE;
		return strcmp(s1->data, s2->data);
	}
	else if(s1 == NULL && s2 == NULL) return FINE;

	return INTERN_ERR; // pro pripad, ze se neprovede ani jeden if
}*/

/*int stringCmpConstString(String *s1, char *s2){ // porovna velikost retezce s konstantnim retezcem
	if (s1 != NULL){ // test, zda-li neni na vstupu NULL
		if(s1->data == NULL && !strlen(s2)) return FINE;
		return strcmp(s1->data, s2);
	}
	else if(!strlen(s2)) return FINE;

	return INTERN_ERR; // pro pripad, ze se neprovede ani jeden if
}*/

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
			(*c) = malloc(sizeof(char)); // naalokuje velikost charu
			(*c)[0] = '\0'; // pro pripad vypisu prazdneho retezce
		}
		else{
			(*c) = malloc((sizeof(char)) * (s->size + 1));
			strncpy(*c, s->data, s->size); // pro zamezeni prekopirovani velmi velkeho poctu znaku (hazelo by chyby)
			(*c)[s->size] = '\0'; // vlozime ukoncovaci znak na konec
		}
	}
}