/* 	autor: Radek Wildmann
*	login: xwildm00
* 	
*	Vytvoreno jako soucast projektu do predmetu IFJ v roce 2017
*/

#include "includes.h"

#define STR_LEN_INC 8 // velikost (byty) na kolik se bude alokovat pocatecni pamet

SPointer *sClearList = NULL;

int stringInit(String** s){ // inicializuje retezec
	if (*s == NULL){ // test, zda-li neni na vstupu NULL

		*s = malloc(sizeof(String)); // Alokujeme dostatek pameti
		if(*s == NULL) return INTERN_ERR; // chyba v alokaci

		(*s)->data = NULL;
		sClearAdd(*s);
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

	return FINE;
}

int stringAddData(String *s1, char c){ // prida na konec retezce 1 znak
	if (s1 != NULL){ // test, zda-li neni na vstupu NULL

		if (s1->size + 1 >= s1->capacity){ // pokud nam nestaci prostor, musime provest realokaci pameti
			s1->data = realloc(s1->data, (s1->size + STR_LEN_INC) * sizeof(char));
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

void makeCharLowerCase(char* s){ // prevede na male znaky
	for(int i = 0; s[i] != '\0'; i++){ // musime zmenit vsechny znaky
		s[i] = tolower(s[i]); // ulozime tentyz znak z vystupu tolower
	}
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

int sClearAdd(String *s) { // Add new pointer to a string
	if(sClearList == NULL) {
		sClearList = malloc(sizeof(SPointer));
		sClearList->p = NULL;
		sClearList->next = NULL;
	}
	if(sClearList == NULL) return INTERN_ERR;

	SPointer *p;
	for(p = sClearList; p->next != NULL; p = p->next) {
		if(p->p == s) return FINE;
	}

	p->p = s;
	p->next = malloc(sizeof(SPointer));
	if(p->next == NULL) return INTERN_ERR;
	p->next->p = NULL;
	p->next->next = NULL;

	return FINE;
}

void sClearRem(String *s) { // Removes a pointer
	if(sClearList == NULL) return; // No allocated

	SPointer *p = sClearList;
	if(p->p == s) { // First in list
		sClearList = p->next;
		stringFree(p->p);
		free(p);
	} else {

		for(; p->next != NULL; p = p->next) {
			if(p->next->p == s) {
				SPointer *tmp = p->next;
				p->next = tmp->next;
				stringFree(tmp->p);
				free(tmp);
				break;
			}
		}
	}
}

void sClear() { // Free all memory
	if(sClearList == NULL) return;

	SPointer *tmp;
	for(SPointer *p = sClearList; p != NULL; p = tmp) {
		tmp = p->next;
		stringFree(p->p);
		free(p);
	}

	sClearList = NULL;
}