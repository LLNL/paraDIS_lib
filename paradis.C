/*   Written by Richard David Cook
     at Lawrence Livermore National Laboratory
     Contact:  wealthychef@gmail.com
     See license.txt for information about usage.
     Spoiler alert:  it's GNU opensource.
*/


/*!
  \file paradis.C
  a new run at libparadis.C from the ground up
  Need to do this because the previous design was intentionally entwined with povray and HTS operations for efficiency, and assumed the whole dataset would be read at once.
*/
#include "paradis.h"
#include <cstring>
#include <fstream>
#include <string>
#include <iostream>
#include "debugutil.h"
#include "stringutil.h"
#include "timer.h"
#include <algorithm>
#include "paradis_version.h"
//#include "visit_writer.h"

#ifndef WIN32
#  include <dirent.h>
#else
#  include <direct.h>
#endif
#include <sys/stat.h>
#include <errno.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


using namespace rclib;
using namespace std;

#ifdef RC_CPP_VISIT_BUILD
void dbstream_printf(int level, const char *fmt, ...) {
  char buf[4096];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf,4095,fmt,ap);
  va_end(ap);
  if (level == 5)  { debug5 << buf; debug5.flush(); }
  if (level == 4)  { debug4 << buf; debug4.flush(); }
  if (level == 3)  { debug3 << buf; debug3.flush(); }
  if (level == 2)  { debug2 << buf; debug2.flush(); }
  if (level < 2)  { debug1 << buf; debug1.flush(); }
  if (level == 0)  cout << buf;
  return;
}
#endif

// GRRR.  Visit hooks are lame.  This is bad code but if I don't structure it like this, the SVN hooks complain.
#ifdef  USE_ABORT
#define errexit abort()
#define errexit1 abort()
#else
#define errexit return
#define errexit1 return err
#endif

#define paradis_assert(test) if (!(test)) { \
	dbecho(0, "ERROR: %s %d: failed test: "#test"\n", __FUNCTION__, __LINE__); \
	errexit; \
  }

std::string GetLibraryVersionString(const char *progname) {
  return str(boost::format("%s: using paraDIS_lib version %s compiled on %s")% progname% RC_PARADIS_VERSION% RC_PARADIS_VERSION_DATE);
}

std::string GetLibraryVersionNumberString(void) {
  return str(boost::format("%1%")% RC_PARADIS_VERSION);
}

std::string INDENT(int i) {
  if (!i) return "";
  return str(boost::format(str(boost::format("%%1$%1%s")%(i*3)))%" ");
}
#define STARTPROGRESS()                                 \
  timer theTimer;  theTimer.start();                    \
  double theTime=theTimer.elapsed_time(), thePercent=0


#define UPDATEPROGRESS(count, total, description)                       \
  if (dbg_isverbose()) {                                                \
    Progress(theTimer, count, total, thePercent, 1, theTime, 1, description);  }

#define COMPLETEPROGRESS(total, description)                        \
  UPDATEPROGRESS(total,total,description); fprintf(stderr, "\n");	\
  theTimer.restart();													\
  theTime=theTimer.elapsed_time();									\
  thePercent=0




// =====================================================================
/*!
  Utility for Povray
*/
string GetPovrayLocationString(vector<float> loc) {
  return str(boost::format("<%f, %f, %f>")%(loc[0])%(loc[1])%(loc[2]));
}

// =====================================================================
/*!
  This prints out a list of arms with their depths to a text file
  and a set of VTK files.
  File created: basename.txt, basename.vtk
*/

namespace paraDIS {
  //===========================================================================

  rclib::Point<float> Node::mBoundsMin, Node::mBoundsMax, Node::mBoundsSize;
  string Node::mTraceFileBasename;
  vector<uint32_t> Node::mTraceNodes;
  std::map<uint32_t, vector<Node *> >  Node::mNodes;  // includes all nodes, even wrapped, since all are drawn in VisIt.
  std::vector<Node *> Node::mNodeVector; // purely for the C interface

  uint32_t Node::mNextNodeIndex = 0;

  std::map<uint32_t, ArmSegment *> ArmSegment::mArmSegments;  // includes decomposed segments but not wrapped, since VisIt will not draw wrapped segments
  uint32_t ArmSegment::mNextSegmentIndex = 0;

  double ArmSegment::mSegLen = 0 ;
  uint32_t ArmSegment::mNumBeforeDecomposition = 0; // happens in ExtendByArm()
  uint32_t ArmSegment::mNumDecomposed = 0; // happens in ExtendByArm()
  uint32_t ArmSegment::mNumClassified = 0;
  uint32_t ArmSegment::mNumArmSegmentsMeasured=0;
  double ArmSegment::mScrewToleranceAngle = 0.05 ;
  double ArmSegment::mScrewToleranceCosine = cos(0.05);
  double ArmSegment::SQRT3 = sqrt(3.0);
  ArmSegment *ArmSegment::mInitialLightTheFuseArmSegment = NULL;
  uint32_t ArmSegment::mMaxLightTheFuseDistance = 0;

  vector<Arm *> Arm::mArms;
  map<int, int32_t> Arm::mNumDecomposed; // statistics
  vector<int32_t> Arm::mTraceArms;
  uint16_t Arm::mTraceDepth;
  string Arm::mTraceFileBasename;
  double Arm::mLongestLength = 0.0;
  double Arm::mDecomposedLength = 0.0;
  int32_t Arm::mNumDestroyedInDetachment = 0;
  double Arm::mTotalArmLengthBeforeDecomposition = 0.0;
  double Arm::mTotalArmLengthAfterDecomposition = 0.0;
  double Arm::mThreshold = -1;

  rclib::Point<float> MetaArm::mWrappedNode(-424242.42, 424242.42, -424242.42);

  rclib::Point<float> DataSet::mDataMin, DataSet::mDataMax, DataSet::mDataSize;

  //===========================================================================

  void WriteTraceFiles(string filebase, string description, vector<Arm*>&arms, vector<uint32_t> &armdepths, vector<int> &action) {
	uint32_t numArms = arms.size();
    // ===================================
    // FIRST, PRINT THE TEXT FILE
    string filename = filebase + ".txt";
    std::ofstream textfile(filename.c_str());
    if (!textfile) {
      string errmsg = str(boost::format("\nWarning:    cannot open text file %1%: %2%.")%filename%strerror(errno));
      cerr << errmsg << endl;
      dbprintf(1, "%s\n", errmsg.c_str());
      return;
    }
    textfile << "Tracefile for " << description << ", written by paraDIS_lib" << endl;
    textfile.flush();
    uint32_t level = 0;
    for (uint32_t arm = 0; arm < arms.size(); arm++) {
      if (armdepths[arm] == level) { // this is weird, don't worry
        textfile << str(boost::format("\n****************\nARM LEVEL %1%\n****************\n\n")%level);
        textfile.flush();
        level++;
      }
      textfile << arms[arm]->Stringify(0) ;
    }

    textfile << endl << endl << "\n****************\nDETAILED ARM INFORMATION: " << description << "\n****************\n" << endl;
    for (uint32_t arm = 0; arm < arms.size(); arm++) {
      textfile << str(boost::format("\n****************\nArm %1%: \n %2%\n\n****************\n\n")%arm%arms[arm]->Stringify(0, false, true));
    }
    string msg = str(boost::format("\n*** Wrote text tracefile %s for %s\n") % filename % description);
    dbprintf(0, msg.c_str());
    cerr << msg;
    textfile.flush();
    textfile.close();



    // ===================================
    // NEXT, PRINT THE VTK FILE FOR ARM SEGMENTS
    filename = filebase+"-segments.vtk";
    std::ofstream vtkfile(filename.c_str());
    if (!vtkfile) {
      string errmsg = str(boost::format("\n*** Warning:    cannot open vtk file %1%: %2%.")%filename%strerror(errno));
      cerr << errmsg << endl;
      dbprintf(1, "%s\n", errmsg.c_str());
      return;
    }

    // First, the header
    vtkfile << "# vtk DataFile Version 3.0" << endl;
    vtkfile << filename + ", written by paraDIS_lib. If action = 2, that's the arm being decomposed.  If action = 1, then it's the arm being extended. Else action = 0.  mBounds are " << Node::mBoundsSize.Stringify() << endl;
    vtkfile << "ASCII" << endl;
    vtkfile << "DATASET POLYDATA" << endl;

    // next, the points
    uint32_t numlines = 0;
	uint32_t nonNullNodes = 0;
	for (uint32_t arm = 0; arm < numArms; arm++) {
	  nonNullNodes += arms[arm]->mNodes.size() - arms[arm]->mNumWrappedSegments;
	  numlines += arms[arm]->mNumNormalSegments;
	  dbprintf(6, "Arm %d: mNumNormalSegments is %d, numwrapped is %d, actual segments is %d, actual nodes is %d\n", arms[arm]->mArmID, arms[arm]->mNumNormalSegments, (int)(arms[arm]->mNumWrappedSegments), arms[arm]->mSegments.size(), arms[arm]->mNodes.size());
	}
    uint32_t armnode = 0;
    vector<float> previous;
    vtkfile << str(boost::format("POINTS %d float") % nonNullNodes) << endl;
	uint32_t nodesWritten = 0;
	for (uint32_t armnum = 0; armnum < numArms; armnum++) {
	  for (deque<Node*>::iterator nodepos = arms[armnum]->mNodes.begin();
		   nodepos != arms[armnum]->mNodes.end(); nodepos++) {

		if (!(*nodepos)) continue; // wrappage or new line

		vector<float> xyz = (*nodepos)->mLocation;
		if (previous.size()) {
		  for (uint32_t i=0; i<3; i++) {
			if (xyz[i] - previous[i] > Node::mBoundsSize[i]/2){
			  xyz[i] -= Node::mBoundsSize[i];
			}
			else if (previous[i] - xyz[i] > Node::mBoundsSize[i]/2){
			  xyz[i] += Node::mBoundsSize[i];
			}
		  }
		}
		armnode++;
		previous = xyz;
		string note = "";
		/*if (armnode == numArmNodes[armnum]) {
		  note = str(boost::format("# End arm %1%")%armnum);
		  } */
		vtkfile << str(boost::format("%1% %2% %3% %4%") % xyz[0] % xyz[1] % xyz[2] % note) << endl;
		nodesWritten++;
	  }
	}
    vtkfile << endl;

	paradis_assert(nodesWritten == nonNullNodes);

    // next the lines
    vtkfile << str(boost::format("LINES %d %d") % numlines % (3*numlines)) << endl;
    uint32_t currentIndex = 0, linesWritten = 0;
	for (uint32_t armnum = 0; armnum < numArms; armnum++) {
	  Node *previousNode = NULL;
	  for (deque<Node*>::iterator nodepos = arms[armnum]->mNodes.begin();
		   nodepos != arms[armnum]->mNodes.end(); nodepos++) {
		if (*nodepos) {
		  if (previousNode) {
			vtkfile << str(boost::format("2 %d %d")% (currentIndex-1) % currentIndex) << endl;
			linesWritten++;
		  }
		  currentIndex++;
		}
		previousNode = *nodepos;
	  }
	}

	paradis_assert(linesWritten == numlines) ;

    vtkfile << endl << endl;

    // next the arm numbers for each line
    vtkfile << str(boost::format("CELL_DATA %d") % numlines) << endl;
    vtkfile << "SCALARS armnum int" << endl;
    vtkfile << "LOOKUP_TABLE default" << endl;
    vtkfile.flush();
	linesWritten = 0;
    for (uint32_t arm = 0; arm < numArms; arm++) {
      if (!arms[arm]->mSegments.size()) {
        continue;
      }
      uint32_t armnum = arms[arm]->mArmID;
      for (uint32_t linenum = 0; linenum < arms[arm]->mNumNormalSegments; linenum++) {
        vtkfile << armnum << " " ;
        vtkfile.flush();
		linesWritten++;
      }
    }
    vtkfile << endl << endl;

	paradis_assert(linesWritten == numlines);

    // next the burgers type for each line
    vtkfile << "SCALARS burgers_type int" << endl;
    vtkfile << "LOOKUP_TABLE default" << endl;
    for (uint32_t arm = 0; arm < numArms; arm++) {
	  if (!arms[arm]->mSegments.size()) {
		continue;
      }
      uint32_t armtype = arms[arm]->GetBurgersType();
      for (uint32_t linenum = 0; linenum < arms[arm]->mNumNormalSegments; linenum++) {
        vtkfile << armtype << " " ;
        vtkfile.flush();
      }
    }
    vtkfile << endl << endl;
    vtkfile.flush();

    // next the arm type for each line
    vtkfile << "SCALARS arm_type int" << endl;
    vtkfile << "LOOKUP_TABLE default" << endl;
    for (uint32_t arm = 0; arm < numArms; arm++) {
	  if (!arms[arm]->mSegments.size()) {
		continue;
      }
      uint32_t armtype = arms[arm]->mArmType;
      for (uint32_t linenum = 0; linenum < arms[arm]->mNumNormalSegments; linenum++) {
        vtkfile << armtype << " " ;
        vtkfile.flush();
      }
    }
    vtkfile << endl << endl;
    vtkfile.flush();

    // next the arm depths for each line
    vtkfile << "SCALARS BFS_depth int" << endl;
    vtkfile << "LOOKUP_TABLE default" << endl;
    for (uint32_t arm = 0; arm < numArms; arm++) {
	  if (!arms[arm]->mSegments.size()) {
        continue;
      }
      for (uint32_t linenum = 0; linenum < arms[arm]->mNumNormalSegments; linenum++) {
        vtkfile << armdepths[arm] << " " ;
      }
    }
    vtkfile << endl << endl;

    // next the segment IDs for each line
    vtkfile << "SCALARS segment_ID int" << endl;
    vtkfile << "LOOKUP_TABLE default" << endl;
	for (uint32_t armnum = 0; armnum < numArms; armnum++) {
	  for (deque<ArmSegment*>::iterator segpos = arms[armnum]->mSegments.begin();
		   segpos != arms[armnum]->mSegments.end(); segpos++) {
		if (!(*segpos)->mWrapped)
 		vtkfile << (*segpos)->mSegmentIndex << " " ;
	  }
	}

    vtkfile << endl << endl;

    // next the "action" for each line
    vtkfile << "SCALARS action int" << endl;
    vtkfile << "LOOKUP_TABLE default" << endl;
    for (uint32_t arm = 0; arm < numArms; arm++) {
     for (uint32_t linenum = 0; linenum < arms[arm]->mNumNormalSegments; linenum++) {
        vtkfile << action[arm] << " " ;
      }
    }
    vtkfile << endl << endl;
    vtkfile.flush();
    vtkfile.close();

    msg = str(boost::format("*** Wrote VTK arm segment tracefile %s for %s\n")% filename % description);
    dbprintf(0, msg.c_str());
    cerr << msg;

    // ===================================
    // NEXT, PRINT THE VTK FILE FOR ARM NODES
    filename = filebase+"-nodes.vtk";
    vtkfile.open(filename.c_str());
    if (!vtkfile) {
      string errmsg = str(boost::format("\n*** Warning:    cannot open vtk file %1%: %2%.")%filename%strerror(errno));
      cerr << errmsg << endl;
      dbprintf(1, "%s\n", errmsg.c_str());
      return;
    }

    // First, the header
    vtkfile << "# vtk DataFile Version 3.0" << endl;
    vtkfile << filename + ", written by paraDIS_lib" << endl;
    vtkfile << "ASCII" << endl;
    vtkfile << "DATASET POLYDATA" << endl;
    vtkfile.flush();

    // next, the points
    armnode = 0;
    vtkfile << str(boost::format("POINTS %d float") % nonNullNodes) << endl;
	for (uint32_t armnum = 0; armnum < numArms; armnum++) {
	  for (deque<Node*>::iterator nodepos = arms[armnum]->mNodes.begin();
		   nodepos != arms[armnum]->mNodes.end(); nodepos++) {

		if (!*nodepos) continue; // wrappage

		vector<float> xyz = (*nodepos)->mLocation;
		if (previous.size()) {
		  for (uint32_t i=0; i<3; i++) {
			if (xyz[i] - previous[i] > Node::mBoundsSize[i]/2){
			  xyz[i] -= Node::mBoundsSize[i];
			}
			else if (previous[i] - xyz[i] > Node::mBoundsSize[i]/2){
			  xyz[i] += Node::mBoundsSize[i];
			}
		  }
		}
		previous = xyz;
		string note = "";
		/*if (armnode == numArmNodes[armnum]) {
		  note = str(boost::format("# End arm %1%")%armnum);
		  } */
		vtkfile << str(boost::format("%1% %2% %3% %4%") % xyz[0] % xyz[1] % xyz[2] % note) << endl;
	  }
	}
	vtkfile << endl;

    // now node vertices to allow node plotting
    vtkfile << str(boost::format("VERTICES %d %d") % nonNullNodes % (2*nonNullNodes)) << endl;
    for (uint32_t point = 0; point < nonNullNodes; point++) {
      vtkfile << "1 " << point << endl;
      vtkfile.flush();
    }
    vtkfile << endl;

    // now node indices from analyzeParaDIS
    vtkfile << str(boost::format("CELL_DATA %d") % nonNullNodes) << endl;
    vtkfile << "SCALARS node_index int" << endl;
    vtkfile << "LOOKUP_TABLE default" << endl;
	for (uint32_t armnum = 0; armnum < numArms; armnum++) {
	  for (deque<Node*>::iterator nodepos = arms[armnum]->mNodes.begin();
		   nodepos != arms[armnum]->mNodes.end(); nodepos++) {

		if (!*nodepos) continue; // wrappage

		vtkfile <<  (*nodepos)->mNodeIndex << " ";
	  }
	}
    vtkfile << endl << endl;
    vtkfile.close();

    msg = str(boost::format("*** Wrote VTK arm node tracefile %s for %s\n")% filename % description);
    dbprintf(0, msg.c_str());
    cerr << msg;
    return;
  }

  string doctext
  = string("DISCUSSION AND DEFINITIONS: \n"
           "ParaDIS data is a set of disconnected undirected graphs, which may contain cycles.  \n"
           "\n"
           "===================================================================\n"
           "NODES AND SEGMENTS\n"
           "All nodes in a paraDIS data set have at least one neighbor given in the paraDIS output file.  The connection, or neighbor relation, between two adjacent nodes is called an \"arm segment.\"  A node is considered an \"interior node\" if it has exactly two neighbors, else it is considered a \"terminal node.\"  The only exception to this is that loops always have one terminal node, which may have two neighbors.  \n"
           "\n"
           "===================================================================\n"
           "ARMS\n"
           "An \"arm\" is a sequence of interior nodes terminated by one or two terminal nodes.  A loop is an arm that has only one terminal node.  \n"
           "Every node is part of at least one arm.  If it's not an interior node, we say the node \"has arms.\"  The number of arms equals the number of neighbors, even if some are loops.  E.g., a terminal node with three neighbors has three arms by definition, even if two of the neighbors are part of the same loop, i.e., two of the arms might be the same arm.\n"
           "Every segment in an arm has the same Burgers vector.  See below.\n"
           "\n"
           "===================================================================\n"
           "Segments, arms, and metaarms have Burgers vectors associated with them.\n")
    + DocumentAllBurgersTypes()
    + string("\n"
             "===================================================================\n"
             "NODE TYPES and MONSTER NODES: \n"
             "Every node has a node type.  The vast majority of nodes are simply have NodeType = number of neighboring nodes.  But some nodes get negative types, and these are known as Monster Nodes.  Oooh, scary.  \n"
             "Only a terminal node may be a \"monster node\" A.K.A. \"M Type node.\" If a node is not an \"M\" then it is an \"N\" for \"non-monster\" or \"normal.\"  Interior nodes are always type \"N\" if anyone cares to ask, but this is usually ignored.  \n"
             "A monster is any node where all four basic 111 type arms are neighbors of the node.  Thus, monster nodes always have 4 or more arms. \n"
             "\n"
             "\n"
             "\n")
    + DocumentAllArmTypes()
    + string("\n"
             "===================================================================\n"
             "METAARMS\n"
             "MetaArms are collections of arms, either a loop, or terminated by two M nodes. Every arm in a MetaArm has the same Burgers vector, and there are always either zero or two M nodes in a MetaArm.  There are three main types, plus an \"unknonwn\" category for misfit arms:\n"
             "#define METAARM_UNKNOWN     0  // Not defined, error, or some other odd state\n"
             "#define METAARM_111         1  // Entirely composed of type 111 arms of the same burgers vector.   Does not include loops. \n"
             "#define METAARM_LOOP_111    2  // Contains a loop, composed entirely of type 111 arms.\n"
             "#define METAARM_LOOP_200    3  // Contains a loop, composed entirely of type 200 arms.\n");

  //===========================================================================
  void Node::PrintAllNodeTraces(string stepname) {
    for (vector<uint32_t>::iterator nodeid = Node::mTraceNodes.begin();
         nodeid != mTraceNodes.end(); nodeid++) {
      if (Node::mNodes.find(*nodeid) == Node::mNodes.end()) {
        if (*nodeid < Node::mNodes.size()) {
          cerr << "Warning: cannot print trace for requested node " << *nodeid <<  "as the node with that ID has been deleted." << endl;
        }
        else {
          cerr << "Warning: cannot print trace for requested node " << *nodeid <<  "as it has not yet been created." << endl;
        }
      }
      else {
		Node::mNodeVector[*nodeid]->WriteTraceFiles(stepname);
	  }
    }
    return;
  }
  //===========================================================================
  /*
    For each node that we want to trace, we write one set of files out before any decompositions, then one for decomposition step of each arm containing this node.
  */
  void Node::WriteTraceFiles(string stepname) {
    dbprintf(5, "Node::WriteTraceFiles(node %d)\n", mNodeIndex);
    string filebase = str(boost::format("%s-%s-Node_%d") % mTraceFileBasename % stepname % mNodeIndex);
    vector<Arm *> arms;
    vector<uint32_t> armdepths;
    vector<int> action;

    if (mNeighborArms.size() ) {
      for (vector<Arm*>::iterator arm = mNeighborArms.begin(); arm != mNeighborArms.end(); arm++) {
        dbprintf(2, "Node::WriteTraceFiles(node %d) adding BFS neighbors for arm %d\n", mNodeIndex, (*arm)->mArmID);
        (*arm)->FindBFSNeighbors(arms, armdepths, action);
      }
    }
    else {
      if (!mNeighborSegments.size()) {
        dbprintf(0, "Warning: requested trace node %d has no neighbor segments and thus is not a member of any arms", mNodeIndex);;
        return;
      }
      dbprintf(2, "Node::WriteTraceFiles(node %d) adding BFS neighbors for arm %d\n", mNodeIndex, mNeighborSegments[0]->mParentArm->mArmID);
      mNeighborSegments[0]->mParentArm -> FindBFSNeighbors(arms, armdepths, action);
    }
    string description = str(boost::format("node %1%")%mNodeIndex);
    paraDIS::WriteTraceFiles(filebase, description, arms, armdepths, action);
    return;


  }
  //===========================================================================
  std::string Node::Stringify(int indent, bool shortform) const {
	int ntype = mNodeType ;
    vector<int> idarray = GetNeighborArmIDs();
    string armids = arrayToString(idarray);
	string locstring = "UNSET";
	if (mLocation.size()) {
	  locstring = str(boost::format("%1% %2% %3%")% mLocation[0] % mLocation[1] % mLocation[2]);
	}
    std::string s = INDENT(indent) +
      str(boost::format("Node index %1%, %2%, type %3%, duplicate = %4%, neighbor arms: %5%, %6% neighbor segments, location: (%7%), address %8%")
          % mNodeIndex % GetNodeIDString() % ntype % ((int)mIsDuplicate) % armids % mNeighborSegments.size() % locstring % this);

    s += "\n" + INDENT(indent) + "Neighbor segments:";
	if (shortform) {
	  if (! mNeighborSegments.size()) {
		s += "<NONE>";
	  } else {
		s += "<";
		uint32_t i=0; while (i < mNeighborSegments.size()) {
		  if (mNeighborSegments[i]) {
			s += intToString(mNeighborSegments[i]->GetID());
		  } else {
			s += "NULL";
		  }
		  if (i == mNeighborSegments.size()-1) {
			s+= ">\n";
		  } else {
			s+= ", ";
		  }
		  ++i;
		}
	  }
    } else {
      s += "\n";
      uint32_t i=0; while (i < mNeighborSegments.size()) {
        s += INDENT(indent) + "neighbor " + intToString(i) + ": ";
        if (mNeighborSegments[i]) {
          s+= mNeighborSegments[i]->Stringify(indent+1);
        } else {
          s += INDENT(indent+1) + "(NULL)\n";
        }
        ++i;
      }
    }

	return s;
  }
  //===========================================================================
  void Node::ComputeNodeType(bool BCC) {
    /*!
      set my own type according to what I know -- this covers all nodes except butterfly ends.
    */
    mNodeType = mNeighborSegments.size();  // true for vast majority of nodes.

    if (!BCC) return;

    if (mNodeType >= 4) {//four-armed is forewarned!  Oh, I'm funny.
      /*!
        check for a multijunction
      */
      int neighbor = mNodeType;
      /*!
        btypes:  used to check for duplicates -- if any two arms are the same type, it's not a monster. There are only 8 types, but they are 1-based, so allocate 9 slots.
      */
      bool btypes[60] = {false};
      while (neighbor--) {
        const ArmSegment *theSegment =
          dynamic_cast<const ArmSegment *>(mNeighborSegments[neighbor]);
        int16_t btype =  theSegment->GetBurgersType();
        if (btypes[btype] || btype < 0) {
          continue; //not a monster, we're done
        }
        btypes[btype] = true;

      }
      /*!
        We have four uniquely valued arms.  Are they of the right mix?
      */
      if (btypes[ BCC_BURGERS_PPP] && btypes[ BCC_BURGERS_PPM] &&
          btypes[ BCC_BURGERS_PMP] && btypes[ BCC_BURGERS_PMM]) {
        mNodeType = -mNodeType;  // FOUND MULTIJUNCTION
      }
    }// end if four-armed or higher

    return;
  }/* end SetNodeType */


  //===========================================================================
  const std::vector< int> Node::GetNeighborArmIDs(void) const {
    vector<int> ids;
    vector<Arm*>::const_iterator armpos = mNeighborArms.begin();
    while (armpos != mNeighborArms.end()) {
      ids.push_back((*armpos)->mArmID);
      ++armpos;
    }
    return ids;
  }


