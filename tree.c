
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#include "cc.h"
#include "syntax.h"
#include "symbol.h"

struct node {
	unsigned char op;
};

struct node_op2 {
	struct node base;
	struct node *left;
	struct node *right;
};

struct nodesym {
	struct node base;
	struct symbol *sym;
};

struct node_comp {
	struct node base;
	uint8_t nr, alloc;
	struct node **body;
};


static unsigned char indent;  /* used for pretty printing the tree*/


struct node *
nodesym(struct symbol *sym)
{
	register struct nodesym *np = xmalloc(sizeof(*np));

	np->base.op = OSYM;
	np->sym = sym;
	return (struct node *) np;
}

struct node *
node(unsigned char op, struct node *l, struct node *r)
{
	register struct node_op2 *np = xmalloc(sizeof(*np));

	np->base.op = op;
	np->left = l;
	np->right = r;

	return (struct node *) np;
}

struct node *
nodecomp(void)
{
	register struct node_comp *np = xmalloc(sizeof(*np));

	np->base.op = OCOMP;
	np->alloc = np->nr = 0;
	np->body = NULL;

	return (struct node *) np;
}

struct node *
addstmt(struct node *p, struct node *stmt)
{
	register uint8_t nr, alloc;
	register struct node_comp *np = (struct node_comp *) p;

	assert(np && np->base.op == OCOMP);
	nr = ++np->nr, alloc = np->alloc;

#define alloc_nr(x) ((((x)+16)*3)/2)
	if (nr > alloc) {
		alloc = alloc_nr(nr);
		np->body = xrealloc(np->body, alloc * sizeof(*np->body));
	}
#undef alloc_nr

	np->body[nr - 1] = stmt;
	np->alloc = alloc;
	return p;
}

static void
prtree_helper(register struct node *np)
{
	static struct optab {
		unsigned char nchild;
		const char *txt;
	} *bp, optab [] = {
		[OCALL] = {2, "()"},
		[OARY] = {2, "[]"},
		[OFIELD] = {2, "."},
		[OPTR] = {2, "->"},
		[OPOSTINC] = {2, ".++"},
		[OPOSTDEC] = {2, ".--"},
		[OPREINC] = {2, "++."},
		[OPREDEC] = {2, "--."},
		[OADDR] = {2, "&."},
		[OINDIR] = {2, "[*]"},
		[OMINUS] = {2, "-."},
		[OPLUS] = {2, "+."},
		[OCPL] = {2, "~"},
		[ONEG] = {2, "!"},
		[OMUL] = {2, "*"},
		[ODIV] = {2, "/"},
		[OMOD] = {2, "%"},
		[OADD] = {2, "+"},
		[OSUB] = {2, "-"},
		[OSHL] = {2, "<<"},
		[OSHR] = {2, ">>"},
		[OLT] = {2, "<"},
		[OGT] = {2, ">"},
		[OGE] = {2, ">="},
		[OLE] = {2, "<="},
		[OEQ] = {2, "=="},
		[ONE] = {2, "!="},
		[OBAND] = {2, "&"},
		[OBXOR] = {2, "^"},
		[OBOR] = {2, "|"},
		[OAND] = {2, "&&"},
		[OOR] = {2, "||"},
		[OTERN] = {2, "?"},
		[OASSIGN] = {2, "="},
		[OA_MUL] = {2, "*="},
		[OA_DIV] = {2, "/="},
		[OA_MOD] = {2, "%="},
		[OA_ADD] = {2, "+="},
		[OA_SUB] = {2, "-="},
		[OA_SHL] = {2, "<<="},
		[OA_SHR] = {2, ">>="},
		[OA_AND] = {2, "&="},
		[OA_XOR] = {2, "^="},
		[OA_OR] = {2, "|="},
		[OSYM] = {0, "sym"},
		[OCOMP] = {255, "comp"},
		[OSWITCH] = {2, "switch"},
		[OIF] = {2, "if"},
		[OFOR] = {2, "for"},
		[OFEXP] = {2, "efor"},
		[ODO] = {2, "do"},
		[OWHILE] = {2, "while"},
		[OLABEL] = {2, "label"},
		[OGOTO] = {2, "goto"},
		[OBREAK] = {2, "break"},
		[OCONT] = {2, "cont"},
		[ORETURN] = {2, "return"},
		[OCASE] = {2, "case"},
		[ODEFAULT] = {2, "default"},
		[OFTN] = {2, "function"},
		[ODEF] = {2, "def"},
		[O2EXP] = { 2, ":"}
	};
	if (!np) {
		fputs(" nil", stdout);
		return;
	}
	assert(np->op < ARRAY_SIZE(optab));
	bp = &optab[np->op];
	if (bp->nchild) {
		register unsigned char i;
		putchar('\n');
		for (i = indent; i != 0; --i)
			putchar(' ');
		printf("(%s", bp->txt);
		indent += 2;
	}
	switch (bp->nchild) {
	case 0: {
		register struct symbol *sym = ((struct nodesym *) np)->sym;
		putchar(' ');
		fputs((sym->name) ? sym->name : ".", stdout);
		return;
	}
	case 2:
		prtree_helper(((struct node_op2 *) np)->left);
		prtree_helper(((struct node_op2 *) np)->right);
		break;
	case 255: {
		register struct node **bp, **lim;

		bp = ((struct node_comp *) np)->body;
		lim = bp + ((struct node_comp *) np)->nr;
		while (bp < lim)
			prtree_helper(*bp++);
		break;
	}
	}
	putchar(')');
	indent -= 2;
}

void
prtree(register struct node *np)
{
	indent = 0;
	prtree_helper(np);
}
