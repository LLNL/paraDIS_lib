/*   Written by Richard David Cook
     at Lawrence Livermore National Laboratory
     Contact:  wealthychef@gmail.com
     See license.txt for information about usage.
     Spoiler alert:  it's GNU opensource.
*/


/*!
  \file paradis.h

  \brief data structures and API for libparadis

      Definition, needed to find a node in a std::set<Node>:  node A > node B iff ( domain A > domain B ) || ( domain A == domain B && node ID A > node ID B ).  segment A > segment B iff segA.nodes[0] > segB.nodes[0].


   ------------------------------------------------------------------

      ALGORITHM OVERVIEW
      THIS IS A SERIAL ALGORITHM and parallelizing it would require multi-pass out of core techniques.

   -- create all relevant IB, OOB, and IOOB nodes and related arm segments
   -- create and classify arms
   -- decompose higher-energy arms into lower-energy arms until there is nothing but loops and type 111 arms in the data set.
   -- wrap any segments that cross periodic boundaries
   -- create meta-arms

   ------------------------------------------------------------------


*/
#ifndef PARADIS_H
#define PARADIS_H

// set this to 1 to re-enable linked loops code
#define LINKED_LOOPS 0


/* now for the API */
#include "boost/cstdint.hpp"
using boost::int16_t;
using boost::int16_t;
using boost::int32_t;
using boost::uint32_t;
#include "boost/shared_ptr.hpp"
#include "boost/format.hpp"

#include <stdio.h>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <ostream>
#include <math.h>

#include "Point.h"
#include "pathutil.h"
#include "stringutil.h" /* from RC_c_lib, is this a good idea? */
#include "debugutil.h" /* from RC_c_lib, now we're committed. */


#include "BurgersTypes.h"

#ifdef RC_CPP_VISIT_BUILD
#define dbprintf dbstream_printf
void dbstream_printf(int level, const char *fmt, ...);
#endif

std::string GetLibraryVersionString(const char *progname);
std::string GetLibraryVersionNumberString(void);

#define dbecho(level, msg...) fprintf(stderr, msg); dbprintf(level, msg)



/* ========================================  */

std::string INDENT(int i);

//=============================================================================
namespace paraDIS {
  extern std::string doctext;

  /*!
    A Node is a segment endpoint.
  */
  class Node {

	// NOTE PROTECTED CONSTRUCTORS
  protected:
    /*!
      ===========================================
      constructor
    */
    Node(int16_t domain, int32_t nodeID, vector<float>location)	 {
      init();
	  mDomainID = domain;
	  mNodeID = nodeID;
	  mLocation = location;
    }
    /*!
      ===========================================
      constructor
    */
    Node(int16_t domain, int32_t nodeID)	 {
      init();
	  mDomainID = domain;
	  mNodeID = nodeID;
    }
	/*!
      ===========================================
      Copy constructor, for decomposing/detaching operations
	  Removes neighbors, but otherwise duplicates the node.
	  When wrapping, use this but then change the location.
    */
    Node(const Node &other) {
      init();
	  uint32_t indexSave = mNodeIndex;
      *this = other;
	  mNodeIndex = indexSave;
	  mNeighborSegments.clear();
	  mNeighborArms.clear();
	  mOriginal = const_cast<Node*>(&other);
	  mIsDuplicate = true;
      return;
    }


  public:
    /*!
      ===========================================
      destructor
    */
    virtual ~Node()  {
	  printf ("Need to remove this node from the global list before destroying.  Not implemented yet.\n");
	  abort();
      return;
    }

	/*
	   Utility function for finding node in map
	*/
  	static uint32_t Hash(uint32_t domain, uint32_t nodeID) {
	  return 10*1000*domain + nodeID;
	}

	/*
	   GetOrCreateNode: used during file parsing when we have the location.
	   This happens in the first line for each node.
	   If the node is already in the global map, then set its location.
	   Else, create it with the given location.
	   When first seen, nodes are arbitrarily assigned location <0,0,0>.
	*/
	static Node *GetOrCreateNode(uint32_t domainID, uint32_t nodeID, vector<float> location) {
	  Node *node = NULL;
	  if (mNodes[Hash(domainID,nodeID)].size()) {
		node = mNodes[Hash(domainID,nodeID)][0];
		node->mLocation = location;
	  } else {
		node = new Node(domainID,nodeID,location);
		mNodes[Hash(domainID,nodeID)].push_back(node);
		mNodeVector.push_back(node);
		if (mNodeVector.size()-1 != node->mNodeIndex) {
		  dbprintf(0, "Error: node vector side does not match node index...\n");
		  abort();
		}
	  }
	  return node;
	}

	/*
	   GetOrCreateNode: used during file parsing when we do not have location.
	   This happens in the neighbor lines for each node.
	   If the node is already in the global map, then just return it.
	   Else, create it without setting location.
	*/
	static Node *GetOrCreateNode(uint32_t domainID, uint32_t nodeID) {
	  Node *node = NULL;
	  if (mNodes[Hash(domainID,nodeID)].size()) {
		node = mNodes[Hash(domainID,nodeID)][0];
	  }
	  else {
		node = new Node(domainID,nodeID);
		mNodes[Hash(domainID,nodeID)].push_back(node);
		mNodeVector.push_back(node);
	  }
	  return node;
	}


	/* For wrapping, detach/fuse, and other duplications: */
	static Node *DuplicateNode(Node *other) {
	  Node *node = new Node(*other);
	  mNodes[Hash(other->mDomainID, other->mNodeID)].push_back(node);
	  return node;
	}


   /*!
      ===========================================
      Clear all nodes
    */
    static void Clear(void) {
      for (map<uint32_t, vector<Node *> >::iterator nodepos = mNodes.begin(); nodepos != mNodes.end(); nodepos++) {
		for (int i = nodepos->second.size(); i; i--) {
		  delete nodepos->second[i];
		}
	  }
      mNodes.clear();
      mTraceFileBasename = "";
      mTraceNodes.clear();
      mNextNodeIndex = 0;
    }

    /*!
      ===========================================
      initializer
    */
    void init(void) {
	  mNodeIndex = mNextNodeIndex++; // "serial number"
 	  //  mLocation.resize(3,0); // do not do this!  Empty is meaningful
      mNodeType = 0;
      mIsLoopNode = false;
	  mWrapped = false;
 	  mIsDuplicate = false; // if true and location changed, we're wrapped
	  mOriginal = NULL; // the node we are a copy of
	  mSeen = false;
    }


    /*!
      Accessor function for C API
    */
    void GetLocation(float loc[3]) {
      for (int i=0; i<3; i++)
		loc[i] = mLocation[i];
	  return;
    }

