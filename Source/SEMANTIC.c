ID (vzdy promenna) = vyraz (stejny/kompatibilni typ)
Input (vypis !"? ") dle typu id do konce odradkovani (neni soucasti vstupu)
	String muze byt v uvozovkach,  v pripade spatneho formatu, ci nemoznosti konverze nacteme 0 nebo 0.0 nebo !""
Print vyraz1;vyraz2;vyraz3;
	integer %d
	double %g
	string??? %s

If vyraz Then EOL
Else EOL
End If

Do While vyraz EOL
Loop

id = nazev_funkce(par1, par2) // par TYPE_ERR

Return vyraz // Pouze v telech funkci, nekompatibilni navratova hodnota SEM_ERR, chybejici return automaticky 0 nebo 0.0 nebo !""

/******************************/
Konverze int na doble a opacne, jinak TYPE_ERR

+,-,* scitani, odecitani, nasobeni

Operandy:
Integer, Integer = Integer
Double, Integer = Double
Double, Double = Double

String1 + String2 = String1.String2

Operand1 / Operand2 = Double (vzdy)
Integer \ Integer = Integer (Celociselne deleni, vzdy INT)

< > <= >= <> =
Integer Double (-> double double)
DOuble DOuble
Integer Integer
String String

/******************************/
/* var = variable
 * symb = expression
 * type = int, float, string[, bool]
 * label = navesti
 */
sprintf(s, "MOVE %s %s", var, var);
sprintf(s, "MOVE %s %d", var, symb);
sprintf(s, "MOVE %s %g", var, symb);
sprintf(s, "MOVE %s %s", var, symb);
sprintf(s, "DEFVAR %s", var);
sprintf(s, "CALL %d", label);
sprintf(s, "PUSHS %d", symbol);
sprintf(s, "PUSHS %g", symbol);
sprintf(s, "PUSHS %s", symbol);
sprintf(s, "POPS %s", var);
sprintf(s, "ADD %s %d %d", var, symb, symb);
sprintf(s, "ADD %s %g %g", var, symb, symb);
sprintf(s, "SUB %s %d %d", var, symb, symb);
sprintf(s, "SUB %s %g %g", var, symb, symb);
sprintf(s, "DIV %s %g %g", var, symb, symb);

sprintf(s, "LT %s %d %d", var, symb, symb);
sprintf(s, "LT %s %g %g", var, symb, symb);
sprintf(s, "LT %s %s %s", var, symb, symb);
sprintf(s, "GT %s %d %d", var, symb, symb);
sprintf(s, "GT %s %g %g", var, symb, symb);
sprintf(s, "GT %s %s %s", var, symb, symb);
sprintf(s, "EQ %s %d %d", var, symb, symb);
sprintf(s, "EQ %s %g %g", var, symb, symb);
sprintf(s, "EQ %s %s %s", var, symb, symb);

sprintf(s, "INT2FLOAT %s %d", var, symb);
sprintf(s, "FLOAT2INT %s %g", var, symb);
sprintf(s, "FLOAT2R2EINT %s %g", var, symb);
sprintf(s, "FLOAT2R2OINT %s %g", var, symb);
sprintf(s, "INT2CHAR %s %d", var, symb);
sprintf(s, "STRI2INT %s %s %d", var, symb, symb);

sprintf(s, "READ %s %s", var, type); // Konverze float na int
sprintf(s, "WRITE %s@%s", f, symb);

sprintf(s, "CONCAT %s %s %s", var, symb, symb);
sprintf(s, "STRLEN %s %s", var, symb);
sprintf(s, "GETCHAR %s %s %d", var, symb, symb);
sprintf(s, "SETCHAR %s %d %s", var, symb, symb);

sprintf(s, "TYPE %s %s %s", var, symb);

sprintf(s, "LABEL %s", label);
sprintf(s, "JUMP %s", label);
sprintf(s, "JUMPIFEQ %s %s %s", label, symb, symb); // Stejny datovy typ, pokud rovnost pak jump
sprintf(s, "JUMPIFNEQ %s %s %s", label, symb, symb); // Stejny datovy typ, pokud nerovnost pak jump
sprintf(s, "JUMPIFEQS %s", label);
sprintf(s, "JUMPIFNEQS %s", label);

sprintf(s, "DPRINT %s", symb);

instruction(s);
instruction("CREATEFRAME");
instruction("PUSHFRAME");
instruction("POPFRAME");
instruction("RETURN");
instruction("CLEARS");
instruction("ADDS");
instruction("SUBS");
instruction("MULS");
instruction("DIVS");
instruction("LTS");
instruction("GTS");
instruction("EQS");
instruction("BREAK");

/*

Booleovske funkce (instrukce pro bool)
AND/OR/NOT
ANDS/ORS/NOTS

*/