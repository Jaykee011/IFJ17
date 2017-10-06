/*
 * IFJ 2017 project
 *	File			scanner.c
 *	Description		Source file for lexical analysis
 *	Author			Michal Zilvar (xzilva02)
 *	Last update		10:08, 06-10-2017
 */

#ifndef SCANNERC
#define SCANNERC

#include "scanner.h"

FILE * s_sourceFile;
int asciiVal = 0;
int asciiCount = 0;

void setFile(FILE *f) {
	s_sourceFile = f;
}

/* TODO: Remove after string.c */
void addCharToString(char *s, char c) {
	int i;
	for(i = 0; s[i] != '\n'; i++);
	s[i] = c;
}

void strcpyTemp(char *s, char *lowerCase) {
	int i;
	for(i = 0; s[i] != '\0' && i < sizeof(lowerCase)-1; i++)
		lowerCase[i] = s[i];
	lowerCase[i] = '\0';
}

void strlwr(char *s) {
	for(int i = 0; s[i] != '\0'; i++)
		s[i] = tolower(s[i]);
}

int isString(char *s1, char *s2) {
	int i;
	for(i = 0; s1[i] != '\0' && s2[i] != '\0'; i++) {
		if(s1[i] > s2[i]) return s1[i] - s2[i];
		else if(s2[i] > s1[i]) return s2[i] - s1[i];
	}


	if(s1[i] > s2[i]) return s1[i] - s2[i];
	else if(s2[i] > s1[i]) return s2[i] - s1[i];
	return 0;
}
/* END */

