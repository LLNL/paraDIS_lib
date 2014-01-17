#!/usr/bin/env python
"""This is stolen from http://dcs.nac.uci.edu/~strombrg/pnetcat.html, an open source project."""
import sys
import posixpath
import socket
import select
import re
import time
import string
import signal
import os
import types

# hmm, this is interesting: 
signal.signal(signal.SIGPIPE, signal.SIG_IGN)

#================================================================
# defaults
default_blocksize=1024*1024L # one megabyte by default
default_tcp_window_size=64*1024

#================================================================
# globals
verbose=0
mode='read from stdin' # purely for user information

readfd = 0  # read from stdin by default
readfn = None # read function
writefd = 1# write to stdout by default
writefn = None # write function

#================================================================
def reset_defaults():
	global verbose,  mode, readfn, writefn, readfd, writefd
	
	verbose=0
	mode='read from stdin'
	
	readfd = 0  # read from stdin by default
	readfn = read_from_file # read function
	writefd = 1# write to stdout by default
	writefn = write_to_file # write function

	return


#================================================================
# if total_to_xfer is 0, then transfer until something interrrupts like EOF or something
def xfer(total_to_xfer=0,block_len=default_blocksize):
	global mode, readfn, writefn
	if verbose:
		sys.stderr.write('%s, %s: total_to_xfer is %d, block_len is %d\n' % (sys.argv[0], mode, total_to_xfer, block_len))
	total_xferred = 0
	if total_to_xfer == 0:
		# read and write forever (or until something interrupts us :)
		# this method is a little faster than the other - less CPU
		if verbose:
			sys.stderr.write('%s, %s: Infinite loop starting\n' % (sys.argv[0], mode))
		while 1:
			block = readfn(block_len)
			if verbose:
				sys.stderr.write('%s, %s: read block (no len for speed)\n' % (sys.argv[0], mode))
			if not block:
				if verbose:
					sys.stderr.write('%s, %s: 0 length block received\n' % (sys.argv[0], mode))
				break
			done=0
			try:
				writefn(block)
			except (socket.error, OSError, IOError), (errno, detail):
				if errno == 32:
					# Broken pipe - just ignore it
					done=1
				else:
					sys.stderr.write('Error: %d %s\n' % (errno, detail))
					sys.exit(1)
			if done:
				break
			#writefn(block[0:actual_block_len])
			if verbose:
				sys.stderr.write('%s, %s: wrote block (no len for speed)\n' % (sys.argv[0], mode))
	else:
		# read and write only up to total_to_xfer bytes
		# this method is a little slower than the other - more CPU
		if verbose:
			sys.stderr.write('%s, %s: Transferring %d bytes (all told)\n' % (sys.argv[0], mode, total_to_xfer))
		while 1:
			if total_xferred + block_len > total_bytes:
				intended_block_len = total_to_xfer - total_xferred
			else:
				intended_block_len = block_len
			if intended_block_len == 0:
				if verbose:
					sys.stderr.write('%s, %s: All bytes transferred\n' % (sys.argv[0], mode))
				break
			block = readfn(intended_block_len)
			if not block:
				if verbose:
					sys.stderr.write('%s, %s: 0 length block received\n' % (sys.argv[0], mode))
				break
			actual_block_len = len(block)
			total_xferred += actual_block_len
			done=0
			try:
				writefn(block)
			except (socket.error, OSError, IOError), (errno, detail):
				if errno == 32:
					# Broken pipe - just ignore it
					done=1
				else:
					sys.stderr.write('Error: %d %s\n' % (errno, detail))
					sys.exit(1)
			#writefn(block[0:actual_block_len])
			if done:
				break
			if verbose:
				sys.stderr.write('%s, %s: Transmitted block of size %d, transferred %d of %d, %3.5s%% complete\n' % (sys.argv[0], mode, actual_block_len, total_xferred, total_to_xfer, (100.0*total_xferred)/total_to_xfer))

#================================================================
# these are used for files, stdio, and TCP sockets:
def read_from_file(length):
	global readfd
	print "readfd is %d and length is %d"%(readfd, length)
	return os.read(readfd,length)

