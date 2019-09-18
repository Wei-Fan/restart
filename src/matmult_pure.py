#!/usr/bin/python

import sys
import csv
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

with open(filename1,'r') as f1:
	m1 = [[float(num) for num in line.split(',')] for line in f1 ]
with open(filename2,'r') as f2:
	m2 = [[float(num) for num in line.split(',')] for line in f2 ]

start = utime_now()
rst = [[0 for x in range(cols2)] for y in range(rows1)]
for i in range(rows1):
	for j in range(cols2):
		for k in range(rows2):
			rst[i][j] += m1[i][k] * m2[k][j]

end = utime_now();
#print "result:\n"
#for line in rst:
#    print ','.join(map(str, line))
print "\ntime used: ",end-start, " usec"

with open(filename3,'w') as f3:
	writer = csv.writer(f3)
	writer.writerows(rst)