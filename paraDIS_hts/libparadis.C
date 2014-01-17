/* $Id: libparadis.C,v 1.9 2007/06/30 00:12:00 rcook Exp $ */
#include "restartConvert.h"
#include "args.h"
#include "debugutil.h"
#include "stringutil.h"
#include "timer.h"
/*#include <tcMesh.h> I don't know what this is... */ 
#include <vector>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include "libparadis.h"

using namespace std;
using namespace ParaDIS; 
  

// Progress(theTimer, nodenum, numnodes, thePercent, 5, theTime, 60, "Reading dumpfile"); 
// if either delta is exceeded, increase oPercent & oTime and report the progress
// return 1 if something is printed out, else return 0 
int Progress(int dblevel, timer &iTimer, double iNum, double iMax, 
         double &oPercent, double iPercentDelta, 
         double &oTime, double iTimeDelta, char *iMsg) {
  if (dblevel > dbg_isverbose()) return 0; 
  double percent = iNum/iMax*100.0;
  double newtime = iTimer.elapsed_time();
  if (iNum == iMax || percent - oPercent > iPercentDelta || newtime-oTime > iTimeDelta) {      
    double remaining = (iMax-iNum)/(iNum!=0?iNum:1)*newtime;
    double persec= iNum/newtime;
    char msg[4096]; 
    sprintf(msg, "%s: %s progress: %g percent complete;  %g items out of %g, after %g seconds for %g items/second, seconds remaining might be %g\n", datestring(), iMsg, percent, iNum, iMax, newtime, persec, remaining);
    dbprintf(1, msg);
    fflush(stdout); 
    oPercent=percent;
    oTime=newtime; 
    return 1; 
  }
  return 0; 
}
   

//============================================================================
//  POVRAY ROUTINES 
void PovraySegment(FILE *fp, float bounds[3], segment *seg, char drawspheres) {
  if (seg->filtered || seg->drawn)
    return; 
  seg->drawn = 1; 

  float newloc[3]; 
  node **nodes=seg->endpoints;
  nodes[0]->distance(nodes[1],newloc, bounds); 
  
  fprintf(fp, "makecylinder(%d, %f,%f,%f, %e,%e,%e, %f,%f, %e,%e,%e, %f,%f)\n", 
      seg->burgersValue, 
      seg->parentArm->_weight, seg->monstrosity, seg->distance_from_monster, 
      nodes[0]->loc[0], nodes[0]->loc[1], nodes[0]->loc[2], 
      nodes[0]->monstrosity, nodes[0]->distance_from_monster, 
      newloc[0], newloc[1], newloc[2], 
      nodes[1]->monstrosity, nodes[1]->distance_from_monster);

  if (drawspheres) {   
    int i=0;
    while (i<2){
      node *nodep = seg->endpoints[i]; 
      float *loc = (i==0) ? nodep->loc : newloc;
      if (!nodep->drawn && !nodep->filtered) {
    nodep->drawn = 1;
    fprintf(fp, "makesphere(%e,%e,%e, %d,%d, %f,%f,%f)\n",
        loc[0], loc[1], loc[2], 
        seg->burgersValue, nodep->numNeighbors, 
        seg->parentArm->_weight, nodep->monstrosity, nodep->distance_from_monster);
      }
      ++i;
    }
  }
  return; 
}
 

// END POVRAY ROUTINES 
//======================================================================================


u_int32_t segment::numsegs = 0;


//==============================================================================
void arm::PrintInfo(int dblevel) {
  if (dbg_isverbose() < dblevel)
    return;
  
  int armnum = _armNumber;
  int termnodes =  _terminalNodes.size(), 
    intnodes = _interiorNodes.size();
  
  dbprintf(dblevel, 
           "\nArm #%d: =====================================================\n", 
           armnum);
  dbprintf(dblevel, "Arm #%d: total length: %g, %d segments, %d terminal nodes, %d interior nodes, burgers type %d, %s butterfly body\n", 
           armnum, _length, _segments.size(), termnodes, intnodes, _burgers,
           (const char*)((_butterflyBody == BUTTERFLY_YES)?"IS":
                         ((_butterflyBody == BUTTERFLY_MONSTER)?"IS TYPE -33":"IS NOT")));
  int i = 0;
  while (i < termnodes) {
    node *theNode = _terminalNodes[i];
    dbprintf(dblevel, "Arm #%d: terminal node (%d, %d): connectivity: %d, %s Monster, %s butterfly end\n", 
             armnum, theNode->domain, theNode->domainID, theNode->numNeighbors, 
             (const char*)((theNode->fourArmedMonster == MONSTER_YES) ? "IS TYPE -4" : 
                           ((theNode->fourArmedMonster == MONSTER_MIXED) ? "IS TYPE -44": "IS NOT")), 
             (const char*)((theNode->butterflyEnd == BUTTERFLY_YES) ? "IS":((theNode->butterflyEnd == BUTTERFLY_MONSTER) ? "IS TYPE -33":"IS NOT")));
    dbprintf(dblevel+1, "Arm #%d: terminal node (%d, %d): location is (%f, %f, %f)\n", 
             armnum, theNode->domain, theNode->domainID,
             theNode->loc[0], theNode->loc[1], theNode->loc[2]);
    ++i;
  }
  i = 0;
  while (i < intnodes) {
    node *theNode = _interiorNodes[i];
    dbprintf(dblevel+1, "Arm #%d: interior node (%d, %d): location is (%f, %f, %f)\n", 
             armnum, theNode->domain, theNode->domainID,
             theNode->loc[0], theNode->loc[1], theNode->loc[2]);
    ++i;
  }
  double distance; 
  float newloc[3]; 
  if (_terminalNodes.size() == 2) {
    distance = _terminalNodes[0]->distance(_terminalNodes[1], newloc, _dfp->size);
    dbprintf(dblevel, "Arm #%d: distance between end nodes is %g\n", _armNumber, distance);
  }
  distance = 0;
  vector<segment*>::iterator segpos = _segments.begin();
  while (segpos != _segments.end()) {
    distance += (*segpos)->length;
    dbprintf(dblevel+1, "Arm #%d: segment %d: length is %g\n", _armNumber, (*segpos)->segnum, (*segpos)->length);
    ++segpos;
  }
  dbprintf(dblevel, "Total given arm length is %g\n", distance);
  return;
}

//============================================================================
void DataSet::ResetData(void) {

  if (_nodes) {
	delete[] _nodes;  
	_numnodes = 0; 
	_nodes = NULL; 
  }
  vector<segment*>::iterator theSegment = _segments.begin();  
  while (theSegment != _segments.end()){
	delete *theSegment; 
	++theSegment; 
  }
  _segments.clear(); 

  vector<arm *>::iterator theArm = theArms.begin(); 
  while (theArm != theArms.end()) {
	delete *theArm;
	++theArm; 
  }
  theArms.clear(); 

  return; 
}
  