  //===========================================================================
  /*!
    Identify all non-loop crossing arms.
    Crossing arms, when removed, must not cause the node to have less than
    three neighbors.
  */
  vector<vector<Arm *> >Node::IdentifyCrossArms(void) {
    vector <vector<Arm *> >crossarms;
    if (mNeighborSegments.size() < 5)
      return crossarms;

    vector<Arm*> armpair(2,NULL);
    vector<bool> matched(mNeighborSegments.size(), false);
    // first, identify looped cross arms
    for (uint32_t segnum = 0; segnum < mNeighborSegments.size()-1; segnum++) {
      if (!matched[segnum] &&
          mNeighborSegments[segnum]->mParentArm->mArmType == ARM_LOOP) {
        dbprintf(4, str(boost::format("Node::IdentifyCrossArms(): node(%1%): found potential crossing loop arm %2%.\n") % GetNodeIDString() % mNeighborSegments[segnum]->mParentArm->mArmID).c_str());
        armpair[0] = NULL;
        for (uint32_t match = segnum+1; match < mNeighborSegments.size(); ++match) {
          if (!matched[match] &&
              mNeighborSegments[match]->mParentArm == mNeighborSegments[segnum]->mParentArm ) {
            matched[segnum] = true;
            matched[match] = true;
            armpair[0] = mNeighborSegments[segnum]->mParentArm;
            armpair[0]->mExtendOrDetach = true;
            crossarms.push_back(armpair);
            dbprintf(4, str(boost::format("Node::IdentifyCrossArms(): node(%1%): Confirmed crossing loop arm %2%.\n") % GetNodeIDString() % mNeighborSegments[segnum]->mParentArm->mArmID).c_str());
            if (mNeighborSegments.size()- 2*crossarms.size() < 5)
              return crossarms;
          }
        }
        if (armpair[0] == NULL) {
          dbprintf(0, "************************************************************************************\n");
          dbprintf(0, str(boost::format("Node::IdentifyCrossArms(): node(%1%): ERROR! Could not find matching arm for crossing loop arm %2%.\n") % GetNodeIDString() % mNeighborSegments[segnum]->mParentArm->mArmID).c_str());
          dbprintf(0, "************************************************************************************\n");
        }
      }
    }


    // next, identify non-looped cross arms
    for (uint32_t segnum = 0; segnum < mNeighborSegments.size()-1; ++segnum) {
      int16_t btype = mNeighborSegments[segnum]->GetBurgersType();
      if (!matched[segnum]) {
        for (uint32_t other = segnum+1; other < mNeighborSegments.size() && !matched[segnum]; ++other) {
          if (!matched[other] &&
              mNeighborSegments[other]->GetBurgersType() == btype) {
            matched[other] = matched[segnum] = true;
            armpair[0] = mNeighborSegments[segnum]->mParentArm;
            armpair[1] = mNeighborSegments[other]->mParentArm;
            armpair[0]->mExtendOrDetach = armpair[1]->mExtendOrDetach = true;
            crossarms.push_back(armpair);
            dbprintf(4, str(boost::format("Node::IdentifyCrossArms(): node(%1%): found crossing arms %2% and %3%.\n") % GetNodeIDString() % (armpair[0]->mArmID) % (armpair[1]->mArmID)).c_str());
            if (mNeighborSegments.size()- 2*crossarms.size() < 5)
              return crossarms;
          }
        }/* other < mNeighborSegments.size(); */
      } /* (!matched[segnum]) */
    } /* segnum < mNeighborSegments.size()-1 */
    return crossarms;
  }

  //===========================================================================
  /*!
    Identify arms which cross over this node and glue them together.
    Simplifies decomposition of arms.
    Unmarks cross arms after detaching them.
  */
  void Node::DetachCrossArms(void) {

    vector <vector<Arm *> >crossarms = IdentifyCrossArms();
    if (crossarms.size()) {
      dbprintf(5, "DetachCrossArms(node %d):  got %d cross arm pairs\n", mNodeIndex, crossarms.size());
    }
    for (uint32_t armpair = 0; armpair < crossarms.size(); ++armpair) {
      crossarms[armpair][0]->mExtendOrDetach = false;
      if (crossarms[armpair][1] == NULL) {
		// The crossarms are actually one loop.  Detach it.
        crossarms[armpair][0]->DetachLoopFromNode(this);
      } else {
        crossarms[armpair][0]->DetachAndFuse(this, crossarms[armpair][1]);
        crossarms[armpair][1]->mExtendOrDetach = false;
      }
    }
    return ;
  }

  //===========================================================================
  int16_t ArmSegment::GetMNType(void) const {
    return mParentArm->mArmType;
  }

  //===========================================================================
  uint32_t ArmSegment::GetArmID(void) {
    return mParentArm->mArmID;
  }

  //===========================================================================
  uint32_t ArmSegment::GetMetaArmID(void) {
    if (!mParentArm) {
      dbprintf(0, "ERROR: GetMetaArmID() called on parentless segment.\n");
      return METAARM_UNKNOWN;
    }
    return mParentArm->GetMetaArmID();
  }

  //===========================================================================
  uint16_t ArmSegment::GetMetaArmType(void) {
    if (!mParentArm) {
      dbprintf(0, "ERROR: GetMetaArmType() called on parentless segment.\n");
      return METAARM_UNKNOWN;
    }
    return mParentArm->GetMetaArmType();
  }

  //===========================================================================
  std::string ArmSegment::Stringify(int indent) const {
	string parentID = "(NONE)";
	if (mParentArm) {
	  parentID = str(boost::format("%1%") % mParentArm->mArmID);
	}
    string s = str(boost::format("ArmSegment at %1%, number %2%, %3%, wrapped = %4%, duplicate = %5%, parentArm %6%, fuse distance %7%\n")% this % mSegmentIndex % ((int)mBurgersType) % ((int)mWrapped) % ((int)mIsDuplicate) % parentID % mLightTheFuseDistance);

    uint32_t epnum = 0;
    while (epnum < 2) {
      s+= INDENT(indent+1) + "ep "+intToString(epnum)+": ";
      if (mEndpoints[epnum]) s+= mEndpoints[epnum]->Stringify(0);
      else s+= "(NULL)";
      s+= "\n";
      epnum++;
    }
    epnum = 0;
    return INDENT(indent) + s;
  }



  //===========================================================================
  // 	# Thanks to http://mcngraphics.com/thelab/blender/connect/
  /* Computes a Y and Z rotation that take the segment from a Z orientation
	 (the blender cylinder uv mesh default) to its proper orientation.
  */
  std::string ArmSegment::BlenderRotationString(void) const {
	vector<float> segdir = SegmentDirection();
	dbprintf(5,str(boost::format("BlenderRotationString: SegmentDirection = <%f, %f, %f>\n") % segdir[0] % segdir[1] % segdir[2]).c_str());
	double distance = 0.0;
	for (int i = 0; i<3; i++) {
	  distance += segdir[i]*segdir[i];
	}
	distance = sqrt(distance);
	double theta = acos(segdir[2]/distance);
	double phi =  atan2(segdir[1], segdir[0]);
	string s = str(boost::format("0 %1% %2%") % theta % phi);
	dbprintf(5,str(boost::format("BlenderRotationString: returning \"%s\"\n") % s).c_str());
	return s;
  }

  //===========================================================================
  vector<float> ArmSegment::SegmentDirection(void) const {
    vector<float> direction(3,0);
    for (uint i=0; i<3; i++) {
      direction[i] = mEndpoints[1]->mLocation[i] - mEndpoints[0]->mLocation[i];
    }
    return direction;
  }


  //===========================================================================
  int16_t ArmSegment::ComputeScrewType(void) {
    vector<float> burg(3,1.0);
    if (mScrewType != SCREW_UNDEFINED)
      return mScrewType;

    mScrewType = BURGER_NO_SCREW;
    if (mBurgersType == BCC_BURGERS_PPM) {
      burg[2] = -1;
    }
    else if  (mBurgersType == BCC_BURGERS_PMP) {
      burg[1] = -1;
    }
    else if  (mBurgersType == BCC_BURGERS_PMM) {
      burg[1] = burg[2] = -1;
    }
    else {
      return mScrewType; // mScrewType = BURGER_NO_SCREW;
    }
    vector<float> dirVec = SegmentDirection();
	double theta =  AngularDifference(dirVec, burg, GetLength(), SQRT3);

    if (theta < mScrewToleranceAngle || theta > M_PI - mScrewToleranceAngle) {
      mScrewType = BURGER_SCREW;
    } else if (theta > M_PI/2.0 - mScrewToleranceAngle && theta < M_PI/2.0 + mScrewToleranceAngle ) {
      mScrewType = BURGER_EDGE;
    }
    return mScrewType;
  }



  //===========================================================================
  /*!
    This prints out an arm and its neighboring arms using BFS order
    to the given depth to a text file and a VTK file.
    File created: basename.txt, basename.vtk
  */
  void Arm::WriteTraceFiles(string stepname) {
    bool trace = false;
    for (uint32_t n = 0; n<mTraceArms.size(); n++) {
      if (mArmID == mTraceArms[n])
        trace = true;
    }
    if (!trace || !mTerminalNodes.size()) return;

    dbprintf(5, "Arm::WriteTraceFiles(arm %d)\n", mArmID);
    string filebase = str(boost::format("%s-%s-arm_%d") % mTraceFileBasename % stepname % mArmID);

    vector<Arm *> arms;
    vector<uint32_t> armdepths;
    vector<int> action;
    FindBFSNeighbors(arms, armdepths, action);
    string description = str(boost::format("arm %1%")%mArmID);
    paraDIS::WriteTraceFiles(filebase, description, arms, armdepths, action);
    return;
  }

  //===========================================================================
  void Arm::FindBFSNeighbors(vector<Arm *> &arms, vector<uint32_t> &armdepths, vector<int> &action) {
    arms.push_back(this);
    armdepths.push_back(0);
    action.push_back(2);

    // first, collect the list of arms in BFS order:
    uint32_t armnum = 0;
    uint32_t depth = 0;
    while (depth++ < mTraceDepth && armnum < arms.size()) {
      uint32_t lastInLevel = arms.size();
      for (; armnum < lastInLevel; armnum++) {
        // append arm's neighbors onto arms
        Arm *arm = arms[armnum];
        arm->mSeen = true;
        uint32_t numneighbors = arm->GetNumNeighborArms();
        dbprintf(5, "Arm::WriteTraceFiles(arm %d): arm %d, depth %d: looking at %d neighbor arms\n", mArmID, arm->mArmID, depth, numneighbors);
        while (numneighbors--) {
          Arm *nei = arm->GetNeighborArm(numneighbors);
          if (!nei->mSeen){
            arms.push_back(nei);
            dbprintf(5, "Arm::WriteTraceFiles(arm %d): arm %d, depth %d: pushed back arm %s\n", mArmID, arm->mArmID, depth, nei->Stringify(0).c_str());
            armdepths.push_back(depth);
            if (arm->mExtendOrDetach) {
              action.push_back(1);
            } else {
              action.push_back(0);
            }
            nei->mSeen = true;
          }
        }
      }
    }
    for (uint32_t arm = 0; arm < arms.size(); arm++) {
      arms[arm]->mSeen = false;
    }
    return ;
  }

  //===========================================================================
  string Arm::StringifyExternalArms(int indent) const {
    if (mTerminalNodes.size() == 0) return "(no terminal nodes)";
    string s1 = INDENT(indent)+"<"; // , s2 = ", <";
    int nodenum = mTerminalNodes.size();
    while (nodenum--) {
      Node *thisNode = mTerminalNodes[nodenum];
      int neighbornum = thisNode->mNeighborArms.size();

      while (neighbornum--) {
        const Arm *arm = thisNode->mNeighborArms[neighbornum];
        /*!
          Only check exterior segments (those which don't belong to *this).
        */
        if (arm == this) continue;
        s1 += str(boost::format("[%1%: %2%]")
                  % arm->mArmID % BurgTypeToName(arm->GetBurgersType()));
		if (neighbornum || nodenum) s1+= ", ";
      }
    }
    return s1 + ">";
  }


  //===========================================================================
  uint32_t Arm::GetMetaArmID(void) {
    if (!mParentMetaArm) {
      dbprintf(0, "Error: GetMetaArmID() called on parentless arm.\n");
      return METAARM_UNKNOWN;
    }
    return mParentMetaArm->GetMetaArmID();
  }

  //===========================================================================
  uint16_t Arm::GetMetaArmType(void) {
    if (!mParentMetaArm) {
      dbprintf(0, "Error: GetMetaArmType() called on parentless arm.\n");
      return METAARM_UNKNOWN;
    }
    return mParentMetaArm->GetMetaArmType();
  }

#if LINKED_LOOPS
  //===========================================================================
  void Arm::CheckForLinkedLoops(void) {
    int err = 0;
    if (mCheckedForLinkedLoop) return;
    mCheckedForLinkedLoop = true;
	if (!mNodes.size()) return;
    /*!
	  Iff
 	  CASE A:  we have one terminal node with four arms and one neighbor arm, or
 	  CASE B:  two terminal nodes with three arms and num (non-self) neighbors == 2, then we are part of a linked loop.
      Note that it's not true that if you have two terminal nodes with N arms and number of arms on each terminal node == N-1 that it's necessarily a linked loop, although it certainly is an interesting beast.
    */
	// CASE A
 	map<Arm*, bool> myNeighbors;
 	if (mArmType == ARM_LOOP) {
 	  if (mNodes[0]->mNeighborSegments.size() == 4) {
 		int n = 4;
 		while (n--) {
 		  Arm *arm = mNodes[0]->mNeighborSegments->mParentArm;
 		  myNeighbors[arm] = true;
		}
	  }
	  if (mFoundArmIDs.size() == 2) {
		mPartOfLinkedLoop = true;
	  }
	}

	else if (mNodes.size() > 1) {
	  // check for CASE B
	  int nodenum = 0;
	  while (true) {
		int n = mNodes[nodenum]->mNeighborSegments.size()
		  if (n != 3) break;
		while (n--) {
		  Arm *arm = mNodes[nodenum]->mNeighborSegments->mParentArm;
		  myNeighbors[arm] = true;
		}
		if (mFoundArmIDs.size() > 3 || nodenum == mNodes.size()-1) break;
		nodenum = mNodes.size()-1;
	  }
	  if (mFoundArmIDs.size() == 3) {
		mPartOfLinkedLoop = true;
	  }
	}

	/*!
	  If we are not part of a linked loop, none of our neighbors are,
	  and if we are part of a linked loop, all of our neighbors are.
	  So mark this arm and all neighbors of both terminal nodes appropriately.
	*/
	for (map<Arm*, bool>::iterator arm = myNeighbors.begin(); arm != myNeighbors.end(); arm++) {
	  arm->first->mCheckedForLinkedLoop = true;
	  arm->first->mPartOfLinkedLoop = mPartOfLinkedLoop;
	}
	return;

  }
#endif // LINKED_LOOPS

  //===========================================================================
  double Arm::ComputeLength(void) {
    if (!mNumNormalSegments) {
      mArmLength = 0;
    }

    else {
      double savedLength = mArmLength;
      mArmLength = 0;
      deque<ArmSegment*>::iterator segpos = mSegments.begin(), endseg = mSegments.end();

      while (segpos != endseg) {
        mArmLength += (*segpos)->GetLength(true);
        ++segpos;
      }
      if (mArmLength > mLongestLength) mLongestLength = mArmLength;
      if (fabs(1.0 - mArmLength/ savedLength) > 0.0001) {
        dbprintf(5, "ComputeLength(): arm %d computed different length %g than saved length %g\n", mArmID, mArmLength, savedLength);
      } else {
        dbprintf(5, "ComputeLength(): arm %d has length %g and saved length %g\n", mArmID, mArmLength, savedLength);
      }
    }
    return mArmLength;
  }

  //===========================================================================
  void Arm::GetNodesAndSegments(Node*startNode, vector<Node*>*outnodes, vector<ArmSegment*>*outsegs) const {
	if (!mTerminalNodes.size()) return;
	dbprintf(6, "Arm::GetNodesAndSegments(arm %d)\n", mArmID);

	// ===============================================
	// Determine start and end nodes
	Node *endNode = NULL;
	if (!startNode) {
	  startNode = mTerminalNodes[0];
	  endNode = mTerminalNodes[mTerminalNodes.size()-1];
	}
	else {
	  for (uint i=0; i<mTerminalNodes.size(); i++) {
		if (startNode == mTerminalNodes[i]) {
		  endNode = mTerminalNodes[mTerminalNodes.size()-1-i];
		}
	  }
	  paradis_assert(endNode);
	}
	// ===============================================
	// Find start segment
	ArmSegment *startSegment = NULL;
	for (vector<ArmSegment*>::iterator seg = startNode->mNeighborSegments.begin();
		 seg != startNode->mNeighborSegments.end() && !startSegment; seg++) {
	  if ((*seg)->mParentArm == this) {
		startSegment = *seg;
	  }
	}
	printNodes();
	printSegments();
	paradis_assert(startSegment);
   dbprintf(6, "Arm::GetNodesAndSegments(arm %d): Found start segment %s and startNode %s\n", mArmID, startSegment->Stringify(0).c_str(), startNode->Stringify(0).c_str());

	// ===============================================
   uint32_t nodenum= 0, segnum = 0;
	ArmSegment *currentSegment = startSegment;
	Node *currentNode = startNode;
	// push back the start node
	if (outnodes) {
	  dbprintf(6, "Arm::GetNodesAndSegments(arm %d): pushing back node %d: %s\n", mArmID, nodenum++, currentNode->Stringify(0,true).c_str());
	  outnodes->push_back(currentNode);
	}

	/* Push back in a loop until the end node is found:
	   A. next segment,
	   B. possible wrapped node,
	   C. next node
	*/
	while (true) {
	  if (outsegs) {
		outsegs->push_back(currentSegment); // even if wrapped!
		dbprintf(6, "Arm::GetNodesAndSegments(%d): pushed back segment %d: %s\n", mArmID, segnum++, currentSegment->Stringify(0).c_str() );
	  }
	  if (outnodes && currentSegment->mWrapped) {
		outnodes->push_back(NULL);
		dbprintf(6, "Arm::GetNodesAndSegments(arm %d): pushed back WRAPPED (null) node %d\n", mArmID, nodenum++);
	  }
	  currentNode = currentSegment->GetOtherEndpoint(currentNode);
	  paradis_assert(currentNode);
	  dbprintf(6, "Arm::GetNodesAndSegments(arm %d): next node: %s\n", mArmID, currentNode->Stringify(0,true).c_str());
	  if (outnodes) {
		dbprintf(6, "Arm::GetNodesAndSegments(arm %d): pushing back node %d: %s\n", mArmID, nodenum++, currentNode->Stringify(0,true).c_str());
		outnodes->push_back(currentNode);
	  }
	  string segstring = "(NULL)";
	  if (currentNode->mNeighborSegments.size() == 2 && currentNode != endNode) {
		currentSegment = currentNode->GetOtherNeighbor(currentSegment);
		segstring = currentSegment->Stringify(0);
		paradis_assert(currentSegment);
	  }
	  else {
		dbprintf(6, "Arm::GetNodesAndSegments(arm %d): I think I found end node.\n", mArmID, currentNode->mNeighborSegments.size());
		break;
	  }
	}
	paradis_assert(currentNode == endNode);
	int numnodes = outnodes ?  outnodes->size() : -1, numsegs = outsegs ? outsegs->size() : -1;
	dbprintf(5, "Arm::GetNodesAndSegments(arm %d): finished.  Returning %d nodes and %d segments.  mNumNormalSegments is %d and mNumWrappedSegments is %d\n", mArmID, numnodes, numsegs, mNumNormalSegments, mNumWrappedSegments);
	paradis_assert(numsegs < 1 || (uint)numsegs == mNumNormalSegments + mNumWrappedSegments);
   return;
  }

  //===========================================================================
  void Arm::Classify(void) {
#ifdef RC_CPP_VISIT_BUILD
    int err = -1;
#endif
    dbprintf(5, "Arm::Classify(%d) called for arm %s\n", mArmID, Stringify(0).c_str());
#if LINKED_LOOPS
    CheckForLinkedLoops();
#endif
	if (!mNodes.size() || !mSegments.size()) {
	  dbprintf(5, "Arm::Classify(%d): empty arm.\n", mArmID);
 	  mArmType = ARM_EMPTY;
	  return;
    }
    int16_t burg = GetBurgersType();
    if (burg == HCP_BURGERS_UNKNOWN || burg >= 1000) { // HCP ARM
      mArmType = burg;
      if (burg != HCP_BURGERS_UNKNOWN &&
          mTerminalNodes.size() == 2 &&
          mTerminalNodes[0]->mNeighborSegments.size() == 3 &&
          mTerminalNodes[1]->mNeighborSegments.size() == 3) {
        mArmType += HCP_ARM_OF_INTEREST;
      }
    }
    else {  // BCC ARM
      int termnode = mTerminalNodes.size();
      while (termnode--) {
        if (mTerminalNodes[termnode]->mNeighborSegments.size() == 1) {
          dbprintf(4, "Arm::Classify(%d): Found terminal node with a single neighbor.  Marking as ARM_BOUNDARY.\n", mArmID);
          mArmType = ARM_BOUNDARY;
        }
      }
      if (mArmType != ARM_BOUNDARY && mArmType != ARM_LOOP) {
        if (mTerminalNodes[0]->IsTypeM() && mTerminalNodes[1]->IsTypeM()) {
          mArmType = ARM_BCC_MM_111;
        } else if (mTerminalNodes[0]->IsTypeM() || mTerminalNodes[1]->IsTypeM()){
          mArmType = ARM_BCC_MN_111;
        } else {
          mArmType = ARM_BCC_NN_111;
        }

        // This changes _111 to _200 by definition
        int btype = mTerminalSegments[0]->GetBurgersType();
        if (btype == BCC_BURGERS_NONE || btype == BCC_BURGERS_UNKNOWN) {
          mArmType = ARM_UNKNOWN;
        }
        else {
          // All non-loop arms should be type 111 now.
          paradis_assert(btype == BCC_BURGERS_PPP || btype == BCC_BURGERS_PPM ||
                         btype == BCC_BURGERS_PMP || btype == BCC_BURGERS_PMM);
        }
      }


      if (mThreshold > 0 && mArmLength < mThreshold) {
        if (mArmType == ARM_BCC_NN_111) mArmType = ARM_BCC_SHORT_NN_111;
      }
    } /* end BCC ARM */
    dbprintf(5, "Arm::Classify(%d): gave arm type %d (%s).\n", mArmID, mArmType, ArmTypeToName(mArmType).c_str());
    return;
  }

  //===========================================================================
  /* Replace terminal node "node" with "replacement"*/
  void Arm::ReplaceTerminalNode(Node* node, Node *replacement) {
    node->RemoveNeighborArm(this, true);
    replacement->mNeighborArms.push_back(this);
    int segnum = node->mNeighborSegments.size();
    while(segnum--) {
      ArmSegment *segment = node->mNeighborSegments[segnum];
      if (segment->mParentArm == this && segment->HasEndpoint(node)) {
        segment->ReplaceEndpoint(node, replacement);
      }
    }
	int nodenum = mTerminalNodes.size();
	while (nodenum--) {
	  if (mTerminalNodes[nodenum] == node) {
		mTerminalNodes[nodenum] = replacement;
	  }
	}
	if (mNodes[0] == node) {
	  mNodes[0] = replacement;
	}
	else if (mNodes[mNodes.size()-1] == node) {
	  mNodes[mNodes.size()-1] = replacement;
	}
	else {
	  dbprintf(5, str(boost::format("Arm::ReplaceTerminalNode(arm %1%).ERROR  Our original node is not at either end of mNodes.\n")%mArmID).c_str());
	  printNodes();
	  errexit;
	}
    dbprintf(5, str(boost::format("Arm::ReplaceTerminalNode(arm %1%).  Our Replacement  node for arm is: %2%.  Original node is now: %3%\n") % mArmID % (replacement->Stringify()) % (node->Stringify())).c_str());
    return;
  }

  //===========================================================================
  void Arm::DetachLoopFromNode(Node *node) {
    dbprintf(4, str(boost::format("Arm::DetachLoopFromNode(arm %1%, node %2%)\n")%mArmID% node->GetNodeIDString()).c_str());

    if (node->mNeighborArms.size() == 1) {
      dbprintf(4, "Arm::DetachLoopFromNode(arm %d): the node to detach from only has us as a neighbor -- nothing to detach from.\n", mArmID);
      return;
    }

    dbprintf(5, str(boost::format("Arm::DetachLoopFromNode(arm %1%): Before detaching, the looped arm looks like this: %2%\n") % mArmID % Stringify(0, false)).c_str());
    dbprintf(4, "Arm::DetachLoopFromNode(arm %d): Creating a duplicate node to serve as a new interior node for the fused arm. \n", mArmID);

    ReplaceTerminalNode(node, Node::DuplicateNode(node));
    dbprintf(5, str(boost::format("Arm::DetachLoopFromNode(arm %1%): After detaching, the looped arm looks like this: %2%\n") % mArmID % Stringify(0, false)).c_str());

    return;
  }

  //===========================================================================
  /* Detach both arms from the node and create a new arm by fusing the two.
	 This is pretty similar to ExtendBySegments
	 Extend this arm by the segments of the other and then delete the other.
     This arm will not be a looped arm.
  */
  void Arm::DetachAndFuse(Node *node, Arm *other) {
    dbprintf(4, str(boost::format("Arm::DetachAndFuse(arm %1%, node %2%, other %3%)\n")% mArmID% node->GetNodeIDString()% other->mArmID).c_str());

    dbprintf(5, str(boost::format("Arm::DetachAndFuse(arm %1%): Before fusing, this arm looks like this: %2%\n") % mArmID % Stringify(0, false)).c_str());
    dbprintf(5, str(boost::format("Arm::DetachAndFuse(arm %1%): Before fusing, the other arm looks like this: %2%\n") % mArmID % other->Stringify(0, false)).c_str());

	// =========================================================
	// we cannot be a loop.
	paradis_assert(mArmType != ARM_LOOP);

	// new code:  we should be able to just set it up so we can call ExtendBySegments()
	// =========================================================
	// See if we need a new node.
    if (node->mNeighborSegments.size() > 2) {
      dbprintf(4, "Arm::DetachAndFuse(arm %d): Node %s does not have two neighbors, so cannot be reused for our interior node; create a duplicate node to serve as a new interior node for the fused arm, leaving other arms unchanged.\n", mArmID, node->GetNodeIDString().c_str());
      Node *newTermNode = Node::DuplicateNode(node);
	  ReplaceTerminalNode(node, newTermNode);
	  other->ReplaceTerminalNode(node, newTermNode);
	  node = newTermNode;
	}  else {
      dbprintf(4, "Arm::DetachAndFuse(arm %d): Node %s already has two neighbors, so can be reused as an interior node; we can just call ExtendBySegments.\n", mArmID, node->GetNodeIDString().c_str());

    }
 	ExtendBySegments(other, node, true);

    mNumDestroyedInDetachment++;

 	// =========================================================
	if (node->mNeighborSegments.size() == 2) {
	  Arm *arm = node->mNeighborSegments[0]->mParentArm,
		*other = node->mNeighborSegments[1]->mParentArm;
	  if (arm != other) {
		dbprintf(4, "After DetachAndFuse, there are two remaining arms as node neighbors of the node from which we detached.  These should to be fused immediately.  Recursively calling DetachAndFuse before cleanup... \n");
		arm->DetachAndFuse(node, other);
	  }
    }

    dbprintf(5, str(boost::format("Arm::DetachAndFuse(arm %1%): Complete. This arm looks like this: %2%\n") % mArmID % Stringify(0, false)).c_str());
    dbprintf(5, str(boost::format("Arm::DetachAndFuse(arm %1%): Complete. The other destroyed arm looks like this: %2%\n") % mArmID % other->Stringify(0, false)).c_str());

    return;
  }


