/*
 * IFJ 2017 project
 *	File			scanner.c
 *	Description		Source file for lexical analysis
 *	Author			Michal Zilvar (xzilva02)
 *	Last update		11:45, 06-10-2017
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

int getToken(String *s){
																											$(function getToken);
	char c;
	int shunt = LEX_WAITING;
	stringClear(s);
	do {
		c = getc(s_sourceFile);
																											$(new Char:);
																											$$("%c\n", c);
		if(c == EOF && shunt != LEX_WAITING && shunt != LEX_KEYWORD) return LEX_ERR;

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
				else if(c == '/') shunt = LEX_BLOCKDIV;

				else if(c == '=') return T_EQ;
				else if(c == ',') return T_COMMA;
				else if(c == EOF) return T_EOF;

				/* Otherwise */
				else {
					/* Numbers and keywords (save the token) */
					if(isdigit(c)) shunt = LEX_DIGIT;
					else if(isalpha(c) || c == '_') shunt = LEX_KEYWORD;
					else return LEX_ERR;

					stringAddData(s, c);
				}
				break;

			/* Is it comment or div */
			case LEX_BLOCKDIV:
				if(c == '/') shunt = LEX_BLOCK;
				else ungetc(c, s_sourceFile);
				break;

			/* We are in a comment */
			case LEX_BLOCK:
				if(c == '/') shunt = LEX_BLOCKE;
				break;

			/* End of block comment */
			case LEX_BLOCKE:
				if(c == '/') shunt = LEX_WAITING;
				else shunt = LEX_BLOCK;
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
				else stringAddData(s, c);
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

						stringAddData(s, asciiVal);
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
						case '"': stringAddData(s, '\"'); break;
						case 'n': stringAddData(s, '\n'); break;
						case 't': stringAddData(s, '\t'); break;
						case '\\':stringAddData(s, '\\'); break;
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
				stringAddData(s, c);
				break;

			/* Float at least 1 digit */
			case LEX_FLOATF:
				if(isdigit(c)) {}
				else return LEX_ERR;
				stringAddData(s, c);
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
				stringAddData(s, c);
				break;

			/* 12e+10 */
			case LEX_EFLOATC:
				if(c == '+' || c == '-') {
					shunt = LEX_EFLOATF;
					stringAddData(s, c);
				}
				else if(isdigit(c)) {
					shunt = LEX_EFLOAT;
					stringAddData(s, c);
				}
				else return LEX_ERR;
				break;

			/* 12e+1 */
			case LEX_EFLOATF:
				if(!isdigit(c)) return LEX_ERR;
				stringAddData(s, c);
				shunt = LEX_EFLOAT;
				break;

			/* 12e+10 */
			case LEX_EFLOAT:
				if(isdigit(c)) {}
				else {
					ungetc(c, s_sourceFile);
					return L_FLOAT;
				}
				stringAddData(s, c);
				break;

			/* ID or keyword */
			case LEX_KEYWORD:

				if(isalnum(c) || c == '_') stringAddData(s, c);
				else {
					ungetc(c, s_sourceFile);

					/* Lower case keyword */
					String *lowerCase = NULL;
					stringInit(&lowerCase);
					if(stringCopyToString(s, lowerCase)) 		return INTERN_ERR;
					makeStringLowerCase(lowerCase);

					/* Valtypes */
					if(!stringCmpChar(lowerCase, "integer"))	return T_INTEGER;
					if(!stringCmpChar(lowerCase, "double"))		return T_DOUBLE;
					if(!stringCmpChar(lowerCase, "string"))		return T_STRING;

					/* Other keywords */
					if(!stringCmpChar(lowerCase, "as"))			return T_AS;
					if(!stringCmpChar(lowerCase, "asc"))		return T_ASC;
					if(!stringCmpChar(lowerCase, "declare"))	return T_DECLARE;
					if(!stringCmpChar(lowerCase, "dim"))		return T_DIM;
					if(!stringCmpChar(lowerCase, "do"))			return T_DO;
					if(!stringCmpChar(lowerCase, "else"))		return T_ELSE;
					if(!stringCmpChar(lowerCase, "end"))		return T_END;
					if(!stringCmpChar(lowerCase, "chr"))		return T_CHR;
					if(!stringCmpChar(lowerCase, "function"))	return T_FUNCTION;
					if(!stringCmpChar(lowerCase, "if"))			return T_IF;
					if(!stringCmpChar(lowerCase, "input"))		return T_INPUT;
					if(!stringCmpChar(lowerCase, "length"))		return T_LENGTH;
					if(!stringCmpChar(lowerCase, "loop"))		return T_LOOP;
					if(!stringCmpChar(lowerCase, "print"))		return T_PRINT;
					if(!stringCmpChar(lowerCase, "return"))		return T_RETURN;
					if(!stringCmpChar(lowerCase, "scope"))		return T_SCOPE;
					if(!stringCmpChar(lowerCase, "subStr"))		return T_SUBSTR;
					if(!stringCmpChar(lowerCase, "then"))		return T_THEN;
					if(!stringCmpChar(lowerCase, "while"))		return T_WHILE;
					if(!stringCmpChar(lowerCase, "and"))		return T_AND;
					if(!stringCmpChar(lowerCase, "boolean"))	return T_BOOLEAN;
					if(!stringCmpChar(lowerCase, "continue"))	return T_CONTINUE;
					if(!stringCmpChar(lowerCase, "elseif"))		return T_ELSEIF;
					if(!stringCmpChar(lowerCase, "exit"))		return T_EXIT;
					if(!stringCmpChar(lowerCase, "false"))		return T_FALSE;
					if(!stringCmpChar(lowerCase, "for"))		return T_FOR;
					if(!stringCmpChar(lowerCase, "next"))		return T_NEXT;
					if(!stringCmpChar(lowerCase, "not"))		return T_NOT;
					if(!stringCmpChar(lowerCase, "or"))			return T_OR;
					if(!stringCmpChar(lowerCase, "shared"))		return T_SHARED;
					if(!stringCmpChar(lowerCase, "static"))		return T_STATIC;
					if(!stringCmpChar(lowerCase, "true"))		return T_TRUE;

					stringFree(lowerCase);

					return T_ID;
				}
				break;
		}
	} while ((2+2*2+2*2) == (3*3+3/3));
}

#endif