def write_to_file(buffer):
	global writefd
	offset = 0
	buffer_length = len(buffer)
	while 1:
		if offset == 0:
			length_actually_written = os.write(writefd,buffer)
		else:
			# dang slicing...  This is going to duplicate part of the buffer, which is slow...
			length_actually_written = os.write(writefd,buffer[offset:])
		offset += length_actually_written
		if offset >= buffer_length:
			return

#================================================================
# read/write with UDP:
def udpreadfn(sockin,length):
	data, remotehost = sockin.recvfrom(length)
	if verbose:
		sys.stderr.write('Received UDP packet from %s %s\n' % (remotehost))
	return data

def udpwritefn(sockout,buffer):
	sockout.sendto(buffer, (hostname, output_portno))


#================================================================
def type_of_socket(is_tcp):
	if is_tcp:
		return socket.SOCK_STREAM
	else:
		return socket.SOCK_DGRAM
	
#================================================================
def connect_input_socket(port, tcp=1, tcp_nodelay=0, tcp_window_size=default_tcp_window_size):
	global mode, readfn
	input_socket = socket.socket(socket.AF_INET,type_of_socket(tcp))
	input_socket.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
	# this would only bind to the primary hostname of the machine:
	# input_socket.bind((socket.gethostname(),port))
	#
	# '' is supposed to mean "bind to all interfaces"
	input_socket.bind(('',port))
	if tcp:
		input_socket.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, tcp_window_size)
		if verbose:
			sys.stderr.write('Setting TCP_NODELAY to %d\n' % tcp_nodelay)
		input_socket.setsockopt(socket.SOL_TCP,socket.TCP_NODELAY, tcp_nodelay)
		input_socket.listen(0)
		if verbose:
			sys.stderr.write('Waiting for connection...\n')
		input_connection, (remotehost, remoteport) = input_socket.accept()
		if verbose:
			sys.stderr.write('Received connection from %s %s\n' % (remotehost, remoteport))
		readfn=input_connection.recv
	else:
		readfn=lambda length: udpreadfn(input_socket,length)
		
#================================================================
def connect_output_socket(connection, tcp=1, tcp_nodelay=0, tcp_window_size=default_tcp_window_size):
	global mode, writefn
	[hostname, port] = connection
	output_socket = socket.socket(socket.AF_INET,type_of_socket(tcp))
	output_socket.connect((hostname,port))
	if tcp:
		writefn=output_socket.send
	else:
		writefn=lambda length: udpwritefn(output_socket,length)
	if tcp:
		# this one's assumed to only be appropriate for TCP, though there may be other reliable types someday that want this too
		output_socket.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, tcp_window_size)
		if verbose:
			sys.stderr.write('Setting TCP_NODELAY to %d\n' % tcp_nodelay)
		output_socket.setsockopt(socket.SOL_TCP,socket.TCP_NODELAY, tcp_nodelay)

#================================================================
def setup(input_type=0, output_type=0, in_verbose=0, tcp_nodelay=0, tcp_window_size=default_tcp_window_size, input_tcp=1, output_tcp=1):
	# input: 0 = stdin, string = file name, nonzero int is an incoming port number
	# output:  0 = stdout, string = filename, else assumed to be a [host,port] tuple
	global verbose, mode, readfd, writefd
	if verbose: print "input_type:", input_type, "output_type:", output_type, "in_verbose:", in_verbose, "tcp_nodelay:", tcp_nodelay, "tcp_window_size:", tcp_window_size, "input_tcp:", input_tcp, "output_tcp:", output_tcp
	reset_defaults()
	
	verbose = in_verbose
	
	if input_type != 0:
		if type(input_type) == types.StringType:
			readfd = os.open(input_type, os.O_RDONLY)
			modein=input_type
		else:
			connect_input_socket(input_type, input_tcp, tcp_nodelay, tcp_window_size)
			modein='%d'%input_type 
	else:
		modein='stdin'

	if output_type != 0:
		if type(output_type) == types.StringType:
			writefd = os.open(output_type, os.O_WRONLY | os.O_CREAT | os.O_TRUNC)
			modeout = output_type
		else:
			connect_output_socket(output_type, output_tcp, tcp_nodelay, tcp_window_size)
			modeout = 'host %s port %d'%(output_type[0], output_type[1])
	else:
		modeout = 'stdout'

	mode = 'input from %s, output to %s'%(modein, modeout)
	return

