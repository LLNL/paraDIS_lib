#!/usr/bin/env python

import sys
sys.path.append('.')

import simpleargs
x = [0]
verbose = []
bull = ['hahaha']

boolopts={
'-x': x,
'-v': verbose,
'--verbose': verbose
}
opts = {
 '-b': bull,
 '--bull' : bull
 }

simpleargs.get_args(opts, boolopts)


print "if called like so: ./argtest.py -b teehee --bull 0 -v -x"
print " expect the following output:"
print
print 'EXPECTED RESULTS (if called as above):'
print """
x is  [1, 0]
verbose is  [1]
bull is ['teehee', 0, 'hahaha']
"""
print
print 'ACTUAL RESULTS:'
print "x is ", x
print "verbose is ", verbose
print 'bull is', bull
