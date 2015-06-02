#pragma once
/* http://en.wikipedia.org/wiki/Pragma_once */
#include "boost/cstdint.hpp"
using boost::int8_t;
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

using namespace std; 

string BurgersTypeNames(int btype);
string ArmTypeNames(int atype);
string MetaArmTypeNames(int mtype);
int InterpretBurgersType(vector<float> burg);
int InterpretHCPBurgersType(vector<float> burg);
int InterpretBCCBurgersType(vector<float> burg);
vector<int> GetAllBurgersTypes(void); 
double AngularDifference(vector<float>v1, vector<float>v2, double v1Length=-1, double v2Length=-1);

//=============================================

#define BURGERS_EPSILON (0.000001)

#define PARADIS_DATATYPE_UNKNOWN (-1)
#define PARADIS_DATATYPE_BCC     (0) 
#define PARADIS_DATATYPE_HCP     (1) 

// These are valued in order of increasing energy levels, corresponding to the sum of the square of the components of the burgers vector.  
#define NUM_BCC_ENERGY_LEVELS  8
#define NUM_BCC_BURGERS_TYPES  26
//  Segment BCC_BURGERS TYPES: (P = plus(+) and M = minus(-))
#define BCC_BURGERS_UNKNOWN    (-1) // analysis failed
#define BCC_BURGERS_NONE         0  // no analysis done yet
#define BCC_BURGERS_PPP         10  // +++ BEGIN ENERY LEVEL 1
#define BCC_BURGERS_PPM         11  // ++- 
#define BCC_BURGERS_PMP         12  // +-+
#define BCC_BURGERS_PMM         13  // +--
#define BCC_BURGERS_200         20  // BEGIN ENERGY LEVEL 2
#define BCC_BURGERS_020         21  
#define BCC_BURGERS_002         22
#define BCC_BURGERS_220         30  // BEGIN ENERGY LEVEL 3
#define BCC_BURGERS_202         31
#define BCC_BURGERS_022         32
#define BCC_BURGERS_311         40  // BEGIN ENERGY LEVEL 4
#define BCC_BURGERS_131         41
#define BCC_BURGERS_113         42
#define BCC_BURGERS_222         50  // BEGIN ENERGY LEVEL 5
#define BCC_BURGERS_004         60  // BEGIN ENERGY LEVEL 6
#define BCC_BURGERS_331         70  // BEGIN ENERGY LEVEL 7
#define BCC_BURGERS_313         71
#define BCC_BURGERS_133         72
#define BCC_BURGERS_420         80  // BEGIN ENERGY LEVEL 8
#define BCC_BURGERS_240         81 
#define BCC_BURGERS_024         82 
#define BCC_BURGERS_042         83 
#define BCC_BURGERS_204         84 
#define BCC_BURGERS_402         85 

/* ========================================  */ 
//  Segment HCP_BURGERS TYPES: Currently an arbitrary pile of meaningless junk
#define HCP_BURGERS_UNKNOWN (-42) 

/* ========================================  */ 

// SCREW TYPE is a characteristic of an arm segment that Jaime Marian uses
#define SCREW_UNDEFINED  -1
#define BURGER_NO_SCREW  0
#define BURGER_SCREW     1
#define BURGER_EDGE      2

// Arm MN types:
#define ARM_EMPTY         -1 // marked for deletion after decomposition step
#define ARM_UNKNOWN        0
#define ARM_UNINTERESTING  1
#define ARM_LOOP           2
#define ARM_MM_111         3 
#define ARM_MN_111         4
#define ARM_NN_111         5 
#define ARM_SHORT_NN_111   6
#define ARM_BOUNDARY       7  // Has a terminal node with one neighbor segment; happens in non-periodic data

// MetaArm types:
#define METAARM_UNKNOWN     0  // Not defined, error, or some other odd state
#define METAARM_111         1  // Entirely composed of type 111 arms of the same burgers vector.  Does not include loops. 
#define METAARM_LOOP_111    2  // Contains a loop, composed entirely of type 111 arms.
#define METAARM_LOOP_HIGH_ENERGY    3  // Contains a loop, composed entirely of type 200 arms or higher.