#================================================================
def usage(retval):
	# old usage:
	# seki-strombrg> pnetcat
	# You must specify exactly one of -c and -s
	# Usage: /Dcs/seki/strombrg/bin/pnetcat [-b blocksize_in_bytes] [-t total_bytes_to_write] [-c hostname port] [-s port] [-v]
	# Wed Sep 21 16:37:08

	sys.stderr.write('Usage: %s [-b blocksize_in_bytes] [-t total_bytes_to_transfer] [-i|-I port] [-o|-O hostname port|-n] [-w window] [-v]\n' % sys.argv[0])
	sys.stderr.write('\n')
	sys.stderr.write('-i\t\tsays to use file I/O to read from stdin\n')
	sys.stderr.write('-I port\t\tsays to use sockets to get input from "port" on the local machine\n')
	sys.stderr.write('\n')
	sys.stderr.write('-o\t\tsays to use file I/O to write to stdout\n')
	sys.stderr.write('-O\t\thostname port\tsays to use sockets to connect to hostname on port\n')
	sys.stderr.write('-n\t\tsays to only read - do not write the data anywhere (should be faster than writing to /dev/null)\n')
	sys.stderr.write('-w size\t\tsays to set the TCP window size\n')
	sys.stderr.write('\n')
	sys.stderr.write('--O-udp\t\tSame as -O but use UDP socket\n')
	sys.stderr.write('--I-udp\t\tSame as -I but use UDP socket\n')
	sys.stderr.write('-u\t\tUse UDP instead of TCP for all sockets\n')
	sys.stderr.write('\n')
	sys.stderr.write('Naturally, -i conflicts with -I, and -o conflicts with -O\n')
	sys.stderr.write('\n')
	sys.stderr.write("-w window conflicts with -u, because it's for setting the TCP window size\n")
	sys.stderr.write('\n')
	sys.stderr.write("-N [0|1] says to set TCP_NODELAY to 0 or 1 (0 enables Nagel, 1 disables) (_N_agel Algorithm)\n")
	sys.stderr.write('\n')
	sys.stderr.write('Also, -u of course requires -I or -O, --O-udp requires -O and --I-tcp requires -I\n')
	sys.stderr.write('Also note that -u may require small blocksizes compared to TCP\n')
	sys.stderr.write('For example 65508 was once the highest UDP blocksize on a\n')
	sys.stderr.write('run of the mill Fedora Core 4 system\n')
	sys.stderr.write('\n')
	sys.stderr.write('Among -i/-I and -o/-O, the lower case letter does file I/O, and the capital does socket I/O\n')

	sys.exit(retval)

#================================================================
# can translate any string of a number or service (eg 'tcp') into a port number
def portno(service, is_tcp):	
	try:
		result = string.atoi(service)
	except:
		protocol='tcp'
		if not is_tcp: 	protocol='udp'
		try:
			result = socket.getservbyname(service, protocol)
		except (socket.error, msg):
			sys.stderr.write("getservbyname(%s,'%s') failed: %s\n" % (service, protocol, msg))
		
	return result

#================================================================
def getip(host):
	numeric=re.compile('^[0-9][0-9]\.*$')
	if not numeric.match(host):
		dict={}
		# test for round robin DNS or other forms of variability
		reps=10
		for i in range(reps):
			res = socket.gethostbyname(host)
			if dict.has_key(res):
				dict[res] += 1
			else:
				dict[res] = 1
		if len(dict.keys()) == 1:
			return dict.keys()[0]
		else:
			sys.stderr.write('Sorry, there is some variability in hostname lookups for %s\n' % host)
			sys.stderr.write('Please use an IP address instead\n')
			sys.exit(1)
	else:
		return host

