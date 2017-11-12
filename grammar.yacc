%token eof id lb rb As eol End Function comma Scope Return semicolon Dim Input If Then Else Do While Loop Integer literal Double String Declare EXPR equals
%% /* LL(1) */
PROGRAM : FUNCTIONS SCOPE eof;

FUNCTIONS : FUNCTION FUNCTIONS
  | /*eps*/;

FUNCTION : Declare Function id lb PARAMS rb As TYPE eol
  | Function id lb PARAMS rb As TYPE eol FCOMMANDS End Function;

PARAMS :  PARAM NPARAM
  |  /*eps*/ ;

NPARAM : comma PARAM NPARAM
  | /*eps*/;

PARAM : id As TYPE;

SCOPE : Scope eol SCOMMANDS End Scope;

FCOMMANDS : FCOMMAND FCOMMANDS
  | COMMAND FCOMMANDS
  | /*eps*/;

FCOMMAND : Return EXPR eol;

SCOMMANDS : SCOMMAND SCOMMANDS
  | COMMAND SCOMMANDS
  | /*eps*/;

SCOMMAND : VARDEF eol;

COMMANDS : COMMAND COMMANDS
  | /*eps*/;

COMMAND : id INIT eol
  | INPUT eol
  | PRINT eol
  | BRANCH eol
  | LOOP eol;

VARDEF : Dim id As TYPE INIT;

INIT : equals EXPR
  | equals FCALL
  | /*eps*/;

FCALL : id lb CPARAMS rb;

CPARAMS : CPARAM NCPARAM
  | /*eps*/;

NCPARAM : comma CPARAM NCPARAM
  | /*eps*/ ;

CPARAM : TERM;

INPUT : Input id;
%token eof id lb rb As eol End Function comma Scope Return semicolon Dim Input If Then Else Do While Loop Integer literal Double String Declare EXPR equals
%% /* LL(1) */
PROGRAM : FUNCTIONS SCOPE eof;

FUNCTIONS : FUNCTION FUNCTIONS
  | /*eps*/;

FUNCTION : Declare Function id lb PARAMS rb As TYPE eol
  | Function id lb PARAMS rb As TYPE eol FCOMMANDS End Function;

PARAMS :  PARAM NPARAM
  |  /*eps*/ ;

NPARAM : comma PARAM NPARAM
  | /*eps*/;

PARAM : id As TYPE;

SCOPE : Scope eol SCOMMANDS End Scope;

FCOMMANDS : FCOMMAND FCOMMANDS
  | COMMAND FCOMMANDS
  | /*eps*/;

FCOMMAND : Return EXPR eol;

SCOMMANDS : SCOMMAND SCOMMANDS
  | COMMAND SCOMMANDS
  | /*eps*/;

SCOMMAND : VARDEF eol;

COMMANDS : COMMAND COMMANDS
  | /*eps*/;

COMMAND : id equals INIT eol
  | INPUT eol
  | PRINT eol
  | BRANCH eol
  | LOOP eol;

VARDEF : Dim id As TYPE DEFINIT;

DEFINIT : equals INIT
  |  /*eps*/;

INIT : EXPR
  | FCALL;

FCALL : id lb CPARAMS rb;

CPARAMS : CPARAM NCPARAM
  | /*eps*/;

NCPARAM : comma CPARAM NCPARAM
  | /*eps*/ ;

CPARAM : TERM;

INPUT : Input id;

PRINT : EXPR semicolon NEXPR;

NEXPR : EXPR semicolon NEXPR
  | /*eps*/;

BRANCH : If EXPR Then eol COMMANDS Else eol COMMANDS End If;

LOOP : Do While EXPR eol COMMANDS Loop;

TYPE : Integer
  | Double
  | String;

TERM : literal
  | id;
PRINT : EXPR semicolon NEXPR;

NEXPR : EXPR semicolon NEXPR
  | /*eps*/;

BRANCH : If EXPR Then eol COMMANDS Else eol COMMANDS End If;

LOOP : Do While EXPR eol COMMANDS Loop;

TYPE : Integer
  | Double
  | String;

TERM : literal
  | id;