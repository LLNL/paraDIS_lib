/*   Written by Richard David Cook
     at Lawrence Livermore National Laboratory
     Contact:  wealthychef@gmail.com
     See license.txt for information about usage.
     Spoiler alert:  it's GNU opensource.
*/


/*
**
**  Abstract:  Calls to support debug print strings
**
**  Author:
**
*/

#ifndef _debugutil_h
#define _debugutil_h

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>

  /*useful macro -- printf the fmt string onto s, modulo current function varargs */
#define myvasprintf(s, fmt)						\
  va_list ap;									\
	va_start(ap, fmt);							\
	vsprintf(s,fmt,ap);							\
	va_end(ap)

#define vafprintf(s, fmt)						\
  va_list ap;									\
	va_start(ap, fmt);							\
	vfprintf(s,fmt,ap);							\
	va_end(ap)


#define dbprintf(level, args...) if (dbg_isverbose() >= level) __dbprintf(level, args)
#define dbfprintf(stream, level, args...) if (dbg_isverbose() >= level) __dbfprintf(stream, level, args)

  void __dbfprintf(FILE *stream, int level, const char *fmt, ...);
  void __dbprintf(int level, const char *fmt, ...);

  char *datestring(void);

  int dbg_setfile(const char *dbgfile);
  int  dbg_isverbose(void);
  void dbg_setverbose(int verbose);

  void dbg_stderr(char *fmt, ...);
  void dbg_maskstderr(int mask,char *fmt, ...);

#define DBGPRINTF   dbg_stderr
#define DBGMPRINTF  dbg_maskstderr
#define DBGVERBOSE  dbg_isverbose


#define varprint(debug, format, __var) if (debug) {printf(#__var " is '" format "'\n", __var); fflush(stdout);}
#define badvalue(format, __var, astring, errval) if (1) {printf("Bad value of "#__var " while reading file " astring ": '" #format "'\n", __var);  fflush(stdout); return errval;}
#define vectprint(debug, array, length) if (debug) {	\
	int __i=0;											\
	printf("printing array "#array":\n");				\
	for (__i=0; __i<length; __i++)						\
	  printf("%g\t", (float)(array[__i]));				\
	printf("\n");										\
	fflush(stdout);										\
  }

#ifdef __cplusplus
}
#endif
#endif


