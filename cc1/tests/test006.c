/*
name: TEST006
description: Basic test for if
output:
G1	M	c
F1
X2	F1	main
G2	F1	main	{
-
	j	L2	#I0	#I0	=I
	yI	#I1
	j	L3
L2
	j	L4	#I0	#I0	=I
	j	L5
L4
	j	L6	#I1	#I0	=I
	j	L7	G1	MI	#I0	=I
	yI	#I1
	j	L8
L7
	yI	#I0
L8
	j	L9
L6
	yI	#I1
L9
L5
L3
	yI	#I1
}
*/

char c;

int
main()
{
	if(0) {
		return 1;
	} else if(0) {
		/* empty */
	} else {
		if(1) {
			if(c)
				return 1;
			else
				return 0;
		} else {
			return 1;
		}
	}
	return 1;
}

