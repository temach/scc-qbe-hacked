
#include <stdio.h>
#include <stdlib.h>

#include "arch.h"
#include "../../cc2.h"
#include "../../../inc/sizes.h"

enum segment {
	CODESEG,
	DATASEG,
	BSSSEG,
	NOSEG
};

static int curseg = NOSEG;
static TSIZE offpar, offvar;

static void
segment(int seg)
{
	static char *txt[] = {
		[CODESEG] = "\tCSEG\n",
		[DATASEG] = "\tDSEG\n",
		[BSSSEG] = "\tASEG\n",
	};

	if (seg == curseg)
		return;
	fputs(txt[seg], stdout);
	curseg = seg;
}

static char *
symname(Symbol *sym)
{
	static char name[IDENTSIZ+1];
	static unsigned short id;

	if (sym->name) {
		switch (sym->kind) {
		case GLOB:
		case EXTRN:
			snprintf(name, sizeof(name), "_%s", sym->name);
			return name;
		case PRIVAT:
			return sym->name;
		}
	}

	if (sym->numid == 0 && (sym->numid = ++id) == 0)
		error(EIDOVER);
	sprintf(name, ".%d", sym->numid);

	return name;
}

void
code(int op, Node *to, Node *from)
{
}

static void
label(Symbol *sym)
{
	int seg, flags = sym->type.flags;
	char *name = symname(sym);

	if (flags & FUNF)
		seg = CODESEG;
	else if (flags & INITF)
		seg = DATASEG;
	else
		seg = BSSSEG;
	segment(seg);

	switch (sym->kind) {
	case EXTRN:
		printf("\tEXTRN\t%s\n", name);
		return;
	case GLOB:
		printf("\tPUBLIC\t%s\n", name);
		break;
	}

	printf("%s:\n", name);
}

static void
emitconst(Node *np)
{
	switch (np->type.size) {
	case 1:
		printf("%d", (int) np->u.i & 0xFF);
		break;
	case 2:
		printf("%d", (int) np->u.i & 0xFFFF);
		break;
	case 4:
		printf("%ld", (long) np->u.i & 0xFFFFFFFF);
		break;
	default:
		abort();
	}
}

static void
emittree(Node *np)
{
	if (!np)
		return;

	switch (np->op) {
	case OSTRING:
		printf("\"%s\"", np->u.s);
		free(np->u.s);
		np->u.s = NULL;
		break;
	case OCONST:
		emitconst(np);
		break;
	case OADDR:
		emittree(np->left);
		break;
	case MEM:
		fputs(symname(np->u.sym), stdout);
		break;
	default:
		emittree(np->left);
		printf(" %c ", np->op);
		emittree(np->right);
		break;
	}
}

static void
size2asm(Type *tp)
{
	char *s;

	/*
	 * In z80 we can ignore the alignment
	 */
	if (tp->flags & STRF) {
		s = "\tDB\t";
	} else {
		switch (tp->size) {
		case 1:
			s = "\tDB\t";
			break;
		case 2:
			s = "\tDW\t";
			break;
		case 4:
			s = "\tDD\t";
			break;
		default:
			s = "\tDS\t%llu,";
			break;
		}
	}
	printf(s, (unsigned long long) tp->size);
}

void
newfun()
{
	offpar = offvar = 0;
}

void
defpar(Symbol *sym)
{
	TSIZE align, size;

	if (sym->kind != REG && sym->kind != AUTO)
		return;
	align = sym->type.align;
	size = sym->type.size;

	offpar -= align-1 & ~align;
	sym->u.off = offpar;
	offpar -= size;
	sym->kind = AUTO;
}

void
defvar(Symbol *sym)
{
	TSIZE align, size;

	if (sym->kind != REG && sym->kind != AUTO)
		return;
	align = sym->type.align;
	size = sym->type.size;

	offvar += align-1 & ~align;
	sym->u.off = offvar;
	offvar += size;
	sym->kind = AUTO;
}

void
defglobal(Symbol *sym)
{
	label(sym);
	if (sym->kind == EXTRN || (sym->type.flags & INITF))
		return;
	size2asm(&sym->type);
	puts("0");
}

void
data(Node *np)
{
	size2asm(&np->type);
	emittree(np);
	putchar('\n');
}

void
writeout(void)
{
}

void
endinit(void)
{
}
