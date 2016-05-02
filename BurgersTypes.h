/*   Written by Richard David Cook
     at Lawrence Livermore National Laboratory
     Contact:  wealthychef@gmail.com
     See license.txt for information about usage.
     Spoiler alert:  it's GNU opensource.
*/


#pragma once
/* http://en.wikipedia.org/wiki/Pragma_once */
#include "boost/cstdint.hpp"
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

// Special commonly-used values
#define BCC_BURGERS_UNKNOWN    -42 // analysis failed
#define BCC_BURGERS_NONE        00 // no analysis done yet
#define BCC_BURGERS_PPP         10
#define BCC_BURGERS_PPM         11
#define BCC_BURGERS_PMP         12
#define BCC_BURGERS_PMM         13

// Arm MN types:
#define ARM_EMPTY          -1 // marked for deletion after decomposition step
#define ARM_UNKNOWN        0
#define ARM_UNINTERESTING  1
#define ARM_LOOP           2
#define ARM_BCC_MM_111         3
#define ARM_BCC_MN_111         4
#define ARM_BCC_NN_111         5
#define ARM_BCC_SHORT_NN_111   6
#define ARM_BOUNDARY       7  // Has a terminal node with one neighbor segment; happens in non-periodic data

// MetaArm types:
#define METAARM_UNKNOWN     0  // Not defined, error, or some other odd state
#define METAARM_111         1  // Entirely composed of type 111 arms of the same burgers vector.  Does not include loops.
#define METAARM_LOOP_111    2  // Contains a loop, composed entirely of type 111 arms.
#define METAARM_LOOP_HIGH_ENERGY    3  // Contains a loop, composed entirely of type 200 arms or higher.

class BurgerTypeInfo {
public:
  BurgerTypeInfo(int num, vector<float> v, int e, string nom):
    burgnum(num), energy(e), burgvec(v), name(nom) {}
  BurgerTypeInfo() = default;
  int burgnum = BCC_BURGERS_UNKNOWN;
  int energy = 0;
  vector<float> burgvec {-42, -42, -42};
  string name {"ERROR_UNKNOWN_BURGER_TYPE"};
};
// ArmTypeToNameMap\[\([^]]*\)\]   ArmTypeToName(\1)
extern vector<BurgerTypeInfo> BurgInfos;  // list of all burgers types

vector<int> GetAllArmTypes(void);
string ArmTypeToName(int atype);
string MetaArmTypeToName(int mtype);
BurgerTypeInfo BurgTypeToBurgInfo(int burgnum);
BurgerTypeInfo BurgVecToBurgInfo(const vector<float> &burgvec);
int BurgVecToBurgType(const vector<float> &burgvec);
string BurgTypeToName(int btype);
string DocumentAllBurgersTypes(void);
string DocumentAllArmTypes(void);

//vector<int> GetAllBurgersTypes(void);
double AngularDifference(vector<float>v1, vector<float>v2, double v1Length=-1, double v2Length=-1);

//=============================================

#define BURGERS_EPSILON (0.000001)

#define PARADIS_DATATYPE_UNKNOWN (42)
#define PARADIS_DATATYPE_BCC     (0)
#define PARADIS_DATATYPE_HCP     (1)
// #define \(BCC_BURGERS_[^ ]*\) *\([0-9]*\).*
// These are valued in order of increasing energy levels, corresponding to the sum of the square of the components of the burgers vector.
#define NUM_BCC_ENERGY_LEVELS  8
#define NUM_BCC_BURGERS_TYPES  26
// Segment BCC_BURGERS TYPES: (P = plus(+) and M = minus(-))

/* ========================================  */
//  Segment HCP_BURGERS TYPES: Currently an arbitrary pile of meaningless junk
#define HCP_BURGERS_UNKNOWN (-43)
#define HCP_ARM_OF_INTEREST (1000)

/* ========================================  */

// SCREW TYPE is a characteristic of an arm segment that Jaime Marian uses
#define SCREW_UNDEFINED  4242
#define BURGER_NO_SCREW  0
#define BURGER_SCREW     1
#define BURGER_EDGE      2



