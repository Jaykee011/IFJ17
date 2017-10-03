/*
 * IFJ 2017 project
 *	File			scanner.c
 *	Description		Source file for lexical analysis
 *	Author			Michal Zilvar (xzilva02)
 *	Last update		13:34, 03-10-2017
 */

#ifndef SCANNERC
#define SCANNERC

#include "tester.h"
#include "scanner.h"

FILE * s_sourceFile;

void setFile(FILE *f){
	s_sourceFile = f;
}

int scannerFailed() {
	fprintf(stderr, "LEX_ERROR");
	return FAIL;
}

int getToken(char *s){
	char c;
	int shunt = LEX_WAITING;

	(void)s;
	//clearString(s);

	do {
		c = getc(s_sourceFile);
		if(c == EOF && shunt != LEX_WAITING) return scannerFailed();

		switch(shunt) {
			/* Normal reading */
			case LEX_WAITING:

				/* Blank before a token, continue reading */
				if(isspace(c)) shunt = LEX_WAITING;

				/* Complicated symbols */
				else if(c == '!') shunt = LEX_NOT;
				else if(c == '>') shunt = LEX_GREATER;
				else if(c == '<') shunt = LEX_SMALLER;
				else if(c == "'") shunt = LEX_COMMENT;

				/* Basic alone symbols */
				else if(c == '(') return LB;
				else if(c == ')') return RB;
				else if(c == ',') return COMMA;
				else if(c == ';') return SEMICOLON;

				/* Operations */
				else if(c == '+') return ADD;
				else if(c == '-') return SUBSTR;
				else if(c == '*') return TIMES;
				else if(c == '/') return DIV;

				else if(c == '=') return EQ;
				//else if(c == ',') shunt = LEX_COMMA;
				else if(c == EOF) return C_EOF;

				/* Otherwise */
				else {
					/* Numbers and keywords (save the token) */
					if(isdigit(c)) shunt = LEX_DIGIT;
					else if(isalpha(c) || c == '_') shunt = LEX_KEYWORD;
					else return scannerFailed();

					//addCharToString(s, c);
				}
				break;

			/* String like !"Hello world"	*/
			case LEX_NOT:
				if(isspace(c)) continue;
				if(c == '"') shunt = LEX_STRING;
				else return scannerFailed();
				break;

			/* We have a string! */
			case LEX_STRING:
				if(c == '"') return L_STRING;
				//addCharToString(s, c);
				break;

			/* > or >= */
			case LEX_GREATER:
				if(c == '=') return GTE;
				return GT;

			/* < or <= or <> */
			case LEX_SMALLER:
				if(c == '=') return LTE;
				else if(c == '>') return NEQ;
				return LT;

			/* Comment like 'comm */
			case LEX_COMMENT:
				if(c == '\n') shunt = LEX_WAITING;
				break;

			/* 123 or 12.3 */
			case LEX_DIGIT:
				if(isdigit(c)) {}
				else if(c == '.') shunt = LEX_FLOATF;
				else if(c == 'e' || c == "E") shunt = LEX_EFLOATC;
				else {
					ungetc(c, s_sourceFile);
					return L_INT;
				}
				//addCharToString(s, c);
				break;

			/* Float at least 1 digit */
			case LEX_FLOATF:
				if(isdigit(c)) {}
				else return scannerFailed();
				//addCharToString(s, c);
				shunt = LEX_FLOAT;
				break;

			/* 12.3 */
			case LEX_FLOAT:
				if(isdigit(c)) {}
				else if(c == 'e' || c == "E") shunt = LEX_EFLOATC;
				else {
					ungetc(c, s_sourceFile);
					return L_FLOAT;
				}
				//addCharToString(s, c);
				break;

			/* 12e+10 */
			case LEX_EFLOATC:
				if(c == '+' || c == '-') {
					shunt = LEX_EFLOATF;
					//addCharToString(s, c);
				}
				else if(isdigit(c)) {
					shunt = LEX_EFLOAT;
					//addCharToString(s, c);
				}
				else return scannerFailed();
				break;

			/* 12e+1 */
			case LEX_EFLOAT:
				if(!sdigit(c)) return scannerFailed();
				//addCharToString(s, c);
				shunt = LEX_EFLOAT;
				break;

			/* 12e+10 */
			case LEX_EFLOAT:
				if(isdigit(c)) {}
				else {
					ungetc(c, s_sourceFile);
					return L_FLOAT;
				}
				//addCharToString(s, c);
				break;

			/* ID or keyword */
			case LEX_KEYWORD:
				if(isalnum(c) || c == '_') addCharToString(s, c);
				else {
					ungetc(c, source);
					/* TODO: defined characters, valtypes */
					return ID;
				}
				break;
		}
	} while ((2+2*2+2*2) == (3*3+3/3));
}

#endif