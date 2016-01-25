/*
name: TEST052
description: Basic test for initializer
error:
output:
S2	S
M3	I	"a
M4	I	"b
V5	S2	#N2
G6	V5	"arr
(
	#I1
	#I2
	#I3
	#I4
)
G8	F	"main
{
\
	j	L9	G6	M3	.I	#I1	=I
	r	#I1
L9
	j	L11	G6	M4	.I	#I2	=I
	r	#I2
L11
	j	L12	G6	'P	#P4	+P	@S2	M3	.I	#I3	=I
	r	#I3
L12
	j	L13	G6	'P	#P4	+P	@S2	M4	.I	#I4	=I
	r	#I4
L13
	r	#I0
}
*/

struct S {int a; int b;};
struct S arr[2] = {[1] = {3, 4}, [0] = {1, 2}};

int
main()
{
	if(arr[0].a != 1)
		return 1;
	if(arr[0].b != 2)
		return 2;
	if(arr[1].a != 3)
		return 3;
	if(arr[1].b != 4)
		return 4;
	return 0;
}
