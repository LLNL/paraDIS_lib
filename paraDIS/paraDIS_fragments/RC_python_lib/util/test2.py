
def testfunc(blah):
    exec('global '+blah)
    cmd=blah+"=5"
    print cmd
    exec(cmd)
    
