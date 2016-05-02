/*   Written by Richard David Cook
     at Lawrence Livermore National Laboratory
     Contact:  wealthychef@gmail.com
     See license.txt for information about usage.
     Spoiler alert:  it's GNU opensource.
*/


// ************************************************************************* //
//                                DebugStream.h                              //
// ************************************************************************* //

#ifndef RCDEBUG_STREAM_H
#define RCDEBUG_STREAM_H


#include <iostream>
#include <fstream>
#include <vector>
#include <signal.h>

namespace rclib {
// ****************************************************************************
//  Class:  DebugStream
//
//  Purpose:
//    An implementation of ostream designed for error and message logging.
//    A DebugStream has a "level" assigned to it as a priority (lower number
//    is higher priority).  Multiple DebugStreams can be created.  When
//    output is written to one DebugStream, it is simultaneously written to
//    all DebugStreams with a higher level.
//
//    In this specific implementation, five debug levels are used.  Only
//    the highest priority messages go to debug1, but every message will go
//    to debug5.
//
//  Programmer:  Jeremy Meredith
//  Creation:    November 16, 2000
//
//  Modified:
//    Jeremy Meredith, Fri Jun  1 17:18:03 PDT 2001
//    Added the ability to check if a DebugStream was enabled.
//
//    Brad Whitlock, Thu Mar 14 13:37:34 PST 2002
//    Added exports.
//
//    Jeremy Meredith, Thu Jan 16 11:40:29 PST 2003
//    Made debug streams macros so that we could skip the logic to
//    do the formatting if the debug output was disabled.  Removed the
//    ability to delete a debug stream automatically on close.
//
// ****************************************************************************
using namespace std;
int get_debug_level(void);
class  DebugStream : public std::ostream {
  public:
  DebugStream(int level_, bool enabled=false);

  ~DebugStream();
    operator       bool()   {
      return enabled;
    };

    void           open(const char *progname);
    void           close();
    static void    Initialize(const char *, int);
    static void    enableTimer(bool onoff=true) {  sTimer = onoff; }
    static void    enableFileLinePrinting(bool onoff=true) { sFileLine = onoff; }
    // example prolog: "server 1:"
    static void    setPrologueText(string prolog) { sPrologueText = prolog;}
    // print the prologue:
    string prologue(char *file, int line) const;

    bool Enabled(void) { return enabled; }
  private:
    class DebugStreamBuf : public std::streambuf
    {
      public:
      DebugStreamBuf();
      ~DebugStreamBuf();

      void           SetLevel(int level_);
      void           close();
      void           open(const char *filename_);
      int            put(int c);
      virtual int    overflow(int c);
      virtual int    underflow() { return 0; };
      private:
        // the priority of the current messages being logged
        static int                            curLevel;
        // the list of all buffers
        static std::vector<DebugStreamBuf*>   allBuffers;

        // the level of this DebugStreamBuf
        int                                   level;
        // the filename being to which to write the messages
        char                                  filename[256];
        // the ostream used to write the log file
        std::ofstream                             *out;
    };


    // the streambuf used for this ostream
    DebugStreamBuf   buf;
    // the level of this DebugStream
    int              level;
    // true if enabled
    bool             enabled;

    // these are set one for all instances of DebugStream used in the program:
    static bool            sTimer, sFileLine;
    static string          sPrologueText;
};
extern  DebugStream debug0_real;
extern  DebugStream debug1_real;
extern  DebugStream debug2_real;
extern  DebugStream debug3_real;
extern  DebugStream debug4_real;
extern  DebugStream debug5_real;
} // end namespace rclib

#ifdef DEBUG_TO_STDERR
#define rcdebug0 if (!rclib::debug0_real.Enabled()) ; else cerr << rclib::debug0_real.prologue(__FILE__,__LINE__)
#define rcdebug1 if (!rclib::debug1_real.Enabled()) ; else cerr << rclib::debug1_real.prologue(__FILE__,__LINE__)
#define rcdebug2 if (!rclib::debug2_real.Enabled()) ; else cerr << rclib::debug1_real.prologue(__FILE__,__LINE__)
#define rcdebug3 if (!rclib::debug3_real.Enabled()) ; else cerr << rclib::debug1_real.prologue(__FILE__,__LINE__)
#define rcdebug4 if (!rclib::debug4_real.Enabled()) ; else cerr << rclib::debug1_real.prologue(__FILE__,__LINE__)
#define rcdebug5 if (!rclib::debug5_real.Enabled()) ; else cerr << rclib::debug1_real.prologue(__FILE__,__LINE__)
#else
#define rcdebug0 if (!rclib::debug0_real) ; else rclib::debug0_real << rclib::debug0_real.prologue(__FILE__,__LINE__)
#define rcdebug1 if (!rclib::debug1_real) ; else rclib::debug1_real << rclib::debug1_real.prologue(__FILE__,__LINE__)
#define rcdebug2 if (!rclib::debug2_real) ; else rclib::debug2_real << rclib::debug1_real.prologue(__FILE__,__LINE__)
#define rcdebug3 if (!rclib::debug3_real) ; else rclib::debug3_real << rclib::debug1_real.prologue(__FILE__,__LINE__)
#define rcdebug4 if (!rclib::debug4_real) ; else rclib::debug4_real << rclib::debug1_real.prologue(__FILE__,__LINE__)
#define rcdebug5 if (!rclib::debug5_real) ; else rclib::debug5_real << rclib::debug1_real.prologue(__FILE__,__LINE__)
#endif

#endif


