#include <stddef.h>

#include "tokens.h"
#include "types.h"
#include "symbol.h"


static struct keyword {
	char *str;
	unsigned char tok;
} keywords [] = {"auto", AUTO,
		 "break", BREAK,
		 "_Bool", CHAR,
		 "case", CASE,
		 "char", CHAR,
		 "const", CONST,
		 "continue", CONTINUE,
		 "default", DEFAULT,
		 "do", DO,
		 "double", DOUBLE,
		 "else", ELSE,
		 "enum", ENUM,
		 "extern", EXTERN,
		 "float", FLOAT,
		 "for", FOR,
		 "goto", GOTO,
		 "if", IF,
		 "int", INT,
		 "long", LONG,
		 "register", REGISTER,
		 "restricted", RESTRICTED,
		 "return", RETURN,
		 "short", SHORT,
		 "signed", SIGNED,
		 "sizeof", SIZEOF,
		 "static", STATIC,
		 "struct", STRUCT,
		 "switch", SWITCH,
		 "typedef", TYPEDEF,
		 "union", UNION,
		 "unsigned", UNSIGNED,
		 "void", VOID,
		 "volatile", VOLATILE,
		 "while", WHILE,
		 NULL, 0,
};

void init_keywords(void)
{
	register struct keyword *bp;
	register struct symbol *sym;
	extern void init_symbol(void);

	init_symbol();
	for (bp = keywords; bp->str; bp++) {
		sym = install(bp->str, hashfun(bp->str));
		sym->tok = bp->tok;
		sym->type = T_KWD;
	}
}