//============================================================================
void DataSet::CreateArms(void) {
  dbprintf(1, "Creating all arms and identifying node and arm types...\n");
  timer theTimer;  
  theTimer.start(); 
  
  vector<segment*>::iterator segpos = _segments.begin();
  arm *theArm = NULL; 
  //  int numSegs = _segments.size(), segNum=0; 
  timer localTimer;  localTimer.restart(); 
  double thePercent=0, theTime=localTimer.elapsed_time(); 
  while (segpos != _segments.end()) {
    if (!theArm) theArm = new arm(this); 
    if (!theArm) throw string("Allocation of theArm failed in CreateArms"); 
    if (theArm->BuildArm(*segpos)) {
      theArms.push_back(theArm); 
      theArm = NULL;
    }   
    ++segpos;
    /*   if (Progress(2, localTimer, ++segNum, numSegs, thePercent, 5, theTime, 60, "Arm Creation")) {
      dbprintf(2, "numArms so far is %g\n", (double)theArms.size()); 
      }*/ 
  }
  dbprintf(2, "Created %d arms at %d bytes each for a total of %ld bytes...\n", theArms.size(), sizeof(arm), sizeof(arm)*theArms.size());

  // now find all monsters:
  dbprintf(1, "Hunting for Monsters...\n");
  node *nodepos = _nodes;
  u_int32_t nodenum = 0; 
  thePercent = 0; 
  localTimer.restart(); theTime=localTimer.elapsed_time(); 
  while (nodenum++<_numnodes) {
    /*  if (nodenum == 414913){
        printf("about to segfault!\n"); 
        }*/
    nodepos++->CheckFourArmedMonster(); 
    /* if (nodenum == 414913){
       printf("reality check?"); 
       }*/
    Progress(2, localTimer, nodenum, _numnodes, thePercent, 5, theTime, 60, "Hunting for monsters");
    /* Note: 
       find butterfly-monsters in a separate pass, as some butterflies depend on neighbor monsters
    */
  }

  // find butterflies:
  dbprintf(1, "Hunting for butterflies...\n");
  vector<arm*>::iterator armpos = theArms.begin(), endpos = theArms.end();
  long armnum=0, numArms = theArms.size(); 
  thePercent = 0; 
  localTimer.restart(); theTime=localTimer.elapsed_time(); 
  while (armpos != endpos) {
    theArm = *armpos++; 
    theArm->CheckForButterflyBody(); 
    Progress(2, localTimer, ++armnum, numArms, thePercent, 5, theTime, 60, "Hunting for butterflies");
  }

  //now mark all nodes which are either butterflies or monsters
  dbprintf(1, "marking nodes with monster and butterfly status...\n");
  nodepos = _nodes;
  nodenum = 0; 
  thePercent = 0; 
  localTimer.restart(); theTime=localTimer.elapsed_time(); 
  while (nodenum<_numnodes) {
    nodepos++->ComputeButterflyOrMonster(); 
    Progress(2, localTimer, ++nodenum, _numnodes, thePercent, 5, theTime, 60, "Node status marking");
  }  

  // precompute weights and MN distances at this point
  dbprintf(1, "precomputing arm weights, and MN distances...\n");
  armpos = theArms.begin();
  armnum=0; numArms = theArms.size(); 
  thePercent = 0; 
  localTimer.restart(); theTime=localTimer.elapsed_time(); 
  while (armpos != endpos) {
    theArm = *armpos++;     
    theArm->ComputeArmType(); 
    theArm->ComputeWeight();
    theArm->ComputeSegmentMonstrosities();
    Progress(2, localTimer, ++armnum, numArms, thePercent, 5, theTime, 60, "arm weight precomputation");
  }
  dbprintf(1, "Time to create arms was %g seconds\n", theTimer.elapsed_time()); 
  return; 
}


//============================================================================
void DataSet::UndrawArms(void) {
  vector<arm*>::iterator armpos = theArms.begin(), endpos = theArms.end();
  while (armpos != endpos) {
    (*armpos)->drawn = 0;
    vector<segment*>::iterator segpos = (*armpos)->_segments.begin(), 
      endsegs =  (*armpos)->_segments.end();
    while (segpos != endsegs) {
      (*segpos)->drawn = (*segpos)->endpoints[0]->drawn = (*segpos)->endpoints[1]->drawn = 0;
      ++segpos; 
    }
    ++armpos;
  }
  return; 
}

