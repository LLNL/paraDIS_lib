#!/usr/bin/env python
"""
$Id: recvfile.py,v 1.16 2007/05/31 23:05:19 rcook Exp $
recvfile.py:
Receive a file over the given port from a sendfile.py instance.
Basically an unencrypted scp.
For usage, type recvfile.py -help
"""

import os, socket, sys, time, select
from errno import *
from DebugUtils import *
mydir = os.path.dirname(sys.argv[0])
if mydir:
    sys.path.append(mydir)
else:
    sys.path.append('.')

import simpleargs
def recvfile(filename, port, timeout, norecv):
    # insert socket stuff here:
    theSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    theSocket.bind(('', port))
    theSocket.listen(10)
    
    time0 = time.time()
    thetime = 0
    input=[]
    output = []
    dbprint(2, "Waiting for incoming connection"); 
    while not input and not output:
        input, output, exc = select.select([theSocket], [], [], 10)
        if timeout > 0:
            thetime =  time.time() - time0
            if thetime> timeout:
                raise "timeout exceeded %g in recvfile"%timeout
        if not input and not output:
            dbprint(1,  "no connection yet; waiting another 10 seconds after "+str(thetime)+ " seconds")
    if not input or input[0] != theSocket:
        raise "connection timeout in recvfile.py"

    dbprint(2, "connection received")
    conxn, junk = theSocket.accept()
    dbprint (2,"connection accepted")

    # this will generally block: 
    #data = conxn.recv(500)    
    thetime=0
    time0 = time.time()
    
    # open file for writing
    if filename:
        f = open(filename, 'w')
    else:
        f = sys.stdout
    while 1:
        data=""
        input, output, exc = select.select([conxn], [], [], 5)
        if input:
            data = conxn.recv(5000)
            if not data:
                dbprint(2, "End of file")
                sys.exit(0)
            else:
                dbprint(4, "received data: \""+data+"\"")
                f.write(data)
                f.flush()
        else:
            if timeout > 0:
                thetime =  time.time() - time0
                if thetime > timeout:
                    raise "data timeout exceeded in recvfile.py"
                dbprint(2, "Waiting 5 seconds for more data after "+str(thetime)+" seconds")
                time.sleep(5)

    if filename:
        f.close()
    theSocket.shutdown(2)
    
    dbprint (1, "file received")
    return 


def usage():
    print "usage:  recvfile.py [-f filename] [-p port] [-t timeout] [-norecv]"
    print "default filename is stdout"
    print "default port is 1993"
    print "default timeout is never to timeout (use 0 to mean forever)"
    print "if norecv is given, then do not expect data, just accept a connection and exit when connection is broken"
    return

if __name__ == '__main__' :
    filename=['']
    port=[1993]
    timeout=[0]
    help=[]
    norecv=[]
    verbose=[0]
    opts = {
        '-f': filename,
        '-p': port,
        '-t': timeout,
        "-v": verbose
        }
    
    boolopts = {
        '-help': help,
        '-h': help,
        '--help': help,
        '-norecv': norecv
        }
    
    simpleargs.get_args(opts, boolopts)

    if help:
        usage()
        sys.exit(0)

    db_to_stderr()
    SetVerbosity(int(verbose[0]))
    #sys.stderr.write("recvfile.py calling recvfile with port %d"%port[0])
    recvfile(filename[0], port[0], timeout[0], norecv)

    sys.exit(0)
