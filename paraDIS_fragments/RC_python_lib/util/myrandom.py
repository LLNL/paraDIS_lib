#!/usr/bin/env python
"""
Generate random number between min and max (0 and 1.0 are the default)
"""


import os, sys, random

mydir = os.path.dirname(sys.argv[0])
if mydir:
    sys.path.append(mydir)
else:
    sys.path.append('.')

import simpleargs

def myrandom(imin, imax, integers=0):
    random.seed()

    if integers:
        return random.randrange(imin, imax+1)

    return imin + (imax-imin)*random.random()

def usage():
    print "usage:  random.py [-min min] [-max max] [-int] [-help/--help]"
    print "min and max are self-explanatory and are inclusive"
    print "if -int is given, generate integer values only"
    print "default is min = 0.0 and max = 1.0"
    return

if __name__ == '__main__' :
    fmin = [0.0]
    fmax=[1.0]
    help=[]
    integers=[0]
    opts = {
        '-min': fmin,
        '-max': fmax,
        }
    
    boolopts = {
        '-help': help,
        '--help': help,
        '-int': integers
        }
    
    
    simpleargs.get_args(opts, boolopts)

    if help:
        usage()
        sys.exit(0)

    print myrandom(fmin[0], fmax[0], integers[0])

