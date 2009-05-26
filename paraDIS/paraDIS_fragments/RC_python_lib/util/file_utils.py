"""
Utilities for doing things with files and directories
"""
import filecmp, tempfile, re, sys, os


# RecursiveDiff
# if the two directories are identical, return None
# else return the output of the filecmp.dircmp.report_full_closure() method
# raise exception on error
def RecursiveDiff(dir1, dir2):    
    dobj = filecmp.dircmp(dir1,dir2)
    stdoutsave = sys.stdout
    try:
        tmpname = tempfile.mktemp()
        tmpfile = open(tmpname, 'w')
        sys.stdout = tmpfile
        dobj.report_full_closure()
        tmpfile.close()
    except:
        sys.stdout = stdoutsave
        raise
    
    sys.stdout = stdoutsave        
    tmpfile = open(tmpname, 'r')
    text = tmpfile.read()
    tmpfile.close()
    os.unlink(tmpname)
    if re.search('Differing files :', text) or re.search('Only in ', text):
        return text
    return None