//============================================================================
void DataSet::FilterArms(void) {
  timer theTimer; 
  theTimer.start(); 
  dbprintf(1, "Filtering arms...\n");

  static long nth=0;
  long numfiltered = 0; 
  vector<arm*>::iterator armpos = theArms.begin(), endpos = theArms.end();
  while (armpos != endpos) {
    (*armpos++)->SetFiltered(0);
  }
  armpos = theArms.begin();
  while (armpos != endpos) {
    dbprintf(4, "Filtering arm %d ... arm was ", (*armpos)->_armNumber); 
    int segsfiltered = 0; 
    //=================================================
    // by arm number
    if (_filter.rangelist && !_filter.rangelist->InRange((*armpos)->_armNumber)){
      dbprintf(4, "FILTERED as it is not in the filter file"); 
      (*armpos)->SetFiltered(1);
    }
    
    //=================================================
    //by monsters -- monsters have armweights of 0.5 or 1.0
    if (_filter.filter_monsters) { 
      // first filter all interior nodes regardless: 
      vector<node*>::iterator nodepos = (*armpos)->_interiorNodes.begin(),
        endnode = (*armpos)->_interiorNodes.end(); 
      while (nodepos != endnode) {
        (*nodepos++)->filtered = 1; 
      }
      if ((*armpos)->_weight < 0.5) {
        (*armpos)->SetFiltered(1);
      }
    }

    //=================================================
    //by armweight -- filter out values less than the given filter
    if (!(*armpos)->filtered && (*armpos)->_weight < _filter.armweightfilter) {
      dbprintf(4, "FILTERED because its weight (%g) is less than armweight filter (%g).", (*armpos)->_weight,  _filter.armweightfilter ); 
      (*armpos)->SetFiltered(1);
    }
    
    
    //=================================================
    //by number of neighbors    
    if (!(*armpos)->filtered && _filter.neighborfilter > 0) {
      //by neighbors is done segment by segment most easily
      if (_filter.neighborfilter == 2) { // accept all interior nodes/segments
        //only filter terminal nodes 
        vector<node*>::iterator nodepos = (*armpos)->_terminalNodes.begin(),
          endnode = (*armpos)->_terminalNodes.end(); 
        while (nodepos != endnode) {
          (*nodepos++)->filtered = 1; 
        }
        // if only 1 segment, filter it because it's technically not an interior segment
        if ((*armpos)->_segments.size() == 1) {
          (*armpos)->_segments[0]->filtered = 1;
        }
      } else {  // filter entire arm iff neither terminal node has matching numNeighbors
        char filterarm = 1;     
        vector<node*>::iterator nodepos = (*armpos)->_terminalNodes.begin(),
          endnode = (*armpos)->_terminalNodes.end(); 
        while (nodepos != endnode) {
          if ((*nodepos)->numNeighbors == _filter.neighborfilter) {
            filterarm = 0; 
          } else {
            (*nodepos)->filtered = 1; 
          }
          ++nodepos; 
        }   
        if (filterarm) {
          (*armpos)->SetFiltered(1);
        }
        if (!(*armpos)->filtered) {
          //filter all interior nodes but not segments; they clutter things
          nodepos = (*armpos)->_interiorNodes.begin(); 
          endnode = (*armpos)->_interiorNodes.end(); 
          while (nodepos != endnode) {
            (*nodepos++)->filtered = 1; 
          }
        }
      }
    }
    
    //=================================================
    //by every nth segment or by location
    if (!(*armpos)->filtered && _filter.drawonly || _filter.filter_locations) {
      vector<segment*>::iterator segpos = (*armpos)->_segments.begin(), endsegs =  (*armpos)->_segments.end();
      while (segpos != endsegs) {
        //by every nth segment (-drawonly flag)
        if (_filter.drawonly > 1 && (nth++ % _filter.drawonly) )  {
          (*segpos)->filtered = 1; 
        }
        // by location
        if (_filter.filter_locations) {
          float newloc[3]; 
          node **nodes=(*segpos)->endpoints;
          nodes[0]->distance(nodes[1],newloc, this->size); 
          int i=-1;
          while (++i<3) {
            if ( _filter.min[i] != DOUBLE_UNDEFINED &&  nodes[0]->loc[i]< _filter.min[i] 
                 ||
                 _filter.max[i] != DOUBLE_UNDEFINED &&  nodes[0]->loc[i] > _filter.max[i] ){
              (*segpos)->filtered = 1;  
              nodes[0]->filtered = 1; 
            }
            if ( _filter.min[i] != DOUBLE_UNDEFINED &&  newloc[i]< _filter.min[i] 
                 ||
                 _filter.max[i] != DOUBLE_UNDEFINED &&  newloc[i] > _filter.max[i] ){
              (*segpos)->filtered = 1;  
              nodes[1]->filtered = 1; 
            }
          }
        } // end if by location 
        ++segpos;
      } //end for each segment 
      
    }// end if by location or by nth segment
    if ((*armpos)->filtered)  {
      dbprintf(4, "FILTERED");
      numfiltered++;
    }
    else {
      dbprintf(4, "NOT FILTERED");
      if (segsfiltered)
        dbprintf(4, " overall, but %d segments were removed due to \"nth\" filtering flag", segsfiltered); 
    }
    dbprintf(4, ".\n");
    ++armpos; 
  } // end for each arm
     
  dbprintf(1, "Filtered %ld arms in %g seconds\n", numfiltered, theTimer.elapsed_time());
 


  return; 
}

//======================================================================================
/* this function is called just once for each time a file is parsed, regardless of the number of times the nodes are filtered. 
 */
void DataSet::WriteArmStats(char *armstatsfile){
  timer theTimer; 
  theTimer.start(); 
  
  dbprintf(1, "Writing statistics to armstats file %s...\n", armstatsfile);
  ofstream statfile(armstatsfile, std::ios::out | std::ios::trunc); 
  if (!statfile.good()){
    fprintf(stderr, "Error: Cannot open stat file %s for writing\n", armstatsfile);
    return ;
  }

  statfile <<  "Arm statistics file.  Format:" << endl;
  statfile << "arm#  length burgers-type num-terminal-nodes arm-type" << endl << endl;
  statfile << "The burgers-type is as follows (A 1 means a value +/-1.14, a + means +0.577 and - means -0.577):\n  " << endl;
  statfile << "1 = \"1 0 0\"" << endl; 
  statfile << "2 = \"0 1 0\"" << endl; 
  statfile << "3 = \"0 0 1\"" << endl; 
  statfile << "4 = \"+ + +\" or \"- - -\" " << endl; 
  statfile << "5 = \"+ + -\" or \"- - +\" " << endl; 
  statfile << "6 = \"+ - +\" or \"- + -\" " << endl; 
  statfile << "7 = \"- + +\" or \"+ - -\" " << endl; 
  statfile << "8 = none of the above" << endl << endl; 
  statfile << "The arm-type is according to the following code: " << endl;
  statfile << "0: ARM_UNKNOWN (you should NEVER see this: it's an error if you do. " << endl; 
  statfile << "1: ARM_UNINTERESTING" << endl; 
  statfile << "2: ARM_LOOP" << endl; 
  statfile << "3: ARM_MM_111" << endl; 
  statfile << "4: ARM_MN_111" << endl; 
  statfile << "5: ARM_NN_111" << endl; 
  statfile << "6: ARM_MM_100" << endl; 
  statfile << "7: ARM_MN_100" << endl; 
  statfile << "8: ARM_NN_100" << endl << endl; 
  statfile << "For each terminal node are two lines as follows:" << endl << endl; 
  statfile << "terminal-ID  domainID nodeID x y z node-type" << endl ;
  statfile << "connectivity armnumber_0 armnumber_1 ... armnumber_n-1" << endl << endl; 
  statfile << "connectivity: number of arms connected to the node. neighbor_i is the arm number of the ith neighbor (including the current arm)" << endl;
  statfile << "node-type:  we use 1 integer to represent the type of terminal node. " << endl;
  statfile << "-44: 4-arm SPECIAL MONSTER node with 4 different-valued arms, 2 type 100, and two type 111"<< endl;
  statfile << "-4: 4-arm MONSTER node with 4 uniquely valued, type 111 arms"<< endl;
  statfile << "-3: butterfly-end, middle arm is 100 type arm, and four exterior arms are distinct 111 type arms" << endl;
  statfile << "-33: \"special butterfly\" -- a 3-armed node which is not a type -3 node, but which is the endpoint of a type 100 arm whose other endpoint is a type -44 node" << endl;
  statfile << "3: normal (non-butterfly) 3-arm node" << endl;
  statfile << "4: normal (non-monster) 4-arm node" << endl;
  statfile << "n: n-arm node" << endl;
  statfile << endl << endl; 
  statfile << "Number of arms: " << theArms.size() << endl;
  statfile << "Number of terminal nodes: " << (_numnodes?_nodes[0].NumTerminalNodes():0) << endl;
  statfile << endl << endl; 
  
  vector<arm*>::iterator armpos = theArms.begin(); 
  while (armpos != theArms.end()) { 
    statfile << (*armpos)->_armNumber << " " << (*armpos)->_length << " " 
             << (int)(*armpos)->_burgers << " " << (*armpos)->_terminalNodes.size() 
             << " " << (int)(*armpos)->_armtype << endl; 
    vector<node*>::iterator nodepos = (*armpos)->_terminalNodes.begin(), endpos = (*armpos)->_terminalNodes.end();
    while (nodepos != endpos){
      node *np = *nodepos;
      int nodetype = np->numNeighbors;
      if (np->fourArmedMonster == MONSTER_UNKNOWN){
        throw string("Error in WriteArmStats: fourArmedMonster == MONSTER_UNKNOWN"); 
      } else if (np->butterflyEnd == BUTTERFLY_UNKNOWN){
        throw string("Error in WriteArmStats: butterflyEnd == BUTTERFLY_UNKNOWN"); 
      } else if (np->fourArmedMonster != MONSTER_NO){
        nodetype = np->fourArmedMonster;
      } else if (np->butterflyEnd != BUTTERFLY_NO){
        nodetype = np->butterflyEnd;    
      }
      statfile << (int)np->terminalNode << " " << np->domain << " " << np->domainID << " " 
               << np->loc[0] << " " << np->loc[1] << " "  << np->loc[2] << " " << nodetype <<endl; 
      if (dbg_isverbose() > 4) {
        statfile << np->numNeighbors << " ";  
        int n_num=0; 
        for (n_num=0; n_num<np->numNeighbors; ++n_num) {
          arm *neighbor =  np->neighbors[n_num]->parentArm; 
          if (!neighbor)
            throw string("Error:  NULL neighbor found in WriteArmStats");
          statfile << neighbor->_armNumber << " "; 
        }
        statfile << endl; 
      }
      ++nodepos;
    }
    statfile << endl; 
    ++armpos;
  }
  dbprintf(1, "Reporting stats took %g seconds\n", theTimer.elapsed_time());

  return; 
}


