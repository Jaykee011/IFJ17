// Magic constants for errors and lexical tokens
// Author: xzaple36 (Jakub Zapletal)

#ifndef DEFINEH
#define DEFINEH

//
//  ERRORS
//

#define LEX_ERR         1
#define SYN_ERR         2
#define DEF_ERR         3
#define TYPE_ERR        4
#define SEM_ERR         6
#define INTERN_ERR      99

// 
//  TOKENS
//
#define ID              100 // IDENTIFIER

//  LITERALS
#define L_INT           101
#define L_FLOAT         102
#define L_STRING        103

//  TYPE KEYWORDS
#define INTEGER         104
#define DOUBLE          105
#define STRING          106

//  OPERATORS
#define EXCLAMATION     107 // !
#define LB              108 // (
#define RB              109 // )
#define COMMA           110 // ,
#define SEMICOLON       111 // ;
#define ADD             112 // +
#define SUBSTR          113 // -
#define TIMES           114 // *
#define DIV             115 // /
#define GT              116 // >
#define LT              117 // <
#define GTE             118 // >=
#define LTE             119 // <=
#define EQ              120 // =
#define NEQ             121 // <>

//
//  OTHER KEYWORDS
//
#define AND             150
#define AS              151
#define ASC             152
#define BOOLEAN         153
#define CHR             154
#define CONTINUE        155
#define DECLARE         156
#define DIM             157
#define DO              158
#define ELSE            159
#define ELSEIF          160
#define END             161
#define EXIT            161
#define FALSE           162
#define FOR             163
#define FUNCTION        164
#define IF              165
#define INPUT           166
#define LENGTH          167
#define LOOP            168
#define OR              169
#define NEXT            170
#define NOT             171
#define PRINT           172
#define RETURN          173
#define SHARED          174
#define SCOPE           175
#define STATIC          176
#define SUBSTR          177
#define THEN            178
#define TRUE            179
#define WHILE           180

#define EOL             198 // \n
#define EOF             199

#endif