#!/usr/bin/env python
"""
$Id: sendfile.py,v 1.16 2007/05/31 23:47:44 rcook Exp $
sendfile.py:
Send a file over the given port to a receivefile.py instance on the other side.
Basically an unencrypted scp that relies on user running receivefile.py to be evidence that they are legit.  For usage, type sendfile.py -help
"""

import os, socket, sys, time
from errno import *
from DebugUtils import *
mydir = os.path.dirname(sys.argv[0])
if mydir:
    sys.path.append(mydir)
else:
    sys.path.append('.')

import simpleargs


def sendfile(filename, host, port, timeout, nosend, wait, nolb, tail):
    dbprint(3, "sendfile called")
    theSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    time0 = time.time()
    thetime = 0
    while 1:
        theSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        err = theSocket.connect_ex((host, port))
        if not err:
            dbprint(1, "sendfile.py connected!")
            break
        if err != ECONNREFUSED:
            raise "sendfile.py error while connecting to %s:%d "%(host, port)+ os.strerror(err)
        dbprint (2,"sendfile.py waiting to connect  to %s:%d after "%(host, port)+str(thetime)+" secs...")
        sys.stdout.flush()
        if timeout > 0:
            thetime =  time.time() - time0
            if thetime> timeout:
                raise "sendfile.py timeout exceeded"
        time.sleep(5)

    if not nosend:
        if filename:
            while 1:
                try:
                    dbprint(5, "opening file "+filename); 
                    f = open(filename, 'r')
                    if f:
                        break
                    else:
                        raise "Could not open file "+filename
                except:
                    dbprint(5, "got exception opening file")
                    if wait:
                        dbprint(3, "sendfile.py waiting another 5 seconds for "+filename+ " to appear")
                        time.sleep(5)                    
                    else:
                        raise
        else:
            f = sys.stdin
            
        dbprint(5, "sedndfile.py file "+filename+" opened")

        if nolb: # no line buffering, send whole file
            contents = f.read()
            dbprint(5, "read "+str(len(contents))+" bytes from file")
            if filename:
                f.close()
                
            dbprint (1, "sendfile.py sending data without line buffering...")
            contents = theSocket.send(contents)
            tosend=len(contents)
            sent=0
            while sent < tosend:
                sent += theSocket.send(contents[sent:])                
        else: # send line buffered:
			oldlen = -1
			f.seek(0, 2)
			newlen = f.tell()
			f.seek(0, 0)
			while (oldlen < newlen):
				dbprint (1, "sendfile.py sending data with linebuffering...")
				contents=f.readline()
				while contents:
					dbprint (4, "sendfile.py sending line: \"%s\""%contents)
					tosend=len(contents)
					sent=0
					while sent < tosend:
						sent += theSocket.send(contents[sent:])
					contents=f.readline()
				oldlen=newlen
				if tail:
					while oldlen == newlen:
						time.sleep(2)                    
						f.seek(0, 2)
						newlen = f.tell()
					f.seek(oldlen, 0)
    else: # nothing to send, just stay connected forever (deadman-switch mode)
        dbprint(1, "sendfile.py connected; sleeping forever")
        while 1:
            time.sleep(600)
    theSocket.close()
    return 

def usage():
    print "usage:  sendfile.py [-f filename] [-p port] [-t timeout] [-h host] [-nolb] [-nosend] [-v 0-2] [-wait]"
    print "default filename is stdin"
    print "default port is 1993"
    print "default timeout is to never timeout"
    print "default host is localhost"
    print "if nosend is set, then do not send data, just connect and remain connected forever without doing anything (good 'dead man's switch')"
    print "by default verbose is 1"
    print "-nolb means do not use line buffering (good for whole files, bad for standard input, probably)"
    print "-wait means wait until the file named appears, then send it and exit"
    return

if __name__ == '__main__' :
    filename=['']
    tail=[]
    host = ['localhost']
    port=[1993]
    timeout=[0]
    wait=[]
    help=[]
    nosend=[]
    verbose=[0]
    nolb=[]
    opts = {
        '-f': filename,
        '-p': port,
        '-t': timeout,
        '-h': host,
        '-v': verbose, 
        '-wait': wait,
        "-v": verbose
        }
    
    boolopts = {
        '-help': help,
        '--help': help,
        "-nolb": nolb,
        '-nosend': nosend,
		'-tail': tail
        }
    
    
    simpleargs.get_args(opts, boolopts)

    if help:
        usage()
        sys.exit(0)

	db_to_stderr()
    SetVerbosity(int(verbose[0]))
    sendfile(filename[0], host[0], port[0], timeout[0], nosend, wait, nolb, tail)

    dbprint(2, "*** sendfile.py exiting ***"); 
    sys.exit(0)
