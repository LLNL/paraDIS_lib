"""
This module handles args parsing.  It is analogous to RC_c_lib/args.c and get_args shell function, but is clumsy due to python's namespace rules:  nothing can be passed by reference other than a list.  Thus, all affected variables must be lists, and any values from the command line will be prepended to them.
A more fancy module that allows you to have preference files and such is Prefs.py in this same directory.
get_args returns the index of the first program arg that is not part of an option or the value of an option, so for
myprog -v 5 file1 file2, it would return 3, since sys.argv[3] is just past the 5.  If no such arg exists, it returns 0.  If rejectbadargs is set, it throws an exception if an argument is bad.  Missing values always throw an exception. 
E.g., example usage:  
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

get_args(opts, boolopts)

If your program is called as

myprog -b teehee --bull 0 -v -x

then after calling get_args,the last item in the options become first in list:
bull = ['teehee', 0, 'hahaha']  
x = [1,0]
verbose = [1]

"""

import sys

def get_args(opts, boolopts, rejectbadargs=1):
    firstNonOptionArg=0
    # REGULAR OPTIONS    
    for opt in opts.keys():
        try:
            index = sys.argv.index(opt)
        except:
            continue
        if index == len(sys.argv) - 1:
            raise "Error: option "+opt+" requires a value but none was given"
        
        try: # try as a python expression:
            exec('opts[opt].insert(0,'+sys.argv[index+1]+')')
        except: # insert as a string as a fallback
            opts[opt].insert(0,sys.argv[index+1])
            
        if index+2 > firstNonOptionArg:
            firstNonOptionArg=index+2

    # BOOLEANS
    for opt in boolopts.keys():
        try:
            index = sys.argv.index(opt)
        except:
            continue
        if opt in sys.argv:
            boolopts[opt].insert(0, 1)
            
        if index+1 > firstNonOptionArg:
            firstNonOptionArg=index+1

    if firstNonOptionArg == len(sys.argv):
        firstNonOptionArg = 0

    if rejectbadargs:
        for arg in sys.argv:
            if not arg:
                continue
            if arg[0] == '-' and arg not in opts.keys() and arg not in boolopts.keys():
                raise "Error:  unknown option given: \""+arg+"\""
            
    return firstNonOptionArg
