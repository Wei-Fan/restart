							Time used (usec)
File name		Size of matrices		Trial 1		Trial 2		Trial 3		Average
matmult_pure.py		10x10				340		223		248		270 		~ 10^2
			100x100				196147		193772		194073		194664 		~ 10^5
			1000x1000			260189851	258230751	263468461	260629688 	~ 10^8
matmult_npy.py		10x10				43		30		45		39 		~ 10
			100x100				166		243		174		194 		~ 10^2
			1000x1000			20712		21503		30689		24301 		~ 10^4
matmult.c		10x10				23		17		37		26		~ 10
			100x100				2826		2765		2711		2767		~ 10^3
			1000x1000			5037760		7487589		4932132		5819160		~ 10^6


Comments:
(1)	When sizes of matrices are relative small, time used: pure Python > numpy ~ C.
	When sizes of matrices become relative large, time used: pure Python >> C >> numpy
(2)	When multiplying matrices by definition, C is faster than pure Python.
(3)	Code optimization methods must be used in Numpy.
(4)	When it comes to code efficiency, a good algorithm sometimes can be more important than coding language. 
