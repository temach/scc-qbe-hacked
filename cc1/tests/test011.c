/*
name: TEST011
description: Basic test for goto
error:
test011.c:13: warning: 'foo' defined but not used
test011.c:13: warning: 'start' defined but not used
output:
G2	F	"main
{
\
L3
	j	L4
	r	#I1
L5
	r	#I0
L4
L6
	j	L5
	r	#I1
}
*/

#line 1

int
main() {
	start:
		goto next;
		return 1;
	success:
		return 0;
	next:
	foo:
		goto success;
		return 1;
}