    /*!
      Accessor function for C API
    */
	int GetNumNeighborSegments(void) { return mNeighborSegments.size();}

    /*!
      Accessor function for C API
    */
	int32_t GetNodeSimulationDomain(void) { return mDomainID; }

    /*!
      Accessor function for C API
    */
	int32_t GetNodeSimulationID(void) { return mNodeID; }

    /*!
      ===========================================
      Add the given arm to the list of arms to trace.
      See WriteTraceFiles()
    */
    static void TraceNode(int32_t nodeID){
      mTraceNodes.push_back(nodeID);
    }

    /*!
      ===========================================
      Give a name to distinguish tracefiles from files from other runs.
    */
    static void SetTraceFileBasename(string basename) {
      mTraceFileBasename = basename;
    }




    /*!
      Compute the distance to another node
    */
    double Distance(const Node &other, bool wrap=false) {
	  double dist[3] = {0}, sum=0;
	  int i=3; while (i--) {
        dist[i] = mLocation[i] - other.mLocation[i];
        if (wrap && fabs(dist[i]) > mBoundsSize[i]/2.0) {
          dist[i] = mBoundsSize[i] - fabs(dist[i]);
        }
        sum += dist[i]*dist[i];
      }
      return sqrt(sum);
    }
    /*!
      Accessor function.
      The node will attempt to determine its own type.
    */
    void ComputeNodeType(bool BCC);

   /*!
      Accessor function set the node type.
    */
    void SetNodeType(int16_t itype) {
      mNodeType = itype;
      return;
    }

    /*!
      Accessor function
    */
    int16_t GetNodeType(void) { return mNodeType; }


    /*!
      Query
    */
    bool IsTypeM(void) {
       return mNodeType < 0;
    }

    bool IsTypeN(void) {
       return mNodeType > 2;
    }

    bool IsLoopNode(void) {
      return mIsLoopNode;
    }

    void SetLoopNode(bool tf = true) {
      mIsLoopNode = tf;
    }

    /*!
      Convert Node to string
    */
    virtual std::string Stringify(int indent=0, bool shortform=true) const;



    /*!
      Handy wrapper
    */
    static void PrintAllNodeTraces(string stepname);

  /*
     For each node that we want to trace, we write one set of files out before any decompositions, then one for decomposition step of each arm containing this node.
   */
    void WriteTraceFiles(string stepname);


     /*!
      Assuming we have two neighbors or less, return the neighbor not passed to us
    */
    class ArmSegment *GetOtherNeighbor (const class ArmSegment* n) {
	  if (mNeighborSegments.size() != 2) {
        dbprintf(6, "Node::GetOtherNeighbor called but mNeighborSegments.size() != 2\n");
		return NULL;
      }
      if (mNeighborSegments[0] == n) return mNeighborSegments[1];
      if (mNeighborSegments[1] == n) return mNeighborSegments[0];
      return NULL;
    }


   /*!
      Accessor -- set the global bounds for every point
    */
    static void SetBounds(rclib::Point<float> &min, rclib::Point<float> &max) {
      mBoundsMin = min;
      mBoundsMax = max;
      mBoundsSize = max-min;
    }

    /*!
      Remove one instance of the neighbor segment if present, leaving duplicates.
    */
    void RemoveNeighborSegment(class ArmSegment *oldseg, bool doall = false) {
      if (doall) {
        mNeighborSegments.erase(remove(mNeighborSegments.begin(), mNeighborSegments.end(), oldseg), mNeighborSegments.end());
      } else {
        mNeighborSegments.erase(find(mNeighborSegments.begin(), mNeighborSegments.end(), oldseg));
     }
      //ComputeNodeType();
    }

   // used to remove an arm which has been decomposed
    void RemoveNeighborArm(struct Arm *neighbor, bool doall = false) {
	  if ( find(mNeighborArms.begin(), mNeighborArms.end(), neighbor) == mNeighborArms.end()) {
		return;
	  }
      if (doall) {
        mNeighborArms.erase(remove(mNeighborArms.begin(), mNeighborArms.end(), neighbor), mNeighborArms.end());
      } else {
        // assumes that the arm exists in mNeighborArms!
        mNeighborArms.erase(find(mNeighborArms.begin(), mNeighborArms.end(), neighbor));
      }
    }

    bool HasNeighbor(struct Arm *neighbor) {
      return (find(mNeighborArms.begin(), mNeighborArms.end(), neighbor) != mNeighborArms.end());
    }

    /*!
      Accessor function
    */
    const std::vector< int> GetNeighborArmIDs(void) const;


    /*!
      Identify arms which cross over this node for  DetachCrossArms();
      Broken out separately to enable query and debug output
    */
    vector<vector<Arm *> >IdentifyCrossArms(void);

    /*!
      Identify arms which cross over this node and glue them together.
      Simplifies decomposition of arms.
    */
    void DetachCrossArms(void);


    string GetNodeIDString(void) const {
	  return str(boost::format("(%1%, %2%)")%mDomainID%mNodeID);
	}


    /*!
      for debugging
    */
    bool mSeen;


    /*!
      Connectivity to our neighboring arms.
      Only used for terminal nodes.
    */
    std::vector<struct Arm *> mNeighborArms;

    /*!
      all fullnodes in the data set.  The map key is a hash of the domain and node ID for the node.  Wrapped nodes are found by looking at mWrappedDouble in the found node.
    */
    static std::map<uint32_t, vector<Node *> > mNodes;
	static std::vector<Node *> mNodeVector; // purely for the C interface

    /*!
      Static member to keep track of subspace bounds for checking if we are in bounds or not
    */
    static rclib::Point<float> mBoundsMin, mBoundsMax, mBoundsSize;

	/* From NodeID: */
	uint32_t mDomainID;
    uint64_t mNodeID;
	/* ------------ */

	/* NEW to handle wrapping, we are going to have a "wrapped double" */
	Node *mOriginal;
    bool mIsDuplicate, mWrapped;

	uint32_t mNodeIndex;
	static uint32_t mNextNodeIndex ;

	/*!
      Absolute location in global space
    */
    vector<float> mLocation;
    /*!
      Node Type is whether we are a butterfly, monster, or normal node (or a placeholder in a segment)
    */
    int16_t mNodeType;

    bool mIsLoopNode;

    /*!
      Connectivity to our neighboring nodes encapsulated in ArmSegments
    */
    std::vector< class ArmSegment *> mNeighborSegments;

    static string mTraceFileBasename;
    static vector<uint32_t> mTraceNodes;

    // statistics:
    std::vector<uint32_t> mNumMonsterNodes;

  }; /* end Node */