  //===========================================================================
  void Arm::ExtendBySegments(Arm *sourceArm, Node *sharedNode, bool reuseSegments) {
	dbprintf(6, "Arm::ExtendBySegments(arm %d): sourceArm = %d, sharedNode = %s, reuse = %d\n", mArmID, sourceArm->mArmID, sharedNode->Stringify(0, true).c_str(), (int)reuseSegments);
	dbprintf(6, "Arm::ExtendBySegments(arm %d): destination arm (*this) looks like this: %s \n", mArmID, Stringify(0,false).c_str());
	paradis_assert(sourceArm->mSegments.size() && mNodes.size() && mSegments.size());

	//======================================================================
	// Figure out which terminal node is shared in each arm and which is not.
    int sharedTermNodeNum = mTerminalNodes.size();
    while (sharedTermNodeNum-- ) {
      if (mTerminalNodes[sharedTermNodeNum] == sharedNode) break;
    }
	paradis_assert(sharedTermNodeNum != -1);
   int sourceSharedTermNodeNum = sourceArm->mTerminalNodes.size();
    while (sourceSharedTermNodeNum-- ) {
      if (sourceArm->mTerminalNodes[sourceSharedTermNodeNum] == sharedNode) break;
    }
	paradis_assert(sourceSharedTermNodeNum != -1);

	paradis_assert(sourceArm->mTerminalNodes.size() == 2);


	/*
	  Find the terminal segment that has the shared node as an endpoint.
	  Replace the share node with our new interior node as its endpoint.
	*/
	int sharedSegmentNum = mTerminalSegments.size();
	while(sharedSegmentNum--) {
	  if (mTerminalSegments[sharedSegmentNum]->HasEndpoint(sharedNode)) {
		break;
	  }
	}
	paradis_assert(sharedSegmentNum != -1);

	if (mTerminalSegments.size() == 1) {
	  // mTerminalSegments[0] is going to get replace later and we will then have two segments -- unless we form a loop.
	  mTerminalSegments.push_back(mTerminalSegments[0]);
	}


	//====================================================
	// NEW METHOD:
	// Copy mNodes and mSegments as needed
	// "forward" means "shared node is at position 0 in array"
	bool sourceForward = true, // source:  0....S  with S+1 elements
	  destForward = true; // dest:  0....D  with D+1 elements
	if (sharedNode != sourceArm->mNodes[0]) {
	  if (sharedNode != sourceArm->mNodes[sourceArm->mNodes.size()-1]) {
		sourceArm->printNodes();
	  }
	  paradis_assert(sharedNode == sourceArm->mNodes[sourceArm->mNodes.size()-1]);
	  sourceForward = false; // source:  S....0
	}
	if (sharedNode !=  mNodes[0]) {
	  paradis_assert(sharedNode == mNodes[mNodes.size()-1]);
	  destForward = false; // dest:  D....0
	}



	if (!destForward) { // 0....D forward order is required
	  reverse(mNodes.begin(), mNodes.end());
	  reverse(mSegments.begin(), mSegments.end());
	  dbprintf(6, "Arm::ExtendBySegments(arm %d): 0....D forward order is required.  Reversing destination arm.\n", mArmID);
	} else {
	  dbprintf(6, "Arm::ExtendBySegments(arm %d): 0....D forward order is detected.  Not reversing destination arm.\n", mArmID);
	}
	printNodes();
	printSegments();

	if (!sourceForward) { // 0....S forward order is required
	  reverse(sourceArm->mNodes.begin(), sourceArm->mNodes.end());
	  reverse(sourceArm->mSegments.begin(), sourceArm->mSegments.end());
	  dbprintf(6, "Arm::ExtendBySegments(arm %d): 0....S forward order is required.  Reversing source arm %d .\n", mArmID, sourceArm->mArmID);
	} else {
	  dbprintf(6, "Arm::ExtendBySegments(arm %d): 0....S forward order is detected.  Did not reverse source arm.\n", mArmID);
	}
	sourceArm->printNodes();
	sourceArm->printSegments();

	// NOW START ADDING SOURCE NODES
	// remove shared node -- it will be replaced as needed
	mNodes.pop_front();

	deque<Node *>::iterator pos = sourceArm->mNodes.begin();
	for (uint32_t nodenum = 0; nodenum < sourceArm->mNodes.size()-1; nodenum++, pos++) {
	  Node *node = (*pos);
	  string nodestring = "(NULL)";
	  if (node) {
		string dupe = "";
		if (!reuseSegments) {
		  dbprintf(6, "Arm::ExtendBySegments(arm %d): Creating duplicate node %d\n", mArmID, nodenum);
		  node = Node::DuplicateNode(*pos);
		  dupe = "DUPLICATE: ";
		}
		nodestring = dupe + node->Stringify();
	  }
	  if (!nodenum) {
		dbprintf(6, "Arm::ExtendBySegments(arm %d): special case:  the first node %d has to be connected to destination first segment %d\n", mArmID, node->mNodeIndex, mSegments[0]->mSegmentIndex);
		node->mNeighborSegments.clear();
		node->mNeighborSegments.push_back(mSegments[0]);
		mSegments[0]->mEndpoints[0] = node;
		if (mSegments[0]->mWrapped) {
		  paradis_assert(!mNodes[0]); // this has to be NULL
		  mSegments[0]->mEndpoints[1] = mNodes[1];
		}
		else {
		  mSegments[0]->mEndpoints[1] = mNodes[0];
		}
	  }
	  dbprintf(6, "Arm::ExtendBySegments(arm %d): push_front(node %d): %s\n", mArmID, nodenum, nodestring.c_str());
	  mNodes.push_front(node);
	}

	dbprintf(6, "Arm::ExtendBySegments(arm %d): push_front(final node): %s\n", mArmID, (*pos)->Stringify().c_str());
	mNodes.push_front(*pos); // the source arm's "far node" will be our new terminal node.

	if (reuseSegments) {
	  // start fixing our shared Node here
	  sharedNode->mNeighborSegments.clear();
	  sharedNode->mNeighborSegments.push_back(mSegments[0]);
	  paradis_assert(mSegments[0]->HasEndpoint(sharedNode));
	}
	uint32_t segnum = 0;
	int16_t myBurgers = mTerminalSegments[0]->mBurgersType;
	for (deque<ArmSegment *>::iterator pos = sourceArm->mSegments.begin(); pos != sourceArm->mSegments.end(); pos++, segnum++) {
	  ArmSegment *seg = *pos;

	  string dupe = "";
	  if (!reuseSegments) {
		seg = new ArmSegment(*seg);
		dbprintf(6, "Arm::ExtendBySegments(arm %d): Creating duplicate segment %d\n", mArmID, segnum);
		dupe = "DUPLICATE: ";
		ArmSegment::mArmSegments[seg->mSegmentIndex] = seg;
		ArmSegment::mNumDecomposed ++;
	  }
	  if (reuseSegments && !segnum) {
		sharedNode->mNeighborSegments.push_back(seg);
		paradis_assert(seg->HasEndpoint(sharedNode));
	  }
	  seg->mParentArm = this;
	  seg->mBurgersType = myBurgers;
	  if (seg->mWrapped) {
		mNumWrappedSegments++;
	  }
	  else {
		mNumNormalSegments++;
		mArmLength += seg->GetLength(true);
	  }
	  dbprintf(6, "Arm::ExtendBySegments(arm %d): push_front(segment %d): %s\n", mArmID, segnum, (dupe + seg->Stringify()).c_str());
	  mSegments.push_front(seg);

	}
	if (!reuseSegments) {
	  deque<Node *>::iterator node1 = mNodes.begin();
	  for (uint32_t segnum = 0; segnum != sourceArm->mSegments.size(); segnum++){
		ArmSegment *seg = mSegments[segnum];
		deque<Node *>::iterator node0 = node1++;
		if (!*node1) {
		  ++node1;
		  paradis_assert(seg->mWrapped);
		}
		seg->mEndpoints[0] = *node0;
		seg->mEndpoints[1] = *node1;
		(*node0)->mNeighborSegments.push_back(seg);
		(*node1)->mNeighborSegments.push_back(seg);
		dbprintf(6, "Arm::ExtendBySegments(arm %d): connected segment %d to nodes %d and %d\n", mArmID, seg->mSegmentIndex, (*node0)->mNodeIndex, (*node1)->mNodeIndex);

	  }
	}

	mTerminalSegments[sharedSegmentNum] = mSegments[0];
	mTerminalNodes[sharedTermNodeNum] = mNodes[0];
	mNodes[0]->mNeighborArms.push_back(this);

	if (mTerminalNodes.size() == 2 && mTerminalNodes[0] == mTerminalNodes[1]) {
    dbprintf(5, "\nArm::ExtendBySegments(%d): After extension the destination arm is a loop (has two matching terminal nodes)", mArmID);
	  mTerminalNodes.clear();
	  mTerminalNodes.push_back(mNodes[0]);
	  mTerminalSegments.clear();
	  mTerminalSegments.push_back(mSegments[0]);
	  mArmType = ARM_LOOP;
	}

    dbprintf(5, "\nArm::ExtendBySegments(%d): After extension the destination arm looks like this: %s", mArmID, Stringify(0,false).c_str());
	printNodes();
	printSegments();

	if (reuseSegments) {
	  // =========================================================
	  // Gut the other arm
	  for (uint32_t n = 0; n < sourceArm->mTerminalNodes.size(); n++) {
		sourceArm->mTerminalNodes[n]->RemoveNeighborArm(sourceArm, true);
	  }
	  sourceArm->mTerminalSegments.clear();
	  sourceArm->mTerminalNodes.clear();
	  sourceArm->mNodes.clear();
	  sourceArm->mSegments.clear();
	  sourceArm->mNumNormalSegments = 0;
	  sourceArm->mNumWrappedSegments = 0;
	  sourceArm->mArmType = ARM_EMPTY;
	  sourceArm->mArmLength = 0;
	}

	return;
    return;
  }

  //===========================================================================
  void Arm::printNodes(void) const {
	if (dbg_isverbose() < 5) return;
	dbprintf(6, "Arm::printNodes (arm %d)\n", mArmID);
	for (uint32_t n = 0; n < mNodes.size(); n++) {
	  string nodestring = "(NULL)";
	  if (mNodes[n]) nodestring =  mNodes[n]->Stringify();
	  dbprintf(5, "Arm %d, Node %d: %s\n", mArmID, n, nodestring.c_str());
	}
	return;
  }

  //===========================================================================
  void Arm::printSegments(void) const {
	if (dbg_isverbose() < 5) return;
	dbprintf(6, "Arm::printSegments (arm %d)\n", mArmID);
	for (uint32_t n = 0; n < mSegments.size(); n++) {
	  string segstring = "(NULL)";
	  if (mSegments[n]) segstring =  mSegments[n]->Stringify();
	  dbprintf(5, "Arm %d, Seg %d: %s\n", mArmID, n, segstring.c_str());
	}
	return;
  }

  //===========================================================================
  void Arm::CheckNodesAndSegments(void) const {
	if (dbg_isverbose() < 6) return;
	// reality check:
	vector<ArmSegment*> testSegs;
	vector<Node*> testNodes;
	GetNodesAndSegments(mNodes[0], &testNodes, &testSegs);
	if (testNodes.size() != mNodes.size()) {
	  dbprintf(5, "testNodes.size(%d) != mNodes.size(%d)\n", testNodes.size(), mNodes.size()) ;
	  printNodes();
	}
	if (mArmType == ARM_LOOP &&
		(mNodes[1] != testNodes[1] ||
		 (!mNodes[1] && mNodes[2] != testNodes[2]))) {
	  reverse(testNodes.begin(), testNodes.end());
	}
	for (uint32_t n = 0; n < mNodes.size(); n++) {
	  if (mNodes[n] != testNodes[n]) {
		string teststring = "(NULL)", nodestring = "(NULL)";
		if (testNodes[n]) teststring = testNodes[n]->Stringify();
		if (mNodes[n]) nodestring = mNodes[n]->Stringify();
		printNodes();
		dbprintf(5, "MISMATCH: testNodes[%d] %s\n\n mNodes[%d] %s\n", n, teststring.c_str(), n, nodestring.c_str());
		paradis_assert(mNodes[n] == testNodes[n]);
	  }
	}

	paradis_assert(testNodes.size() == mNodes.size());
	if (testSegs.size() != mSegments.size()) {
	  dbprintf(5, "testSegs.size(%d) != mSegments.size(%d)\n", testSegs.size(), mSegments.size()) ;
	  printSegments();
	}
	if (mArmType == ARM_LOOP && testSegs[0] != mSegments[0]) {
	  reverse(testSegs.begin(), testSegs.end());
	}
	for (uint32_t n = 0; n < mSegments.size(); n++) {
	  if (testSegs[n] !=  mSegments[n]) {
		string teststring = "(NULL)", segstring = "(NULL)";
		if (testSegs[n]) teststring = testSegs[n]->Stringify();
		if (mSegments[n]) segstring = mSegments[n]->Stringify();
		printSegments();
		dbprintf(5, "MISMATCH: testSegs[%d] %s \n\n mSegments[%d] %s\n", n, teststring.c_str(), n, segstring.c_str());
		paradis_assert(testSegs[n] ==  mSegments[n]);
	  }
	}
	paradis_assert(testSegs.size() == mSegments.size());
	dbprintf(5, "CheckNodesAndSegments(arm %d) PASSED\n", mArmID);
	return;
  }
  //===========================================================================
  void Arm::ExtendByArm(Arm *sourceArm, Node *sharedNode, bool reuseSegments) {

    // identify the shared terminal node in the neighbor arm:
    dbprintf(5, "\n======================================\n   ExtendByArm(source = %d): Extending arm: %s\n", sourceArm->mArmID, Stringify(0, false).c_str());

    ArmSegment * otherSharedSegment = NULL;
    if (mArmType == ARM_LOOP) {
	  dbprintf(4, "ExtendByArm(%d): Extending a looped arm.  We need to duplicate our terminal segment so we can \"double extend\" the arm.\n", mArmID);
	  // we need to find our other terminal segment so we can "double extend"
	  otherSharedSegment = mSegments[0];
	  if (otherSharedSegment == mTerminalSegments[0]) {
		otherSharedSegment = mSegments[mSegments.size()-1];
	  }
	  // For loops, both terminal segments must have shared node as endpoint
	  paradis_assert(otherSharedSegment->HasEndpoint(sharedNode));

	  mTerminalSegments.push_back(otherSharedSegment);

	  dbprintf(4, "ExtendByArm(%d): We are extending a looped arm, so have to duplicate our terminal node for the algorithm to proceed correctly.\n", mArmID);
      mTerminalNodes.push_back(sharedNode);
    }
	paradis_assert( mTerminalNodes.size() == 2);

	paradis_assert(sourceArm->mArmType != ARM_LOOP);

    if (mArmType != ARM_LOOP) {
      dbprintf(5, "ExtendByArm(%d): CASE 1: no loops: extend by source once.\n", mArmID);
      ExtendBySegments(sourceArm, sharedNode, reuseSegments);
    }
    else /* if (mArmType == ARM_LOOP && !sourceIsLoop) */  {
      dbprintf(5, "ExtendByArm(%d): CASE 2: This arm is a loop, extend by source twice, possibly reuseing the segments the second time.\n", mArmID);
      ExtendBySegments(sourceArm, sharedNode, false);
      ExtendBySegments(sourceArm, sharedNode, reuseSegments);
    }

	MakeAncestor(sourceArm);

    dbprintf(5, "\nExtendByArm(%d): After extension the source arm looks like this: %s", sourceArm->mArmID, sourceArm->Stringify(0,false).c_str());
	sourceArm->printNodes();
	sourceArm->printSegments();
	CheckNodesAndSegments();
    return;
  }

  //===========================================================================
  /*!
    Decompose an arm by absorbing its nodes and segments into lower-energy neighbors
  */
  bool Arm::Decompose(int energy) {

	int termnode = mTerminalNodes.size();
	while (termnode--) {
	  if (mTerminalNodes[termnode]->mNeighborSegments.size() == 1) {
		dbprintf(4, "Arm::Classify(%d): Found terminal node with a single neighbor.  Marking as ARM_BOUNDARY.\n", mArmID);
		mArmType = ARM_BOUNDARY;
		dbprintf(4, "Arm::Decompose(energy %d, arm %d): found ARM_BOUNDARY arm; will not decompose.\n", energy,  mArmID);
		return false;
	  }
	}


	int16_t burgtype = GetBurgersType();
    if (burgtype/10 != energy)
      return false; // not yet

    mDecomposing = true;

    // Find which terminal node to use in decomposing ourself.
    // We prefer the terminal node that decomposes us into the smallest number of arms.
    // This will be the one with the least number of neighbors.  If two terminal nodes have the same number of neighbors, use the one that has the lowest maximum energy level.
    // I believe this greedy algorithm results in the global minimum decomposition too.

    uint32_t numTermNodes = mTerminalNodes.size();
    vector<int> maxEnergies; // we'll analyze this later.
    vector<int> numneighbors;
    vector<int> extendedArmIDs;

    if (mArmType == ARM_LOOP) {
      dbprintf(4, "Arm::Decompose(energy %d, arm %d): Looped arm, will not decompose\n", energy,  mArmID);
      WriteTraceFiles("0-loop-no-decomposition");
      mDecomposing = false;
      return false;
    }

	if (energy == 1)
	  return false;

    vector<int> allNeighborArmIDs;
    dbprintf(5, "\n================================================================\n Arm::Decompose(energy %d, arm %d): Found arm : %s\n", energy, mArmID, Stringify(0, false).c_str());

    // First, mark cross arms for debug output
	for (uint32_t n = 0; n<mTerminalNodes.size(); n++) {
      mTerminalNodes[n]->IdentifyCrossArms();
    }

    WriteTraceFiles("1-before-detachment");

    // Actually perform detachment
    for (uint32_t n = 0; n<mTerminalNodes.size(); n++) {
      mTerminalNodes[n]->DetachCrossArms();
    }

    // Note that cross-arm detachment might cause this arm to become invalid.
    if (!mTerminalNodes.size()) {
      dbprintf(4, "Arm::Decompose(energy %d, arm %d): This arm was eliminated by DetachCrossArms, cannot decompose.\n", energy, mArmID);
      mDecomposing = false;
      return false;
    }

    WriteTraceFiles("2-before-decomposition");
    int sharedNodeNum = -1;
    for (uint32_t termnode = 0; termnode < numTermNodes; termnode++) {
      numneighbors.push_back(mTerminalNodes[termnode]->mNeighborArms.size());
      maxEnergies.push_back(0);
      int neighbor = 0;
      while (neighbor < numneighbors[termnode]) {
        Arm *neighborArm = mTerminalNodes[termnode]->mNeighborArms[neighbor];
        if (neighborArm != this &&
            neighborArm->GetBurgersType()/10 > maxEnergies[termnode]) {
          allNeighborArmIDs.push_back(neighborArm->mArmID);
          maxEnergies[termnode] = neighborArm->GetBurgersType()/10;
        }
        ++neighbor;
      }
      dbprintf(5, "Arm::Decompose(energy %d, arm %d): numneighbors[%d] = %d, maxEnergies[%d] = %d\n", energy, mArmID, termnode, numneighbors[termnode], termnode, maxEnergies[termnode]);
    }
    if (numneighbors[0] < numneighbors[1]) {
      dbprintf(5, "Arm::Decompose(energy %d, arm %d): Choosing terminal node 0 because it has fewer neighbors.\n", energy, mArmID);
      sharedNodeNum = 0;
    } else if  (numneighbors[1] < numneighbors[0]) {
      dbprintf(5, "Arm::Decompose(energy %d, arm %d): Choosing terminal node 1 because it has fewer neighbors.\n", energy, mArmID);
      sharedNodeNum = 1;
    } else {
      // have to look at max energy levels now.  :-(
      if (maxEnergies[0] < maxEnergies[1]) {
        dbprintf(5, "Arm::Decompose(energy %d, arm %d): Choosing terminal node 0 because it has lower max energy.\n", energy, mArmID);
        sharedNodeNum = 0;
      } else {
        dbprintf(5, "Arm::Decompose(energy %d, arm %d): Choosing terminal node 1 because it has lower max energy.\n", energy, mArmID);
        sharedNodeNum = 1;
      }
    }
    //dbprintf(5, "mTerminalNodes[%d] is %s\n",sharedNodeNum,  mTerminalNodes[sharedNodeNum]->Stringify(0).c_str());
    Node *sharedNode = mTerminalNodes[sharedNodeNum];

    /*
      Extend all neighbor arms from the shared node using our segments.
      Formerly terminal sharedNode will become an interior discretization node
	  in each currently shared neighbor arm, and this arm will vaporize.
	  Our segments will get reused in the last neighbor arm extended.
    */
    // copy our neighborArms vector before it changes, not putting ourselves into it...
    vector <Arm*> neighborArms;
	uint32_t i = sharedNode->mNeighborArms.size();
	while (i-- ) {
	  if (sharedNode->mNeighborArms[i] != this) {
		neighborArms.push_back(sharedNode->mNeighborArms[i]);
	  }
	}


    // uniquify list, to prevent arms from being extended twice during decomposition, leading to invalid states.  Loops will be properly extended at both ends, don't worry.
    sort(neighborArms.begin(), neighborArms.end());
    neighborArms.erase(unique(neighborArms.begin(), neighborArms.end()), neighborArms.end());
    int neighbornum = neighborArms.size();
    uint32_t decomposed = 0;
    while (neighbornum--) {
      Arm *neighborArm = neighborArms[neighbornum];
	  bool lastNeighbor  = (neighbornum == 0); //
	  dbprintf(5, "Arm::Decompose(energy %d, arm %d): Adding %d segments from self to arm %d.\n", energy, mArmID, mSegments.size(), neighborArm->mArmID, Stringify(0).c_str());
	  neighborArm->ExtendByArm(this, sharedNode, lastNeighbor);
	  if (!lastNeighbor) {
		mDecomposedLength +=  mArmLength;
	  }
	  extendedArmIDs.push_back(neighborArm->mArmID);
	  neighborArm->mExtendOrDetach = true;
	  WriteTraceFiles(str(boost::format("%d-after-decompose-%d_ID-%d")%(2+decomposed)%decomposed%neighborArm->mArmID));
	  neighborArm->mExtendOrDetach = false;
	  ++decomposed;
	}

    mNumDecomposed[energy]++;
	mNodes.clear();
	mSegments.clear();
    dbprintf(4, "Arm::Decompose(energy %d, arm %d): Arm decomposition complete. After arm decomposition, the arm looks like this: %s", energy, mArmID, Stringify(0,false).c_str());
    mDecomposing = false;
    return true;
  }

  //===========================================================================
  std::string Arm::Stringify(int indent, bool shortform, bool allnodes) const {
    string seen = "false";
    if (mSeen) seen = "true";
    string seenMeta = "false";
    if (mSeenInMeta) seenMeta = "true";
    int btype = GetBurgersType();
    int atype = mArmType;
    string btypestring = BurgTypeToName(GetBurgersType());
    string armtypestring = ArmTypeToName(mArmType);
    string parentMetaArmIDString = "(NONE)";
    if (mParentMetaArm) parentMetaArmIDString = str(boost::format("%1%")%(mParentMetaArm->GetMetaArmID()));
    std::string s  = INDENT(indent) +
      str(boost::format("(arm): number %1%, mSeen = %2%, mSeenInMeta = %3%, numNormalSegments = %4%, numWrappedSegments = %5%, mSegments.size = %6%, mNodes.size = %7%, mTermNodes.size = %8%, length = %9%, Burgers = %10% (%11%), Type = %12% (%13%), ExternalArms = %14%, AncestorArms = %15%, mParentMetaArm ID = %16%")
          % mArmID % seen % seenMeta % mNumNormalSegments  % (int)mNumWrappedSegments % mSegments.size() % mNodes.size() % mTerminalNodes.size()  % GetLength() % btype % btypestring % atype % armtypestring  % StringifyExternalArms(0) % arrayToString(mAncestorArms) % parentMetaArmIDString);

#if LINKED_LOOPS
    if (mPartOfLinkedLoop) {
      s += ", is part of linked loop.";
    } else {
      s += ", is NOT part of linked loop.";
    }
#else
    s += "\n";
#endif

    if (!shortform) {

      int num = 0, max = mTerminalNodes.size();
      while (num < max) {
        s+= INDENT(indent+1) + "Terminal Node " + intToString(num) + string(": ");
        if (mTerminalNodes[num]) {
          s += mTerminalNodes[num]->Stringify(0) + string("\n");
        } else {
          s += "(NULL)\n"; //deleted because it was useless.
        }
        ++num;
      }
      max = mTerminalSegments.size(); num = 0;
      while (num < max) {
        s+= INDENT(indent+1) + "Terminal Segment " + intToString(num) + string(": ");
        if (mTerminalSegments[num]) {
          s += mTerminalSegments[num]->Stringify(indent + 1) + string("\n");
        } else {
          s+= "(NULL)\n";
        }
        ++num;
      }
    }
    if (allnodes) {
      /*vector<Node*> nodes;
	  vector<ArmSegment*> segs;
	  GetNodesAndSegments(NULL, &nodes, &segs);*/
      s += str(boost::format("\n***********************\nALL NODES (%1%) FOR ARM %2%: \n***********************\n") %(mNodes.size())%(mArmID));
      uint32_t nodenum = 0;
      for (deque<Node*>::const_iterator pos = mNodes.begin(); pos != mNodes.end(); pos++, nodenum++) {
        if (!*pos) {
          s += "(WRAPPED HERE)\n";
        } else {
          s += str(boost::format("Node %1%: %2%\n")%nodenum%((*pos)->Stringify(0)));
        }
      }
      s += str(boost::format("\n***********************\nALL SEGMENTS (%1%) FOR ARM %2%: \n***********************\n") %(mSegments.size())%(mArmID));
      uint32_t segnum = 0;
      for (deque<ArmSegment*>::const_iterator pos = mSegments.begin(); pos != mSegments.end(); pos++, segnum++) {
        s += str(boost::format("Segment %1%: %2%\n")%segnum%((*pos)->Stringify(0)));
      }
    }
    return s;
  }

  //===========================================================================
  string MetaArm::Stringify(int indent) {
    int atype = mMetaArmType;
    string s = INDENT(indent) + str(boost::format("(MetaArm): mLength: %1%, mMetaArmType: %2% (%3%), mMetaArmID: %4%\n\n")%mLength % atype % MetaArmTypeToName(mMetaArmType) % mMetaArmID);
    uint32_t i = 0;
    while (i<mTerminalNodes.size()) {
      string s2 = mTerminalNodes[i]->Stringify(indent+1);
      s += INDENT(indent+1);
      s+=  str(boost::format("MetaArm %1% Terminal Node %2%: %3%\n") % mMetaArmID % i % s2);
      ++i;
    }
    s += "\n";
    i = 0;
    while (i<mTerminalArms.size()) {
      s += INDENT(indent+1) + str(boost::format("MetaArm %1% Terminal Arm %2%: %3%")  % mMetaArmID % i % mTerminalArms[i]->Stringify(indent + 1));
      ++i;
    }
    i = 0;
    while (i<mAllArms.size()) {
      s += INDENT(indent+1) + str(boost::format("MetaArm %1% Arm %2%: %3%")  % mMetaArmID % i % mAllArms[i]->Stringify(indent + 1));
      ++i;
    }
    return s;
  }

