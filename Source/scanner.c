/*
 * IFJ 2017 project
 *	File			scanner.c
 *	Description		Source file for lexical analysis
 *	Author			Michal Zilvar (xzilva02)
 *	Last update		03:37, 10-10-2017
 */

#ifndef SCANNERC
#define SCANNERC

#include "includes.h"

FILE *s_inputFile;
int asciiVal = 0;
int asciiCount = 0;

int openInput(char *s) {
	s_inputFile = fopen(s, "r");
	if(!s_inputFile) return FAIL;

	return FINE;
}

void closeInput() {
	fclose(s_inputFile);
}

int getToken(String *s){
	char c;
	int shunt = LEX_WAITING;
	stringClear(s);
	do {
		c = getc(s_inputFile);

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
				else if(c == '\\') return T_IDIV;
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
				if(c == '\'') shunt = LEX_BLOCK;
				else ungetc(c, s_inputFile);
				break;

			/* We are in a comment */
			case LEX_BLOCK:
				if(c == '\'') shunt = LEX_BLOCKE;
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
					ungetc(c, s_inputFile);
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
					ungetc(c, s_inputFile);
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
					ungetc(c, s_inputFile);
					return L_FLOAT;
				}
				stringAddData(s, c);
				break;

			/* ID or keyword */
			case LEX_KEYWORD:

				if(isalnum(c) || c == '_') stringAddData(s, c);
				else {
					ungetc(c, s_inputFile);

					/* Lower case keyword */
					char *lowerCase = NULL;
					stringToChar(&lowerCase, s);
					makeCharLowerCase(lowerCase);

					/* Valtypes */
					if(!strcmp(lowerCase, "integer"))	{free(lowerCase); return T_INTEGER;}
					if(!strcmp(lowerCase, "double"))	{free(lowerCase); return T_DOUBLE;}
					if(!strcmp(lowerCase, "string"))	{free(lowerCase); return T_STRING;}

					/* Other keywords */
					if(!strcmp(lowerCase, "as"))		{free(lowerCase); return T_AS;}
					if(!strcmp(lowerCase, "asc"))		{free(lowerCase); return T_ASC;}
					if(!strcmp(lowerCase, "declare"))	{free(lowerCase); return T_DECLARE;}
					if(!strcmp(lowerCase, "dim"))		{free(lowerCase); return T_DIM;}
					if(!strcmp(lowerCase, "do"))		{free(lowerCase); return T_DO;}
					if(!strcmp(lowerCase, "else"))		{free(lowerCase); return T_ELSE;}
					if(!strcmp(lowerCase, "end"))		{free(lowerCase); return T_END;}
					if(!strcmp(lowerCase, "chr"))		{free(lowerCase); return T_CHR;}
					if(!strcmp(lowerCase, "function"))	{free(lowerCase); return T_FUNCTION;}
					if(!strcmp(lowerCase, "if"))		{free(lowerCase); return T_IF;}
					if(!strcmp(lowerCase, "input"))		{free(lowerCase); return T_INPUT;}
					if(!strcmp(lowerCase, "length"))	{free(lowerCase); return T_LENGTH;}
					if(!strcmp(lowerCase, "loop"))		{free(lowerCase); return T_LOOP;}
					if(!strcmp(lowerCase, "print"))		{free(lowerCase); return T_PRINT;}
					if(!strcmp(lowerCase, "return"))	{free(lowerCase); return T_RETURN;}
					if(!strcmp(lowerCase, "scope"))		{free(lowerCase); return T_SCOPE;}
					if(!strcmp(lowerCase, "subStr"))	{free(lowerCase); return T_SUBSTR;}
					if(!strcmp(lowerCase, "then"))		{free(lowerCase); return T_THEN;}
					if(!strcmp(lowerCase, "while"))		{free(lowerCase); return T_WHILE;}
					if(!strcmp(lowerCase, "and"))		{free(lowerCase); return T_AND;}
					if(!strcmp(lowerCase, "boolean"))	{free(lowerCase); return T_BOOLEAN;}
					if(!strcmp(lowerCase, "continue"))	{free(lowerCase); return T_CONTINUE;}
					if(!strcmp(lowerCase, "elseif"))	{free(lowerCase); return T_ELSEIF;}
					if(!strcmp(lowerCase, "exit"))		{free(lowerCase); return T_EXIT;}
					if(!strcmp(lowerCase, "false"))		{free(lowerCase); return T_FALSE;}
					if(!strcmp(lowerCase, "for"))		{free(lowerCase); return T_FOR;}
					if(!strcmp(lowerCase, "next"))		{free(lowerCase); return T_NEXT;}
					if(!strcmp(lowerCase, "not"))		{free(lowerCase); return T_NOT;}
					if(!strcmp(lowerCase, "or"))		{free(lowerCase); return T_OR;}
					if(!strcmp(lowerCase, "shared"))	{free(lowerCase); return T_SHARED;}
					if(!strcmp(lowerCase, "static"))	{free(lowerCase); return T_STATIC;}
					if(!strcmp(lowerCase, "true"))		{free(lowerCase); return T_TRUE;}
																											$$("identifier %s\n", stringGetString(s));
					free(lowerCase);

					return T_ID;
				}
				break;
		}
	} while ((2+2*2+2*2) == (3*3+3/3));
}

void pushbackAttr(int l) {
	fseek(s_inputFile, -l, SEEK_CUR);
																											$$("pushbackAttr(%d);\n", l);
}

#endif