  // ==================================================================
  // ==================================================================
  /*!
    Arm segments contain neighbor relationships, encoded as pointers to nodes.
	They also contain burgers and arm-type information for later analysis.
  */
  class ArmSegment
    {
  public:
    ArmSegment(const ArmSegment &other){
      init();
      uint32_t saved = mSegmentIndex;
      *this = other;
      mSegmentIndex = saved;
	  mIsDuplicate = true;
    }

	/* This is called by ReadNodeFromFile() and WrapBoundarySegments() */
	ArmSegment(Node *ep0, Node *ep1, int burgersType) {
	  init();
	  // It turns out that *ep0 < *ep1 in terms of file order
	  mEndpoints[0] = ep0;
	  ep0->mNeighborSegments.push_back(this);
	  mEndpoints[1] = ep1;
	  ep1->mNeighborSegments.push_back(this);
	  mOriginalBurgersType = mBurgersType = burgersType;
	  return;
	}

    void init(void) {
      mScrewType = SCREW_UNDEFINED;
      mSegmentIndex = mNextSegmentIndex;
      mNextSegmentIndex++;
	  mParentArm = NULL;
	  mLightTheFuseDistance = 0;
      mOriginalBurgersType = mBurgersType = BCC_BURGERS_UNKNOWN;
      mSeen = false;
      int i=2; while (i--) {
        mEndpoints[i] = NULL;
      }
	  mIsDuplicate = false;
	  mWrapped = false;
    }
    /*!
      Destructor
    */
    ~ArmSegment() {
      mArmSegments.erase(mSegmentIndex); // mArmSegments[mSegmentIndex] = NULL;
      //mNumArmSegments--;
	  int i=2; while (i--) {
        if (mEndpoints[i]) {
          delete mEndpoints[i]; // memory leak fix
        }
      }
      return;
    }

    static void Clear(void) {

      for (map<uint32_t, ArmSegment *>::iterator pos = mArmSegments.begin(); pos != mArmSegments.end(); ++pos) {
        delete pos->second;
      }
      mArmSegments.clear();

      mSegLen = 0;
      mNumClassified = 0;
      mNumDecomposed = 0;
      mNumArmSegmentsMeasured = 0;
      mNextSegmentIndex = 0;
    }

    /*!
      Accessor function for C API
    */
    int32_t GetNodeIndex(int num) {
      return mEndpoints[num]->mNodeIndex;
    }


    int16_t GetBurgersType(void) const { return mBurgersType; }

    int16_t GetOriginalBurgersType(void) const { return mOriginalBurgersType; }

   /*!
      Return the distance between the endpoints
    */
    double GetLength(bool wrap=false) const {
      return mEndpoints[0]->Distance(*mEndpoints[1], wrap);
    }

   /*!
      Set the segment id to the next available global ID
    */
    void SetIndex(uint32_t id) {
      mSegmentIndex = id;
      return;
    }
    /*!
      accessor -- noop if not debug mode
    */
    uint32_t GetID(void) {
      return mSegmentIndex;
    }

     /*!
      Accessor for MN type
    */
    int16_t GetMNType(void) const;

     /* Get the arm ID for the parent of this segment */
    uint32_t GetArmID(void);

    /* Get the metaarm ID for the parent of this segment */
    uint32_t GetMetaArmID(void);

    /* Get the metaarm Type for the parent of this segment */
    uint16_t GetMetaArmType(void);


    /*!
      Replace one segment endpoint with the given new one.
	  Remove self from oldEP neighbors, and add to newEP neighbors.
    */
    void ReplaceEndpoint(Node *oldEP, Node *newEP) {
      if (mEndpoints[0] == oldEP) {
        mEndpoints[0] = newEP;
      } else if (mEndpoints[1] == oldEP) {
        mEndpoints[1] = newEP;
      } else {
        throw string("Error in ReplaceEndpoint -- there is no such endpoint");
      }
	  oldEP->RemoveNeighborSegment(this);
	  newEP->mNeighborSegments.push_back(this);
      return;
    }

   /*!
      Accessor function
    */
    void GetNodeIndices(uint32_t indices[2]) {
      indices[0] = mEndpoints[0]->mNodeIndex;
      indices[1] = mEndpoints[1]->mNodeIndex;
    }

    /*!
      convert ArmSegment to string
    */
    std::string Stringify(int indent=0) const;


	// specific to JSON files for blender ingestion
	std::string BlenderRotationString(void) const;

	/*!
      return a vector ep1 - ep0
     */
    vector<float> SegmentDirection(void) const;

    /*!
      Compute the mScrewType value
    */
    int16_t ComputeScrewType(void);

    int16_t GetScrewType(void) {
      return ComputeScrewType();
    }

    static void SetScrewToleranceAngle(double angle) {
      mScrewToleranceAngle = angle;
      mScrewToleranceCosine = fabs(cos(angle)); // this is more useful computationally
    }


 	// ========================
    bool HasEndpoint(Node *ep) {
      return (mEndpoints[0] == ep || mEndpoints[1] == ep);
    }

	/* Some segments connect a node at one edge of the subspace to a node outside the subspace.  I think these are ok to draw as-is.  But some segments connect a node to a node on the other side due to periodic boundary conditions.  This segment needs to be "wrapped".  For a "wrapped" segment, create a new "wrapped" node inside the subspace boundary by adding an entire boundary dimension as needed such that the distance to its neighbor is less than half the size of the bounds of the dataset.  Create a new "wrapper segment" connecting the original node to the new "wrapped" node.  This new wrapper segment will not contribute to any distance calculations; it's like a hyperwarp to the wrapped double node. Then change the original segment to connect from the wrapped node to its neighbor.
    */
    // bool Wrap(const rclib::Point<float> &dataSize);

    /*!
      Accessor function.
      \param epnum Must be 0 or 1
    */
    Node *GetEndpoint(int epnum) const { return mEndpoints[epnum]; }

    /*!
      Common Accessor operation: we have one node ID, but we're looking to see what the other end of the segment is.
    */
    Node *GetOtherEndpoint(const Node *node) const {
      int idx = 0;
      while (idx < 2) {
        if (mEndpoints[idx] == node  ) {
          break;
        }
        ++idx;
      }
      if (idx == 2) {
        throw string("Error in GetOtherEndpoint: cannot find node corresponding to ")+node->Stringify(0);

      }
      return mEndpoints[1-idx];
    }
    /*!
      purely for debugging
    */
    int32_t mSegmentIndex;


    /*!
      If this segment is a duplicate, that's potentially useful to know for visualization as there will be two or more superposed segments.
    */
    bool mWrapped, mIsDuplicate;


    static double mSegLen;
    static uint32_t mNumClassified, mNumBeforeDecomposition,
      mNumDecomposed, mNumArmSegmentsMeasured;
    static uint32_t mNextSegmentIndex;
	static ArmSegment *mInitialLightTheFuseArmSegment;
	static uint32_t mMaxLightTheFuseDistance;
	uint32_t mLightTheFuseDistance;