  //===========================================================================
  // MetaArm::FindEndpoint()
  // recursive function, called by FindEndpoints())
  // Given a type 111 arm as seed, the node connecting the candidate to the previous arm in the chain, and a candidate, see if it can extend to an endpoint.  Return false when it fails, and true when it succeeds.
  //  previous == node connecting the candidate to the last examined arm in the calling chain.
  //  On first call, candidate != seed, but if we see the seed again, we have looped.  This is guaranteed because the seed is the last thing ever checked of any neighbor of an arm.
  //
  //===========================================================================
  bool MetaArm::FindEndpoint(Arm *seed, Node *previous, Arm* candidate) {
#ifndef  USE_ABORT
    bool err = false;
#endif

    dbprintf(4, "\n-------------------------------------------  \n");
    dbprintf(4, "FindEndpoint(metaarm %d): seed = %d, previous node = %d, candidate = %d\n", mMetaArmID, seed->mArmID, previous->mNodeIndex, candidate->mArmID);

    if (candidate->mArmType == ARM_EMPTY) {
      dbprintf(4, "FindEndpoint(metaarm %d): candidate arm is empty.  This happens when decomposing high energy arms.  Skipping.\n", mMetaArmID);
      return false;
    }

    if (candidate->mSeenInMeta) {
      dbprintf(4, "FindEndpoint(metaarm %d): candidate arm is already seen in this arm -- return false\n", mMetaArmID);
      return false;
    }

    if (candidate->mSeen) {
      dbprintf(4, "FindEndpoint(metaarm %d): candidate arm is type111 and already seen somewhere  -- return false\n", mMetaArmID);
      return false;
    }
    if ( candidate->GetBurgersType() != seed->GetBurgersType()) {
      dbprintf(4, "FindEndpoint(metaarm %d): candidate arm burgers (%s) does not match seed burgers (%s)-- return false\n", mMetaArmID, BurgTypeToName(candidate->GetBurgersType()).c_str(), BurgTypeToName(seed->GetBurgersType()).c_str());
      return false;
    }

    if (candidate->isTypeMM()) {
      dbprintf(4, "FindEndpoint(metaarm %d): Error: candidate arm is type MM -- This should never happen.\n", mMetaArmID);
      return false;
    }

    if (candidate->mArmType == ARM_LOOP) {
      dbprintf(4, "FindEndpoint(metaarm %d): Candidate is a loop. Do not explore.\n", mMetaArmID);
      return false;
    }

    candidate->mSeen = true;
    candidate->mSeenInMeta = true;

    if (candidate->isType111()) {
      dbprintf(4, "Candidate is type 111 and we shall try to recurse..\n", mMetaArmID);
      mFound111 = true;
    }
    else {
      dbprintf(4, "Candidate is type 200 and we shall try to recurse..\n", mMetaArmID);
    }
    uint32_t nodenum = candidate->mTerminalNodes.size();
	paradis_assert(candidate->mArmType != ARM_LOOP); // cannot be a loop

    Node * otherNode = NULL;
    while (nodenum --) {
      Node * node = candidate->mTerminalNodes[nodenum];
      if (node == previous) {
        continue;
      }
      otherNode = node;
      if (node->GetNodeType() < 0) {
        dbprintf(4, "FindEndpoint(metaarm %d): Candidate has M node on other end.  Terminating and returning true.\n", mMetaArmID);
        AddTerminalNode(node);
        AddTerminalArm(candidate);
        return true;
      }

      uint32_t neighbornum = node->mNeighborArms.size();
      dbprintf(4, "FindEndpoint(metaarm %d): Candidate has N node on other end. Recursing on %d neighbors.\n", mMetaArmID, neighbornum);
      Arm *foundseed = NULL;
      while (neighbornum--) {
        Arm *arm = node->mNeighborArms[neighbornum];
        if (arm == candidate) {
          continue; // no need to examine ourselves.
        }
        if (arm == seed) {
          dbprintf(4, "FindEndpoint(metaarm %d): Hey!  Neighbor %d is the seed arm.  Are we a loop? Defer.\n", mMetaArmID, neighbornum);
          foundseed = seed;
          continue;
        }
        dbprintf(4, "FindEndpoint(metaarm %d): Recursing on candidate neighbor arm...\n", mMetaArmID);
        if (FindEndpoint(seed, node, node->mNeighborArms[neighbornum])) {
          AddArm(candidate);
          return true;
        }
      }
      dbprintf(4, "FindEndpoint(metaarm %d): Done Checking candidate neighbor arms.\n", mMetaArmID);

      if (foundseed) {
        dbprintf(4, "FindEndpoint(metaarm %d): We found the seed after all other neighbors are exhausted.  Thus we are in a looped arm.\n", mMetaArmID);
        AddArm(candidate);
        AddTerminalNode(node);
        mMetaArmType = METAARM_LOOP_111;
        return true;
      }
    }
    dbprintf(4, "FindEndpoint(metaarm %d): Done recursing on neighbors.\n", mMetaArmID);

    if (candidate->isType111()) {
      dbprintf(4, "FindEndpoint(metaarm %d):  Candidate is a type111 arm with no way to extend beyond its type N node.  Terminate and mark as METAARM_UNKNOWN.\n", mMetaArmID);
      mMetaArmType = METAARM_UNKNOWN;
      AddTerminalNode(otherNode);
      AddTerminalArm(candidate);
      return true;
    }

    dbprintf(4, "FindEndpoint(metaarm %d):  Candidate is a dead end.\n", mMetaArmID);
    mDeadEnds.push_back(candidate); // so you can mark them as not seen later.
    return false;
  }


  //===========================================================================
  void MetaArm::FindEndpoints(Arm *seed) {

    dbprintf(4, "=======================================================\n", seed->Stringify(0).c_str());
    dbprintf(4, "FindEndpoints(metaarm %d) called with seed %s\n", mMetaArmID, seed->Stringify(0, false).c_str());

    mLength = 0;
    mMetaArmType = METAARM_UNKNOWN;

    // FIRST, CHECK FOR A SIMPLE LOOP SITUATION.
    seed->mSeen = true;
    // mLength = seed->GetLength();
    if (seed->mArmType == ARM_LOOP) {
      if (seed->isHighEnergy()) {
        dbprintf(4, "FindEndpoints(metaarm %d) (seed %d): Seed is HIGH_ENERGY LOOP arm, so this is METAARM_LOOP_HIGH_ENERGY.\n", mMetaArmID, seed->mArmID);
        mMetaArmType = METAARM_LOOP_HIGH_ENERGY;
      }
      else {
        dbprintf(4, "FindEndpoints(metaarm %d) (seed %d): Seed arm is Type 111 LOOP arm, so this is METAARM_LOOP_111.\n", mMetaArmID, seed->mArmID);
        mMetaArmType = METAARM_LOOP_111;
      }
      dbprintf(4, "FindEndpoints(metaarm %d) (seed %d): Seed arm is a loop. Adding as its own metaArm. Seed arm: %s\n", mMetaArmID, seed->mArmID, seed->Stringify(0).c_str());
      AddTerminalArm(seed);
      mTerminalNodes = seed->mTerminalNodes;
      CapturePath(false);
      return;
    }

    // Another special case:  the metaarm is composed of a single MM arm
    if (seed->isTypeMM()) {
      if (seed->isHighEnergy()) {
        dbprintf(4, "FindEndpoints(metaarm %d) (seed %d): Seed arm is Type 200 MM arm.  This is METAARM_UNKNOWN, since all non-loop type 200 arms should have been decomposed.\n", mMetaArmID, seed->mArmID);
        mMetaArmType = METAARM_UNKNOWN;
      }
      else if (seed->isType111()){
        dbprintf(4, "FindEndpoints(metaarm %d) (seed %d): Seed arm is Type 111 MM arm, so this is METAARM_111.\n", mMetaArmID, seed->mArmID);
        mMetaArmType = METAARM_111;
      }
      else {
        dbprintf(0, "FindEndpoints(metaarm %d) (seed %d): Seed arm is Type %d MM arm.  This is supposed to be impossible.  Aborting.\n", mMetaArmID, seed->mArmID, seed->mArmType);
        errexit;
      }

      AddTerminalArm(seed);
      mTerminalNodes = seed->mTerminalNodes;
      CapturePath(false);
      return;
    }


    if (seed->isTypeUnknown()) {
      dbprintf(4, "FindEndpoints(metaarm %d) (seed %d): ERROR: Seed arm is Type Unknown.  Giving up and trying new seed.\n", mMetaArmID, seed->mArmID);
      return;
    }

    // consider this to be a completely self contained metaarm.
    // Best algorithm:

    // A metaarm can continue in up to two directions, call them paths.
    //uint32_t pathsTaken = 0;

    // First, check each terminal node of the arm.  If it's a monster, add it as a meta arm terminal node and mark off a path.  If it's not, then recurse on it to extend the arm.
    dbprintf(4, "FindEndpoints(metaarm %d) (seed %d): Checking terminal nodes of seed arm.\n", mMetaArmID, seed->mArmID);
    uint32_t nodenum = 0; // seed->mTerminalNodes.size();
    bool seedIsTerminal = false;
    while (nodenum <  seed->mTerminalNodes.size() && mMetaArmType != METAARM_LOOP_111) {
      Node * node = seed->mTerminalNodes[nodenum];
      if (node->GetNodeType() < 0 || node->GetNodeType() == 1) {
		string nodetype("monster");
		if ( node->GetNodeType() == 1) {
		  nodetype = "single-neighbor";
		}
        dbprintf(4, "FindEndpoints(metaarm %d) (seed %d): Terminal node %d is a %s node. Add seed as terminal arm and node as terminal node.\n", mMetaArmID, seed->mArmID, nodenum, nodetype.c_str());
        // Monster node
        if (!mTerminalArms.size() || mTerminalArms[0] != seed) {
          AddTerminalArm(seed);
          seedIsTerminal = true;
        }
        AddTerminalNode(node);
      }
      else {
        dbprintf(4, "FindEndpoints(metaarm %d) (seed %d): Terminal node %d is not a monster node. Recurse on its neighbors.\n", mMetaArmID, seed->mArmID, nodenum);
        int neighborNum = node->mNeighborArms.size();
        while (neighborNum--) {
          Arm *arm = node->mNeighborArms[neighborNum];
          if (arm == seed) {
            dbprintf(4, "FindEndpoints(metaarm %d) (seed %d): Node neighbor %d is seed.  Ignore.\n", mMetaArmID, seed->mArmID, nodenum);
            continue;
          }
          dbprintf(4, "FindEndpoints(metaarm %d) (seed %d): call FindEndpoint on neighbor %d...\n", mMetaArmID, seed->mArmID, nodenum);
          if (FindEndpoint(seed, node,  arm)) {
            dbprintf(4, "FindEndpoints(metaarm %d) (seed %d): Node neighbor %d  resulted in valid endpoint.\n", mMetaArmID, seed->mArmID, nodenum);
            if (nodenum == seed->mTerminalNodes.size()-1) {
              if (!seedIsTerminal) {
                dbprintf(4, "FindEndpoints(metaarm %d) (seed %d): Adding seed as it is not a terminal arm and we are on the final terminal node.\n", mMetaArmID, seed->mArmID);
                AddArm(seed);
              }
              else {
                dbprintf(4, "FindEndpoints(metaarm %d) (seed %d): We are on the final terminal node but we are not adding the seed as it is already a terminal arm.\n", mMetaArmID, seed->mArmID);
              }
            }
            break; // only one path allowed per terminal node.
          }
        }
      }
      if (mMetaArmType == METAARM_LOOP_111) {
        AddTerminalArm(seed);
      }
      CapturePath(nodenum); // Arms are collected in reverse, so reverse that for last node
      ++nodenum;
    }
    if (mTerminalNodes.size() == 1 || mTerminalArms.size() == 1) {
      // we are a loop
      if (seed->isHighEnergy()) {
        dbprintf(4, "FindEndpoints(metaarm %d) (seed %d): Seed is Type 200 LOOP arm, so this is METAARM_LOOP_HIGH_ENERGY.\n", mMetaArmID, seed->mArmID);
        mMetaArmType = METAARM_LOOP_HIGH_ENERGY;
      }
      else {
        dbprintf(4, "FindEndpoints(metaarm %d) (seed %d): Seed arm is Type 111 LOOP arm, so this is METAARM_LOOP_111.\n", mMetaArmID, seed->mArmID);
        mMetaArmType = METAARM_LOOP_111;
      }
    }

    // we must be METAARM_111 if we are not a loop here
    if (mMetaArmType == METAARM_UNKNOWN) {
      mMetaArmType = METAARM_111;
    }


    int armnum = mAllArms.size();
    while (armnum--) {
      mAllArms[armnum]->mSeenInMeta = false;
    }
    armnum = mTerminalArms.size();
    while (armnum--) {
      mTerminalArms[armnum]->mSeenInMeta = false;
    }
    armnum = mDeadEnds.size();
    while (armnum--) {
      mDeadEnds[armnum]->mSeenInMeta = false;
    }
    int atype = mMetaArmType;
    dbprintf(4, "FindEndpoints(metaarm %d) (seed %d): MetaArm is type %d, and found111 is %d\n", mMetaArmID, seed->mArmID, atype, (int)mFound111);

    return;

  }


  //===========================================================================
  /*!
    Return a list of nodes in the metaArm, in order.  If the MetaArm
    contains any wrapped segments, then the interruption will be notated with
    mWrappedNode.
  */
  vector<Node * > MetaArm::GetNodes(void) {
    vector<Node*> nodes;
    if (mMetaArmType == METAARM_LOOP_111 || mMetaArmType == METAARM_LOOP_HIGH_ENERGY) {
      dbprintf(5, "MetaArm::GetNodes(MetaArm %d): Get all loop node locations for all member arms.\n", mMetaArmID);

      Node *lastNode = mTerminalNodes[0];
      for (vector<Arm*>::iterator armpos = mAllArms.begin(); armpos != mAllArms.end(); armpos++) {

        Arm *currentArm = *armpos;
        dbprintf(5, "MetaArm::GetNodes(MetaArm %d): Got %d nodes from new armID %d\n", mMetaArmID, currentArm->mNodes.size(), currentArm->mArmID);
        if (!currentArm->mNodes.size()) {
          dbprintf(5, "MetaArm::GetNodes(MetaArm %d): ERROR! Failed to get nodes from arm!\n", mMetaArmID);
          return nodes;
        }
        if (nodes.size() && *(nodes.end()-1) == currentArm->mNodes[0]) {
          dbprintf(5, "MetaArm::GetNodes(MetaArm %d): deleting duplicate end node %s.\n", mMetaArmID, (*(nodes.end()-1))->Stringify(0).c_str());
          nodes.pop_back();
        }
        nodes.insert(nodes.end(), currentArm->mNodes.begin(), currentArm->mNodes.end());
        lastNode = *(nodes.end()-1);
        dbprintf(5, "MetaArm::GetNodes(MetaArm %d): lastNode is %d\n", mMetaArmID, lastNode->mNodeIndex);
      }


      dbprintf(5, "MetaArm::GetNodes(MetaArm %d): returning %d nodes.\n", mMetaArmID, nodes.size());
      return nodes;

    }
    else {
      dbprintf(5, "MetaArm::GetNodes(MetaArm %d): returning 2 terminal nodes.\n", mMetaArmID, nodes.size());
      return mTerminalNodes;
    }
  }

  //===========================================================================
  /*!
    Return a list of locations for the nodes in the metaArm if it's a loop,
    otherwise just the endpoints.  The endpoints can be wrapped if desired.
    If the MetaArm
    contains any wrapped segments, then the interruption will be notated with
    mWrappedNode.
  */
  vector<rclib::Point<float> > MetaArm::GetNodeLocations(bool wrapEndpoints) {
    vector<rclib::Point<float> > points;
    if (mMetaArmType == METAARM_LOOP_111 || mMetaArmType == METAARM_LOOP_HIGH_ENERGY) {
      vector<Node *>nodes = GetNodes();
      uint32_t  p = 0;
      while (p < nodes.size()-1) {
        vector<float> f = nodes[p]->mLocation;
        points.push_back(f);
        dbprintf(5, "MetaArm::GetNodeLocations(), MetaArm %d: push back node (%f, %f, %f).\n", mMetaArmID, f[0], f[1], f[2]);
        mNumNodes ++;
        if (!nodes[p+1]) {
          // wrapped nodes always have the same NodeID.
          points.push_back(mWrappedNode);
          ++p;
          dbprintf(5, "MetaArm::GetNodeLocations(), MetaArm %d: push back wrapped node\n", mMetaArmID);
        } else {
          mNumSegments++;
        }
        ++p;
      }
      vector<float> f = nodes[p]->mLocation;
      points.push_back(f);
      dbprintf(5, "MetaArm::GetNodeLocations(), MetaArm %d: push back %dth node (%f, %f, %f).\n", mMetaArmID, ++p, f[0], f[1], f[2]);
      dbprintf(5, "MetaArm::GetNodeLocations(), MetaArm %d: mNumSegments is %d;\n", mMetaArmID, mNumSegments);

    } else {
      mNumSegments = 0;
      dbprintf(5, "MetaArm::GetNodeLocations(), MetaArm %d Not a loop -- we can simply return the two terminal nodes.\n", mMetaArmID);
      if (mTerminalNodes.size() != 2) {
        dbprintf(0, "MetaArm::GetNodeLocations(), MetaArm %d ERROR: We are in a non-loop meta arm that has %d terminal nodes.\n", mMetaArmID, mTerminalNodes.size());
        return points;
      }
      vector<float> loc1 = mTerminalNodes[0]->mLocation,
		loc2 = mTerminalNodes[1]->mLocation;

      dbprintf(5, "MetaArm::GetNodeLocations(),MetaArm %d:  non-loop: pushing back 2 nodes: (%f, %f, %f) and (%f, %f, %f).\n", mMetaArmID, loc1[0], loc1[1], loc1[2], loc2[0], loc2[1], loc2[2]);

      if (wrapEndpoints && DataSet::Wrap(loc1, loc2)) {
        dbprintf(5, "MetaArm::GetNodeLocations(), non-loop: wrapped nodes to new location: (%f, %f, %f) -> (%f, %f, %f).\n", loc1[0], loc1[1], loc1[2], loc2[0], loc2[1], loc2[2]);
      }
	  points.push_back(loc1);
	  points.push_back(loc2);
	  mNumSegments = 1;
    }
    dbprintf(5, "MetaArm::GetNodeLocations(), MetaArm %d:  COMPLETE returning %d points\n", mMetaArmID, points.size());
    return points;
  }

  //===========================================================================
  void DataSet::ReadBounds(void) {
    dbprintf(2,"Beginning ReadBounds\n");

    try {
      ifstream dumpfile(mDataFilename.c_str());
      if (!dumpfile.good()){
        throw string("Cannot open dumpfile ") + mDataFilename;
      }
      else {
        dbprintf(2, "Parsing file %s\n", mDataFilename.c_str());
      }
      mFileVersion = 42;
      dbprintf(2, "Looking for file version... ");
      int lineno = 0;
      //  we need to check file version -- if not found, then
      char linebuf[2048]="";
      while (dumpfile.good()) {
        dbprintf(5, "scanning new line \"%s\"\n", linebuf);
        if (strstr(linebuf, "File version number")) {
          // bingo
          dumpfile.getline(linebuf, 2048); // get comment
          dumpfile.getline(linebuf, 2048); // blank line
          dumpfile >> mFileVersion;
          break;
        }
        else if (strstr(linebuf, "dataFileVersion =")) {
          // for versions 4 and newer
          sscanf(linebuf, "dataFileVersion = %lu", &mFileVersion);
          break;
        }
        else if (strstr(linebuf, "Primary lines")) {
          // no version available
          mFileVersion = 0; // just to be clear
          break;
        }
        dumpfile.getline(linebuf, 2048);
        lineno++;
      }
      dumpfile.seekg(0, std::ios::beg);// seek to beginning again
      dbprintf(2, "file version set to %u\n", mFileVersion);

      if (mFileVersion == 42) {
        throw string("Cannot find file version -- not a paraDIS file?");
      }
      int i=0;
      if (mFileVersion == 0) {
        string token;
        while (dumpfile.good() && token != "minSideX")
          dumpfile >> token;
        while (dumpfile.good() && i<3) {
          dumpfile >> token >> mDataMin[i] >> token  >> token  >> mDataMax[i] >>token;
          i++;
        }
      }
      else  if (mFileVersion < 4) {
        char linebuf[2048]="";
        while (dumpfile.good() && !strstr(linebuf, "Minimum coordinate values"))
          dumpfile.getline(linebuf, 2048);
        if (!dumpfile.good()){
          throw string("Cannot find minimum bounds from dumpfile ") + mDataFilename;
        }
        dumpfile.getline(linebuf, 2048);//get that stupid "#"
        dumpfile >> mDataMin[0] >> mDataMin[1] >> mDataMin[2];

        while (dumpfile.good() && !strstr(linebuf, "Maximum coordinate values"))
          dumpfile.getline(linebuf, 2048);
        if (!dumpfile.good()){
          throw string( "Cannot find maximum bounds from dumpfile ")+ mDataFilename;
        }
        dumpfile.getline(linebuf, 2048);//get that stupid "#"
        dumpfile >> mDataMax[0] >> mDataMax[1] >> mDataMax[2];
      }
      else {  // must be file version 4
        char linebuf[2048]="";
        while (dumpfile.good() && !strstr(linebuf, "minCoordinates")){
          dumpfile.getline(linebuf, 2048);
        }
        dumpfile >> mDataMin[0] >> mDataMin[1] >> mDataMin[2];

        while (dumpfile.good() && !strstr(linebuf, "maxCoordinates")){
          dumpfile.getline(linebuf, 2048);
        }
        dumpfile >> mDataMax[0] >> mDataMax[1] >> mDataMax[2];
      }
      mDataSize  = mDataMax - mDataMin;
    } catch (string err) {
      throw string("Error in GetBounds:\n")+err;
    }

    dbprintf(2,"Done with ReadBounds\n");
    return;
  }

  //======================================================================
  /* loc0 stays unchanged; loc1 is modified as needed */
  bool DataSet::Wrap(vector<float> &loc0, vector<float> &loc1) {
    float shift_amt[3] = {0};
    bool wrap = false;
    int i=3; while (i--) {
      float dist = fabs(loc0[i] - loc1[i]);
      if (mDataSize[i]/2.0 < dist) {
        wrap = true;
        shift_amt[i] = mDataSize[i];
      }
    }
    if (wrap) {
      i = 3; while (i--) {
        if (loc0[i] < loc1[i]) {
          /*!
            Wrap loc1 "backward" in space
          */
          loc1[i] -= shift_amt[i];

        } else {
          /*!
            Wrap loc1 "forward" in space
          */
           loc1[i] += shift_amt[i];

        }

      }
    }
    return wrap;
  }

  //======================================================================
  void DataSet::CopyNodeFromFile(uint32_t &lineno, std::ifstream &datafile, std::ofstream &tagfile) {
    // read the first line of the next node in the file:
    char comma;
    int domainID, nodeID;
    long old_id_junk, tag, numNeighbors;
    double x,y,z;
    if (mFileVersion > 0) {
      datafile >> domainID >> comma >> nodeID;
    }
    else { // old-style
      datafile >> nodeID >> old_id_junk;
    }
    if (!datafile.good()) {
      throw str(boost::format("DataSet::CopyNodeFromFile line %1%: error reading node")% __LINE__);
    }
    //dbprintf(5, "Got node ID (%d, %d) at line %d\n", domainID, nodeID, lineno);
    datafile >> x >> y >> z >> numNeighbors >> tag;
    if (mFileVersion == 0){
      datafile  >> domainID >>  old_id_junk;
    }
    if (!datafile.good()) {
      throw str(boost::format("DataSet::CopyNodeFromFile line %1%: error reading node")% __LINE__);
    }
    //dbprintf(5, "Got loc (%f, %f, %f), neighbors %d, and tag %d at line %d\n", x,y,z,numNeighbors, tag, lineno);
    ++lineno;

    Node *matchNode = Node::mNodes[Node::Hash(domainID, nodeID)][0];
    if (!matchNode) {
      throw str(boost::format("DataSet::CopyNodeFromFile line %1%: cannot match NodeID (%2%,%3%) in Nodes hash")% __LINE__ % domainID % nodeID);
    }

    // match the ID to our fullNode
    int nodetag = 0;
    if (matchNode->IsLoopNode()) nodetag += 8192;
    // if (fullnode->IsTypeM()) nodetag += 256;
    // if (fullnode->IsTypeN()) nodetag += 512;
    if (tag != nodetag) {
      dbprintf(5, "DataSet::CopyNodeFromFile: Node %d: Old tag %d --> New tag %d\n", matchNode->mNodeIndex, tag, nodetag);
      tag = nodetag;
    }

    string linebuf;
    // Write back out the first line of node information
    if (mFileVersion > 0) {
      linebuf = str(boost::format("%1%,%2% ")%domainID%nodeID);
    }
    else { // old-style
      linebuf = str(boost::format("%1%,%2% ")%nodeID%old_id_junk);
    }
    linebuf +=  str(boost::format("%.8f %.8f %8f %d %d")%x%y%z%numNeighbors % tag);
    if (mFileVersion == 0){
      linebuf +=  str(boost::format("%1% %2%")% domainID % old_id_junk);
    }
    tagfile << linebuf << endl;

    getline(datafile, linebuf); // clear the '\n' waiting in datafile.
    int neighbornum = 0;
    while (neighbornum < numNeighbors && datafile.good()) {
      // read and write two lines per neighbor
      getline(datafile, linebuf);
      tagfile << linebuf << endl;
      getline(datafile, linebuf);
      tagfile << linebuf << endl;
      if (!datafile.good()) {
        throw str(boost::format("DataSet::CopyNodeFromFile line %1%: error reading node index %1%, neighbor %2%")% __LINE__% matchNode->mNodeIndex% neighbornum);
      }
      ++neighbornum;
      lineno += 2;
    }
    return;
  }


  //===========================================================================
  bool DataSet::Mkdir(const char *dirname) {
    dbprintf(3, "Mkdir(%s)",dirname);
#ifndef WIN32
    DIR *dir = opendir(dirname);
    if (!dir)
      {
        if (mkdir(dirname, S_IRWXU | S_IRWXG | S_IRWXO )) {
          string errmsg = string("DataSet::Mkdir() Warning:   could not create directory: ") + mOutputDir+ "." ;
          cerr << errmsg << endl;
          dbprintf(1, errmsg.c_str());
          return false;
        } else {
          dbprintf(1, "Created directory %s\n", dirname);
        }
      }else {
      closedir(dir);
    }
#else
    if (_mkdir(dirname) != 0)
      {
        string errmsg = string("DataSet::Mkdir() Warning:  could not create directory: ") + dirname+ "." ;
        cerr << errmsg << endl;
        dbprintf(1, errmsg.c_str());
        return false;
      }
#endif
    return true;
  }


