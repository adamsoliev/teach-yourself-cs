#!/usr/bin/python3

import sys

n = len(sys.argv)

file_name = sys.argv[0]

for i in range(1, n):
    dval = int(sys.argv[i])
    hval = hex(dval)
    bval = bin(dval).replace("0b", "")
    print(str(dval) + " -- " + str(hval) + " -- " + str(bval)) 



