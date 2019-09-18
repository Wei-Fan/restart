#!/usr/bin/python

import numpy.matlib
import numpy as np
import sys
import time

def utime_now():
	return int(time.time()*1E6)

filename1 = "A.csv" #default
filename2 = "B.csv" #default
filename3 = "C.csv" #default

a = sys.argv
if len(a)!=5:
	print("ERROR: wrong input format")
	exit(0)
else:
	try:
		rows1 = int(a[1])
		cols1 = int(a[2])
		rows2 = int(a[3])
		cols2 = int(a[4])
	except Exception as e:
		print("ERROR: expected four integer arguments")

assert(rows1>1 and cols1>1 and rows2>1 and cols2>1), "dimensions for matrix should be larger than 1"
assert(a[2]==a[3]),"ERROR: invalid matrix dimensions for multiplication"

m1 = np.loadtxt(filename1,delimiter=",")
m2 = np.loadtxt(filename2,delimiter=",")

start = utime_now()
rst = np.dot(m1,m2)

end = utime_now();
# print "result:"
# for line in rst:
    # print ','.join(map(str, line))
print "\ntime used: ",end-start, " usec"

np.savetxt(filename3,rst,delimiter=",")