     /*!
      The burgers-type is defined above.
    */
    int16_t mBurgersType, mOriginalBurgersType;

     /*!
      The screw type is defined above and is derived from the burgers type.
    */
    int16_t mScrewType;
    static double mScrewToleranceAngle,  // Meaning: angle deviation inside which a segment is 'screw' or 'edge'
      mScrewToleranceCosine;// the computational twin derived from mScrewToleranceAngle and used in computations many times
    static double SQRT3; // sqrt(3), used many times
    /*!
      The MN_type of the segment is set by its parent arm.  See Arm struct for definitions, but it describes whether the segment is 200 or 111 and whether its parent arm has any monsters at either end.
    */
    //int16_t mMNType;

    /* The metaArm type that it belongs to.  */
    int16_t mMetaArmType;

    /*!
      Marker used for "once-through" operations like building arms that must look at every segment, but which will usually discover echo particular segment more than once.
    */
    bool mSeen;

	/* every segment has two endpoints */
    Node * mEndpoints[2];


    /*!
      The global list of valid arm segments
    */
    static std::map<uint32_t, ArmSegment *> mArmSegments;
    struct Arm *mParentArm;

  }; /* end ArmSegment */



  //==============================================
  /*!
    Arms are conceptually a list of segments, but all we need to store is the two (or one, for a cycle) terminal ArmSegments for the arm, and the two (or one) terminal Nodes.  This allows us to recreate the arm for drawing or analysis, but saves lots of memory. (Tradeoff of speed to get memory). The assumption is that traversing the arm will be fast enough and not done very often.  If it starts using lots of time, we can always store more information if it makes it faster later.
    Arms are used just for classifying nodes and segments and are not expected to be useful to the user of this library;
  */
  struct Arm {
    Arm() {
      this->init();
    }

    ~Arm() {
      mArms[mArmID] = NULL;
    }

    void init(void) {
      mArmType = ARM_UNKNOWN;
      mArmLength=0;
      mSeen=false;
      mSeenInMeta=false;
      mParentMetaArm=NULL;
      mNumNormalSegments = 0;
      mNumWrappedSegments = 0;
      mTerminalSegments.clear();
      mTerminalNodes.clear();
      mDecomposing = false;
      mExtendOrDetach = false;
      mArmID = mArms.size();
      mArms.push_back(this);

#if LINKED_LOOPS
      mPartOfLinkedLoop=false;
      mCheckedForLinkedLoop=false;
#endif
      //mTraceArms.clear();
      //mTraceDepth = 2;
    }

    // bookkeeping for e.g. timestep changes.
    static void Clear(void) {
      for (vector<Arm*>::iterator arm = mArms.begin(); arm != mArms.end(); arm++) {
        if (*arm) delete *arm;
      }
      mArms.clear();
      mTraceArms.clear();
      mTraceDepth = 2;
      mTraceFileBasename = "";
      mLongestLength = 0.0;
      mDecomposedLength = 0.0;
      mNumDecomposed.clear();
      mNumDestroyedInDetachment = 0;
      mTotalArmLengthBeforeDecomposition = 0.0;
      mTotalArmLengthAfterDecomposition = 0.0;
      mThreshold = -1;
    }

    /*!
      When one arm is gobbled up by another, the gobblee becomes ancestor to the gobbler
    */
    void MakeAncestor(Arm *sourceArm) {
      mAncestorArms.push_back(sourceArm->mArmID);
      for  (uint32_t a = 0; a < sourceArm->mAncestorArms.size(); a++) {
        mAncestorArms.push_back(sourceArm->mAncestorArms[a]);
      }
      return;
    }

    /*!
      Helper for DetachLoopFromNode and DetachAndFuse, does the detach part
    */
    void ReplaceTerminalNode(Node *node, Node *replacement);

    /*!
      During decomposition, cross arms are removed from terminal nodes.
      If the cross arm is a loop, then this is called to detach it.
    */
    void DetachLoopFromNode(Node *node);

     /*!
      During decomposition, cross arms are removed from terminal nodes.
      This actually detaches one such cross arm from a node, reusing the contents of other arm except the detached node.
    */
   void DetachAndFuse(Node *node, Arm *other);

    /*!
      A helper for ExtendByArm() function.
    */
    void ExtendBySegments(Arm *sourceArm, Node *sharedNode, bool reuseSegments);

    /*!
      A helper for Decompose() function.
    */
    void ExtendByArm(Arm *sourceArm, Node *sharedNode, bool reuseSegments);

	/* temporary helper to check mNodes and mSegments vs. old method */
	void printNodes(void) const;

	/* temporary helper to check mNodes and mSegments vs. old method */
	void printSegments(void) const;

	/* temporary helper to check mNodes and mSegments vs. old method */
	void CheckNodesAndSegments(void) const;

    /*!
      Merge with neighbor arms.
    */
    bool Decompose(int energyLevel);

   /*!
        create a string out of the arm
    */
    std::string Stringify(int indent, bool shortform=true, bool allnodes=false) const ;

    /*!
      Give the exact Burgers type of its segments.
      Return 0 if no terminal segments.
    */
    int16_t GetBurgersType(void) const {
      if (!mTerminalSegments.size() || !mTerminalSegments[0])  {
        return BCC_BURGERS_UNKNOWN;
      }
      return mTerminalSegments[0]->GetBurgersType();
    }

    /*!
      Give the original Burgers type of its segments.
      All segment original burgers type will be the same for any given arm.
      Return 0 if no terminal segments.
    */
    int16_t GetOriginalBurgersType(void) const {
      if (!mTerminalSegments.size() || !mTerminalSegments[0])  {
        return BCC_BURGERS_UNKNOWN;
      }
      return mTerminalSegments[0]->GetOriginalBurgersType();
    }

    /* Get the metaarm ID for the parent of this arm */
    uint32_t GetMetaArmID(void);

    /* Get the metaarm Type for the parent of this arm */
    uint16_t GetMetaArmType(void);

  #if LINKED_LOOPS
   /*!
      A linked loop is defined as:
      A) Two arms which have the same four-armed terminal nodes
      OR
      B) Three arms which all have the same three-armed terminal nodes.
    */
    void CheckForLinkedLoops(void);
#endif

 	// =========================================================
   /*!
      Return vectors with pointers to all nodes and segments in arm,
      computed in order from end to end.  In a loop, the startNode is
      repeated as first and last node.
	  wrapping is signified by a NULL node
    */
    void GetNodesAndSegments(Node *startNode, vector<Node*>*outnodes, vector<ArmSegment*>*outsegs) const;



