#!/usr/bin/python

import numpy.matlib
import numpy as np
import sys

filename = "A.csv" #default

a = sys.argv

if len(a)!=3 and len(a)!=4:
	print("ERROR: wrong input format, please type in right format: rows cols filename")
	exit(0)
else:
	try:
		rows = int(a[1])
		cols = int(a[2])
	except Exception as e:
		print("ERROR: expected two integer arguments")

	if len(a)==4:
		try:
			filename = a[3]
		except Exception as e:
			print("ERROR: invalid file name")

rst = np.matlib.randn(rows,cols)
np.savetxt(filename,rst,delimiter=",")
		

print "done"