  //===========================================================================
  /* NEW!  Just read the node and use the new map to connect to neighbors.
	 What I should have done in the first place.
	 Wrapping can be done right here.
  */
  void DataSet::ReadNodeFromFile(uint32_t &lineno, std::ifstream &datafile){
    char comma;
    long old_id_junk, constraint_junk, numNeighbors;
    float float_junk;
	vector<float> location(3,0);
    int domainID, nodeID, neighborDomain, neighborID;
    dbprintf(5, "ReadNodeFromFile\n");

    try {
	  //-----------------------------------------------
	  // read the first line of node information and see if it matches theNode
	  if (mFileVersion > 0) {
		datafile >> domainID >> comma >> nodeID;
		if (!datafile.good()) {
		  throw string("DataSet::ReadNodeFromFile: error reading domainID and nodeID of node ");
		}
		dbprintf(5, "DataSet::ReadNodeFromFile: Got node id (%d,%d)\n", domainID, nodeID);
	  } else {
		datafile >> nodeID >> old_id_junk;
		if (!datafile.good()) {
		  throw string("DataSet::ReadNodeFromFile: error reading nodeID ");
		}
	  }
	  /*!
		read location
	  */
	  int i=0; while (i < 3) datafile >> location[i++];
	  if (!datafile.good()) {
		throw string("DataSet::ReadNodeFromFile: error reading location ");
	  }
	  dbprintf(5, "DataSet::ReadNodeFromFile: read location (%f, %f, %f)\n", location[0], location[1], location[2]);
	  datafile >> numNeighbors >> constraint_junk;
	  if (!datafile.good()) {
		throw string("DataSet::ReadNodeFromFile: error reading numNeighbors ");
	  }
	  if (mFileVersion == 0){
		datafile  >> domainID >>  old_id_junk;
		if (!datafile.good()) {
		  throw string("DataSet::ReadNodeFromFile: error reading domainID");
		}
	  }
	  if (!datafile.good()) {
		throw string("DataSet::ReadNodeFromFile: error reading node");
	  }

	  Node *node = Node::GetOrCreateNode(domainID, nodeID, location);

	  dbprintf(5, "DataSet::ReadNodeFromFile: parsing node (%d, %d) from line %d\n", domainID, nodeID, lineno);
  	  lineno++;

      //------------------------------------------
      // read neighbor information
      int neighbornum = 0;
      vector<float> burgers(3,0);
      try {
        while (neighbornum < numNeighbors && datafile.good()) {
          if (mFileVersion > 0) {
            datafile >> neighborDomain >> comma >> neighborID;
            if (!datafile.good()) {
              throw string("DataSet::ReadNodeFromFile: error reading neighborDomain and neighborID of neighbor");
            }
          }
          else {
            neighborDomain   = 0;
            datafile >> neighborID;
            if (!datafile.good()) {
              throw string("DataSet::ReadNodeFromFile: error reading neighborID");
            }
          }
 		  lineno++;

		  // If the neighbor exists already, then
          // read burgers value and discard "nx, ny, nz".
          datafile >> burgers[0] >> burgers[1] >> burgers[2] >> float_junk >> float_junk >> float_junk;
          if (!datafile.good()) {
            throw string("DataSet::ReadNodeFromFile: error reading burgers values");
          }

		  Node *neighbor = Node::GetOrCreateNode(neighborDomain, neighborID);
		  if (!neighbor->mLocation.size() ) {
			/* If the location has been set, then all the segments that point to the
			   neighbor have already been created, including the one that points to the
			   current node.  If not, then we need to create a new arm segment  here.
			*/
			ArmSegment *seg = new ArmSegment(node, neighbor, BurgVecToBurgType(burgers));
			ArmSegment::mArmSegments[seg->mSegmentIndex] = seg;
			dbprintf(6, "DataSet::ReadNodeFromFile created segment %s\n", seg->Stringify().c_str());
		  }
          ++neighbornum;
		  lineno++;
        }; // done reading neighbor information
		dbprintf(5, "DataSet::ReadNodeFromFile: Done creating full node %s\n", node->Stringify(false).c_str());
      } catch (string err) {
        throw str(boost::format("DataSet::ReadNodeFromFile: Error in DataSet::ReadNode reading neighbor %d: %s") % neighbornum % err);
      }
    } catch (string err) {
	  throw str(boost::format("DataSet::ReadNodeFromFile: Error trying to read full node info on line %d: %s")%lineno% err);
    }

    return;

  } /* end ReadNodeFromFile */
  //===========================================================================
  void DataSet::CreateNodesAndArmSegments(void){
    dbprintf(2, "CreateNodesAndArmSegments started...\n");
    timer theTimer;
    theTimer.start();
    char linebuf[2048]="";
    uint32_t nodenum = 0;
    uint32_t lineno = 1;
    try {
      ifstream datafile(mDataFilename.c_str());
      if (!datafile.good()){
        throw string("Error: cannot open datafile ")+ mDataFilename;
      }

       if (mFileVersion == 0) {
        while (datafile.good() && !strstr(linebuf, "Number of nodes")) {
          datafile.getline(linebuf, 2048);
          ++lineno;
        }
        if (!mTotalDumpNodes && datafile.good())
          datafile >> mTotalDumpNodes; // did not read the whole line...
      }
      else if (mFileVersion < 4 ) {
        while (datafile.good() && !strstr(linebuf, "Node count")) {
          datafile.getline(linebuf, 2048);
        }
        datafile.getline(linebuf, 2048);//should be "#"
        ++lineno;
        if (!strstr(linebuf, "#")) {
          dbprintf(0, "ERROR: Expected line:\n#\n but got:\n%s\n", linebuf);
        }
        datafile >> mTotalDumpNodes;
        if (!datafile) {
          throw string("Error getting number of nodes");
        }
      }
      else { //
        while (datafile.good() && !strstr(linebuf, "nodeCount =")) {
          datafile.getline(linebuf, 2048);
          ++lineno;
        }
        // dataFileVersion 4  just has different header, just parse it and treat it like the old "new" version
        long nodeCount = 0;
        sscanf(linebuf, "nodeCount = %lu", &nodeCount);
        mTotalDumpNodes = nodeCount;
      }

      if (!mTotalDumpNodes) {
        throw string( "Cannot find number of nodes in datafile");
      }
      // got number of nodes
      // =================================================

      while (datafile.good() && !strstr(linebuf, "Secondary lines:")) {
        datafile.getline(linebuf, 2048);
         ++lineno;
      }

      if (mFileVersion > 0 && mFileVersion < 4) {
        datafile.getline(linebuf, 2048);//should be "#"
        if (!strstr(linebuf, "#")) {
          dbprintf(0, "WARNING: Expected line:\n#\n but got:\n%s\n", linebuf);
          throw string("Bad file format in line: ")+string(linebuf);
        }
      }

      if (!datafile.good()) {
        throw string( "Error: cannot find first node in data file");
      }
      STARTPROGRESS();
      //dbprintf(2, "\n");
      for ( nodenum = 0; datafile.good() && nodenum< mTotalDumpNodes; nodenum++ ) {
        UPDATEPROGRESS(nodenum, mTotalDumpNodes,  "CreateNodesAndArmSegments: ");
        ReadNodeFromFile(lineno, datafile);
	  }
      COMPLETEPROGRESS(mTotalDumpNodes,  "CreateNodesAndArmSegments: ");
      if (nodenum !=  mTotalDumpNodes) throw string("terminating before nodenum == mTotalDumpNodes\n");
    } catch (string err) {
      throw string("Error in DataSet::CreateNodesAndArmSegments while reading node ") + intToString(nodenum) +":\n" + err;
    }
    if (ArmSegment::mArmSegments.size()) {
      ArmSegment *seg = ArmSegment::mArmSegments[0];
      if (seg->mBurgersType >= 1000) {
        mDataType = PARADIS_DATATYPE_HCP;
      } else {
        mDataType = PARADIS_DATATYPE_BCC;
      }
    }
    dbprintf(2, "CreateNodesAndArmSegments ended...\n");
    return;
  }


  //===========================================================================
    /*!
      Fix segments that have large distances as an artifact of the periodicity of the data. */
  void DataSet::WrapBoundarySegments(void){
	vector<ArmSegment*> wrappedSegments;  // collect them here to avoid modifying the global map while iterating
	for (std::map<uint32_t, ArmSegment *>::iterator pos = ArmSegment::mArmSegments.begin(); pos != ArmSegment::mArmSegments.end(); pos++) {
	  ArmSegment *seg = pos->second;
	  vector<float> loc0 = seg->mEndpoints[0]->mLocation,
		loc1 = seg->mEndpoints[1]->mLocation;
	  if (Wrap(loc0, loc1)) {
		// Wrap shifts loc1 to a wrapped location, so we are wrapping mEndpoints[1]
		Node *wrappedNode = Node::DuplicateNode(seg->mEndpoints[1]);
		wrappedNode->mWrapped = true;
		wrappedNode->mLocation = loc1; // changed by Wrap() call
		ArmSegment *wrappedSegment = new ArmSegment(wrappedNode, seg->mEndpoints[1], seg->mBurgersType);
		wrappedSegments.push_back(wrappedSegment);

		seg->mEndpoints[1]->RemoveNeighborSegment(seg);
		seg->mEndpoints[1] = wrappedNode;
		wrappedNode->mNeighborSegments.push_back(seg);
	  }
	}

	for (vector<ArmSegment*>::iterator seg = wrappedSegments.begin(); seg != wrappedSegments.end(); seg++) {
	  ArmSegment::mArmSegments[(*seg)->mSegmentIndex] = *seg;
	  (*seg)->mWrapped = true;
	}
	return;
  }


  //===========================================================================
  // see also DebugPrintArms() and PrintArmFile() -- this just tabulates a summary
  string DataSet::ArmSummary(void) {
    //if (!dbg_isverbose()) return;
    //dbprintf(3, "Beginning PrintArmStats()");
    string summary;
    map<int, double>  armLengths;
    map<int, double>  armEPDistances;
    double totalArmLength=0;


    map<int, uint32_t> numArmsByType;  // number of arms of each type
    uint32_t totalArms=0;
#if LINKED_LOOPS
    double linkedLoopLength = 0;
    uint32_t numLinkedLoops = 0;
#endif

	map<int, double> shortLengths, longLengths, armLengthBins;
 	map<int, uint32_t> numShortArms, numLongArms, armBins;

    vector<Arm *>::iterator armpos = Arm::mArms.begin(), armend = Arm::mArms.end();
    while (armpos != armend) {
      double length = (*armpos)->GetLength();
      int armType = (*armpos)->mArmType;
      if (armType == ARM_EMPTY) {
        ++armpos;
        continue;
      }
      if (mThreshold >= 0) {
        int16_t btype = (*armpos)->GetBurgersType();
        if (!btype) {
          printf("Error:  armpos has no terminal segments!\n");
        }
        if (armType == ARM_BCC_SHORT_NN_111) {
          numShortArms[btype-1]++;
          shortLengths[btype-1] += length;
        }
        else {
          numLongArms[btype-1]++;
          longLengths[btype-1] += length;
        }
      }
      if (mNumBins) {
        int binNum = (int)((double)length/Arm::mLongestLength * mNumBins);
        if (binNum == mNumBins) binNum = mNumBins-1;
        if (binNum > mNumBins || binNum < 0) {
          printf("Error:  binNum %d is invalid (num bins is %d)\n", binNum, mNumBins);
        } else {
          armLengthBins[binNum] += length;
          armBins[binNum]++;
        }
      }
      armLengths[armType] += length;

      int numtermnodes = (*armpos)->mTerminalNodes.size();
      if (numtermnodes == 2) {
        armEPDistances[armType] +=  (*armpos)->mTerminalNodes[0]->Distance(*( (*armpos)->mTerminalNodes[1]), true);
      } else {
        armEPDistances[armType] += 0.0;
      }

      totalArmLength += length;
      numArmsByType[armType]++;

      totalArms++;
#if LINKED_LOOPS
      if ((*armpos)->mPartOfLinkedLoop) {
        numLinkedLoops ++;
        linkedLoopLength += length;
      }
#endif
      ++armpos;
    }

    summary += "\n===========================================\n\n";
    summary += str(boost::format("%50s%12d\n")%"total number of non-empty arms:" % totalArms);
    summary += str(boost::format("%50s%12.2f\n")%"total length of all arms before decomposition:" % Arm::mTotalArmLengthBeforeDecomposition);
    summary += str(boost::format("%50s%12.2f\n")%"total length of all arms after decomposition:" % Arm::mTotalArmLengthAfterDecomposition);

    double delta = Arm::mTotalArmLengthAfterDecomposition - (Arm::mDecomposedLength + Arm::mTotalArmLengthBeforeDecomposition);

    summary += str(boost::format("%50s%12.2f\n")%"Total decomposed length (computed independently):" % Arm::mDecomposedLength);

    double ratio = delta / Arm::mTotalArmLengthAfterDecomposition;

    if (Arm::mTotalArmLengthAfterDecomposition && fabs(ratio) > 0.00001 ) {
      string errmsg = str(boost::format("\n\nError: mDecomposedLength %1% + mTotalArmLengthBeforeDecomposition %2% != mTotalArmLengthAfterDecomposition %3% (ratio is %4%)!\n\n\n")%Arm::mDecomposedLength%Arm::mTotalArmLengthBeforeDecomposition%Arm::mTotalArmLengthAfterDecomposition%ratio);
      cerr  << errmsg;
      summary +=  errmsg;
    }

    summary += "\n=================================================================\n\n";

    summary += str(boost::format("%50s%12d\n")%"Total number of segments before decomposition:" % ArmSegment::mNumBeforeDecomposition);
    summary += str(boost::format("%50s%12d\n")%"Total number of segments decomposed:" % ArmSegment::mNumDecomposed);
    summary += str(boost::format("%50s%12d\n")%"Total number of segments after decomposition:" % (ArmSegment::mArmSegments.size()));
    summary += "=================================================================\n";
    totalArmLength = 0;
    vector<int> energyLevels;
    string armtypes, armnums, totallengths, avglengths, totalepdists, avglepdists;
    string ByTypeSummaries;
    int numtypes = 0;
    summary += "---------------------------------------------------------------\n" ;
    //  for (vector<BurgerTypeInfo>::iterator pos = BurgInfos.begin(); pos != BurgInfos.end(); pos++) {
    vector<int> types = GetAllArmTypes();
    for (vector<int>::iterator pos =  types.begin(); pos != types.end(); pos++) {
      int armtype = *pos;
      armtypes += str(boost::format("%=12d")%armtype);
      armnums += str(boost::format("%=12d")%numArmsByType[armtype]);
      totallengths += str(boost::format("%=12d")%armLengths[armtype]);
      totalepdists += str(boost::format("%=12d")%armEPDistances[armtype]);
      avglengths += str(boost::format("%=12d")%(numArmsByType[armtype] ? armLengths[armtype]/(numArmsByType[armtype]) : 0));
      avglepdists += str(boost::format("%=12d")%(numArmsByType[armtype] ? armEPDistances[armtype]/(numArmsByType[armtype]) : 0));
      totalArmLength += armLengths[armtype];

      if (++numtypes == 10) {
        summary += str(boost::format("%=18s%s\n")%"Arm Types:"        % armtypes);
        summary += str(boost::format("%=18s%s\n")%"Total Lengths:"    % totallengths);
        summary += str(boost::format("%=18s%s\n")%"Average Lengths:"  % avglengths);
        summary += str(boost::format("%=18s%s\n")%"Total EP Dists:"   % totalepdists);
        summary += str(boost::format("%=18s%s\n")%"Average EP Dists:" % avglepdists);
        summary += "---------------------------------------------------------------\n" ;
        armtypes = armnums = totallengths = avglengths = totalepdists = avglepdists = "";
        numtypes = 0;
      }
      ByTypeSummaries += str(boost::format("%60s = %d\n")%(str(boost::format("Number of type %d (%s) arms") % armtype % ArmTypeToName(armtype))) % numArmsByType[armtype]);
      ByTypeSummaries += str(boost::format("%60s = %.2f (%.2f average)\n")%(str(boost::format("Total length of type %d (%s) arms") % armtype % ArmTypeToName(armtype))) % armLengths[armtype] % (numArmsByType[armtype] ? armLengths[armtype]/numArmsByType[armtype] : 0));
      ByTypeSummaries += str(boost::format("%60s = %.2f (%.2f average)\n")%(str(boost::format("Total EP Distance of type %d (%s) arms") % armtype % ArmTypeToName(armtype))) % armEPDistances[armtype] % (numArmsByType[armtype] ? armEPDistances[armtype]/numArmsByType[armtype] : 0));
      ByTypeSummaries += "-------------------------------------------------------\n";
    }
    if (numtypes) {
      summary += str(boost::format("%=18s%s\n")%"Arm Types:"        % armtypes);
      summary += str(boost::format("%=18s%s\n")%"Total Lengths:"    % totallengths);
      summary += str(boost::format("%=18s%s\n")%"Average Lengths:"  % avglengths);
      summary += str(boost::format("%=18s%s\n")%"Total EP Dists:"   % totalepdists);
      summary += str(boost::format("%=18s%s\n")%"Average EP Dists:" % avglepdists);
      summary += "---------------------------------------------------------------\n" ;
    }
    summary += str(boost::format("%40s = %.2f\n")%"Total length of all arm types" % totalArmLength);
    summary += "\n=================================================================\n\n";

    summary += ByTypeSummaries;

#if LINKED_LOOPS
    summary += str(boost::format("LINKED LOOPS: total number of arms = %d\n") % numLinkedLoops);
    summary += str(boost::format("LINKED LOOPS: total length of arms = %.2f\n") % linkedLoopLength);
    summary += "---------------------------------------------------------\n";
#endif

    // write a row of arm lengths to make analysis via spreadsheet easier
    summary += "Key: \nUNKNOWN\tUNINTRSTNG\tLOOP\tMM_111\tMN_111\tNN_111\tMM_200\tMN_200\tNN_200\tSHORT_NN_111\tSHORT_NN_200\n";
    int n = 0;
    while (n<11) {
      summary += str(boost::format("%.2f\t") %  armLengths[n]);
      ++n;
    }



    if (mThreshold >= 0.0) {
      summary += "\n\n--------------------------------------------------\n";
      summary += str(boost::format("THRESHOLD data.  Threshold = %.2f\n") % mThreshold);
	  for (vector<BurgerTypeInfo>::iterator burg = BurgInfos.begin(); burg != BurgInfos.end(); burg++) {
		summary += "--------------------------------------------------\n";
		summary += str(boost::format("Total number of %s arms: %d\n") % burg->name % (numShortArms[burg->burgnum] + numLongArms[burg->burgnum]));
		summary += str(boost::format("Total length of %s arms: %.2f\n") % burg->name % (shortLengths[burg->burgnum] + longLengths[burg->burgnum]));
		summary += str(boost::format("Number of %s arms SHORTER than threshold = %d\n") % burg->name % numShortArms[burg->burgnum]);
		summary += str(boost::format("Total length of %s arms shorter than threshold = %.2f\n") % burg->name % shortLengths[burg->burgnum]);
		summary += str(boost::format("Number of %s arms LONGER than threshold = %d\n") % burg->name % numLongArms[burg->burgnum]);
		summary += str(boost::format("Total length of %s arms longer than threshold = %.2f\n") % burg->name % longLengths[burg->burgnum]);
		summary += "\n";
	  }
    }

    // write a row of arm lengths to make analysis via spreadsheet easier
    summary += "--------------------------------------------------\n";
    summary += "SHORT ARM LENGTHS:\n";
    for (vector<BurgerTypeInfo>::iterator burg = BurgInfos.begin(); burg != BurgInfos.end(); burg++) {
      summary += str(boost::format("%.2f\t") %  shortLengths[burg->burgnum]);
      ++n;
    }
    summary += "\nLONG ARM LENGTHS:\n";
    for (vector<BurgerTypeInfo>::iterator burg = BurgInfos.begin(); burg != BurgInfos.end(); burg++) {
      summary += str(boost::format("%.2f\t") %  longLengths[burg->burgnum]);
      ++n;
    }
    summary += "\n";

    if (mNumBins) {
      summary += "\n========================================\n\n";
      // print a row of bin values
      summary += "BINS: \n";
      summary += str(boost::format("max length = %.3f\n") % Arm::mLongestLength);

      long totalArms = 0; // reality check
      double totalLength = 0;  // reality check
      string line;
      int binNum = 0;
      summary += str(boost::format("%-12s%-12s%-12s\n") % "Bin" % "Arms" % "Lengths");
      for (binNum = 0; binNum < mNumBins; ++binNum) {
        summary += str(boost::format("%-12d%-12ld%-12.3f\n") % binNum % armBins[binNum] % armLengthBins[binNum]);
        totalArms += armBins[binNum];
        totalLength += armLengthBins[binNum];
      }
      summary += str(boost::format("%-12s%-12ld%-12.3f\n") % "TOTAL" % totalArms % totalLength);
    }

    summary += "\n========================================\n\n";
    summary += "DECOMPOSITION STATISTICS\n";
    summary += "----------------------\n";
    int energy = 0;
    while (energy < NUM_BCC_ENERGY_LEVELS+1) {
      summary += str(boost::format("Decomposed arms, energy level %d: %d\n") % energy % Arm::mNumDecomposed[energy]);
      energy++;
    }

    summary += str(boost::format("Detached/absorbed arms: %d\n") % Arm::mNumDestroyedInDetachment);

    summary += "\n========================================\n\n";
#ifdef DEBUG_SEGMENTS
    // check against segment lengths:
    uint32_t numSegments[11] = {0}, totalSegments=0, culledSegments=0;  // number of arms of each type
    double segmentLengths[11] = {0}, totalSegmentLength=0, culledLength=0;
    for (std::map<uint32_t, ArmSegment *>::iterator segpos = ArmSegments.begin(); segpos != mArmSegments.end(); ++segpos) {
      ArmSegment *seg = segpos->second;
      double length = seg->GetLength();
      // Cull out half the wrapped segments in such a way that for each culled, there is an identical one remaining:
      if (!seg->Cullable()) {
        segmentLengths[seg->GetMNType()] += length;
        totalSegmentLength += length;
        numSegments[seg->GetMNType()]++;
        totalSegments++;
      } else {
        culledSegments++;
        culledLength +=length;
      }
    }

    summary += "\n===========================================\n\n";
    summary += "REALITY CHECK:  total length of all segments, skipping wrapped segments\n";
    summary += str(boost::format("total Number of segments: %d\n") % totalSegments);
    summary += str(boost::format("total length of all segments: %.2f\n") % totalSegmentLength);
    summary += "\n===========================================\n\n";
    for (i=0; i<11; i++) {
      summary += str(boost::format("%s: number of segs = %d\n") % ArmTypeToName(i).c_str(), numArmsByType[i]);
      summary += str(boost::format("%s: total length of segments = %.2f\n") % ArmTypeToName(i).c_str() % armLengths[i]);
      summary += "----------------------\n";
    }

    summary += str(boost::format("CULLED segments = %d\n") % culledSegments);
    summary += str(boost::format("CULLED length = %.2f\n") % culledLength);

    summary += str(boost::format("Wrapped lengths: %.2f\n") % ArmSegment::mSegLen);
    summary += "----------------------\n\n\n";
#endif
    return summary;
  }

  //===========================================================================
  string DataSet::MonsterNodeSummary(void) {
    vector<uint32_t> monsterTypes(1);
    for (vector<Node*>::iterator nodepos = Node::mNodeVector.begin(); nodepos !=  Node::mNodeVector.end(); nodepos++) {
	  Node *node = *nodepos;
	  if (node && node->GetNodeType() < 0 && !node->mWrapped) {
		uint16_t theType = - node->GetNodeType();
		if (theType >= monsterTypes.size()) {
		  monsterTypes.resize(theType+1);
		}
		monsterTypes[theType]++;
		monsterTypes[0]++;

	  }
    }
    string s;
    s += "MONSTER NODE SUMMARY STATISTICS \n";
    s += "=========================================================================\n";
    s += str(boost::format("Total monster nodes: %1%\n") %monsterTypes[0]);
    for (uint16_t t = 1; t < monsterTypes.size(); t++) {
      if (monsterTypes[t]) {
        s += str(boost::format("Type -%1% monster nodes: %2%\n") % (int)t % monsterTypes[t]);
      }
    }
    s += "=========================================================================\n";
   return s;
  }

  //===========================================================================
  string DataSet::MetaArmSummary(void) {
    if (mDataType == PARADIS_DATATYPE_HCP) return "NO METAARM INFO AVAILABLE FOR HCP";
    uint32_t a = Arm::mArms.size();
    while (a--) {
      Arm::mArms[a]->mSeen = false;
    }
    uint32_t armnum = 0, metaarmcounts[NUM_BCC_ENERGY_LEVELS+1]={0};
    double metaarmtypelengths[NUM_BCC_ENERGY_LEVELS+1] = {0.0}, totalEPDist = 0.0;
    uint32_t numarms = 0;
    for (vector<boost::shared_ptr<MetaArm> >::iterator pos = mMetaArms.begin(); pos != mMetaArms.end(); ++pos) {
      if ((*pos)->mTerminalNodes.size() == 2) {
        totalEPDist += (*pos)->mTerminalNodes[0]->Distance(*( (*pos)->mTerminalNodes[1]), true);
      }
      metaarmcounts[(*pos)->mMetaArmType]++;
      metaarmtypelengths[(*pos)->mMetaArmType] += (*pos)->mLength;
      numarms += (*pos)->mAllArms.size();
      for (vector<Arm*>::iterator armpos = (*pos)->mAllArms.begin(); armpos != (*pos)->mAllArms.end(); ++armpos) {
		paradis_assert(!(*armpos)->mSeen);
        (*armpos)->mSeen = true;
      }
    }
    armnum = 0;
    for  (vector<Arm*>::iterator armpos = Arm::mArms.begin(); armpos != Arm::mArms.end(); armpos++, armnum++) {
	  paradis_assert((*armpos)->mArmType == ARM_EMPTY || (*armpos)->mSeen);
    }

    string summary = "METAARM SUMMARY STATISTICS\n";
    summary += "=========================================================================\n";
    summary += str(boost::format("%9s%28s%20s%20s\n") % "TypeID" % "MetaArmType" % "NumMetaArms" % "MetaArmLengths");
    int i=0;
    double totalLength = 0;
    while (i<4) {
      summary += str(boost::format("%9d%28s%20d%20.2f\n") % i
                     % MetaArmTypeToName(i).c_str()
                     % metaarmcounts[i]
                     % metaarmtypelengths[i]);
      totalLength += metaarmtypelengths[i];
      ++i;
    }
    double totalLoopLength = metaarmtypelengths[2] + metaarmtypelengths[3];
    double ma111beforeDecomp = Arm::mTotalArmLengthBeforeDecomposition - totalLoopLength;
    summary += "\n";
    summary += str(boost::format("%60s%12d\n") % "Total number of arms in metaarms:" % numarms);
    summary += str(boost::format("%60s%12.2f\n") % "Total length of all loops (loops cannot be decomposed):" %  totalLoopLength);
    summary += str(boost::format("%60s%12.2f\n") % "Total Length of all metaarms after decomposition:" % totalLength);
    summary += str(boost::format("%60s%12.2f\n") % "Total EP-Dist of all metaarms:" % totalEPDist);
    summary += str(boost::format("%60s%12.2f\n") % "Computed METAARM_111 length \"before decomposition\":" %  ma111beforeDecomp);
    summary += "\nNote: metaarms are formed after decomposition occurs.  We compute a notional \"length of METAARM_111 before decomposition\" by subtracting the length of the loops from the total lengths of all arms before decomposition.\n";
    summary += "=========================================================================\n";
    return summary;
  }

  //===========================================================================
  void DataSet::DebugPrintNodes(void) {
    std::string filename = mOutputDir + "/" + mOutputBasename + "-Nodes-debug.txt";
    dbecho(1, str(boost::format("Writing nodes to debug file %s... ")%filename).c_str());


    ofstream debugfile (filename.c_str());
    debugfile << endl << endl;
    debugfile << "Full Nodes file written on " << timestamp() << " by " << GetLibraryVersionString("DebugPrintNodes") << endl;
    debugfile << "NODE SUMMARY" << endl;
    debugfile << "=================================================" << endl;
    debugfile <<"Total full nodes: " << Node::mNodes.size() << endl;

    debugfile << MonsterNodeSummary();

    debugfile <<"Total memory for nodes and their pointers: " << Node::mNodes.size() * (sizeof(Node) + sizeof(Node *)) << endl;
    debugfile << "=================================================" << endl;
    debugfile << endl << endl;

	STARTPROGRESS();
	uint32_t nodenum = 0, numnodes = Node::mNodes.size();
    for (map<uint32_t, vector<Node*> >::iterator nodepos = Node::mNodes.begin(); nodepos != Node::mNodes.end(); nodepos++) {
      uint32_t key = nodepos->first;
	  for (vector<Node*>::iterator nodep = nodepos->second.begin(); nodep != nodepos->second.end(); nodep++) {
       UPDATEPROGRESS(nodenum, numnodes,  "DebugPrintNodes: ");
 		Node *node = *nodep;
		debugfile << "mNodes[" << key <<"]: " << node->Stringify(false ) << endl;
		debugfile << "**************************************************************" << endl << endl;
	  }
	}
    COMPLETEPROGRESS(numnodes,  "DebugPrintNodes: ");
	//dbecho(1, "Wrote full nodes to debug file\n"); ;
	return;
  }