#================================================================
if __name__ == "__main__":
	
	reset_defaults()

	input_tcp = 1
	infile=''
	file_in = 0
	do_stdin = 0
	socket_in = 0
	output_tcp = 1
	outfile=''
	do_stdout = 0
	file_out = 0
	socket_out = 0
	tcp_window_size = -1
	input_type = 0
	output_type = 0
	tcp_nodelay = 0
	total_bytes=0L
	blocksize=default_blocksize
	# we are in global scope here, don't need to declare globals
	while sys.argv[1:]:
		# note the del sys.argv[1] at the bottom of the loop - so we only delete sys.argv[1] if there's one or more arguments to the option
		if sys.argv[1] == '-b' and sys.argv[2:]:
			blocksize=string.atol(sys.argv[2])
			del sys.argv[1]
		elif sys.argv[1] == '-if' and sys.argv[2:]:
			file_in = 1
			infile=sys.argv[2]
			del sys.argv[1]
		elif sys.argv[1] == '-i':
			do_stdin=1
		elif sys.argv[1] == '-I' and sys.argv[2:]:
			socket_in=1
			input_port_description = sys.argv[2]
			del sys.argv[1]
		elif sys.argv[1] == '-o':
			do_stdout = 1
		elif sys.argv[1] == '-of' and sys.argv[2:]:
			file_out=1
			outfile=sys.argv[2]
			if os.path.exists(outfile):
				print "%s exists, are you sure you want to clobber it (y/n)?"%outfile
				answer=raw_input()
				if answer != 'y':
					print "OK, aborting, try again with another file name"
					sys.exit(0)
			del sys.argv[1]
		elif sys.argv[1] == '-O' and sys.argv[3:]:
			socket_out=1
			hostname=getip(sys.argv[2])
			output_port_description = sys.argv[3]
			del sys.argv[1]
			del sys.argv[1]
		elif sys.argv[1] == '-N' and sys.argv[2:]:
			tcp_nodelay_selected = 1
			tcp_nodelay_flag = string.atoi(sys.argv[2])
			del sys.argv[1]
		elif sys.argv[1] == '-t' and sys.argv[2:]:
			total_bytes=string.atol(sys.argv[2])
			del sys.argv[1]
		elif sys.argv[1] == '-h':
			usage(0)
		elif sys.argv[1] == '-v':
			verbose=1
		elif sys.argv[1] == '-u':
			output_tcp = 0
			input_tcp = 0
		elif sys.argv[1] == '--O-udp':
			socket_out = 1
			output_tcp = 0
		elif sys.argv[1] == '--I-udp':
			socket_in = 1
			input_tcp = 0
		elif sys.argv[1] == '-w' and sys.argv[2:]:
			tcp_window_size = string.atoi(sys.argv[2])
			del sys.argv[1]
		else:
			sys.stderr.write('Illegal option: %s\n' % sys.argv[1])
			usage(1)
		del sys.argv[1]

	if do_stdin + file_in + socket_in!= 1:
		sys.stderr.write('You must specify exactly one of -i, -if and -I\n')
		usage(1)

	if do_stdout + file_out + socket_out  != 1:
		sys.stderr.write('You must specify exactly one of -o, -of, -O\n')
		usage(1)

	if not input_tcp and not socket_in:
		sys.stderr.write('-u requires -I and/or -O\n')
		usage(1)

	if not output_tcp and not socket_out:
		sys.stderr.write('-u requires -I and/or -O\n')
		usage(1)

	if tcp_window_size != -1 and not input_tcp and not output_tcp:
		sys.stderr.write('-u and -w conflict\n')
		usage(1)

	if tcp_window_size == -1:
		tcp_window_size = default_tcp_window_size

	if socket_in:
		input_type=portno(input_port_description, input_tcp)
	elif infile:
		input_type=infile
	else: # assume stdin
		input_type=0
		
	if socket_out:
		output_type=[hostname,portno(output_port_description, output_tcp)]
	elif outfile:
		output_type=outfile
	else: # assume stdout
		output_type=0		
		
	if verbose:
		sys.stderr.write('%s starting up: %s\n' % (sys.argv[0], mode))

	setup(input_type, output_type, verbose, tcp_nodelay, tcp_window_size, input_tcp, output_tcp)

	
	if verbose:
		sys.stderr.write('%s starting transfer: %s\n' % (sys.argv[0], mode))
		
	xfer(total_bytes,blocksize)

	if verbose:
		sys.stderr.write('Terminating normally: %s\n' % mode)

	sys.exit(0)