//===========================================================================
void DataSet::ReportDrawnStats(void) {
  timer  reportTimer; reportTimer.restart(); 
  dbprintf(1, "\n DataSet::ReportDrawnStats\n\n");
  //=================================================
  double drawnlength=0, totallength=0, weightedlength=0;
  long numsegs=0, drawnsegs = 0, drawnarms = 0, drawnnodes=0;
  vector<arm*>::iterator armpos = theArms.begin(), armend = theArms.end();
  while (armpos != armend) {
    (*armpos)->PrintInfo(3);
    drawnarms += (*armpos)->drawn; 
    vector<segment *>::iterator segpos = (*armpos)->_segments.begin(), 
      endsegs = (*armpos)->_segments.end();
    while (segpos != endsegs) {
      totallength += (*segpos)->length; 
      numsegs++;
      if ((*segpos)->drawn) {
        drawnlength += (*segpos)->length; 
        weightedlength += (*armpos)->_weight * (*segpos)->length; 
        drawnsegs++;
      }
      ++segpos; 
    }
    ++armpos; 
  }

  u_int32_t i=0; while (i<_numnodes) if (_nodes[i++].drawn) drawnnodes++; 

  dbprintf(1, "\n OVERALL DRAWN ARMS STATISTICS \n\n");
  dbprintf(1, "Wrote %d arms, (%d spheres and %d cylinders)\n", drawnarms, drawnnodes, drawnsegs);
  
  //fullWeight nodes are the ones with a 1.0 weighting
  //double fullWeight = 2 * df->weightedArmLengths - df->drawnArmLengths; // (since weighted = 0.5*dull + 1.0*fullWeight and 1.0*dull + 1.0*weighted = drawn)
  dbprintf(1, "Arm length statistics:\n");
  dbprintf(1, "Drawn: %g\n", drawnlength);
  dbprintf(1, "Weighted total: %g\n", weightedlength);
  dbprintf(1, "Total: %g\n", totallength);
  dbprintf(1, "Drawn Ratio: %g\n", (drawnlength/totallength));  
  dbprintf(1, "Weighted Ratio: %g\n", (weightedlength/totallength));
  dbprintf(1, "\n\n");
  dbprintf(1, " Time to report stats was %g seconds\n", reportTimer.elapsed_time()); 
  return;
}



//============================================================================
void DataSet::DumpToPovray(char *outfile) {
  timer theTimer; 
  
  theTimer.start(); 
  dbprintf(1, "DumpToPovray called\n"); 
  
  FILE *fp = fopen(outfile, "w");
  if (!fp) {
    throw string("Cannot open povray file ")+string(outfile);
  }
  
  fprintf(fp, "#include \"wormparams.inc\"\n");
  fprintf(fp, "union {\nunion {\n"); 

  int numArms = theArms.size(), armNum=0; 
  double thePercent=0, theTime=theTimer.elapsed_time(); 
  vector<arm*>::iterator armpos = theArms.begin();
  while (armpos != theArms.end()) {
    if (!(*armpos)->filtered) {
      dbprintf(4, "Drawing arm %d ...\n", (*armpos)->_armNumber);
      vector <segment *>::iterator segpos = (*armpos)->_segments.begin(), segend = (*armpos)->_segments.end();
      while (segpos != segend){
        PovraySegment(fp, size, *segpos, _filter.drawspheres);
        ++segpos;
      }            
    }
    ++armpos;
    Progress(2, theTimer, ++armNum, numArms, thePercent, 5, theTime, 60, "Povray Dump"); 
    
  }
     
  fprintf(fp, "\t}\n\tAdditionItems01\n}\n");
  fclose(fp);
  dbprintf(1, "Total time to dump arms to povray was %g seconds\n", theTimer.elapsed_time()); 
  
  if (_reportStats) {
    ReportDrawnStats();
  }
 
  return; 
}