  //===========================================================================
  void DataSet::FindEndOfArm(Node *iStartNode, Node **oEndNode,  ArmSegment *iStartSegment,  ArmSegment *&oEndSegment, Arm *theArm, bool reverse ) {
	dbprintf(5, "DataSet::FindEndOfArm(arm %d): startNode = %s\n", theArm->mArmID, iStartNode->Stringify().c_str());
    //NodeIterator currentNode = iStartNode, otherEnd;
 	uint32_t nodenum = 0, segnum = 0;
	Node *currentNode = iStartNode, *otherEnd;
    ArmSegment *currentSegment = iStartSegment;

	if (!theArm->mNodes.size()) {
	  theArm->mNodes.push_back(iStartNode);
	  dbprintf(6, "DataSet::FindEndOfArm: Arm %d: (reverse = %d): node %d: adding iStartNode %s\n", theArm->mArmID, (int)reverse, nodenum++, iStartNode->Stringify().c_str());
	}
    /* loop, don't recurse */
    while(true) {
	  paradis_assert (!currentSegment->mSeen);

	  if (currentSegment->mWrapped) {
		theArm->mNumWrappedSegments++;
		dbprintf(6, "DataSet::FindEndOfArm: Arm %d: (reverse = %d): node %d: pushing back NULL node\n", theArm->mArmID, (int)reverse, nodenum++);
		if (reverse){
		  theArm->mNodes.push_front(NULL); // "wrapped node" signal
		} else{
		  theArm->mNodes.push_back(NULL); // "wrapped node" signal
		}
	  } else {
		theArm->mNumNormalSegments++;
		theArm->mArmLength += currentSegment->GetLength(true);
	  }

	  dbprintf(5, "DataSet::FindEndOfArm: Arm %d: pushing back segment %d: %s\n", theArm->mArmID, segnum++, currentSegment->Stringify(0).c_str());

	  if (reverse){
		theArm->mSegments.push_front(currentSegment);
	  }
	  else {
		theArm->mSegments.push_back(currentSegment);
	  }

      currentSegment->mSeen = true;
      currentSegment->mParentArm = theArm;
	  if (currentSegment->mEndpoints[0] == currentNode)
		otherEnd = currentSegment->mEndpoints[1];
	  else
		otherEnd = currentSegment->mEndpoints[0];

	  dbprintf(6, "DataSet::FindEndOfArm: Arm %d: (reverse = %d): node %d: adding %s\n", theArm->mArmID, (int)reverse, nodenum++, otherEnd->Stringify().c_str());
	  if (reverse) {
		theArm->mNodes.push_front(otherEnd);
	  }
	  else {
		theArm->mNodes.push_back(otherEnd);
	  }

      if (otherEnd == iStartNode ||
		  otherEnd->mNeighborSegments.size() != 2) {
        if (otherEnd == iStartNode) {
          dbprintf(4, "DataSet::FindEndOfArm: Arm %d: LOOP detected\n", theArm->mArmID);
		  theArm->mArmType = ARM_LOOP;
		}
        /*!
          we have looped or found a terminal node -- stop
        */
        oEndSegment = currentSegment;
        *oEndNode = otherEnd;
		theArm->printNodes();
		theArm->printSegments();
        return;
      }

      /*!
        Move on to the next segment, we are not done yet.
      */
      if (currentSegment == otherEnd->mNeighborSegments[0]) {
        currentSegment = otherEnd->mNeighborSegments[1];
      } else {
        currentSegment = otherEnd->mNeighborSegments[0];
      }
      currentNode = otherEnd;
    }
	dbprintf(4, "DataSet::FindEndOfArm: Arm %d: returning non-loop arm\n", theArm->mArmID);
	theArm->printNodes();
	theArm->printSegments();
    return;
  }

  //===========================================================================
  void DataSet::BuildArms(void) {
    dbprintf(2, "BuildArms started.\n");

    STARTPROGRESS();


    int nodenum = 0, totalNodes = Node::mNodes.size();
    try {
      for (map<uint32_t, vector<Node*> >::iterator nodepos = Node::mNodes.begin(); nodepos != Node::mNodes.end(); ++nodepos, ++nodenum) {
		// only need first node in vector; any others are wrapped copies, which always have two neighbors.
		Node *node = nodepos->second[0];
        int neighbornum = 0, numneighbors = node->mNeighborSegments.size();
        if (numneighbors == 2) {
		  if (!node->mNeighborSegments[0]->mSeen) {
			Node *endNode0, *endNode1;
			ArmSegment *endSegment0 = NULL, *endSegment1 = NULL;
			Arm *theArm = new Arm;
			dbprintf(5, "DataSet::BuildArms(): Starting arm %d in middle of arm\n", theArm->mArmID);
			FindEndOfArm(node, &endNode0, node->mNeighborSegments[0], endSegment0, theArm, false);
			theArm->mTerminalNodes.push_back(endNode0);
			endNode0->mNeighborArms.push_back(theArm);
            theArm->mTerminalSegments.push_back(endSegment0);
			if (node->mNeighborSegments[1]->mSeen) {
			  dbprintf(5, "DataSet::BuildArms(): Skipping second half of arm %d as it is a loop.\n", theArm->mArmID);
			}
			else {
			  dbprintf(5, "DataSet::BuildArms(): Searching out second half of arm %d.\n", theArm->mArmID);
			  FindEndOfArm(node, &endNode1, node->mNeighborSegments[1], endSegment1 , theArm, true);
			  if (endNode0 == endNode1) {
				dbprintf(5, "DataSet::BuildArms(): Arm %d is a loop as both directions terminated in the same endpoint.\n", theArm->mArmID);
				theArm->mArmType = ARM_LOOP;
				theArm->mTerminalSegments.push_back(endSegment1);
			  }
			  else {
				theArm->mTerminalNodes.push_back(endNode1);
				endNode1->mNeighborArms.push_back(theArm);
				theArm->mTerminalSegments.push_back(endSegment1);
			  }
			}
			dbprintf(5, "DataSet::BuildArms() (from middle of arm):  Pushing back arm %d: %s\n", theArm->mArmID, theArm->Stringify(0, false).c_str());

            Arm::mTotalArmLengthBeforeDecomposition += theArm->mArmLength;

          }
        } else { // starting with a terminal node; create multiple arms
          while (neighbornum < numneighbors) {
			Node *endNode;
            ArmSegment *startSegment =
              node->mNeighborSegments[neighbornum], *endSegment;
            if (!startSegment->mSeen) {
              Arm *theArm = new Arm;
              dbprintf(5, "DataSet::BuildArms(): Starting arm %d at one end of arm\n", theArm->mArmID);
              FindEndOfArm(node, &endNode, startSegment, endSegment, theArm, false);
              theArm->mTerminalNodes.push_back(node);
              node->mNeighborArms.push_back(theArm);
              if (endNode != node ) {
                theArm->mTerminalNodes.push_back(endNode);
                endNode->mNeighborArms.push_back(theArm);
              }
              theArm->mTerminalSegments.push_back(startSegment);
              if (endSegment != startSegment) {
                theArm->mTerminalSegments.push_back(endSegment);
              }
              dbprintf(5, "DataSet::BuildArms(): (from end of arm) Pushing back arm %d: %s\n",  theArm->mArmID, theArm->Stringify(0).c_str());
              Arm::mTotalArmLengthBeforeDecomposition += theArm->mArmLength;
           }
            ++neighbornum;
          }
        }

        UPDATEPROGRESS(nodenum, totalNodes, str(boost::format("BuildArms: %1% arms created.")%(Arm::mArms.size())));
     }
    } catch (string err) {
      throw str(boost::format("Arm #%1%: %2%")%(Arm::mArms.size())%err);
    }
    COMPLETEPROGRESS(totalNodes, str(boost::format("BuildArms: %1% arms created.")%(Arm::mArms.size())));

    //  print a quick detail of all traced arms
    if (Arm::mTraceArms.size()) {
      for (vector<Arm*>::iterator pos = Arm::mArms.begin(); pos!= Arm::mArms.end(); ++pos) {
        (*pos)->WriteTraceFiles("After-BuildArms");
      }
    }
    dbprintf(1, "BuildArms complete\n");
    return;
  }


  //===========================================================================
  void DataSet::DecomposeArms(void) {
	ArmSegment::mNumBeforeDecomposition = ArmSegment::mArmSegments.size();
	uint32_t armnum = 0;
    vector<Arm*> newArms;
    int energyLevel = NUM_BCC_ENERGY_LEVELS+1, numarms=Arm::mArms.size();
    vector<int32_t> numDecomposed(NUM_BCC_ENERGY_LEVELS+1, 0);
    while (energyLevel-- > 1) {
      if (energyLevel == 1 && !Arm::mTraceArms.size()) {
        // skip level 1; the only reason to do level 1 is to trace arms
        continue;
      }

      STARTPROGRESS();
      armnum = 0;
      for (vector<Arm*>::iterator pos = Arm::mArms.begin(); pos!= Arm::mArms.end(); ++pos, ++armnum) {
        if ((*pos)->Decompose(energyLevel)) {
          numDecomposed[energyLevel]++;
        }
        UPDATEPROGRESS(armnum, numarms, str(boost::format("DecomposeArms: level %1% (%2%), arm %3%, %4% decomp.") % energyLevel %  BurgTypeToName(energyLevel*10) % armnum % numDecomposed[energyLevel]));
      }
      COMPLETEPROGRESS(numarms, str(boost::format("DecomposeArms: level %1% (%2%), arm %3%, %4% decomp.") % energyLevel % BurgTypeToName(energyLevel*10) % armnum % numDecomposed[energyLevel]));
    }
    Arm::mTotalArmLengthAfterDecomposition = ComputeArmLengths();

    if (Arm::mTraceArms.size()) {
      for (vector<Arm*>::iterator pos = Arm::mArms.begin(); pos!= Arm::mArms.end(); ++pos) {
        (*pos)->WriteTraceFiles("After-DecomposeArms");
      }
    }
  }

  //===========================================================================
  void DataSet::WriteSummaryFile(const char *altname) {
    std::string filename = mOutputDir + "/" + mOutputBasename + "-summary.txt";
    if (altname) {
      filename = mOutputDir + string("/summary-") + altname + ".txt";
    }
    ofstream summaryfile (filename.c_str());

    summaryfile << "SUMMARY FILE written  on " << timestamp() << " by " << GetLibraryVersionString("PrintSummaryFile") << endl;
    summaryfile << "Please see the bottom of this file for a discussion of the meanings of \"segment\", \"node\", \"arm\" and \"metaarm.\""<< endl << endl;
    summaryfile << MetaArmSummary() << endl << endl;
    summaryfile << MonsterNodeSummary() << endl << endl;
    summaryfile << ArmSummary() << endl << endl;
    summaryfile << doctext << endl;
    dbecho(1, str(boost::format("Wrote summary file %s\n")% filename).c_str());

    return;
  }

  //===========================================================================
  void DataSet::DebugPrintArms(const char *altname) {
    std::string filename = mOutputDir + "/" + mOutputBasename + "-Arms-debug.txt";
    if (altname) {
      filename = mOutputDir + string("/Arms-debug-") + altname + ".txt";
    }
    dbecho(1, str(boost::format("Writing arms to debug file %s... ")% filename).c_str());
    if (!Mkdir (mOutputDir.c_str())) {
      cerr << "Error: Cannot create output directory for arm file";
      return;
    }

    ofstream debugfile (filename.c_str());

    debugfile << "Arms file written on " << timestamp() << " by " << GetLibraryVersionString("DebugPrintArms") << endl;

    uint32_t armnum = 0, empty=0, numarms = Arm::mArms.size();
    debugfile <<"There are " << numarms << " arms." << endl;
    STARTPROGRESS();
    for (vector<Arm*>::iterator pos = Arm::mArms.begin(); pos != Arm::mArms.end(); ++pos, ++armnum) {
      UPDATEPROGRESS(armnum, numarms, "DebugPrintArms");
      debugfile << "Arm #" << armnum << ": " << (*pos)->Stringify(0, false, (dbg_isverbose() > 5)) << endl;
      debugfile << "******************************************************" << endl << "******************************************************" << endl << endl;
      if ((*pos)->mArmType == ARM_EMPTY)
        ++empty;
    }
    COMPLETEPROGRESS(armnum, "DebugPrintArms");

    debugfile << "Number of arms: " << armnum<< endl;
    debugfile << "Number of EMPTY arms: " << empty<< endl;
    debugfile << "Number of non-EMPTY arms: " << armnum - empty<< endl;
    debugfile << "Total memory used by arms: " << Arm::mArms.size() * sizeof(Arm) << endl;
    debugfile << "Number of arm segments: " << ArmSegment::mArmSegments.size() << endl;
    debugfile << "Memory used by arm segments and their pointers: " << ArmSegment::mArmSegments.size() * (sizeof(ArmSegment)) << endl;

    return;
  }

  //===========================================================================
  void DataSet::DebugPrintMetaArms(void) {
    std::string filename = mOutputDir + "/" + mOutputBasename + "-metaarms-debug.txt";
    dbecho(1, "Writing MetaArms to debug file %s... ", filename.c_str());
    if (!Mkdir (mOutputDir.c_str())) {
      cerr << "Error: Cannot create output directory for arm file";
      return;
    }

    ofstream debugfile (filename.c_str());

    debugfile << "Meta-Arms file written on " << timestamp() << " by " << GetLibraryVersionString("DebugPrintMetaArms") << endl;

    debugfile <<"There are " << mMetaArms.size() << " MetaArms." << endl;
	uint32_t manum = 0, nummaarms = mMetaArms.size();
	STARTPROGRESS();
    vector<boost::shared_ptr<MetaArm> >::iterator pos = mMetaArms.begin(), endpos = mMetaArms.end();
    uint32_t armnum = 0;
    while (pos != endpos) {
	  UPDATEPROGRESS(manum, nummaarms,  "Printing MetaArms: ");
      debugfile << "MetaArm #" << armnum << ": " << (*pos)->Stringify(0) << endl;
      debugfile.flush();
#if INSANE_DEBUG
      debugfile << (*pos)->mAllArms.size() << " arms included in metaarm " << armnum << ": " << endl;debugfile.flush();
      vector<Arm*>::iterator armpos = (*pos)->mAllArms.begin(), armendpos = (*pos)->mAllArms.end();
      while (armpos < armendpos) {
        debugfile << (*armpos)->Stringify(1); debugfile.flush();
        ++armpos;
      }
#endif
      debugfile << "******************************************************" << endl << endl;
      ++armnum;
      ++pos;
    }
	  debugfile << "Number of Metaarms: " << mMetaArms.size()<< endl;
    debugfile << "Total memory used by arms: " << mMetaArms.size() * sizeof(MetaArm) << endl;

    COMPLETEPROGRESS(nummaarms,  "Creating MetaArms: ");
    dbecho(1, "Finished writing MetaArm details to debug file %s\n", filename.c_str());
    return;
  }


  //===========================================================================
  void DataSet::TagNodes(void) {
    // First, have to tag all nodes.  Luckily, we only care about LOOPs.
    int manum = 0, numMetaArms = mMetaArms.size();
    STARTPROGRESS();
    while (manum < numMetaArms) {
      vector<Node*> nodes = mMetaArms[manum]->GetNodes();
      if (mMetaArms[manum]->GetMetaArmType() == METAARM_LOOP_111 || mMetaArms[manum]->GetMetaArmType() == METAARM_LOOP_HIGH_ENERGY) {
        int n = nodes.size();
        while (n--) {
          if (nodes[n]) {
            nodes[n]->SetLoopNode(true);
          }
        }
      }
      UPDATEPROGRESS(manum, numMetaArms, "TagNodes: tagging METAARM_LOOP nodes.");
      ++manum;
    }
    COMPLETEPROGRESS(numMetaArms, "TagNodes: tagging METAARM_LOOP nodes.");
    return;
  }

  // =========================================================================
  // Write a PovRay file with lots of segments
  void DataSet::WritePov(void) {

	/*	for (std::map<uint32_t, ArmSegment *>::iterator seg = ArmSegment::mArmSegments.begin(); seg != ArmSegment::mArmSegments.end(); seg++) {
	  seg->second->mSeen = false;
	  }*/
	/* Write two povray files simultaneously.  "povdeclfile" is a set of declarations of things that can be used to create objects in the povray scene, but contains no actual objects.
	   "povobjfile" creates scene objects that requires functions to operate correctly, such as setting the color of a sweep based on burgers value.
	   Because the order of includes is different between the two, it's convenient to break them into two separate files.  The order is povdeclfile --> render.inc --> povobjfile
	*/

	STARTPROGRESS()   ;
	uint32_t segsWritten = 0;
    string povobjfilename = str(boost::format("%s/%s-obj.pov")%mOutputDir%mOutputBasename);
    dbecho (0, str(boost::format("Writing POV object file %s...\n")%povobjfilename).c_str());
	FILE * povobjfile = fopen(povobjfilename.c_str(), "w");
	// ===============================================================================
    string povdeclfilename = str(boost::format("%s/%s-decl.pov")%mOutputDir%mOutputBasename);
    dbecho (0, str(boost::format("Writing POV declarations file %s...\n")%povdeclfilename).c_str());
	FILE * povdeclfile = fopen(povdeclfilename.c_str(), "w");
	fprintf(povdeclfile, "/* %s: generated automatically by analyzeParaDIS\n */\n", povdeclfilename.c_str());

	uint32_t marmnum = 0, numMarms = mMetaArms.size();
	fprintf(povdeclfile, "#include \"math.inc\"\n");
	fprintf(povdeclfile, "#declare ParaDIS_NumMetaArms = %d;\n", numMarms);
	fprintf(povdeclfile, "#declare ParaDIS_MetaArmPoints = array[%d]\n", numMarms);
	fprintf(povdeclfile, "#declare ParaDIS_MetaArmSegments = array[%d]\n", numMarms);
	fprintf(povdeclfile, "#declare ParaDIS_MetaArmSplines = array[%d]\n", numMarms);
	fprintf(povdeclfile, "#declare ParaDIS_MetaArmSmoothSplines = array[%d]\n", numMarms);
	fprintf(povdeclfile, "#declare ParaDIS_MetaArmSweeps = array[%d]\n", numMarms);
	fprintf(povdeclfile, "#declare ParaDIS_MetaArmIDs = array[%d]\n", numMarms);
	fprintf(povdeclfile, "#declare ParaDIS_MetaArmBurgers = array[%d]\n", numMarms);
	fprintf(povdeclfile, "#declare ParaDIS_MetaArmTypes = array[%d]\n", numMarms);
	fprintf(povdeclfile, "#macro InterpVCos(GC, GS, GE, TS, TE, Method)\n\t <Interpolate(GC, GS, GE, TS.x, TE.x, Method), Interpolate(GC, GS, GE, TS.y, TE.y, Method), Interpolate(GC, GS, GE, TS.z, TE.z, Method)>\n#end\n");

	fprintf(povdeclfile, "#declare ParaDIS_MaxLightTheFuseDistance = %d;\n", ArmSegment::mMaxLightTheFuseDistance);
	fprintf(povdeclfile,
			str(boost::format("#declare ParaDIS_Bounds = array[2] { <%1%, %2%, %3%>, <%4%, %5%, %6%> }\n")
				% Node::mBoundsMin[0]
				% Node::mBoundsMin[1]
				% Node::mBoundsMin[2]
				% Node::mBoundsMax[0]
				% Node::mBoundsMax[1]
				% Node::mBoundsMax[2]).c_str());

	// ===============================================================================
	// NOW the sweep file.  This needs to be done here, because getting it from JSON can jumble the node order due to the undefined order of python dictionary iteratiion
	/*string povsweepfilename = str(boost::format("%s/%s.pov")%mOutputDir%mOutputBasename);
    dbecho (0, str(boost::format("Writing POV file %s...\n")%povsweepfilename).c_str());
	FILE * povsweepfile = fopen(povsweepfilename.c_str(), "w");

	fprintf(povsweepfile, "// %s: generated automatically by analyzeParaDIS\n \n", povsweepfilename.c_str());
	fprintf(povsweepfile, "#include \"math.inc\"\n");
	fprintf(povsweepfile, "#declare ParaDIS_NumMetaArms = %d;\n", numMarms);

	fprintf(povsweepfile,
			str(boost::format("#declare ParaDIS_Bounds = array[2] { <%1%, %2%, %3%>, <%4%, %5%, %6%> }\n")
				% Node::mBoundsMin[0]
				% Node::mBoundsMax[0]
				% Node::mBoundsMin[1]
				% Node::mBoundsMax[1]
				% Node::mBoundsMin[2]
				% Node::mBoundsMax[2]).c_str());
	*/
	fprintf(povdeclfile, "#ifndef (SweepRadius)\n#declare SweepRadius = 50;\n#end\n");
	// ===============================================================================
	std::vector<boost::shared_ptr<MetaArm> >::iterator marm;
	fprintf(povdeclfile, "/* ======================================== */ \n");
	fprintf(povdeclfile, "/* BEGIN METAARM LIST */ \n");
	fprintf(povdeclfile, "/* ======================================== */ \n");
	//	fprintf(povdeclfile, "union {\n\tunion {\n");
	for (marm = mMetaArms.begin(), marmnum=0; marm != mMetaArms.end(); marm++, marmnum++) {
	  UPDATEPROGRESS(marmnum, numMarms, "Writing MetaArms to povray file");
	  vector<Node *> marmnodes = (*marm)->GetNodes();
	  if (!marmnodes.size()) continue;
	  int32_t metaArmID = (*marm)->GetMetaArmID();
	  int16_t metaArmType = (*marm)->GetMetaArmType(),
		burgers = (*marm)->GetBurgersType();
	  fprintf(povdeclfile, "\n/* ======================================== */ \n");
	  fprintf(povdeclfile, "#declare ParaDIS_MetaArmIDs[%d] = %d;\n", marmnum, metaArmID);
	  fprintf(povdeclfile, "#declare ParaDIS_MetaArmBurgers[%d] = %d;\n", marmnum, burgers);
	  fprintf(povdeclfile, "#declare ParaDIS_MetaArmTypes[%d] = %d;\n", marmnum, metaArmType);
	  uint32_t numnodes = marmnodes.size();
	  // fprintf(povdeclfile, "makeMetaArm(%d, %d, %d, array[%d] {", metaArmID, metaArmType, burgers, numnodes);
	  int nodenum = 0, numwraps = 0;
	  string povLocString;
	  fprintf(povdeclfile, "#declare ParaDIS_MetaArmPoints[%d] = array[%d] { ", marmnum,numnodes);
	  for (vector<Node*>::iterator node = marmnodes.begin();
		   node != marmnodes.end(); node++, nodenum++) {
		if (*node) {
		  povLocString = GetPovrayLocationString((*node)->mLocation);
		}
		if (nodenum) fprintf(povdeclfile, ", ");
		if (!*node) {
		  fprintf(povdeclfile, "/* WRAP */");
		  numwraps++;
		}
		fprintf(povdeclfile, "%s", povLocString.c_str());
	  }
	  fprintf(povdeclfile, "} \n");

	  vector<string> smoothPoints; // collect an alternative subsampled spline to reduce camera jitter
	  fprintf(povdeclfile, "#declare ParaDIS_MetaArmSplines[%d] = spline { \n\tnatural_spline\n", marmnum);
	  nodenum = 0;
	  numnodes += numwraps * 19;  // Insert additional 19 nodes at each wrap, increasing one step to 20.
	  Node *previousNode = NULL;
	  for (vector<Node*>::iterator node = marmnodes.begin();
		   node != marmnodes.end(); node++, nodenum++) {
		if (*node) {
		  povLocString = GetPovrayLocationString((*node)->mLocation);
		  fprintf(povdeclfile, "\t%0.5f, %s\n", nodenum/(numnodes-1.0), povLocString.c_str());
		  if (nodenum % 20 == 0) {
			smoothPoints.push_back(str(boost::format("%0.5f, %s")%(nodenum/(numnodes-1.0))%povLocString));
		  }
		}
		if (!*node) {
		  // always push a wrapped node to smoothed points as they are important
		  smoothPoints.push_back(str(boost::format("%0.5f, %s")%(nodenum/(numnodes-1.0))%(GetPovrayLocationString(previousNode->mLocation))));
		  ++node;
		  string nextLocString = GetPovrayLocationString((*node)->mLocation);
		  smoothPoints.push_back(str(boost::format("%0.5f, %s")%(nodenum/(numnodes-1.0))%(nextLocString)));
		  fprintf(povdeclfile, "\t/* WRAP 20 transition points. */\n");
		  float GS = nodenum/(numnodes-1.0), GE = (nodenum + 19.0)/(numnodes-1.0);
		  for (int step = 0; step < 20; step++) {
			nodenum++;
			float GC = nodenum/(numnodes-1.0);
			fprintf(povdeclfile, "\t%0.5f, InterpVCos(%f, %f, %f, %s, %s, 0)\n", GC, GC, GS, GE,  povLocString.c_str(), nextLocString.c_str());
			if (nodenum % 10 == 0) {
			  smoothPoints.push_back(str(boost::format("%0.5f, InterpVCos(%f, %f, %f, %s, %s, 0)") %GC %GC %GS %GE %povLocString %nextLocString));
			}
		  }
		  fprintf(povdeclfile, "\t/* END WRAP */\n");
		}
		previousNode = *node;
	  }
	  fprintf(povdeclfile, "} /* end spline */ \n");

	  // ======================================================================
	  // Now write the smoothed spline which will be used for camera animation
	  fprintf(povdeclfile, "#declare ParaDIS_MetaArmSmoothSplines[%d] = spline { \n\tnatural_spline\n", marmnum);
	  for (vector<string>::iterator pos = smoothPoints.begin(); pos != smoothPoints.end(); pos++) {
		fprintf(povdeclfile, "\t%s\n", pos->c_str());
	  }
	  fprintf(povdeclfile, "} /* end smoothed spline */ \n");


	  // ======================================================================
	  // Now write sphere sweeps to union into the sweep file for metaarm.
	  // There will be multiple sweeps due to wrapping (periodic bounds)
	  // This is actually a poor way to render -- very slow -- use segments instead.
	  vector<vector<string> > sweepnodelists;
	  vector<string>sweepnodes;
	  for (vector<Node*>::iterator node = marmnodes.begin();
		   node != marmnodes.end(); node++, nodenum++) {
		if (*node) {
		  sweepnodes.push_back(GetPovrayLocationString((*node)->mLocation));
		}
		else {
		  // we just ended one sweep
		  if (sweepnodes.size() == 1) {
			sweepnodes.push_back(sweepnodes[0]);
		  }
		  sweepnodelists.push_back(sweepnodes);
		  sweepnodes.clear();
		}
	  }
	  // collect the last sweep:
	  sweepnodelists.push_back(sweepnodes);
	  if (sweepnodelists.size() > 1) {
		fprintf(povdeclfile, "#declare ParaDIS_MetaArmSweeps[%d] = union { \n", marmnum);
	  } else {
		fprintf(povdeclfile, "#declare ParaDIS_MetaArmSweeps[%d] = ", marmnum);
	  }
	  for (uint snum = 0; snum < sweepnodelists.size(); snum++) {
		if (sweepnodelists[snum].size()) {
		  // Just ended a sweep, create the actual object
		  fprintf(povdeclfile, "\tsphere_sweep { linear_spline\n");
		  fprintf(povdeclfile, "\t\t%d,\n", (int)sweepnodelists[snum].size());
		  for (uint i = 0; i < sweepnodelists[snum].size(); i++) {
			fprintf(povdeclfile, "\t\t%s, SweepRadius\n", sweepnodelists[snum][i].c_str());
		  }
		  fprintf(povdeclfile, "\t} /* end sphere_sweep */\n");
		}
	  }
	  if (sweepnodelists.size() > 1) {
		fprintf(povdeclfile, "} /* end union */ \n");
	  }

	  // ======================================================================
	  // Finally write segments to union into the sweep file for metaarm.

	}
	COMPLETEPROGRESS(numMarms, "Writing MetaArms to povray  files");
	for (std::map<uint32_t, ArmSegment *>::iterator pos = ArmSegment::mArmSegments.begin(); pos != ArmSegment::mArmSegments.end(); pos++) {
	  ArmSegment * seg = pos->second;

	  if (seg->mWrapped) continue;

	  int segid = seg->GetID();
	  int armid = seg->mParentArm->mArmID;
	  int maid = seg->mParentArm->GetParentMetaArm()->GetMetaArmID();
	  int burgers = seg->GetBurgersType();
	  uint32_t distance = seg->mLightTheFuseDistance;
	  vector<float> loc0 = seg->GetEndpoint(0)->mLocation, loc1 = seg->GetEndpoint(1)->mLocation;
	  /* vector<float> saved = loc1;
	  Wrap(loc0, loc1);
	  int i = 0;
	  for ( i=0; i<3; i++) {
		if (loc0[i] != loc1[i]) break;
	  }
	  if (i==3) {
		cerr << str(boost::format("Wrap created identical endpoints in segment ID %1%, arm %2%")% segid % armid) << endl;
		abort();
		}*/
	  string segstring =
		str(boost::format("segment(%d, %d, %d, %s, %s, %d, %d)\n")
			% segid
			% armid
			% maid
			% GetPovrayLocationString(loc0)
			% GetPovrayLocationString(loc1)
			% burgers
			% distance);
	  // dbprintf(5, segstring.c_str());
	  fprintf(povobjfile, segstring.c_str());
	  segsWritten++;
	  /*	  if (!seg->second->mSeen) {
		dbprintf(5, "Not written: %s\n", seg->second->Stringify().c_str());
		seg->second->mSeen = false;
		}
	  */
	}
	//fprintf(povdeclfile, "\t}\n}\n");
	dbecho(1, "Wrote %d segments out of %d into povray object file\n", (int)segsWritten, (int)(ArmSegment::mArmSegments.size()));
	fclose(povobjfile);
	fclose(povdeclfile);

  }