	// =========================================================
	// Returns number of nodes, not counting "NULL" nodes for wrapping
    uint32_t GetNumNodes(void) {
      return mNumNormalSegments ? mNumNormalSegments + mNumWrappedSegments + 1: 0;
    }


 	// =========================================================
   string GetSegmentsAsString(Node *startNode = NULL) {
      string s;
      vector<ArmSegment *> segments;
	  GetNodesAndSegments(startNode, NULL, &segments);
      int seg = 0, numsegs = segments.size();
      s += intToString(numsegs) + string(" segments: \n");
      while (seg < numsegs) {
        s += (string("   ") + segments[seg]->Stringify(0) + "\n");
         ++ seg;
      }
      return s;
    }

     double ComputeLength(void);

  /*!
      Classify the arm as one of NN, MN or MM, combined with 200 or 111...
    */
    void Classify(void) ;

    bool isTypeMM(void) const {
      return mArmType == ARM_BCC_MM_111;
    }

    bool isTypeUnknown(void) const {
      return mArmType == ARM_UNKNOWN;
    }

    bool isType111(void) {
      return mTerminalSegments.size() && BurgTypeToBurgInfo(mTerminalSegments[0]->GetBurgersType()).energy == 1;
    }

    bool isHighEnergy(void) {
      return  mTerminalSegments.size() && BurgTypeToBurgInfo(mTerminalSegments[0]->GetBurgersType()).energy > 1;
    }

    Node *GetCommonNode(Arm *other) {
      uint32_t myNode = mTerminalNodes.size();
      while (myNode--) {
        uint32_t otherNode = other->mTerminalNodes.size();
        while (otherNode--) {
          if (mTerminalNodes[myNode] == other->mTerminalNodes[otherNode]) {
            return mTerminalNodes[myNode];
          }
        }
      }
      return NULL;
    }

    /*!
      Return number of neighbor arms, not including this arm, but including duplicates
    */
    uint32_t GetNumNeighborArms(void) {
      uint32_t num = 0;
      for  (uint32_t node = 0; node < mTerminalNodes.size(); node++) {
        for (uint32_t arm=0; arm < mTerminalNodes[node]->mNeighborArms.size(); arm++) {
          if (mTerminalNodes[node]->mNeighborArms[arm] != this) {
            num++;
          }
        }
      }
      return num;
    }

    /*!
      Return nth neighbor arm, not including this but including duplicates
    */
    Arm *GetNeighborArm (int num) {
      for  (uint32_t node = 0; node < mTerminalNodes.size(); node++) {
        for (uint32_t arm=0; arm < mTerminalNodes[node]->mNeighborArms.size(); arm++) {
          if (mTerminalNodes[node]->mNeighborArms[arm] != this) {
            if (!num)
              return mTerminalNodes[node]->mNeighborArms[arm];
            else
              num--;
          }
        }
      }
      return NULL;
   }

    /*!
      Return the sum of the length of all segments in the arm
    */
    double GetLength(void) const {
      return mArmLength;
    }

    uint16_t GetArmType(void) const { return mArmType; }

    struct MetaArm *GetParentMetaArm(void) const { return mParentMetaArm; }

    void SetParentMetaArm(struct MetaArm *ma) {
     mParentMetaArm = ma;
    }

   /*!
      This prints out an arm and its neighboring arms using BFS order
      to the given depth to a text file and a VTK file.
      File created: basename.txt, basename.vtk
    */
     void WriteTraceFiles(string stepname);

    /*!
      Helper function for Arm::WriteTraceFiles and Node::WriteTraceFiles
    */
    void FindBFSNeighbors(vector<Arm *> &arms, vector<uint32_t> &armdepths, vector<int> &action);

    /*!
      Add the given arm to the list of arms to trace.
      See WriteTraceFiles()
    */
    static void TraceArm(int32_t armID){
      mTraceArms.push_back(armID);
    }

    /*!
      Set the tracing depth for all traced arms.
      See WriteTraceFiles()
    */
    static void SetTraceDepth(uint32_t depth) {
      mTraceDepth = depth;
    }

    /*!
      Give a name to distinguish tracefiles from files from other runs.
    */
    static void SetTraceFileBasename(string basename) {
      mTraceFileBasename = basename;
    }

    /*!
      This is a necessary component to CheckForButterfly, broken out for readability in the code.
    */
    string StringifyExternalArms(int indent) const ;

    vector < ArmSegment *> mTerminalSegments; // All arms have two of these, even loops, unless there is only a single segment in the arm.
    vector <Node *> mTerminalNodes;  // All arms have two of these, except loops.

	// speed up at cost of memory
	std::deque<Node*> mNodes;
	std::deque<ArmSegment*> mSegments;

    int16_t mArmType;
    int16_t mMetaArmType; // of its parent if it exists
    double mArmLength;
    /*!
      This is useless to the user, only used for classification
    */
    static std::vector<Arm *> mArms;
    static double mThreshold; // shorter than this and an arm is "short"
    static double mDecomposedLength; // statistics
    static map<int, int32_t> mNumDecomposed; // statistics
    static int32_t mNumDestroyedInDetachment; // statistics
    static double mTotalArmLengthBeforeDecomposition,
      mTotalArmLengthAfterDecomposition;
    bool mDecomposing, mExtendOrDetach;
#if LINKED_LOOPS
    bool mPartOfLinkedLoop, mCheckedForLinkedLoop;
#endif
    static double mLongestLength; // for binning
    bool mSeen, mSeenInMeta; // for tracing MetaArms -- each arm need only be viewed once
   /*!
      number of non-wrapped segments and nodes in arm
    */
    uint32_t mNumNormalSegments, mNumNodes;
   /*!
      number of segments that got wrapped (affects node and segments counts)
    */
    uint16_t mNumWrappedSegments;
    /*!
      purely for debugging
    */
    int32_t mArmID;


    /*!
      An ancestor of this arm was one which was assimilated into this arm,
      during decomposition.  Useful for history tracing.
    */
    vector<int32_t> mAncestorArms;

    /*!
      To trace out arms in a text and vtk file, add them to this vector
    */
    static vector<int32_t> mTraceArms;

    /*!
      uniquify files from this run
    */
    static string mTraceFileBasename;

    /*!
      When tracing arms, how deep to BFS for neighbors?
      0 is no neighbors. 1 is immediate neighbors, etc.
    */
    static uint16_t mTraceDepth;

    private:
    struct MetaArm * mParentMetaArm;
  };



  //=============================================
  /*!
    MetaArm:  A collection of arms.  Theoretically should span from M to M, but this is not always the case yet.
  */
  struct MetaArm {
    MetaArm(): mLength(0.0),  mMetaArmType(METAARM_UNKNOWN),  mNumSegments(0) , mCombo(false) , mFound111(false)
    {return;}
    ~MetaArm() {}