//===========================================================================
void DataSet::GetBounds(char *filename, int oldstyle=0) {
  ifstream dumpfile(filename);
  if (!dumpfile.good()){
    cerr << "Error: cannot open dumpfile " << filename << endl;
    exit(2);
  }
  else {
    dbprintf(1, "Parsing file %s\n", filename);
  }
  
  int i=0; 
  if (oldstyle) {
    string token;
    while (dumpfile.good() && token != "minSideX")
      dumpfile >> token;
    while (dumpfile.good() && i<3) {
      dumpfile >>token >> min[i] >>token  >> token  >> max[i] >>token; 
      i++;
    }
  }
  else {
    char linebuf[2048]="";
    while (dumpfile.good() && !strstr(linebuf, "Minimum coordinate values"))
      dumpfile.getline(linebuf, 2048);
    if (!dumpfile.good()){
      cerr << "Error: cannot find minimum bounds from dumpfile " << filename << endl;
      exit(2);
    }
    dumpfile.getline(linebuf, 2048);//get that stupid "#"
    dumpfile >> min[0] >> min[1] >> min[2];
    
    while (dumpfile.good() && !strstr(linebuf, "Maximum coordinate values"))
      dumpfile.getline(linebuf, 2048);
    if (!dumpfile.good()){
      cerr << "Error: cannot find maximum bounds from dumpfile " << filename << endl;
      exit(2);
    }
    dumpfile.getline(linebuf, 2048);//get that stupid "#"
    dumpfile >> max[0] >> max[1] >> max[2];
  }
  for (i=0; i<3; i++)
    size[i]  = max[i] - min[i];
       
  return; 
}
//===========================================================================
void DataSet::ParseDumpFile(char *filename, int oldstyle=0) {
  timer theTimer; 
  theTimer.start(); 
  GetBounds(filename, oldstyle);

  ifstream dumpfile(filename);
  if (!dumpfile.good()){
    cerr << "Error: cannot open dumpfile " << filename << endl;
    exit(2);
  }
  else {
    dbprintf(1, "Parsing file %s\n", filename);
  }
  dbprintf(1, "Size of a node is %d and size of a segment is %d\n", sizeof(node), sizeof(segment)); 
  
  
  char linebuf[2048]="";

  if (!oldstyle) {
    while (dumpfile.good() && !strstr(linebuf, "Node count")) 
      dumpfile.getline(linebuf, 2048);  
    dumpfile.getline(linebuf, 2048);//should be "#"
    if (!strstr(linebuf, "#"))
      dbprintf(0, "WARNING: Expected line:\n#\n but got:\n%s\n", linebuf);
    dumpfile.getline(linebuf, 2048);  
  }
  else {
    while (dumpfile.good() && !strstr(linebuf, "Number of nodes")) 
      dumpfile.getline(linebuf, 2048);  
  }
  u_int32_t numnodes = 0;
  if (dumpfile.good()) 
    dumpfile >> numnodes;
  
  if (numnodes) {
    dbprintf(1, "numnodes is %d\n", numnodes);
  }
  else {
    throw string( "Error: cannot find number of nodes in dumpfile");
  }
  
  dbprintf(2, "initializing nodes data structure with %d elements, at %d bytes each, for an estimated %ld bytes...\n", numnodes, sizeof(node), (long)numnodes*sizeof(node));
  resize(numnodes);
  node *nodes = _nodes;

  while (dumpfile.good() && !strstr(linebuf, "Secondary lines:"))
    dumpfile.getline(linebuf, 2048);  

  if (!oldstyle) {
    dumpfile.getline(linebuf, 2048);//should be "#"
    if (!strstr(linebuf, "#"))
      dbprintf(0, "WARNING: Expected line:\n#\n but got:\n%s\n", linebuf);
  }

  if (!dumpfile.good()) {
    throw string( "Error: cannot find first node in dumpfile");
  }


  u_int32_t  nodenum=0;
  node *current = nodes;
  //u_int32_t currentDomain, currentDomainID; 
  vector<nodeDomain> domains;//need to organize nodes by domain for new file format
  domains.resize(20);
  char comma;
  double theTime=theTimer.elapsed_time(), thePercent=0;
  long old_id_junk, constraint_junk; 
  while (dumpfile.good() && nodenum++ < numnodes) {
    Progress(2, theTimer, nodenum, numnodes, thePercent, 5, theTime, 60, "Reading dumpfile"); 
    current->clear();//necessary for when nodes are reused
    if (!oldstyle) {
      dumpfile >> current->domain >> comma >> current->domainID;
    }
    else
      dumpfile >> current->domainID >> old_id_junk;
    dumpfile >> current->loc[0] >> current->loc[1] >> current->loc[2] >> current->numNeighbors >> constraint_junk;
    if (oldstyle){
      //dumpfile  >> currentDomain >> currentDomainID ; //not sure but this looks wrong
      dumpfile  >> current->domain >>  old_id_junk; 
    }

    //current->id = nodenum; // set global ID.  This is actually redundant to its array position, but might be useful.  Note that in "oldstyle" files, this is one less than the id assigned above from the file, and is equal to the domainID.  
    
    if (!oldstyle) {
      //place the current node into its domain for fast retrieval later
      while (current->domain >= domains.size())
        domains.resize(2*domains.size());
      while (current->domainID >= domains[current->domain].nodes.size())
        domains[current->domain].nodes.resize
          (2*domains[current->domain].nodes.size(), NULL);
      domains[current->domain].nodes[current->domainID] = current;
    }

    if (!dumpfile.good()) {
      throw (string("error reading node number ") + doubleToString(nodenum));
    }

    //read in the neighbors
    int numn = current->numNeighbors;
    while (numn-- && dumpfile.good()) {
      u_int32_t domain, domainID;
      float burgers[3], nx, ny, nz; 

      if (!oldstyle)
        dumpfile >> domain >> comma >> domainID;   
      else {
        domain = 0;
        dumpfile >> domainID;
      }
	  /*!
		Check if we have seen this node before 
	  */
	  if (domain >= domains.size() || domainID >= domains[domain].nodes.size() || domains[domain].nodes[domainID] == NULL ) {
        dbprintf(5, "Found neighbor for node (%d, %d) that hasn't been seen yet.  Creating new segment.\n", 
                 domain, domainID);
        // the neighbor node is still not created yet
        // If the node is not an interior node of an arm (i.e., has more than two neighbors), then
        // create a new segment and store what you can until you next see this relation
        segment *newseg = new segment; 
        if (!newseg) throw string("Allocation of newseg failed in ParseDumpFile()"); 
        _segments.push_back(newseg);
        newseg->endpoints[0] = current; 
        newseg->endpoints[1] = NULL;
        newseg->unk_domain = domain;
        newseg->unk_domainID = domainID; 
        dumpfile >> burgers[0] >> burgers[1]  >> burgers[2] 
#ifdef DEBUG
                 >> newseg->nx >> newseg->ny >> newseg->nz;
#else
        >> nx >> ny >> nz;
#endif
        newseg->ComputeBurger(burgers);
        current->neighbors.push_back(newseg);
        dbprintf(4, "New segment (neighbor not yet found): ");
        newseg->dbprint(4);
      }
      else {
        // the node has been created already, so find the segment in that node that refers to us, add ourselves to it, compute its length, and do a reality check:
        dbprintf(5, "Current neighbor points to previously created segment.\n");
        node *previous = domains[domain].nodes[domainID]; 
        vector<segment *>::iterator pos = previous->neighbors.begin(), end = previous->neighbors.end(); 
        while (pos != end && ( (*pos)->unk_domain != current->domain || (*pos)->unk_domainID != current->domainID) )  ++pos;
        if (pos == end) {
          fprintf(stderr, "Error:  segment connecting previously found node was not found in current segments\n");
          exit (1);
        }
        dumpfile >> burgers[0] >> burgers[1] >> burgers[2] >> nx >> ny >> nz;
        /*  if (
            (*pos)->burg[0] != -burg[0] ||
            (*pos)->burg[1] != -burg[1] ||
            (*pos)->burg[2] != -burg[2] ) {
            dbprintf(4, "burgers vector to be the negative of the original)\n");
            dbprintf(4, "Current segment: burgers = (%f, %f, %f) and previous segment: burgers = (%f, %f, %f)\n",  
            burg[0], burg[1], burg[2], 
            (*pos)->burg[0], (*pos)->burg[1], (*pos)->burg[2]);
        */
#ifdef DEBUG
        if ( (*pos)->nx != nx || (*pos)->ny != ny ||(*pos)->nz != nz  ) {
          dbprintf(4, "Warning:  segment connecting previously found node does not agree with specs for current segment.  We expect ");
          dbprintf(4, "nx,ny,nz for new segment (%f, %f, %f) to be the same as for old segment (%f, %f, %f).   ", nx, ny, nz, (*pos)->nx, (*pos)->ny, (*pos)->nz);
        }
#endif  
        current->neighbors.push_back(*pos);
        (*pos)->endpoints[1] = current;

        float newloc[3];
        (*pos)->length = (*pos)->endpoints[0]->distance(current, newloc, size);
        if (newloc[0] != current->loc[0] || newloc[1] != current->loc[1] || newloc[2] != current->loc[2])
          (*pos)->wrapped = 1;
        dbprintf(5, "Completed old segment (neighbor not yet found): ");
        (*pos)->dbprint(5);
      }
      if (!dumpfile.good()) {
        throw (string("error reading neighbor ")+ doubleToString(current->numNeighbors-numn)+string(" for node number ")+doubleToString(nodenum));
      }
    }
    
    ++current;

  }
  if (dbg_isverbose() >= 5) {
    dbprintf(5, "\n********************************\n");
    current = nodes; nodenum = 0; 
    while (nodenum++ < numnodes) 
      (current++)->dbprint(5);
  }
  dbprintf(1, "All %d nodes have been read in\n", nodenum);
  dbprintf(1, "Created %ld segments at %d bytes each for a total of %ld bytes\n", _segments[0]->numsegs, sizeof(segment), sizeof(segment)*_segments[0]->numsegs);

  dbprintf(1, "Time to parse dumpfile was %g seconds\n", theTimer.elapsed_time()); 

  return;
}


