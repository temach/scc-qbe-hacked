/*
name: TEST045
description: Basic test of initializers
error:
output:
G1	I	"x
(
	#I5
)
G3	F	"main
{
\
	j	L4	G1	#I5	=I
	r	#I1
L4
	r	#I0
}
*/



int x = 5;

int
main()
{
	if(x != 5) 
		return 1;
	return 0;
}