The files in this distribution are for creating paraDIS_lib.  
Files are built into $INSTALL_DIR if defined, else a directory called "./$SYS_TYPE" if SYS_TYPE is defined, else a directory called  "./uname -n". 

Built files:
$SYS_TYPE/lib/libparadis.a
$SYS_TYPE/bin/analyzeParaDIS

Also included are two example scripts for rendering the output of analyzeParaDIS to blender, in parallel:  
blender_script.py
render_parallel.sh


========================================================================
ParaDIS data is a set of disconnected undirected graphs, which may contain cycles.  
All nodes in a paraDIS data set have at least one neighbor given in the paraDIS output file.  The connection, or neighbor relation, between two adjacent nodes is called an "arm segment."  A node is considered an "interior node" if it has exactly two neighbors, else it is considered a "terminal node."  The only exception to this is that loops always have one terminal node, which may have two neighbors.  
An "arm" is a sequence of interior nodes terminated by one or two terminal nodes.  A loop is an arm that has only one terminal node.  
Every node is part of at least one arm.  If it's not an interior node, we say the node "has arms."  The number of arms equals the number of neighbors, even if some are loops.  E.g., a terminal node with three neighbors has three arms by definition, even if two of the neighbors are part of the same loop, i.e., two of the arms might be the same arm.

Segments and arms have Burgers vectors associated with them.  
  //  Segment BURGERS TYPES: (P = plus(+) and M = minus(-))
#define BURGERS_NONE 0
#define BURGERS_100  1
#define BURGERS_010  2
#define BURGERS_001  3
#define BURGERS_PPP  4  // +++
#define BURGERS_PPM  5  // ++-
#define BURGERS_PMP  6  // +-+
#define BURGERS_PMM  7  // +--
#define BURGERS_UNKNOWN  8  

NODE TYPES and MONSTER NODES: 
Every node has a node type.  The vast majority of nodes are simply have NodeType = number of neighboring nodes.  But some nodes get negative types, and these are known as Monster Nodes.  Oooh, scary.  
Only a terminal node may be a "monster node" A.K.A. "M Type node." If a node is not an "M" then it is an "N" for "non-monster" or "normal."  Interior nodes are always type "N" if anyone cares to ask, but this is usually ignored.  
A Monster node is any node that has a NodeType < 0.  Monster nodes always have either 3 or 4 arms.  

FOUR ARMED MONSTERS:  A four armed node with four uniquely valued burger vector types is a monster:
  If it has four 111 arms:  type := -4
  If it has 2 "111" arms and 2 "100" arms:  type := -44
  Else type is just 4 (non-monster)

THREE ARMED MONSTERS:  If a node has three arms, you have to look at the other endpoint of all its arms to determine its "monstrosity."  It is a monster iff for any of its arms: 
   It's an unzipped monster, AKA "butterfly" node:  The other endpoint is also a three-armed node and all arms not shared by the endpoints have different "111" burgers types.    In this case, the node has type -3. 
   The other endpoint is a type -44 node. (see above) In this case, the node has type -33 unless it's already a -3. 
   Else type is just 3 (non-monster)


Arms also have "MN type," which simply indicates whether its endpoints are monsters or not and what their burgers vector are at the same time.  
  // Arm MN types:
#define ARM_UNKNOWN 0
#define ARM_UNINTERESTING 1
#define ARM_LOOP 2
#define ARM_MM_111 3 
#define ARM_MN_111 4
#define ARM_NN_111 5 
#define ARM_MM_100 6
#define ARM_MN_100 7
#define ARM_NN_100 8
#define ARM_SHORT_NN_111 9
#define ARM_SHORT_NN_100 10