u_int32_t node::_numTerminalNodes = 0; 

//==================
void node::dbprint(int dblevel) {
  dbprintf(dblevel, "\n****************");
  dbprintf(dblevel, "Node (domain, domainDI) (%d, %d): \n", domain, domainID);
  dbprintf(dblevel, "\tcoords = (%f, %f, %f)\n", loc[0], loc[1], loc[2]);
  
  dbprintf(dblevel, "\t%d neighbors: \n", numNeighbors);
  vector<segment*>::iterator pos = neighbors.begin(); 
  int i=0;
  for (i=0; i<numNeighbors && pos != neighbors.end(); i++, ++pos) {
    dbprintf(dblevel, "\t\tneighbor %d:\n", i);
    (*pos)->dbprint(dblevel);
  }
  if (i != numNeighbors || pos != neighbors.end()) {
    dbprintf(0, "Error: node (%d, %d): numNeighbors=%d, actual number = %d (number printed was %d)\n", 
             domain, domainID, numNeighbors, neighbors.size(), i);
    exit(50);
  }
  dbprintf(dblevel, "\n****************\n");
  return ;
}

//=============================================================================
/* find the "real" distance between nodes, taking into account periodicity 
   if the distance is adjusted due to "wrapping," then store the location of the
   other node in newloc 
*/
double node::distance(node *other, float newloc[3], float boundsize[3]) {  
  float d[3]; 
  float *otherloc=other->loc;
  int i=3; while (i--) {
    newloc[i] = otherloc[i];
    d[i] = loc[i]-newloc[i];
    if (fabs(d[i]) > boundsize[i]/2.0) {
      if (loc[i] > otherloc[i])
        newloc[i] += boundsize[i];
      else
        newloc[i] -= boundsize[i];  
      d[i] = loc[i]- newloc[i];
    }
    d[i] *= d[i];
  }
  return  sqrt(d[0]+d[1]+d[2]);
}
//=============================================================================
void node::ComputeButterflyOrMonster(void) {   
  if (numNeighbors == 3 &&  butterflyEnd == BUTTERFLY_UNKNOWN) {
    throw string("Error in ComputeButterflyOrMonster(): butterflyEnd is BUTTERFLY_UNKNOWN for 3 neighbored node (")+doubleToString(domain) +", " + doubleToString(domainID) +")";
  }
  if ( fourArmedMonster == MONSTER_UNKNOWN) {
    throw string("Error: fourArmedMonster is MONSTER_UNKNOWN in ComputeButterflyOrMonster()"); 
  }
  
  if (butterflyEnd == BUTTERFLY_YES ||  butterflyEnd == BUTTERFLY_MONSTER ||  fourArmedMonster == MONSTER_YES || fourArmedMonster == MONSTER_MIXED)
    _isButterflyOrMonster = YES; 
  else 
    _isButterflyOrMonster = NO; 
  return; 
}

//=============================================================================
MonsterInfo node::CheckFourArmedMonster(void) {  
  if (numNeighbors != 3) butterflyEnd = BUTTERFLY_NO;
  //figure out if we are a four armed monster of the evil kind 
  if (fourArmedMonster == MONSTER_UNKNOWN) {
    fourArmedMonster = MONSTER_NO;
    if (numNeighbors == 4 ) {
      int burgers[8] = {0};
      
      vector<segment *>::iterator pos = neighbors.begin();
      while (pos != neighbors.end()) {
        if ((*pos)->burgersValue > 7 || (*pos)->burgersValue < 1) {
          break; 
        } else {
          burgers[(*pos++)->burgersValue] = 1;
        }
      }
      if (pos == neighbors.end() ) {
        if (burgers[4] && burgers[5] && burgers[6] && burgers[7]) {
          fourArmedMonster = MONSTER_YES;   
          butterflyEnd = BUTTERFLY_NO;
          dbprintf(3, "Found four-armed monster in node (%d, %d)\n", domain, domainID);
        }
        else {
          if ( 
              ( 
               (burgers[1] && burgers[2]) || 
               (burgers[1] && burgers[3]) ||
               (burgers[2] && burgers[3])
               ) &&
              ( 
               (burgers[4] && burgers[5]) || 
               (burgers[4] && burgers[6]) || 
               (burgers[4] && burgers[7]) || 
               (burgers[5] && burgers[6]) || 
               (burgers[5] && burgers[7]) || 
               (burgers[6] && burgers[7]) 
               )
              ) {
            fourArmedMonster = MONSTER_MIXED;           
            butterflyEnd = BUTTERFLY_NO;
          }
        }
      }// pos == neighbors.end()
    }// numneighbors == 4
  }//fourArmedMonster == MONSTER_UNKNOWN
  return fourArmedMonster;  
}