  // =========================================================================
  // Dump a JSON file containing the indicated arms.
  // JSON can be imported into python for use in blender scripts
  void DataSet::WriteJson(void) {
    string jsonfile = str(boost::format("%s/%s.json")%mOutputDir%mOutputBasename);
    dbecho (0, str(boost::format("Writing json file %s...\n")%jsonfile).c_str());

    string bounds = str(boost::format("%1% %2% %3% %4% %5% %6%")
                        % Node::mBoundsMin[0]
                        % Node::mBoundsMax[0]
                        % Node::mBoundsMin[1]
                        % Node::mBoundsMax[1]
                        % Node::mBoundsMin[2]
                        % Node::mBoundsMax[2]);
    using boost::property_tree::ptree;
    {
      // Make a map of all nodes, to avoid creating duplicate nodes for endpoints, then we can add each map entry to the JSON ptree and write it out.  Memory explosion!
      STARTPROGRESS()   ;
	  ptree pt;
      pt.put("Bounds", bounds);
      map<int32_t, Node *> nodemap;
	  uint32_t metaArmNum = 0, numMetaArms = mMetaArms.size();
	  for (vector<boost::shared_ptr<MetaArm> >::iterator marm = mMetaArms.begin(); marm != mMetaArms.end(); marm++, metaArmNum++) {
		UPDATEPROGRESS(metaArmNum+1.0, numMetaArms, "Writing metaarms to json");
		//fprintf(stderr, "Writing metaarm %d of %d ( %4.1f%%, %f remaining )\r", metaArmNum+1, numMetaArms, 100.0 * (metaArmNum+1.0)/numMetaArms);
		int32_t marmBurgers = 0;
		vector<Arm*> &arms = (*marm)->mAllArms;
		uint32_t armnum = 0;
		for (vector<Arm*>::iterator arm = arms.begin(); arm != arms.end(); arm++, armnum++){
		  if (!(*arm)->mNodes.size()){
			continue;
		  }
		  int32_t armid = (*arm)->mArmID;
		  pt.put(str(boost::format("MetaArms.MetaArm %1%.ID") % metaArmNum),
				 (*marm)->mMetaArmID);
		  pt.put(str(boost::format("MetaArms.MetaArm %1%.Arms.Arm %2%.ID")
					 % metaArmNum % armnum),
				 armid);

		  int nodenum = 0;
		  for (deque<Node*>::iterator node = (*arm)->mNodes.begin(); node != (*arm)->mNodes.end(); node++) {
			if (*node) {
			  nodemap[(*node)->mNodeIndex] = *node;
			  pt.put(str(boost::format("Arms.Arm %1%.Nodes.Node %2%.ID")
						 % armid % nodenum),
					 (*node)->mNodeIndex);
			} else {
			  pt.put(str(boost::format("Arms.Arm %1%.Nodes.Node %2%.ID")
						 % armid % nodenum),
					 "WRAP");
			}
			nodenum++;
		  }
		  int segnum = 0, burgers = (*arm)->mSegments[0]->GetBurgersType();
		  pt.put(str(boost::format("Arms.Arm %1%.burgers") % armid), burgers);

		  if (!marmBurgers) {
			marmBurgers = burgers;
			pt.put(str(boost::format("MetaArms.MetaArm %1%.burgers") % metaArmNum),
				   burgers);
		  }

		  for (deque<ArmSegment*>::iterator seg = (*arm)->mSegments.begin(); seg != (*arm)->mSegments.end(); seg++, segnum++) {
			pt.put(str(boost::format("Arms.Arm %1%.Segments.Segment %2%.ID")
					   % armid % segnum),
				   (*seg)->GetID());
			pt.put(str(boost::format("Segments.Segment %1%.burgers")
					   % ((*seg)->GetID())),
				   (*seg)->GetBurgersType());
			pt.put(str(boost::format("Segments.Segment %1%.rotation")
					   % ((*seg)->GetID())),
				   (*seg)->BlenderRotationString());
			for (int i=0; i<2; i++) {
			  vector<float>location = (*seg)->GetEndpoint(i)->mLocation;
			  pt.put(str(boost::format("Segments.Segment %1%.EP %2%")
						 % ((*seg)->GetID())%i),
					 str(boost::format("%1% %2% %3%")
						 % location[0] % location[1] % location[2]));
			}
		  }
		}
		for (map<int32_t, Node *>::iterator pos = nodemap.begin(); pos != nodemap.end(); pos++) {
		  Node *node = pos->second;
		  vector<float>location = node->mLocation;
		  pt.put(str(boost::format("Nodes.Node %1%.location")
					 % (node->mNodeIndex)),
				 str(boost::format("%1% %2% %3%")
					 % location[0] % location[1] % location[2]));
		  pt.put(str(boost::format("Nodes.Node %1%.NumNeighbors")
					 % (node->mNodeIndex)),
				 str(boost::format("%1%")
					 % (node->mNeighborSegments.size())));
		}
	  }

      write_json(jsonfile, pt);
	  COMPLETEPROGRESS(numMetaArms, "Writing metaarms");
      dbecho (0, "\n...done\n");
    }
    return;
  }


  //===========================================================================
  // Write a vtk fileset containing all nodes and segments.
  void DataSet::WriteVTKFiles(void) {
    // =======================================================
    // SEGMENT FILES
    // We go by arm by arm to save writing duplicate points everywhere.
    uint32_t numsegfiles = ArmSegment::mArmSegments.size()/mElementsPerVTKFile + 1;
    dbecho(0, str(boost::format("DataSet::WriteVTKFiles() will write %d segment files. \n")% numsegfiles).c_str());
    vector<FILE *> segfiles;
    vector<string> segfilenames;
    for (uint32_t fileno = 0; fileno < numsegfiles; fileno++) {
      string filename = str(boost::format("%s/%s-segments-%04d.vtk")%mOutputDir%mOutputBasename%fileno);
      FILE * fp = fopen(filename.c_str(), "w");
      if (!fp) {
        dbprintf(0, "DataSet::WriteVTKFiles(): ERROR: cannot open VTK file %s for writing\n", filename.c_str());
        return;
      }
      segfilenames.push_back(filename);
      segfiles.push_back(fp);
    }

    // ----------------------------------------------------------
    // WRITE THE SEGMENT FILES
    float armsperfile = (float)(Arm::mArms.size())/segfiles.size();
    for (uint32_t fileno = 0; fileno < segfiles.size(); fileno++) {
	  dbprintf(5, "Writing segment file %s\n", segfilenames[fileno].c_str());

	  STARTPROGRESS();

	  FILE *segfile = segfiles[fileno];
      string filename = segfilenames[fileno];
      uint32_t firstarm =  armsperfile * fileno;
      uint32_t lastarm = armsperfile *(fileno + 1) - 1;
      if (fileno == segfiles.size()-1)
        lastarm = Arm::mArms.size() - 1;
      uint32_t numarms = lastarm-firstarm+1;
      // ----------------------------------------------------------
      // 1. Segment file: the header
      fprintf(segfile,"# vtk DataFile Version 3.0\n" );
      fprintf(segfile, str(boost::format("%s, written by paraDIS_lib. mBounds are %s, number of arms = %d\n")% filename % (Node::mBoundsSize.Stringify()) % (lastarm-firstarm+1)).c_str());
      fprintf(segfile, "ASCII\n" );
      fprintf(segfile, "DATASET POLYDATA\n" );

      // ----------------------------------------------------------
      // 2. Segment file: Points.  We iterate by arms because
      //   going by segment would
      //   cause us to write too many duplicate points.  This of course
      //   comes at a minor expense in imperfect load balancing due to
      //   unequal unpredictable arm lengths.  <Shrug>
      uint32_t numnodes = 0, numsegs = 0;
      for (uint32_t armnum = 0; armnum < numarms; armnum++) {
		numnodes += Arm::mArms[armnum+firstarm]->mNodes.size() - Arm::mArms[armnum+firstarm]->mNumWrappedSegments;
      }
      fprintf(segfile, "\nPOINTS %d float\n", numnodes);
	  uint32_t nodeswritten = 0;
      for (uint32_t armnum = 0; armnum < numarms; armnum++) {
		Arm *arm = Arm::mArms[armnum+firstarm];
		uint32_t armsegs = 0, armnodes = 0;
        Node *previous = NULL;
        for (deque<Node*>::const_iterator node = arm->mNodes.begin(); node != arm->mNodes.end(); node++) {
          if (*node) {
            fprintf(segfile, "%f %f %f\n", (*node)->mLocation[0], (*node)->mLocation[1], (*node)->mLocation[2]);
			armnodes++;
			nodeswritten++;
            if (previous) {
              numsegs ++;
			  armsegs++;
            }
          }
          previous = *node;
        }
		if (armsegs != arm->mNumNormalSegments) {
		  arm->printNodes();
		  arm->printSegments();
		  dbecho(0, "Arm %d: armsegs %d != theArm->mNumNormalSegments %d for arm: %s\n", arm->mArmID, armsegs,arm ->mNumNormalSegments, arm->Stringify(0).c_str());
		  errexit;
		}
		if (armnodes != arm->mNodes.size() - arm->mNumWrappedSegments) {
		  dbecho (0, "Arm %d: armnodes (%d) != arm->mNodes.size - arm->mNumWrappedSegments (%d) in WriteVTKFile()\n", arm->mArmID, armnodes, (int)(arm->mNodes.size() - arm->mNumWrappedSegments));
		paradis_assert(nodeswritten == numnodes) ;
	  }

		UPDATEPROGRESS(armnum, numarms, "Writing points for arms");
      }
	  COMPLETEPROGRESS(numarms,  "Writing points for arms");
	  if (nodeswritten != numnodes) {
		dbecho (0, "nodeswritten (%d) != numnodes (%d) in WriteVTKFile()\n", nodeswritten, numnodes);
		paradis_assert(nodeswritten == numnodes) ;
	  }

      // ----------------------------------------------------------
      // 3. Segment file: Segments (connectivity)
      fprintf(segfile, "\nLINES %d %d\n", numsegs, 3*numsegs);
	  uint32_t segswritten = 0;
      uint32_t nodenum = 0;
      for (uint32_t armnum = 0; armnum < numarms; armnum++) {
        Arm *theArm = Arm::mArms[armnum+firstarm];
		uint32_t armsegs = 0;
        if (!theArm->mNumNormalSegments)
          continue;

		//fprintf(segfile, "# BEGIN ARM %d\n", theArm->mArmID);
        if (theArm->mNumWrappedSegments) {
		  uint numwrappedNodes = 0;
          Node *previous = NULL;
          for (deque<Node *>::const_iterator node = theArm->mNodes.begin(); node != theArm->mNodes.end(); node++){
            if (*node) {
              if (previous) {
                fprintf(segfile, "2 %d %d\n", nodenum-1, nodenum);
 				segswritten++;
				armsegs++;
              }
              ++nodenum;
            }
			else {
			  // should not have a NULL node following a NULL node:
			  paradis_assert(previous);

			  //fprintf(segfile, "# (WRAP)\n");
			  numwrappedNodes++;

			}
            previous = *node;
          }
        }
        else {
          dbprintf(5, "DataSet::WriteVTKFiles(): no wrapped segments for arm %d\n", (theArm->mArmID));
		  for (uint32_t segnum = 0; segnum < theArm->mNumNormalSegments; segnum++) {
			fprintf(segfile, "2 %d %d\n", nodenum, nodenum+1);
			++nodenum;
			segswritten++;
			armsegs++;
		  }
		  ++nodenum;
		}
		dbprintf(6, "Wrote %d segments for arm %d\n", armsegs, theArm->mArmID);
		if (armsegs != theArm->mNumNormalSegments) {
		  theArm->printNodes();
		  theArm->printSegments();
		  dbecho(0, "armsegs %d != theArm->mNumNormalSegments %d for arm: %s\n", armsegs, theArm->mNumNormalSegments, theArm->Stringify(0).c_str());
		}
		paradis_assert(armsegs == theArm->mNumNormalSegments);
		UPDATEPROGRESS(armnum, numarms, "Writing segment locations for arms");

      }
	  //COMPLETEPROGRESS(numarms,  "Writing segment locations for arms");
	  if (segswritten != numsegs) {
		dbprintf(0, "ERROR: segswritten %d != numsegs %d\n", segswritten, numsegs);
	  }
	  paradis_assert(segswritten == numsegs);

      // ----------------------------------------------------------
      // 4. Segment file: Segment Arm numbers -- this should be much more straightforward as we do not need to reference any nodes now.
      fprintf(segfile, "\nCELL_DATA %d\n", numsegs);
      fprintf(segfile, "SCALARS armnum int\n");
      fprintf(segfile, "LOOKUP_TABLE default\n");
      uint32_t testsegs = 0;
      for (uint32_t armnum = 0; armnum < numarms; armnum++) {
        Arm *theArm = Arm::mArms[armnum+firstarm];
        if (!theArm->mNumNormalSegments)
          continue;
        for (uint32_t segnum = 0; segnum < theArm->mNumNormalSegments; segnum++) {
          fprintf(segfile, "%d ", armnum);
          ++testsegs;
        }
		UPDATEPROGRESS(armnum, numarms, "Writing segment arm numbers for arms");
      }
      fprintf(segfile, "\n");
      //COMPLETEPROGRESS(numarms,  "Writing segment arm numbers for arms");
	  if (testsegs != numsegs) {
        dbecho(0, str(boost::format("DataSet::WriteVTKFiles(): ERROR!  testsegs %d != numsegs %d\n")%testsegs%numsegs).c_str());
      }
      // ----------------------------------------------------------
      // 5. Segment file: Segment Burgers type
      fprintf(segfile, "SCALARS burgers_type int\n");
      fprintf(segfile, "LOOKUP_TABLE default\n");
      for (uint32_t armnum = 0; armnum < numarms; armnum++) {
        Arm *theArm = Arm::mArms[armnum+firstarm];
        if (!theArm->mNumNormalSegments)
          continue;
        uint32_t burgertype = theArm->GetBurgersType();
        for (uint32_t segnum = 0; segnum < theArm->mNumNormalSegments; segnum++) {
          fprintf(segfile, "%d ", burgertype);
        }
		UPDATEPROGRESS(armnum, numarms, "Writing segment burgers types for arms");
      }
      fprintf(segfile, "\n");
	  //COMPLETEPROGRESS(numarms, "Writing segment burgers types for arms");

      // ----------------------------------------------------------
      // 6. Segment file: Segment Original Burgers type before decomposition
      // Work in progress:  haven't figured out yet how to print these in the correct order in an efficient way.  The information is there, though, in each segment, stored as mOriginalBurgersType
      // So for now, commenting this out.
      fprintf(segfile, "SCALARS original_burgers_type int\n");
      fprintf(segfile, "LOOKUP_TABLE default\n");
      for (uint32_t armnum = 0; armnum < numarms; armnum++) {
        Arm *theArm = Arm::mArms[armnum+firstarm];
        if (!theArm->mNumNormalSegments)
          continue;
        uint32_t burgertype = theArm->GetOriginalBurgersType();
        for (uint32_t segnum = 0; segnum < theArm->mNumNormalSegments; segnum++) {
          fprintf(segfile, "%d ", burgertype);
        }
		UPDATEPROGRESS(armnum, numarms, "Writing segment burgers types for arms");
      }
      fprintf(segfile, "\n");

	  //COMPLETEPROGRESS(numarms, "Writing segment burgers types for arms");


      // ----------------------------------------------------------
      // 7a. Segment file: Segment arm type
      // ----------------------------------------------------------
      fprintf(segfile, "SCALARS arm_type int\n");
      fprintf(segfile, "LOOKUP_TABLE default\n");
      for (uint32_t armnum = 0; armnum < numarms; armnum++) {
        Arm *theArm = Arm::mArms[armnum+firstarm];
        if (!theArm->mNumNormalSegments)
          continue;
        uint32_t atype = theArm->mArmType;
        for (uint32_t segnum = 0; segnum < theArm->mNumNormalSegments; segnum++) {
          fprintf(segfile, "%d ", atype);
        }
 		UPDATEPROGRESS(armnum, numarms, "Writing segment arm types for arms");
     }
      fprintf(segfile, "\n");
      COMPLETEPROGRESS(numarms, "Writing segment arm types for arms");

      // ----------------------------------------------------------
      // 7b. Segment file: Segment metaarm type
      // ----------------------------------------------------------
      fprintf(segfile, "SCALARS metaarm_type int\n");
      fprintf(segfile, "LOOKUP_TABLE default\n");
      for (uint32_t armnum = 0; armnum < numarms; armnum++) {
        Arm *theArm = Arm::mArms[armnum+firstarm];
        if (!theArm->mNumNormalSegments)
          continue;
        uint32_t matype = theArm->GetMetaArmType();
        for (uint32_t segnum = 0; segnum < theArm->mNumNormalSegments; segnum++) {
          fprintf(segfile, "%d ", matype);
        }
 		UPDATEPROGRESS(armnum, numarms, "Writing segment metaarm types for arms");
      }
      fprintf(segfile, "\n");
	  //COMPLETEPROGRESS(numarms, "Writing segment metaarm types for arms");

      // ----------------------------------------------------------
      // 8. Segment file: block (file) number
      // ----------------------------------------------------------
      fprintf(segfile, "SCALARS block_number int\n");
      fprintf(segfile, "LOOKUP_TABLE default\n");
      for (uint32_t armnum = 0; armnum < numarms; armnum++) {
        Arm *theArm = Arm::mArms[armnum+firstarm];
        if (!theArm->mNumNormalSegments)
          continue;
        for (uint32_t segnum = 0; segnum < theArm->mNumNormalSegments; segnum++) {
          fprintf(segfile, "%d ", fileno);
        }
 		UPDATEPROGRESS(armnum, numarms, "Writing segment block (file) numbers for arms");
      }
      fprintf(segfile, "\n");
      //COMPLETEPROGRESS(numarms, "Writing segment block (file) numbers for arms");

      // ----------------------------------------------------------
      // 9. Segment file: segment Screw Type for Jaime Marian
      // ----------------------------------------------------------
      fprintf(segfile, "SCALARS screw_type int\n");
      fprintf(segfile, "LOOKUP_TABLE default\n");
      uint32_t segnum = 0;
      for (uint32_t armnum = 0; armnum < numarms; armnum++) {
        Arm *theArm = Arm::mArms[armnum+firstarm];
        if (!theArm->mNumNormalSegments)
          continue;
        for (deque<ArmSegment*>::iterator seg = theArm->mSegments.begin(); seg != theArm->mSegments.end(); seg++) {
          uint32_t screwtype = (*seg)->GetScrewType();
          fprintf(segfile, "%d ", screwtype);
          ++segnum;

        }
 		UPDATEPROGRESS(armnum, numarms, "Writing segment screw types for arms");
      }
	  fprintf(segfile, "\n");
      COMPLETEPROGRESS(numarms, "Writing segment screw types for arms");


	  COMPLETEPROGRESS(numarms, str(boost::format("Writing segment file %s\n")% filename));
      // ----------------------------------------------------------
      dbecho(1, str(boost::format("DataSet::WriteVTKFiles(): Wrote segment file %s\n")% filename).c_str());
      fclose(segfile);
    }
    // ============================================================
    // Write out the VisIt file declaring the segfiles as blocks
    string visitsegfilename = str(boost::format("%s/%s-segments.visit")%mOutputDir%mOutputBasename);
    FILE *visitsegfile = fopen(visitsegfilename.c_str(), "w");
    fprintf(visitsegfile, "!NBLOCKS %d\n", (int)segfiles.size());
    for (uint32_t fileno = 0; fileno < segfiles.size(); fileno++) {
      fprintf(visitsegfile, "%s\n", Basename(segfilenames[fileno]).c_str());
    }
    fclose(visitsegfile);
    dbecho(1, str(boost::format("DataSet::WriteVTKFiles(): Wrote visit file %s\n")% visitsegfilename).c_str());


    // ============================================================
    // NODE FILES
    // We use mElementsPerVTKFile to limit our node file size too.
    uint32_t numnodefiles = Node::mNodes.size()/mElementsPerVTKFile + 1;
    dbecho(0, str(boost::format("DataSet::WriteVTKFiles() will write %d node files. \n")% numnodefiles).c_str());
    vector<FILE *> nodefiles;
    vector<string> nodefilenames;
    for (uint32_t fileno = 0; fileno < numnodefiles; fileno++) {
      string filename = str(boost::format("%s/%s-nodes-%04d.vtk") % mOutputDir % mOutputBasename % fileno);
      FILE * fp = fopen(filename.c_str(), "w");
      if (!fp) {
        dbecho(0, str(boost::format("Error: cannot open VTK file %s for writing\n")% filename).c_str());
        return;
      }
      nodefilenames.push_back(filename);
      nodefiles.push_back(fp);
    }

    // ----------------------------------------------------------
    // WRITE THE NODE FILES
    vector<Node *>::iterator nodepos = Node::mNodeVector.begin(),
      nodeend =  Node::mNodeVector.end();
    float nodesperfile = (float)(Node::mNodeVector.size())/numnodefiles;
    uint32_t firstnode = 0;

    for (uint32_t fileno = 0; fileno < nodefiles.size(); fileno++) {
      FILE *nodefile = nodefiles[fileno];
      string filename = nodefilenames[fileno];
      uint32_t lastnode = nodesperfile *(fileno + 1.0) - 1;
      if (fileno == nodefiles.size()-1)
        lastnode = Node::mNodeVector.size()-1;
      uint32_t numnodes = lastnode - firstnode + 1;

      vector<Node *>::iterator firstnodepos = nodepos;

      // ----------------------------------------------------------
      // 1. Node file: the header
      fprintf(nodefile,"# vtk DataFile Version 3.0\n" );
      fprintf(nodefile, str(boost::format("%s, written by paraDIS_lib. mBounds are %s\n")% filename % (Node::mBoundsSize.Stringify())).c_str());
      fprintf(nodefile, "ASCII\n" );
      fprintf(nodefile, "DATASET POLYDATA\n" );

      // ----------------------------------------------------------
      // 2. Node file: Points.
      fprintf(nodefile, "\nPOINTS %d float\n", numnodes);
      uint32_t nodecount = 0;
      for (nodecount = 0;
           nodecount < numnodes && nodepos != nodeend;
           nodecount++, nodepos++) {
        fprintf(nodefile, "%f %f %f\n", (*nodepos)->mLocation[0], (*nodepos)->mLocation[1], (*nodepos)->mLocation[2]);

      }
      if (nodecount < numnodes) {
        dbecho(0, str(boost::format("DataSet::WriteVTKFiles() PROGRAMMING ERROR: ran out of nodes.  \n")).c_str());
      }
      fprintf(nodefile, "\n");

      // ----------------------------------------------------------
      // 2. Node file: Vertices
      fprintf(nodefile, "VERTICES %d %d\n", numnodes, 2*numnodes);
      for (uint32_t nodenum = 0; nodenum < numnodes; nodenum++) {
        fprintf(nodefile, "1 %d\n", nodenum);
      }
      fprintf(nodefile, "\n");


      // ----------------------------------------------------------
      fprintf(nodefile, "CELL_DATA %d\n", numnodes);

      // ----------------------------------------------------------
      // Node file: Node Index
      fprintf(nodefile, "SCALARS node_index int\n");
      fprintf(nodefile, "LOOKUP_TABLE default\n");
      nodepos = firstnodepos;
      for (uint32_t nodenum = 0;  nodenum < numnodes; nodenum++, nodepos++) {
        fprintf(nodefile, "%d ", (*nodepos)->mNodeIndex);
      }
      fprintf(nodefile, "\n");
      // ----------------------------------------------------------
      // Node file: Node IsLoop
      fprintf(nodefile, "SCALARS node_isLoop int\n");
      fprintf(nodefile, "LOOKUP_TABLE default\n");
      nodepos = firstnodepos;
      for (uint32_t nodenum = 0;  nodenum < numnodes; nodenum++, nodepos++) {
        fprintf(nodefile, "%d ", (int)((*nodepos)->IsLoopNode()));
      }
      fprintf(nodefile, "\n");
      // ----------------------------------------------------------
      // Node file: Node Type
      fprintf(nodefile, "SCALARS node_type int\n");
      fprintf(nodefile, "LOOKUP_TABLE default\n");
      nodepos = firstnodepos;
      for (uint32_t nodenum = 0;  nodenum < numnodes; nodenum++, nodepos++) {
        fprintf(nodefile, "%d ", (int)((*nodepos)->GetNodeType()));
      }
      fprintf(nodefile, "\n");
      // ----------------------------------------------------------
      // Node file: Node IsTypeM
      fprintf(nodefile, "SCALARS node_is_Type_M int\n");
      fprintf(nodefile, "LOOKUP_TABLE default\n");
      nodepos = firstnodepos;
      for (uint32_t nodenum = 0;  nodenum < numnodes; nodenum++, nodepos++) {
        fprintf(nodefile, "%d ", (int)((*nodepos)->IsTypeM()));
      }
      fprintf(nodefile, "\n");
      // ----------------------------------------------------------

      dbecho(1, str(boost::format("DataSet::WriteVTKFiles(): Wrote node file %s with %d nodes\n")% filename %(lastnode-firstnode+1)).c_str());

      firstnode = lastnode + 1;
      fclose(nodefile);
    }

    // ============================================================
    // Write out the VisIt file declaring the nodefiles as blocks
    string visitnodefilename = str(boost::format("%s/%s-nodes.visit")%
                                   mOutputDir%mOutputBasename);
    FILE *visitnodefile = fopen(visitnodefilename.c_str(), "w");
    fprintf(visitnodefile, "!NBLOCKS %d\n", (int)nodefiles.size());
    for (uint32_t fileno = 0; fileno < nodefiles.size(); fileno++) {
      fprintf(visitnodefile, "%s\n", Basename(nodefilenames[fileno]).c_str());
    }
    fclose(visitnodefile);
    dbecho(1, str(boost::format("DataSet::WriteVTKFiles(): Wrote visit file %s\n")% visitnodefilename).c_str());


    return;
  }

  //===========================================================================
  // Write a file containing all tagged nodes with their tag values.
  void DataSet::WriteTagFile(void) {
    // First, create a fast searchable map for lookups, as our global map is ordered by global Node Index, which is not necessarily in file order any more at this point.

    string tagfilename = mOutputDir + "/" + mOutputBasename + "-tagged.data";
    dbecho(1, str(boost::format("DataSet::WriteTagFile: Writing node tags to file %s... ")% tagfilename).c_str());


    uint32_t nodenum = 0;
    uint32_t lineno = 1;
    char linebuf[2048]="";
    STARTPROGRESS();

    try {
      ofstream tagfile (tagfilename.c_str());
      if (!tagfile.good()){
        throw string("Error: cannot open tagfile for writing: ")+ tagfilename;
      }

      tagfile << "# paraDIS dump file with added tags, generated from datafile " << mDataFilename << " on " << timestamp() << " by " << GetLibraryVersionString("WriteTagFile") << endl;


      ifstream datafile(mDataFilename.c_str());
      // =================================================
      /* Read in old header and copy to new file verbatim */
      while (datafile.good() && !strstr(linebuf, "Secondary lines:")) {
        datafile.getline(linebuf, 2048);
        tagfile << linebuf << endl;
        ++lineno;
      }

      if (mFileVersion > 0 && mFileVersion < 4) {
        datafile.getline(linebuf, 2048); //should be "#"
        ++lineno;
        if (!strstr(linebuf, "#")){
          dbprintf(0, "ERROR: Expected line:\n#\n but got:\n%s\n", linebuf);
          throw string("Error in file format");
        }
        tagfile << linebuf << endl;
      }

      // the nodes in the file should correspond exactly to our Node list.  But let's be cautious the first time through and check it out.
      while (datafile.good() && nodenum++ < mTotalDumpNodes) {
        UPDATEPROGRESS(nodenum, mTotalDumpNodes, "WriteTagFile: creating new tagged datafile");
        CopyNodeFromFile(lineno, datafile, tagfile);
      }
      COMPLETEPROGRESS(mTotalDumpNodes, "WriteTagFile: creating new tagged datafile");
    } catch (string err) {
      string outs = str(boost::format("Error in WriteTagFile while translating node %1% at line %2%: %3%\n") % nodenum % lineno %  err);
      dbprintf(0, outs.c_str());
      throw outs;
    }
    dbecho(1, "Successfully wrote tagged data file\n" );
    return;
  }


