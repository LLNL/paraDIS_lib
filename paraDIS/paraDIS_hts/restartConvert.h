/* $Id: restartConvert.h,v 1.62 2007/06/30 00:12:00 rcook Exp $ 
 $File$
*/
#ifndef RESTART_CONVERT_H
#define RESTART_CONVERT_H

#include <vector>
#include <string>
#include <math.h>
#include "debugutil.h"
#include "RangeList.h"

using namespace std;

#define HEADER_ID "$Revision: 1.62 $"
//===========================
#define FILTER_BUTTERFLIES (-1)
#define FILTER_MONSTERS (-2)
#define DOUBLE_UNDEFINED (-42.121867351)
struct  filter {
  filter () {
    min[0]=min[1]=min[2]=max[0]=max[1]=max[2]=DOUBLE_UNDEFINED; 
    drawspheres = 0; 
    neighborfilter = 0; 
    drawonly=1;
    rangelist = NULL;
    armweightfilter = -1; 
    filter_monsters = filter_locations =  false; 
  }
  void ParseFilterFile(char *filename, char verbose); //create and parse a filter file
  RangeList *rangelist; 

  bool filter_locations, 
    filter_monsters; 
  double armweightfilter, min[3], max[3]; 
  int32_t neighborfilter; 
  int drawspheres;
  int drawonly; 
 };


//stuff to rewrite so they are class members:
     void FilterArms(void);
    void UndrawArms(void);
    void WriteArmStats(char *armstatsfile);
    void ReportDrawnStats(void); 
    void DumpToPovray(char *outfile);
 
#endif