int getToken(char *s){
	char c;
	int shunt = LEX_WAITING;

	(void)s;
	//clearString(s);

	do {
		c = getc(s_sourceFile);
		if(c == EOF && shunt != LEX_WAITING) return LEX_ERR;

		switch(shunt) {
			/* Normal reading */
			case LEX_WAITING:

				/* Blank before a token, continue reading */
				if(isspace(c)) shunt = LEX_WAITING;

				/* Complicated symbols */
				else if(c == '!') shunt = LEX_NOT;
				else if(c == '>') shunt = LEX_GREATER;
				else if(c == '<') shunt = LEX_SMALLER;
				else if(c == '\'') shunt = LEX_COMMENT;

				/* Basic alone symbols */
				else if(c == '(') return T_LB;
				else if(c == ')') return T_RB;
				else if(c == ',') return T_COMMA;
				else if(c == ';') return T_SEMICOLON;

				/* Operations */
				else if(c == '+') return T_ADD;
				else if(c == '-') return T_SUB;
				else if(c == '*') return T_TIMES;
				else if(c == '/') return T_DIV;

				else if(c == '=') return T_EQ;
				else if(c == ',') return T_COMMA;
				else if(c == EOF) return T_EOF;

				/* Otherwise */
				else {
					/* Numbers and keywords (save the token) */
					if(isdigit(c)) shunt = LEX_DIGIT;
					else if(isalpha(c) || c == '_') shunt = LEX_KEYWORD;
					else return LEX_ERR;

					addCharToString(s, c);
				}
				break;

			/* String like !"Hello world"	*/
			case LEX_NOT:
				if(isspace(c)) continue;
				if(c == '"') shunt = LEX_STRING;
				else return LEX_ERR;
				break;

			/* We have a string! */
			case LEX_STRING:
				/* End of string? */
				if(c == '"') return L_STRING;

				/* If escape sequence or add char */
				if(c == '\\') shunt = LEX_STRINGE;
				else addCharToString(s, c);
				break;

			case LEX_STRINGE:
				if(isdigit(c)) {
					/* ASCII value of a char */
					asciiVal = 10*asciiVal + (c-'0');
					asciiCount++;

					/* We have its value */
					if(asciiCount == 3) {
						/* Is valid? */
						if(asciiVal < 1 || asciiVal > 255) return LEX_ERR;

						addCharToString(s, asciiVal);
						asciiCount = 0;
						asciiVal = 0;

						shunt = LEX_STRING;
					}
				}
				else
				{
					/* Looked as an ascii value, but was not */
					if(asciiVal) return LEX_ERR;

					shunt = LEX_STRING;
					/* Checking valid */
					switch(c) {
						case '"': addCharToString(s, '\"'); break;
						case 'n': addCharToString(s, '\n'); break;
						case 't': addCharToString(s, '\t'); break;
						case '\\':addCharToString(s, '\\'); break;
						default: return LEX_ERR;
					}
				}
				break;

			/* > or >= */
			case LEX_GREATER:
				if(c == '=') return T_GTE;
				return T_GT;

			/* < or <= or <> */
			case LEX_SMALLER:
				if(c == '=') return T_LTE;
				else if(c == '>') return T_NEQ;
				return T_LT;

			/* Comment like 'comm */
			case LEX_COMMENT:
				if(c == '\n') shunt = LEX_WAITING;
				break;

			/* 123 or 12.3 */
			case LEX_DIGIT:
				if(isdigit(c)) {}
				else if(c == '.') shunt = LEX_FLOATF;
				else if(c == 'e' || c == 'E') shunt = LEX_EFLOATC;
				else {
					ungetc(c, s_sourceFile);
					return L_INT;
				}
				addCharToString(s, c);
				break;

			/* Float at least 1 digit */
			case LEX_FLOATF:
				if(isdigit(c)) {}
				else return LEX_ERR;
				addCharToString(s, c);
				shunt = LEX_FLOAT;
				break;

			/* 12.3 */
			case LEX_FLOAT:
				if(isdigit(c)) {}
				else if(c == 'e' || c == 'E') shunt = LEX_EFLOATC;
				else {
					ungetc(c, s_sourceFile);
					return L_FLOAT;
				}
				addCharToString(s, c);
				break;

			/* 12e+10 */
			case LEX_EFLOATC:
				if(c == '+' || c == '-') {
					shunt = LEX_EFLOATF;
					addCharToString(s, c);
				}
				else if(isdigit(c)) {
					shunt = LEX_EFLOAT;
					addCharToString(s, c);
				}
				else return LEX_ERR;
				break;

			/* 12e+1 */
			case LEX_EFLOATF:
				if(!isdigit(c)) return LEX_ERR;
				addCharToString(s, c);
				shunt = LEX_EFLOAT;
				break;

			/* 12e+10 */
			case LEX_EFLOAT:
				if(isdigit(c)) {}
				else {
					ungetc(c, s_sourceFile);
					return L_FLOAT;
				}
				addCharToString(s, c);
				break;

			/* ID or keyword */
			case LEX_KEYWORD:
				if(isalnum(c) || c == '_') addCharToString(s, c);
				else {
					ungetc(c, source);

					/* Lower case keyword */
					char lowerCase[9];
					strcpyTemp(s, lowerCase);
					strlwr(lowerCase);
					/* Valtypes */
					if(!isString(lowerCase, "integer")) return T_INTEGER;
					if(!isString(lowerCase, "double"))	return T_DOUBLE;
					if(!isString(lowerCase, "string"))	return T_STRING;

					/* Other keywords */
					if(!isString(lowerCase, "as"))		return T_AS;
					if(!isString(lowerCase, "asc"))		return T_ASC;
					if(!isString(lowerCase, "declare"))	return T_DECLARE;
					if(!isString(lowerCase, "dim"))		return T_DIM;
					if(!isString(lowerCase, "do"))		return T_DO;
					if(!isString(lowerCase, "else"))	return T_ELSE;
					if(!isString(lowerCase, "end"))		return T_END;
					if(!isString(lowerCase, "chr"))		return T_CHR;
					if(!isString(lowerCase, "function"))return T_FUNCTION;
					if(!isString(lowerCase, "if"))		return T_IF;
					if(!isString(lowerCase, "input"))	return T_INPUT;
					if(!isString(lowerCase, "length"))	return T_LENGTH;
					if(!isString(lowerCase, "loop"))	return T_LOOP;
					if(!isString(lowerCase, "print"))	return T_PRINT;
					if(!isString(lowerCase, "return"))	return T_RETURN;
					if(!isString(lowerCase, "scope"))	return T_SCOPE;
					if(!isString(lowerCase, "subStr"))	return T_SUBSTR;
					if(!isString(lowerCase, "then"))	return T_THEN;
					if(!isString(lowerCase, "while"))	return T_WHILE;
					if(!isString(lowerCase, "and"))		return T_AND;
					if(!isString(lowerCase, "boolean"))	return T_BOOLEAN;
					if(!isString(lowerCase, "continue"))return T_CONTINUE;
					if(!isString(lowerCase, "elseif"))	return T_ELSEIF;
					if(!isString(lowerCase, "exit"))	return T_EXIT;
					if(!isString(lowerCase, "false"))	return T_FALSE;
					if(!isString(lowerCase, "for"))		return T_FOR;
					if(!isString(lowerCase, "next"))	return T_NEXT;
					if(!isString(lowerCase, "not"))		return T_NOT;
					if(!isString(lowerCase, "or"))		return T_OR;
					if(!isString(lowerCase, "shared"))	return T_SHARED;
					if(!isString(lowerCase, "static"))	return T_STATIC;
					if(!isString(lowerCase, "true"))	return T_TRUE;

					return T_ID;
				}
				break;
		}
	} while ((2+2*2+2*2) == (3*3+3/3));
}

#endif