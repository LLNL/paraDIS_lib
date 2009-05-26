"""
Utilities for doing things with environment variables
"""

import re, string, os, types
 
# ============================================================
# Expanding environment variables in a string:
matcher=re.compile(r'(\$\{)([^}]*)(\})')
def replacefunc(matchobj):
    #print "key is ", matchobj.group(2)
    if os.environ.has_key(matchobj.group(2)):
        #print "replacing"
        return os.environ[matchobj.group(2)]
    else:
        #print "not replacing"
        return string.join(matchobj.groups(), '')
    
def ExpandEnvVars(item):
    #print "item is", item
    if type(item) == types.ListType:
        item = map(ExpandEnvVars, item)
        return item
    if type(item) == types.StringType:
        value = matcher.sub(replacefunc, item)
        #print "replaced value=", value
        return value
    return item
# ============================================================