    std::string Stringify(int indent);
    // vector<ArmSegment*> GetArmSegments(void);

    vector<Node* > GetNodes(void);

    vector<rclib::Point<float> > GetNodeLocations(bool wrapEndpoints);

    uint32_t GetNumSegments(bool wrapEndpoints) {
      if (!mNumSegments) {
        dbprintf(1, "WARNING: MetaArm::GetNumSegments(): mNumSegments is 0.  You should call GetNodeLocations() before GetNumSegments() to avoid a permormance penalty.\n");
        GetNodeLocations(wrapEndpoints);
      }
      return mNumSegments;
    }

    bool FindEndpoint(Arm *seed, Node *previous, Arm* candidate);
    void FindEndpoints(Arm *seedArm);

    // ======================
    /* Just to clean the code, I encapsulate this to make debugging easier */
    void AddArm(Arm *candidate ) {
      if (candidate->GetParentMetaArm() == this)  {
        dbprintf(0, "WARNING! Already added arm %d to this metaarm!\n", candidate->mArmID);
      }
      candidate->SetParentMetaArm(this);
      mLength += candidate->GetLength();
      mFoundArms.push_back(candidate);
      return;
    }
    // ======================
    inline void AddTerminalNode(Node *node) {
      uint16_t nodenum = mTerminalNodes.size();
      while (nodenum--) {
        if (mTerminalNodes[nodenum] == node) {
          dbprintf(4, "Warning: node not added as it is a duplicate.\n");
          return;
        }
      }
      mTerminalNodes.push_back(node);
      return;
    }
    // ======================
    inline void AddTerminalArm(Arm *arm) {
      uint16_t armnum = mTerminalArms.size();
      while (armnum--) {
        if (mTerminalArms[armnum] == arm) {
          dbprintf(4, "Warning: arm not added as it is a duplicate.\n");
        }
      }
      AddArm(arm);
      mTerminalArms.push_back(arm);
      return;
    }
    inline void CapturePath(bool doreverse) {
      dbprintf(4, "Capture Path %d called\n", (int)doreverse);
      if (doreverse) {
        reverse(mFoundArms.begin(), mFoundArms.end());
      }
      uint32_t i = 0;
      while (i < mFoundArms.size()) {
        dbprintf(4, "Capture Path: add arm %d\n", mFoundArms[i]->mArmID);
        i++;
      }
      mAllArms.insert(mAllArms.end(), mFoundArms.begin(), mFoundArms.end());
      mFoundArms.clear();
      return;
    }

    int16_t GetMetaArmType(void) { return mMetaArmType; }
    int32_t GetMetaArmID(void) { return mMetaArmID; }

	int16_t GetBurgersType(void) const {
	  if (mTerminalArms.size()) {
		return mTerminalArms[0]->GetBurgersType();
	  }
	  return BCC_BURGERS_UNKNOWN;
	}

	vector<Node *>mTerminalNodes; //
    vector<Arm*> mTerminalArms; // one or two arms
    double mLength;
    int16_t mMetaArmType;
    uint32_t mMetaArmID;
    uint32_t mNumSegments, mNumNodes;
    static rclib::Point<float> mWrappedNode; // Used in GetNodeLocations -- used to indicate a gap in the point list from a MetaArm where a segment is skipped due to wrapping.
    bool mCombo;
    bool mFound111; // if we see a 111 on our search
    vector<Arm*> mFoundArms, mAllArms, mDeadEnds;
  };

  //=============================================
  /*!
    The paraDIS data set.  Note that all you really want is to read the data and return the segments and nodes.  Identifying specific arms in a scene is not likely to be of interest, so arm information is kept internal.
  */
  class DataSet {
  public:
    DataSet() { init(); }

    ~DataSet() {
      return;
    }

    /*!
      if this is true, then complete dumps of all data are done,
      into files named mDebugOutputPrefix + <type> + ".debug"
    */
    void EnableDebugOutput(bool tf=true) {
      mDoDebugOutput = tf;
    }
    /*!
      if this is true, then create arm and metaarm files.
    */
    void EnableStatsOutput(bool tf=true) {
      mDoStats = tf;
    }
    /*!
      if this is true, then create brief summary file.
    */
    void EnableSummaryOutput(bool tf=true) {
      mDoSummaryFile = tf;
    }
    /*!
      if this is true, then create tag file.
    */
    void EnableTagFileOutput(bool tf=true) {
      mDoTagFile = tf;
    }

    /*!
      if this is true, then create tag file.
    */
    void EnablePovRayFileOutput(bool tf=true) {
      mDoPovRayFiles = tf;
    }

    /*!
      if this is true, then create tag file.
    */
    void EnablePovRayFuseCalc(bool tf=true) {
      mDoPovRayFuse = tf;
    }

	/*!
      if this is true, then create tag file.
    */
    void EnableJSONFileOutput(bool tf=true) {
      mDoJSONFiles = tf;
    }
    /*!
      if this is true, then create tag file.
    */
    void EnableVTKFileOutput(bool tf=true) {
      mDoVTKFile = tf;
    }
    /*!
s      Tell the data set which file to read
    */
    void SetDataFile(std::string datafile) {
      mDataFilename = datafile;
      if (mOutputBasename == "") {
        string basename = Replace(mDataFilename, ".data", "");
        basename = Basename(Replace(basename, ".dat", ""));
        SetOutputBaseName(basename);
      }
      SetOutputDir();
    }

    /*!
      If mOutputDir is set, leave it alone.
      If not, then set it based on the mOutputBaseName
    */
    void SetOutputDir(void) {
      if (mOutputDir != "") {
        return;
      }
      std::string::size_type slash = mOutputBasename.rfind("/");
      if (slash == 0) {
        SetOutputDir("/");
      } else if (slash != std::string::npos) {
        SetOutputDir(mOutputBasename.substr(0, slash));
      } else {
        SetOutputDir(".");
      }
    }


    /*!
      By default, this is "./paradis-output".
    */
    void SetOutputDir(std::string dir) {
       mOutputDir = dir;
       if (!Mkdir(mOutputDir.c_str())) {
         cerr << "Warning: could not create output directory " << mOutputDir << endl;
       }
       return;
    }

    /*!
      By default, this is the datafile name without its extension.
    */
    void SetOutputBaseName(std::string name) {
       mOutputBasename = name;
       SetOutputDir();
       Node::SetTraceFileBasename(mOutputDir + "/" + mOutputBasename);
       Arm::SetTraceFileBasename(mOutputDir + "/" + mOutputBasename);
    }