//==================
void segment::dbprint(int dblevel) {
  dbprintf(dblevel, "\tsegment id = %d, length = %f, burgersValue = %d, monstrosity = %f", segnum, length, burgersValue, monstrosity);
#ifdef DEBUG
  dbprintf(dblevel, ", (nx,ny,nz) = (%f, %f, %f)",  nx, ny, nz);
#endif
  dbprintf(dblevel, "\n"); 

  int i=0; 
  while (i<2){  
    node *np = endpoints[i]; 
    dbprintf(dblevel, "\t\tendpoint[%d]: ", i); 
    if (!np) dbprintf(dblevel, " (NULL) (not read yet?)\n");
    else 
      dbprintf(dblevel, " id = (%d, %d)\n", np->domain, np->domainID);
    ++i;
  }
  return;
}


//=============================================================================
u_int32_t arm::_numArms = 0;
DataSet *arm::_dfp = NULL; 
 
//===========================================================================
void arm::ComputeSegmentMonstrositiesHelper(segment *seg, node *previous, int segnum, int numsegs, double distance){  

  if (segnum >= numsegs) return; 

  node *next = seg->endpoints[0]; 
  if (seg->endpoints[0] == previous) next = seg->endpoints[1];
  seg->monstrosity = previous->monstrosity - 1.0;
  next->monstrosity = previous->monstrosity - 2.0;
  if (seg->monstrosity < 1.0) seg->monstrosity = 1.0;
  if (next->monstrosity < 1.0) next->monstrosity = 1.0;

  distance += seg->length;
  if (distance < next->distance_from_monster) 
    next->distance_from_monster = distance; 
  seg->distance_from_monster = 
    (previous->distance_from_monster + next->distance_from_monster)/2.0; 

  // bail if we've been here before or are at an endpoint
  if (distance > next->distance_from_monster || next->numNeighbors != 2) return; 
  
  segment *nextseg = next->neighbors[0]; 
  if (nextseg == seg) nextseg = next->neighbors[1]; 
  ComputeSegmentMonstrositiesHelper(nextseg, next, segnum+1, numsegs, distance); 
  return; 
}

//============================================================================
void arm::SetFiltered(int filterval){
  filtered = filterval; 
  
  vector<segment*>::iterator segpos = _segments.begin(), 
    endsegs =  _segments.end();
  while (segpos != endsegs) {
    (*segpos)->filtered = filterval; 
    (*segpos)->endpoints[0]->filtered = (*segpos)->endpoints[1]->filtered = filterval;
    
    ++segpos; 
  }
  vector <node *>::iterator nodepos = _terminalNodes.begin(), endpos = _terminalNodes.end();
  while (nodepos != endpos) {
    (*nodepos++)->filtered = filterval; 
  }
  
  return; 
}

//==========================================================================
void arm::ComputeSegmentMonstrosities(void){
  dbprintf(5, "ComputeSegmentMonstrosities called"); 
  if (_terminalNodes.size() == 0 || _terminalNodes.size() == 1 || 
      (!_terminalNodes[0]->_isButterflyOrMonster && !_terminalNodes[1]->_isButterflyOrMonster)) {
    //first, if no terminal nodes or both terminal nodes are type N, then all segments are 0
    return; //0 is the default value in a segment's constructor
  }
  else if (_terminalNodes[0]->_isButterflyOrMonster &&  _terminalNodes[1]->_isButterflyOrMonster) {
    if (_segments.size() == 1){
      _terminalNodes[0]->monstrosity = _terminalNodes[1]->monstrosity = 8.0;
      _terminalNodes[0]->distance_from_monster = _terminalNodes[1]->distance_from_monster = 0.0;
      _segments[0]->monstrosity = 8.0;
      _segments[0]->distance_from_monster = _segments[0]->length/2.0;
      return; 
    }
    else {
      //do each end halfway in
      int i=0; for (i=0; i<2; ++i) {
        segment *sp = _terminalSegments[i]; 
        node *termnode = sp ->endpoints[0]->_isButterflyOrMonster ? 
          sp->endpoints[0] : sp->endpoints[1]; 
        if (!termnode->_isButterflyOrMonster)
          throw string("error: MM segment has no M!\n");
        termnode->monstrosity = 8.0;
        termnode->distance_from_monster = 0.0;
        ComputeSegmentMonstrositiesHelper(sp, termnode, 0, _segments.size(), 0.0);
      }
      return; 
    }
  } 
  else {
    // we are in an M-N arm 
    //start from segment by monster node; give it value 1
    dbprintf(5, "we have %d segments and are in an M-N arm -- making an MN chain\n", _segments.size()); 
    segment *seg = _terminalSegments[0]; 
    if (!seg->endpoints[0]->_isButterflyOrMonster && !seg->endpoints[1]->_isButterflyOrMonster)
      //we guessed wrong;  it's the other endpoint
      seg = _terminalSegments[1]; 
    node *termnode = seg->endpoints[0]; 
    if (! termnode->_isButterflyOrMonster) termnode = seg->endpoints[1]; 
    if (! termnode->_isButterflyOrMonster)
      throw string("error:  MN chain has no M!\n");
    dbprintf(5, "M node is as follows:\n");
    termnode->monstrosity = 8.0;
    termnode->distance_from_monster = 0.0;
    termnode->dbprint(5);    

    //chain all of them
    ComputeSegmentMonstrositiesHelper(seg, termnode, 0, _segments.size(), 0.0);
 
    return; 
  }

  return; 
}

//======================================================================================
void arm::ComputeWeight(void) {
  _weight = 0.0; 
  if (_armtype == ARM_MN_111 || _armtype == ARM_MN_100 )
    _weight = 0.5;
  else if (_armtype == ARM_MM_111 || _armtype == ARM_MM_100)
    _weight = 1.0;
  dbprintf(5, "Arm %d: computed weight is %f\n", _armNumber, _weight); 
  return; 
}

//======================================================================================
// figure out if we have monsters on our endpoints, necessary info for later
void arm::ComputeMNType(void) {
  _mnType = MN_NOTA; 
  if (_terminalNodes.size() == 2) {
    node *node1 = _terminalNodes[0], 
      *node2 = _terminalNodes[1];
    
    if ((node1->_isButterflyOrMonster) || node2->_isButterflyOrMonster) {
      if ( node1->_isButterflyOrMonster && node2->_isButterflyOrMonster ) 
        _mnType  = MN_MONSTER_MONSTER;
      else
        _mnType = MN_MONSTER_NONMONSTER;
    }
    else
      _mnType = MN_NONMONSTER_NONMONSTER; 
  }
  dbprintf(5, "mn for arm %d is %d\n", _armNumber, _mnType);
  return;
}

