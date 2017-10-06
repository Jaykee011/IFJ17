/* 	autor: Radek Wildmann
*	login: xwildm00
* 	
*	Vytvoreno jako soucast projektu do predmetu IFJ v roce 2017
*/
#ifndef IALH 	// obaleni pro vicenasobny preklad
#define IALH 	// definujeme hlavickovy soubor pro praci s nekonecnym stringem

int length(char *s);
int IntToStr(char **String, int i);
int DoubleToStr(char **String, double d);
char *SubStr(char* s, int i, int j);
int Asc(char* s, int i);
char *Chr(int i);

#endif // #ifndef IALH