    /*!
      verbosity goes from 0-5, based on dbg_setverbose() from librc.a
      filename if null means stderr
    */
    void SetVerbosity(int level, string filename = "") {
      if (filename != "") {
        if (filename[0] != '/') {
          filename = mOutputDir + "/" + filename;
        }
        if (!Mkdir(Dirname(filename).c_str())) {
          cerr << "Warning: could not create output directory " << Dirname(filename) << endl;
        }
        dbg_setfile(filename.c_str());
      }
      dbg_setverbose(level);
    }

    /*!
      Trace decomposition of the given arm to the given depth.
    */
    void TraceNode(int32_t nodeID){
      Node::TraceNode(nodeID);
    }

    /*!
      Trace decomposition of the given arm to the given depth.
    */
    void TraceArm(int32_t armID){
      Arm::TraceArm(armID);
    }

    /*!
      Set trace depth for arm tracing
    */
    void SetTraceDepth(uint32_t depth) {
      Arm::SetTraceDepth(depth);
    }


    void SetThreshold(double threshold) {
      mThreshold = threshold;
      Arm::mThreshold = threshold;
      return;
    }

    void SetScrewToleranceAngle(double angle) {
      ArmSegment::SetScrewToleranceAngle(angle);
      return;
    }

    void SetNumBins(long numbins) {
      mNumBins = numbins;
      return;
    }


   /*!
      Print all arms in a simple format into a file for analysis
    */
    void PrintMetaArmFile(void);

    /*!
      Print all arms in a simple format into a file for analysis
      altname provides a way to write a 2nd file for debugging arms before decomposition
    */
    void PrintArmFile(char*altname = NULL);

    /*!
      Parse the input file just for the data bounds.
    */
    void GetBounds( rclib::Point<float> &outDataMin,
                   rclib::Point<float> &outDataMax,
                   std::string filename="") {
      if (filename!="") {
        mDataFilename = filename;
      }
      ReadBounds();
      outDataMin = mDataMin;
      outDataMax = mDataMax;
      return;
    }

   /*!
      Accessor function for C api
    */
	uint32_t GetNumNodes(void) {
	  return Node::mNodes.size();
	}

   /*!
      Accessor function for C api.
	  warning:  returns NULL if nodeIndex out of bounds!
    */
    Node * GetNode(uint32_t nodeIndex) {
	  return Node::mNodeVector[nodeIndex];
    }



    /*!
      Tag all METAARM_LOOP nodes
    */
    void TagNodes(void);

    /*!
      Write out a copy of the input file that has all Node tags in it.
    */
    void WriteTagFile(void);

    /*!
      Write a json file for the nodes and another json file for the segments
      in the given arms.
    */
    void WritePov(void);

    /*!
      Write a json file for the nodes and another json file for the segments
      in the given arms.
    */
    void WriteJson(void);

    /*!
      Write out a vtk fileset containing all nodes and segments.
      Also writes a .visit file identifying each file as a separate block.
      There is no ordering intentionally in the file.
    */
    void WriteVTKFiles(void);

    /*!
      Using 3-way binary decomposition, determine our chunk of subspace based on our processor number
    */
    void ComputeSubspace(void);

    /*!
      For parallel processing, tell dataset which procnum we are.
      If this is set, then the data set will use a certain subspace of the data based on a decomposition in space
    */
    void SetProcNum(int procnum, int numprocs) {
      mProcNum = procnum;
      mNumProcs = numprocs;
    }

    /*!
      Accessor -- procnum (0 based) for parallelism
    */
    int GetProcNum(void) { return mProcNum; }

    /*!
      Accessor -- number of procs in parallel
    */
    int GetNumProcs(void) { return mNumProcs; }

    /*!
      Run a self-diagnostic.  used by paradisTest
    */
    bool Test(void);

   /*!
      Parse the paradis data file and create a full set of arms and nodes
      I've made it optional.
    */
    void ReadData(std::string datafile="", bool createVectors=false);


    /* Starting from the given arm, trace out the containing meta arm and return a shared_ptr to the result */
    boost::shared_ptr<MetaArm> TraceMetaArm(vector<Arm>::const_iterator arm);


    /*!
      Set the bounds for our chunk of space
    */
    void SetSubspace(const rclib::Point<float> &min, const rclib::Point<float> &max){
      if (min.Exceeds(max)) {
        throw std::string("Error in DataSet::SetSubspace: A value in min vector exceeds max");
      }
      mSubspaceMin = min; mSubspaceMax = max;
      return;
    }

    /*!
      Wrap one or the other locations if they cross a periodic boundary.
      Return true if any change was made.
    */
    static bool Wrap(vector<float> &loc0, vector<float> &loc1);

    /*!
        Free up all memory and go back to virgin state.
    */
    void Clear(void) {
      init();
    }

    /*!
      Optionally create the list of meta-arms to trace out the M-N network on a higher level.
    */
    void FindMetaArms(void);

	/*!
	  Used to set the first segment for the light the fuse animation
	*/
	void SetInitialLightTheFuseSegment(uint32_t num = 0) {
	  ArmSegment::mInitialLightTheFuseArmSegment = ArmSegment::mArmSegments[num];
	}

    /*!
      Compute the distance of all segments from mInitialLightTheFuseArmSegment.
    */
	void ComputeLightTheFuseSegmentDistances(void);

     /*!
      Accessor function
    */
    uint32_t GetNumMetaArms(void) { return mMetaArms.size(); }

    /*!
      Accessor function
    */
    boost::shared_ptr<MetaArm> GetMetaArm(uint32_t metaArmNum) {
      return mMetaArms[metaArmNum];
    }

    /*!
      Accessor function
    */
    ArmSegment *GetArmSegment(uint32_t segnum) {
     return ArmSegment::mArmSegments[segnum];
    }

    /*!
      Accessor function.
    */
    uint32_t GetNumArmSegments(void)  {
      return  ArmSegment::mArmSegments.size();
    }

    void PrintArmStats(FILE *file=NULL) {
      if (!file) file = stdout;
      fprintf(file, "%s", ArmSummary().c_str());
    }

    //=======================================================================
    // PRIVATE
    //======================================================================
  private:
    /*!
      Initialize things to virginal values.
    */
    void init(void) {
      mDataType = PARADIS_DATATYPE_UNKNOWN;
      mNumBins = 0;
      mThreshold = -1.0;
      mFileVersion = 0;
      Arm::Clear();
      ArmSegment::Clear();
      Node::Clear();
      // Note:  There is no MetaArm::Clear();

      mProcNum = mNumProcs = mFileVersion = 0;
      mTotalDumpNodes =0;
      mElementsPerVTKFile = 50*1000;
      mDoDebugOutput=false;
      mDoTagFile = false;
      mDoVTKFile = false;
      mDoJSONFiles = false;
      mDoPovRayFiles = false;
      mDoPovRayFuse = false;
      mDoStats = false;
      mDoSummaryFile = false;
      mOutputDir = "./paradis-debug";
      mDataMin = mDataMax = mDataSize = mSubspaceMin = mSubspaceMax =
        rclib::Point<float>(0.0);
    }
    /*!
      Parse the input file just for the data bounds.
    */
    void ReadBounds(void);
    /*!
      make a directory or return false.  prints error if fails
    */
    bool Mkdir(const char *);