//=============================================================================
/* DO NOT CALL THIS BEFORE ALL ARMS HAVE BEEN CHECKED FOR BUTTERFLY-NESS */
void arm::ComputeArmType(void){

  ComputeMNType();

  _armtype=ARM_UNINTERESTING;
  if ( _terminalNodes.size() == 0 || _terminalNodes.size() == 1 ) 
    _armtype=ARM_LOOP;
  else if (_burgers < 8 ){
    if (_burgers > 3) {
      if (_mnType == MN_MONSTER_MONSTER) _armtype=ARM_MM_111;
      else if (_mnType == MN_MONSTER_NONMONSTER) _armtype = ARM_MN_111;
      else if (_mnType == MN_NONMONSTER_NONMONSTER) _armtype = ARM_NN_111;
    }
    else {
      if (_mnType == MN_MONSTER_MONSTER) _armtype = ARM_MM_100;
      else if (_mnType == MN_MONSTER_NONMONSTER) _armtype = ARM_MN_100;
      else if (_mnType == MN_NONMONSTER_NONMONSTER) _armtype = ARM_NN_100;
    }
  }
  return; 
}


//=============================================================================
void arm::CheckForButterflyBody(void){
  int i=_terminalNodes.size();
  while (i--) {
    if (_terminalNodes[i]->butterflyEnd == BUTTERFLY_UNKNOWN)
      _terminalNodes[i]->butterflyEnd = BUTTERFLY_NO;
  }
  // trivial case: 
  if (_terminalNodes.size() != 2) {
    _butterflyBody = BUTTERFLY_NO; 
    return;
  }
  

  
  // check for "traditional" butterfly, which is four armed monster which has been unzipped, i.e., the terminal nodes are 3-armed and have four unique burgers vectors.  I check to make sure the burgers vectors are of the expected type, and issue a warning if they are not, but still accept it.  
  //reality check: 
  int burgs[9]={0};
  int numBurgsTypes = 0; 
  i = 2;
  while (i--) {
    node *theNode = _terminalNodes[i];
    int n = theNode->numNeighbors;
    if (n != 3) 
      break;
    while (n--) {
      if (theNode->neighbors[n]->parentArm != this) {
        int burger = theNode->neighbors[n]->burgersValue;
        if (!burgs[burger])
          numBurgsTypes++; 
        burgs[burger] = 1;
      }
    }
  }
  
  _butterflyBody = BUTTERFLY_YES;
  for (i=0; i<4; i++){
    if (burgs[i]) {
      _butterflyBody =BUTTERFLY_NO; 
      break; 
    }   
  } 
  if (_butterflyBody == BUTTERFLY_YES) {
    while (i < 8) {
      if (!burgs[i]) {
        _butterflyBody =BUTTERFLY_NO; 
        break; 
      }   
      i++; 
    }    
  }
  if (_butterflyBody == BUTTERFLY_YES) {
    i=2; 
    while (i--) {
      _terminalNodes[i]->butterflyEnd =  BUTTERFLY_YES;
    }
  } else if (_burgers >= 1 && _burgers <= 3 ) { //type 100 arm
    // check for a -33 node (see definition in WriteArmStats() ) 
    i=2;
    while (i--) {
      if ( _terminalNodes[i]->numNeighbors == 3 &&  
           _terminalNodes[1-i]->fourArmedMonster == MONSTER_MIXED) 
        {
          _butterflyBody = BUTTERFLY_MONSTER; 
          if (_terminalNodes[i]->butterflyEnd != BUTTERFLY_YES) {
            _terminalNodes[i]->butterflyEnd = BUTTERFLY_MONSTER;  
          }      
        }
    }
  }
  
  return;
  
}
//=============================================================================
void arm::ComputeLength(void) {
  if (!_terminalNodes.size() && !_interiorNodes.size()) {
    dbprintf(0, "ERROR:  an arm was found with zero nodes in it!\n");
    exit(1);
  }
  _length = 0; 

  vector<segment *>::iterator segIter = _segments.begin();   
  while (segIter != _segments.end()) {  
    _length += (*segIter)->length;
    ++segIter;
  }
  dbprintf(3, "ComputeLength: Set length of arm #%d to %g\n", _armNumber, _length);
    
  return; 
}

//=============================================================================
/* Starting with the given segment, build up an arm until you find two terminal nodes or end up 
   where you started.  
   If an arm is built (i.e., segment hasn't been done yet), the return 1, else return 0. 
*/
int arm::BuildArm(segment *theSegment) {
  Clear();
  if (theSegment->parentArm != NULL)
    return 0; 
  _armNumber = _numArms++;
  dbprintf(5, "BuildArm (Arm #%d)\n", _armNumber);
  int i=2; 
  while (i--) {
    segment *next =  theSegment; 
    node *ep = theSegment->endpoints[i];   
    while (next) {
      next = BuildArmPrivate(next, ep, theSegment);
    }
  }
  ComputeLength();
  return 1; 
}

//=============================================================================
/* 
   Add the current segment to this arm.  
   If the non-ignore endpoint is not terminal (is internal) and is not the inital segment, then return a pointer to the next segment and set ignore to be the non-ignored endpoint so it is ignored on the next segment.  
   If the next segment is the inital segment, return NULL. 
   If the non-ignore endpoing is a terminal node, return NULL and mark the node and segment as being terminal.
*/ 
segment *arm::BuildArmPrivate(segment *theSegment, node *&ignore, segment *initial) {
  segment *nextSegment = NULL; 
  if (theSegment->parentArm != this) {
    dbprintf(5, "\nfound new arm segment\n");
    theSegment->dbprint(5);
    _segments.push_back(theSegment);
    theSegment->parentArm=this;
  }
  if (_burgers == -1) _burgers = theSegment->burgersValue;
  else if (_burgers != theSegment->burgersValue) {
    fprintf(stderr, "Error:  new segment for arm has different burgers value than earlier segments.\n");
    exit(3);
  }
  node *ep=theSegment->endpoints[0]; 
  if (ep == ignore) ep = theSegment->endpoints[1]; 
  if (ep->numNeighbors != 2) {
    // ep is a terminal node 
    ep->MarkTerminal();
    if (!_terminalNodes.size() ||  _terminalNodes[0] != ep) {
      _terminalNodes.push_back(ep);
      assert(_terminalNodes.size() < 3); 
    }
    // there may possibly be only one "terminal" segment (if the arm has only one segment), so only push it back 
    if (!_terminalSegments.size() || theSegment != _terminalSegments[0])
      _terminalSegments.push_back(theSegment); 
  }
  else  {
    nextSegment = ep->neighbors[0];
    if (nextSegment == theSegment) nextSegment = ep->neighbors[1];
    if (nextSegment  == initial) {
      nextSegment = NULL; 
    } else {
      // ep is a new, interior node
      _interiorNodes.push_back(ep);   
      ignore = ep; 
    }
  }
 
  return nextSegment; 
}


//===========================================================
void libparadis_print_version(void) {
  printf("This is restartConvert $Revision: 1.9 $ with header %s)\n", HEADER_ID);
  return ;
}
