/*
name: TEST046
description: Basic test for initializators
error:
output:
V1	I	#3
G2	V1	x
(
	#I1
	#I2
	#I3
)
F3	I	E
G4	F3	main
{
\
	j	L5	G2	#I1	=I
	r	#I1
L5
	j	L7	G2	'P	#P2	+P	@I	#I2	=I
	r	#I2
L7
	j	L8	G2	'P	#P4	+P	@I	#I3	=I
	r	#I3
L8
	r	#I0
}
*/

int x[3] = {1, 2, 3};

int
main()
{
	if(x[0] != 1) 
		return 1;
	if(x[1] != 2) 
		return 2;
	if(x[2] != 3) 
		return 3;
	return 0;
}