    /*!
    Statistical summaries:
    */
    std::string MonsterNodeSummary(void);

    std::string ArmSummary(void);

    std::string MetaArmSummary(void);
    /*!
      Prints out all full nodes
    */
    void DebugPrintNodes(void);

   /*!
      Prints out summary of arms and metaarms in a short file
    */
    void WriteSummaryFile(const char *altname = NULL);

   /*!
      Prints out all arms
    */
    void DebugPrintArms(const char *altname = NULL);

    /*!
      Prints out all MetaArms
    */
    void DebugPrintMetaArms(void);

    /*!
      Read a node from the input file and write it out with its tag to the tagfile
    */
    void CopyNodeFromFile(uint32_t &lineno, std::ifstream &datafile, std::ofstream &tagfile);

   /*!
      Read the data file until we find the node specified in theNode.  Create a Node from the information in the file. Find each Neighbor as it is seen and create  and its neighbors from a file.  This has to be done in DataSet because we avoid duplicate neighbor structs by using pointers into a global neighbor array.
    */
    void ReadNodeFromFile(uint32_t &lineno, std::ifstream &datafile);

	/*!
	  Parse the data file and create the basic structures
	*/
	void CreateNodesAndArmSegments(void);

	// Fix segments that have large distances as an artifact of the periodicity of the data.
    void WrapBoundarySegments(void);

  /*!
    Starting at the given first node and heading out in the direction of the given first segment in an arm, trace along the arm until you find its other endpoint (terminal segment and node).  When wrapped nodes are found, use their real counterparts instead.
    This will be where we actually discriminate between node types, etc.  But as mentioned for BuildArms, we don't do that yet.
	the "reverse" parameter if true says prepend found nodes and segments in reverse order to mNodes and mSegments, used when going from middle of arm
  */
    void FindEndOfArm(Node *firstNode, Node **oFoundEndNode,
                      ArmSegment *firstSegment,  ArmSegment *&foundEndSegment, Arm *theArm, bool reverse = false);
    /*!
      Create all arms for our region. This function is a bit long because we are avoiding recursion by using loops.  Recursion for these arms would get pretty deep.
      IN THE FUTURE, to save memory, we will implement the following:
      Segments that connect two PartialNodes are PartialSegments.  Segments that connect two OOB Nodes are CollapsedSegments.  Segments that connect an IB Node to another node are FullSegments.  Only FullSegments are drawn.
      HOWEVER, For the first implementation, we will not distinguish between Partial and Full Segments, and we will not even collapse arms, so that we can get a good view of what things are like.  After it is clear that we are making sensible things, we can collapse arms to save memory.
    */
    void BuildArms(void);


   /*!
      Find all arms that are decomposible, and make them into two arms each.
      Since this will destroy all the original burgers and add NodeID information,
      statistics might need to be gathered here,
    */
     void DecomposeArms(void);

     /* put all arm segments into mArmSegments */
     // void CollectAllArmSegments(void);

    /*!
      Makethe final classification on arms as ARM_XX_YYY.
    */
    void ClassifyArms(void);

    double ComputeArmLengths(void);

    /*!
      Compute all node types
    */
    void ComputeNodeTypes(void);



    /*!
      A MetaArm is a chain of arms with all the same Burgers vector value, but can also include 200, 020, and 002 types as "unzipped" portions along the meta-arm.
    */
     std::vector<boost::shared_ptr<MetaArm> > mMetaArms;

    /*!
      Extents of the full data set:
    */
    static rclib::Point<float> mDataMin, mDataMax, mDataSize;
    /*!
      Subspace of interest:
    */
    rclib::Point<float> mSubspaceMin, mSubspaceMax;



    /*!
      When writing out VTK files, this is how many to put per block max.
      Aids in speedy visualization.
    */
    uint32_t mElementsPerVTKFile;

    /*!
      Number of nodes in full dump data
    */
    uint32_t mTotalDumpNodes;

    /*!
      if this is true, then complete dumps of all data are done,
      into files named mOutputPrefix + <type> + ".debug"
    */
    bool mDoDebugOutput;
    /*!
      if this is true, then create arm and metaarm files.
    */
    bool mDoStats;
    /*!
      if this is true, then create short summary file.
    */
    bool mDoSummaryFile;
    /*!
      if this is true, then create tag file.
    */
    bool mDoTagFile;

    /*!
      if this is true, then create VTK file at end of analysis.
    */
    bool mDoVTKFile;

    /*!
      if this is true, then create Povray files at end of analysis.
    */
    bool mDoPovRayFiles;
    /*!
      if this is true, then compute "light the fuse" distances for povray output
    */
    bool mDoPovRayFuse;

    /*!
      if this is true, then create JSON files at end of analysis.
    */
    bool mDoJSONFiles;

      /*!
      By default, this is "./paradis-output".  See mDoOutput.
    */
    std::string mOutputDir;

    /*!
      By default, this is mDataFilename without the extension.
      E.g., mDataFilename = rs0020.data ---->  mOutputBasename = rs0020
    */
    std::string mOutputBasename;

    /*!
      The name of the data file that will be read.
    */
    std::string mDataFilename;


    /*!
      If set, then the file to be read is "old school" paraDIS data, of a slightly different format.  If there is a dataFileVersion string, then mFileVersion will reflect that.  Otherwise:
      0 == "old style" -- probably not needed any more
      1 == "new style" -- before versioning was done
    */
    unsigned long mFileVersion;

    /*!
      processor number for parallelism.
    */
    int mProcNum;
    /*!
      number of processors for parallelism.  If zero or one, then serial.
    */
    int mNumProcs;
    /*!
      A hack to get at some interesting linked loops for Moono Rhee.
      If an arm is less than a certain length, then he assumes
      it is part of a particular loop configuration I call a "linked loop."
    */
    double mThreshold;

    /*!
      Moono would like to print out binned arm lengths.  He will give a number of bins and I will bin the arms into those many buckets when examining them at the end.
    */
    int mNumBins;

    /*!
      mDataType is one of PARADIS_BCC, PARADIS_HCP, PARADIS_UNKNOWN
    */
    int mDataType;
  };


} //end namespace paraDIS

#endif /* defined PARADIS_H */