  //===========================================================================
  // Print out all MetaArms in a simple format in a file for analysis, just for Meijie
  void DataSet::PrintMetaArmFile(void) {
    string metaArmFile = mOutputDir + "/" + mOutputBasename + ".metaarms";
    dbecho(0, str(boost::format("Writing metaarms to metaarm file %s... ")% metaArmFile).c_str());


    FILE *armfile = fopen (metaArmFile.c_str(), "w");
    if (!armfile) {
      cerr << "ERROR:  Cannot open output file to write out arms" << endl;
      return;
    }
    fprintf(armfile, "\n%s\n", doctext.c_str());
    fflush(armfile);

    fprintf(armfile, "\n\n%s\n\n\n\n%s\n\n",
            MetaArmSummary().c_str(),
            MonsterNodeSummary().c_str());


    fprintf(armfile, "\n\n");
    fprintf(armfile, "METAARM DETAILED STATISTICS\n");
    fprintf(armfile, " (for more, use the -debugfiles option and look in MetaArms-debug.txt )\n");
    fprintf(armfile, "=========================================================================\n");

    fprintf(armfile, "%-8s%-5s%-12s%-12s%-10s%-12s%-12s%-12s%-12s\n%-10s%-12s%-12s%-12s%-12s%-12s%s\n",
            "ID", "Type", "Length", "EP-Dist",
            "EP1-Type", "EP1-ID",   "EP1-X", "EP1-Y", "EP1-Z",
            "EP2-Type", "EP2-ID",   "EP2-X", "EP2-Y", "EP2-Z",
            "BurgersType","(NumArms):<Arm List>");
    uint32_t armnum = 0;
    for (vector<boost::shared_ptr<MetaArm> >::iterator pos = mMetaArms.begin(); pos != mMetaArms.end(); ++pos) {
      int numtermnodes = (*pos)->mTerminalNodes.size();
      int numtermarms = (*pos)->mTerminalArms.size();
      double eplength = 0.0;
      string ids[2]; ids[1] = "--";
      float loc[2][3]={{0}};
      int ntypes[2]={0};
      if (numtermarms < 1 || numtermarms > 2) {
        dbprintf(0,  "WARNING: arm # %d has %d terminal arms\n", armnum, numtermarms);
        dbprintf(0,  (*pos)->Stringify(0).c_str());
      }
      if (numtermnodes < 1 || numtermnodes > 2) {
        dbprintf(0,  "WARNING: arm # %d has %d terminal nodes\n", armnum, numtermnodes);
        dbprintf(0,  (*pos)->Stringify(0).c_str());
      } else {
        uint32_t i = numtermnodes, numarms = (*pos)->mAllArms.size();
        while (i--) {
          (*pos)->mTerminalNodes[i]->GetLocation(loc[i]);
          ntypes[i] = (*pos)->mTerminalNodes[i]->GetNodeType();
          ids[i]  = str(boost::format("(%1%,%2%)")
                        % (*pos)->mTerminalNodes[i]->GetNodeSimulationDomain()
                        % (*pos)->mTerminalNodes[i]->GetNodeSimulationID());
        }
        if (numtermnodes == 2) {
          eplength = (*pos)->mTerminalNodes[0]->Distance(*( (*pos)->mTerminalNodes[1]), true);
        }
        string burgstring =   BurgTypeToName((*pos)->mTerminalArms[0]->GetBurgersType());
        int matype = (*pos)->mMetaArmType;
        fprintf(armfile, "%-8d%-5d%-12.3f%-12.3f%-10d%-12s%-12.3f%-12.3f%-12.3f\n%-10d%-12s%-12.3f%-12.3f%-12.3f%-12s(%d):< ",
                armnum, matype, (*pos)->mLength, eplength,
                ntypes[0], ids[0].c_str(), loc[0][0],loc[0][1],loc[0][2],
                ntypes[1], ids[1].c_str(), loc[1][0],loc[1][1],loc[1][2],
                burgstring.c_str(), numarms);
        if (!numarms) {
          fprintf(armfile, "NONE >\n");
        }
        else {
          i = 0;
          while (i<numarms-1) {
            fprintf(armfile, "%d, ", (*pos)->mAllArms[i]->mArmID);
            i++;
          }
          fprintf(armfile, "%d >\n", (*pos)->mAllArms[i]->mArmID);
        }
      }
      fflush(armfile);
      ++armnum;
    }

    dbecho(1, "Successfully wrote meta arms\n");
    return ;

  }

  //===========================================================================
  // Print out all arms in a simple format in a file for analysis, just for Meijie
  void DataSet::PrintArmFile(char *altname) {

    string armfilename = mOutputDir + "/" + mOutputBasename + ".arms";
    if (altname) {
      armfilename = mOutputDir + "/" + mOutputBasename + "-" + altname + ".arms";
    }
    dbecho(0, str(boost::format("Writing arms to arm file %s... ") % armfilename).c_str());

    FILE *armfile = fopen (armfilename.c_str(), "w");
    if (!armfile) {
      cerr << "ERROR:  Cannot open output file to write out arms" << endl;
      return;
    }

    fprintf(armfile, "This file is a printout of all arms.\n");
    fprintf(armfile, "DISCUSSION: \n%s\n", doctext.c_str()); // includes key for this file

    fprintf(armfile, "-----------------------------------------------------\n");

    PrintArmStats(armfile);

    fprintf(armfile,
            str(boost::format("%=12s%=6s%=6s%=15s%=15s\n")
                % "Arm-ID" % "Type" % "Burg" % "Length" % "EP-Distance").c_str());
    fprintf(armfile,
            str(boost::format("%10s%=10s%=10s%=10s%=15s%=15s%=15s\n")
                % "**" % "EP1-ID" % "EP1-Type" % "EP1-Nbrs"
                % "EP1-X" % "EP1-Y" % "EP1-Z").c_str());
    fprintf(armfile,
            str(boost::format("%10s%=10s%=10s%=10s%=15s%=15s%=15s\n")
                % "**" % "EP2-ID" % "EP2-Type" % "EP2-Nbrs"
                % "EP2-X" % "EP2-Y" % "EP2-Z").c_str());
    vector<Arm*>::iterator pos = Arm::mArms.begin(), endpos = Arm::mArms.end();
    uint32_t armnum = 0;
    while (pos != endpos) {
      if ((*pos)->mArmType == ARM_EMPTY) {
        ++pos;
        continue;
      }
      int numtermnodes = (*pos)->mTerminalNodes.size(), numNeighbors[2] = {0};
      double eplength = 0.0;
      float loc[2][3]={{0}};
      char ntypes[2]={'-','-'};
      string ids[2];

      if (numtermnodes == 2) {
        eplength = (*pos)->mTerminalNodes[0]->Distance(*( (*pos)->mTerminalNodes[1]), true);
      }
      int i = numtermnodes;
      while (i--) {
        (*pos)->mTerminalNodes[i]->GetLocation(loc[i]);
        ntypes[i] = (*pos)->mTerminalNodes[i]->IsTypeM()? 'M':'N';
        numNeighbors[i] = (*pos)->mTerminalNodes[i]->mNeighborSegments.size();
        ids[i]  = str(boost::format("(%1%,%2%)")
                      % (*pos)->mTerminalNodes[i]->GetNodeSimulationDomain()
                      % (*pos)->mTerminalNodes[i]->GetNodeSimulationID());
      }

      fprintf(armfile,
              str(boost::format( "%=12d%=6d%=6d%=15f%=15f\n")
                  % (*pos)->mArmID % (*pos)->mArmType % (*pos)->GetBurgersType() %
                  (*pos)->mArmLength % eplength).c_str());
      fprintf(armfile,
              str(boost::format( "%10s%=10s%=10c%=10d%=15f%=15f%=15f\n")
                  % "**" % ids[0] % ntypes[0] % numNeighbors[0]
                  % loc[0][0] % loc[0][1] % loc[0][2]).c_str());
      fprintf(armfile,
              str(boost::format( "%10s%=10s%=10c%=10d%=15f%=15f%=15f\n")
                  % "**" % ids[1] % ntypes[1] % numNeighbors[1]
                  % loc[1][0] % loc[1][1] % loc[1][2]).c_str());


      ++armnum;
      ++pos;
    }
    int s = Arm::mArms.size();
    fprintf(armfile, "Size of Arm::mArms: %d\n",  s);
    fprintf(armfile, "Total printed non-EMPTY arms: %d\n",  armnum);
    dbecho(0, "Wrote arms file \n");
    return;
  }

  //===========================================================================
  void DataSet::ClassifyArms(void) {
    dbprintf(2, "ClassifyArms starting...\n");
    STARTPROGRESS()   ;
    int armNum=0, totalArms = Arm::mArms.size();
    vector<Arm*>::iterator armpos = Arm::mArms.begin(), armend = Arm::mArms.end();
    while (armpos != armend) {
      (*armpos)->Classify();
      ++armpos; ++armNum;
      UPDATEPROGRESS(armNum, totalArms, "Classify arms... ");
    }
    COMPLETEPROGRESS(totalArms, "Classify arms... ");

    return;
  }

  //===========================================================================
  double DataSet::ComputeArmLengths(void) {
    dbprintf(2, "ComputeArmLengths starting...\n");
    STARTPROGRESS()   ;
    double totalLength = 0;
    int armNum=0, totalArms = Arm::mArms.size();
    vector<Arm*>::iterator armpos = Arm::mArms.begin(), armend = Arm::mArms.end();
    while (armpos != armend) {
      //totalLength += (*armpos)->ComputeLength();
      totalLength += (*armpos)->mArmLength;
#ifdef DEBUG_ARMLENGTHS
      if (totalLength != (*armpos)->ComputeLength()) {
        dbprintf(0, "ERROR: DataSet::ComputeArmLengths(arm %d): totalLength != (*armpos)->ComputeLength()\n", (*armpos)->mArmID);
      }
#endif
      ++armpos; ++armNum;
      UPDATEPROGRESS(armNum, totalArms, "Compute arm lengths... ");
    }
    COMPLETEPROGRESS(totalArms, "Compute arm lengths... ");
    return totalLength;
  }


  //==========================================================================
  /*!
    To be used with for_each() to delete all nodes in the container
  */
  void DeleteNode(Node *node) {
    delete node;
  }

  //===========================================================================
  void DataSet::ComputeSubspace(void){
    dbprintf(2, "starting ComputeSubspace\n");
    mSubspaceMax = mDataMax;
    mSubspaceMin = mDataMin;
    rclib::Point<float> chunksize = mDataSize;
    if (mNumProcs > 1) {
      // 2) return;

      /*!
        Break space up into chunks
      */
      int numsplits = 0;
      int numpieces = 1;
      int current_dimension = 0; // X, Y or Z
      int  numchunks[3] = {1, 1, 1};
      while (numpieces < mNumProcs) {
        ++numsplits;
        numpieces *= 2;
        chunksize[current_dimension] /= 2.0;
        numchunks[current_dimension] ++;
        current_dimension = (current_dimension + 1)%3;
      }


      /*!
        Now exactly locate our chunk bounds
      */
      int leftover_pieces = numpieces % mNumProcs;
      int currentproc = 0;
      rclib::Point<float> currentMin = mSubspaceMin;
      int i,j,k;
      bool keepGoing = true;
      for (k = 0, currentMin[0] = mSubspaceMin[0];
           k < numchunks[2] && keepGoing;
           k++, currentMin[2] += chunksize[2]){
        for (j = 0, currentMin[1] = mSubspaceMin[1];
             j < numchunks[1] && keepGoing;
             j++, currentMin[1] += chunksize[1]){
          for (i = 0, currentMin[0] = mSubspaceMin[0];
               i < numchunks[0] && keepGoing;
               i++,  currentMin[0] += chunksize[0]){
            if (currentproc < mProcNum) {
              if (currentproc < leftover_pieces) {
                /*!
                  The leftover pieces get given to the first procs
                */
                currentMin[0] += chunksize[0];
                i++; // it's ok, i is always a multiple of two
              }
            } else {
              mSubspaceMin = currentMin;
              mSubspaceMax = mSubspaceMin + chunksize;
              if (currentproc < leftover_pieces) {
                mSubspaceMax[0] += chunksize[0];
              }
              keepGoing = false;
            }
            ++currentproc;
          }
        }
      }

      dbprintf(2, "done with ComputeSubspace\n");
    }

    dbprintf(2, "Computed subspace min = %s and max = %s\n", mSubspaceMin.Stringify().c_str(), mSubspaceMax.Stringify().c_str());

    return;
  }


  //=========================================================================
  void DataSet::ComputeNodeTypes(void) {
    for (vector<Node*>::iterator pos = Node::mNodeVector.begin(); pos !=  Node::mNodeVector.end(); pos++) {
      (*pos)->ComputeNodeType(mDataType == PARADIS_DATATYPE_BCC);
    }
  }

  //===========================================================================
  void DataSet::FindMetaArms(void){
    for (vector<Arm*>::iterator currentArm = Arm::mArms.begin(); currentArm != Arm::mArms.end(); ++currentArm) {
      (*currentArm)->mSeen = false;
    }

    uint32_t numMetaArms = 0, numArms = 0, totalArms = Arm::mArms.size();
    STARTPROGRESS();
    dbprintf(4, "FindMetaArms: %s\n", datestring());
    for (vector<Arm*>::iterator currentArm = Arm::mArms.begin(); currentArm != Arm::mArms.end(); ++currentArm, ++numArms) {
      if ( ! (*currentArm)->mSeen  &&
           ! (*currentArm)->isTypeUnknown() &&
           (*currentArm)->mArmType != ARM_EMPTY
           ) {
        boost::shared_ptr<MetaArm> metaArmSPtr = boost::make_shared<MetaArm>();
        metaArmSPtr->mMetaArmID = mMetaArms.size();
        metaArmSPtr->FindEndpoints(*currentArm);
        if (metaArmSPtr->mMetaArmType != METAARM_UNKNOWN) {
          dbprintf(4, "Adding meta arm %d with seed %d, terminal arms <", mMetaArms.size(), (*currentArm)->mArmID);
          int numterms = metaArmSPtr->mTerminalArms.size();
          int tnum = 0;
          for ( tnum = 0; tnum < numterms; tnum++) {
            dbprintf(5, "%d",metaArmSPtr->mTerminalArms[tnum]->mArmID);
            if (tnum < numterms-1) {
              dbprintf(5, ", ");
            }
          }
          if (!tnum) {
            dbprintf(5, "NONE");
          }

          dbprintf(4, ">, and arms <");
          int nummaarms = metaArmSPtr->mAllArms.size();
          int maarm = 0;
          for ( maarm = 0; maarm < nummaarms; maarm++) {
            dbprintf(5, "%d",metaArmSPtr->mAllArms[maarm]->mArmID);
            if (maarm < nummaarms-1) {
              dbprintf(5, ", ");
            }
          }
          if (!maarm) {
            dbprintf(5, "NONE");
          }
          dbprintf(5, ">\n");
          metaArmSPtr->mMetaArmID = mMetaArms.size();
          mMetaArms.push_back(metaArmSPtr);
          numMetaArms++;
        }
      } else {
        dbprintf(4, "Skipping currentArm %d (type %d) as it was either seen (%d) and was type 111 or an unknown arm, or else FindEndpoints could not handle it.\n", (*currentArm)->mArmID, (*currentArm)->mArmType, (*currentArm)->mSeen);
      }
      UPDATEPROGRESS(numArms, totalArms, str(boost::format("FindMetaArms: %1% MetaArms created")%numMetaArms));
    }
    COMPLETEPROGRESS(totalArms, str(boost::format("FindMetaArms: %1% MetaArms created")%numMetaArms));

    for (vector<Arm*>::iterator currentArm = Arm::mArms.begin(); currentArm != Arm::mArms.end(); ++currentArm) {
	  paradis_assert((*currentArm)->mSeen || (*currentArm)->mArmType == ARM_EMPTY);
      (*currentArm)->mSeen = false;
    }

    return;
  }

  //===========================================================================
  /*!
	Compute the BFS distance of all segments from mInitialLightTheFuseArmSegment.
  */
  void DataSet::ComputeLightTheFuseSegmentDistances(void) {
	STARTPROGRESS();
	dbecho(0, "ComputeLightTheFuseSegmentDistances() called...\n");
	ArmSegment *firstSegment = ArmSegment::mInitialLightTheFuseArmSegment;
	if (!firstSegment) {
	  dbecho(0, "ComputeLightTheFuseSegmentDistances() skipping computation...\n");
	  return;
	}
	vector<ArmSegment*> SegmentLayers[2]; // dual-buffered traveling wavefront of distances during BFS
	int currentLayer = 0;
	SegmentLayers[0].push_back(firstSegment);
	uint32_t currentDistance = 1;
	uint32_t segnum = 0, numsegs = ArmSegment::mArmSegments.size();

	while (SegmentLayers[currentLayer].size()) {
	  UPDATEPROGRESS(segnum, numsegs, str(boost::format("Computing light the fuse distances on layer %d.")%currentDistance));
	  SegmentLayers[1-currentLayer].clear();
	  for (vector<ArmSegment*>::iterator segpos = SegmentLayers[currentLayer].begin();
		   segpos != SegmentLayers[currentLayer].end(); segpos++) {
		ArmSegment *seg = *segpos;
		seg->mLightTheFuseDistance = currentDistance;
		dbprintf(5, str(boost::format("ComputeLightTheFuseSegmentDistances() set segment %1% fuse distance to %2%\n") % (seg->GetID()) % currentDistance).c_str());
		for (int i = 0; i<2; i++) {
		  Node *ep = seg->mEndpoints[i];
		  int numNeighborSegs = ep->mNeighborSegments.size();
		  for (int n = 0 ; n < numNeighborSegs; n++) {
			ArmSegment *neighbor = ep->mNeighborSegments[n];
			if (neighbor && !neighbor->mLightTheFuseDistance) {
			  SegmentLayers[1-currentLayer].push_back(neighbor);
			  segnum++;
			}
		  } //
		}// end current endpoint
	  }// end current layer
	  currentLayer = 1-currentLayer; //toggle
	  currentDistance++;
	} // end while current layer has segments;
	ArmSegment::mMaxLightTheFuseDistance = currentDistance;
	COMPLETEPROGRESS(numsegs, "Computing light the fuse distances.");
	return;
  }

  //===========================================================================
  bool DataSet::Test(void) {
    dbprintf(0, "DataSet::Test(): called\n");

    vector<uint32_t>segsperarm(Arm::mArms.size(), 0), segspermetaarm(mMetaArms.size(), 0);

    //================================================================
    dbprintf(0, "DataSet::Test(): enumerating segsperarm\n");
    for (map<uint32_t, ArmSegment*>::iterator segpos = ArmSegment::mArmSegments.begin(); segpos != ArmSegment::mArmSegments.end(); ++segpos) {
      ArmSegment * seg = segpos->second;
      if (!seg->mParentArm) {
        dbprintf(0, "DataSet::Test(): seg %d has NULL parent \n", seg->mSegmentIndex);
        return false;
      }
      if (!seg->mParentArm->GetParentMetaArm()) {
        dbprintf(0, "DataSet::Test(): seg %d has NULL parent metaarm \n", seg->mSegmentIndex);
        return false;
      }
      segsperarm[seg->mParentArm->mArmID]++;
      segspermetaarm[seg->mParentArm->GetParentMetaArm()->GetMetaArmID()]++;
    }


    //================================================================
    dbprintf(0, "DataSet::Test(): checking arms \n");
    bool success = true;
    uint32_t numArms = 0, numsegs = 0, numnodes = 0;
    for (vector<Arm*>::iterator arm = Arm::mArms.begin(); arm != Arm::mArms.end(); ++arm, ++numArms) {
      numsegs += (*arm)->mSegments.size();
      if ((*arm)->mSegments.size() != segsperarm[numArms]) {
        dbprintf(0, "DataSet::Test(): numsegs %d != segsperarm[%d] (%d)\n", (*arm)->mSegments.size(), numArms, segsperarm[numArms]);
        int segnum = 0;
        if ((*arm)->mSegments.size() < 5){
          dbprintf(0, "Searching for missing segment ...\n");
          for (deque<ArmSegment*>::iterator seg = (*arm)->mSegments.begin(); seg != (*arm)->mSegments.end(); seg++, ++segnum) {
            dbprintf(0, "armseg %d: %s\n", segnum, (*seg)->Stringify(0).c_str());
          }
          for (map<uint32_t, ArmSegment*>::iterator seg = ArmSegment::mArmSegments.begin(); seg != ArmSegment::mArmSegments.end(); ++seg) {
            if (seg->second->mParentArm == *arm) {
              deque<ArmSegment*>::iterator pos = find((*arm)->mSegments.begin(), (*arm)->mSegments.end(), seg->second);
              if (pos == (*arm)->mSegments.end()) {
                dbprintf(0, "Segment not in arm but in data set: %s\n", seg->second->Stringify(0).c_str());
              }
            }
          }
        }

        success = false;
      }


      for (deque<Node *>::const_iterator node = (*arm)->mNodes.begin(); node != (*arm)->mNodes.end(); node++) {
        if ((*node) && !(*node)->mSeen) {
          numnodes++;
          (*node)->mSeen = true;
        }
      }
    }
    if (!success) return false;

    if (numsegs != ArmSegment::mArmSegments.size()) {
      dbprintf(0, "DataSet::Test(): numsegs in Arms %d != ArmSegment::mArmSegments.size() %d \n", numsegs, ArmSegment::mArmSegments.size());
      return false;
    }

    //================================================================
    /*!
      Checking metaarm segments counts is wrong -- for VisIt, a 'MetaArm' always has only a single segment, connecting its endpoints.  The entire metaarm is returned only if it is a LOOP.

      dbprintf(0, "DataSet::Test(): checking meta-arms \n");
      uint32_t manum = 0;
      numsegs = 0;
      for (vector<boost::shared_ptr<MetaArm> >::iterator metaarm = mMetaArms.begin(); metaarm != mMetaArms.end(); ++metaarm, ++manum) {
      uint32_t masegs = (*metaarm)->GetNumSegments(false);
      uint32_t shouldbe = segspermetaarm[(*metaarm)->GetMetaArmID()];
      if (masegs != shouldbe) {
      dbprintf(0, "DataSet::Test(): numsegs %d != segspermetaarm[%d] %d \n", masegs, (*metaarm)->GetMetaArmID(), shouldbe);
      return false;
      }
      numsegs += masegs;
      }
      if (numsegs != ArmSegment::mNumArmSegments) {
      dbprintf(0, "DataSet::Test(): numsegs in meta MetaArms %d != ArmSegment::mArmSegments.size() %d \n", numsegs, ArmSegment::mArmSegments.size());
      return false;
      }
    */

    //================================================================
    dbprintf(0, "DataSet::Test(): checking arm segments \n");
    if (numnodes != Node::mNodes.size()) {
      dbprintf(0, "DataSet::Test(): numnodes %d != Node::mNodes.size() %d.  Searching for extra node...\n", numnodes, Node::mNodes.size());
      for (vector<Node*>::iterator node = Node::mNodeVector.begin(); node != Node::mNodeVector.end(); ++node) {
        if (!(*node)->mSeen) {
          dbprintf(0, "DataSet::Test(): unseen node: %s\n", (*node)->Stringify(0, false).c_str());
        }
      }

      return false;
    }

    dbprintf(0, "\n============================================\n");
    dbprintf(0, "DataSet::Test(): All tests PASSED.\n");
    dbprintf(0, "============================================\n\n");
    return true;
  }

  //===========================================================================
  void DataSet::ReadData(std::string filename, bool createVectors){
    dbprintf(1, "ReadData called with debug level %d\n", dbg_isverbose());
    try {
      if (filename != "") {
        mDataFilename = filename;
      }
      int dblevelSave = dbg_isverbose();
      dbprintf(2, "mDataFilename is %s\n",  mDataFilename.c_str());
      if (mDataFilename == "") throw string("Empty filename in ReadData");

      ReadBounds();

      if (mSubspaceMin == mSubspaceMax && mSubspaceMax == rclib::Point<float>(0.0)) {
        /*!
          if user hasn't set min and max, then figure out our spatial bounds.
        */
        ComputeSubspace();
      }

#ifdef DEBUG_SUBSPACES
      /*!
        Testing code generates bounds for lots of processor configs and prints them out to make sure that subspaces are being correctly computed.
      */
      ReadBounds();
      mNumProcs = 8;
      while (mNumProcs) {
        cout << endl << "*************************************************" << endl;
        cout << "mNumProcs = " << mNumProcs << endl;
        mProcNum = mNumProcs;
        while (mProcNum--) {
          cout << "******************" << endl;
          cout << "mProcNum = " << mProcNum << endl;
          ComputeSubspace();
          cout << "mSubspaceMin: " << mSubspaceMin.Stringify(0) << endl;
          cout << "mSubspaceMax: " << mSubspaceMax.Stringify(0) << endl;
        }
        mNumProcs --;
      }
#endif

      /*!
        Announce to the nodes what their bounds are for wrapping
      */
      Node::SetBounds(mSubspaceMin, mSubspaceMax);

      if (0 && dblevelSave > 2)
        SetVerbosity(2);

      CreateNodesAndArmSegments();

	  WrapBoundarySegments();

      SetVerbosity(dblevelSave);

      BuildArms();

      if (mDoStats && mDoDebugOutput) {
        PrintArmFile((char *)"debug-before-decomp");
      }

      Node::PrintAllNodeTraces("before-decomposition");

      if (mDataType !=  PARADIS_DATATYPE_HCP) {
        DecomposeArms();
      }
      // return; OK
      ComputeNodeTypes();
      // return;  bad
      ClassifyArms();

#if INSANE_DEBUG
      if (mDoDebugOutput) {
        DebugPrintNodes("NodesBeforeDeletion");
      }
#endif


      if (mDataType !=  PARADIS_DATATYPE_HCP) {
        FindMetaArms();
      }

      TagNodes(); // for Meijie

	  if (mDoPovRayFuse) {
		ComputeLightTheFuseSegmentDistances();
	  }

	  if (mDoDebugOutput) {
		DebugPrintArms();
		DebugPrintNodes();
		DebugPrintMetaArms();
	  }

    } catch (string err) {
      cerr << "An exception occurred" << endl;
      throw string("Error in DataSet::ReadData reading data from file ")+mDataFilename+":\n" + err;
    }
    if (mDoStats) {
      if (mDataType !=  PARADIS_DATATYPE_HCP) {
        PrintMetaArmFile();
      }
      PrintArmFile();
    }

    if (mDoTagFile) {
      WriteTagFile();
    }

    if (mDoSummaryFile) {
      WriteSummaryFile();
    }

    if (mDoVTKFile) {
      WriteVTKFiles();
    }

    if (mDoJSONFiles) {
      WriteJson();
    }

    if (mDoPovRayFiles) {
      WritePov();
    }


    dbprintf(1, "ReadData complete\n");
    return;
  }




} // end namespace paraDIS


