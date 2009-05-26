fragments version 0    5
# there are five fragments in this example file.  I hope I have covered enough
# cases here to completely guide you. Please ask if you have any questions.

# fragment 0 has two interior nodes and two 4-neighbor endpoints, so 
# this fragment is actually a whole arm (or chain).  
fragment  0.5773503000    0.5773503000    0.5773503000  2 2
	endpoint  0,2 30.0 40.0 20.0 4   
    endpoint  0,7 -50 -10 0. 4   
    # the interior nodes are given in order starting with the one next to 
    # the first endpoint listed above and ending with the neighbor to the 
    # last endpoint.  So we know from this that the order of nodes in the 
    # arm is:   0,2 --> 0,1 --> 0,0 --> 0,7
    interior 0,1 -30 40 10 
    interior 0,0 -40. 10. 10. 

# fragment 1 has one interior nodes, and one of its endpoints has 
# numneighbors == -3, so it is a ghost, node, so we know that this arm is 
# truly a fragment rather than  a whole arm.  There is no other difference in 
# format between fragments and whole arms. It's of course perfectly reasonable
# for both endpoints to be ghost nodes.  
fragment 0.5773503000    0.5773503000    0.5773503000   2 1
    endpoint  0,2 30.0 40.0 20.0 4   
    endpoint  0,9 20 -20 0 -3
    interior  0,13 55 90 20 


# fragment 2 is a loop, with no endpoints. That means that all its nodes
# are interior nodes, with two neighbors, and that it does not leave our
# subspace.  i.e.:
# 0,4 --> 0,5 --> 0,6 --> 0,4 
fragment 0.5773503000    0.5773503000    0.5773503000   0 3    
    interior 0,4  55 -20 20 3 
    interior 0,5 30 -50 0. 3 
    interior 0,6 -10 -35 0. 3 

# fragment 3 is a loop with one endpoint.  that means that it contains one
# node that has more than two arms, but that two of its arms are really 
# the same arm, looping back around.  i.e.: 
# 0,2 --> 0,4 --> 0,5 --> 0,6 --> 0,2 
fragment 0.5773503000    0.5773503000    0.5773503000   1 3    
    endpoint 0,2 30.0 40.0 20.0 4   
    interior 0,4  55 -20 20 3 
    interior 0,5 30 -50 0. 3 
    interior 0,6 -10 -35 0. 3 

# fragment 4 consists of only two endpoints, with no interior nodes.  One of 
# its endpoints is a ghost node here, just to add interest.  However, since 
# the "ghost" zone has three neighbors, this fragment is really a whole arm, 
# which will be duplicated in an adjacent domain. 
# 0,2 --> 0,9
fragment 0.5773503000    0.5773503000    0.5773503000   2 0   
    endpoint  0,2 30.0 40.0 20.0 4   
    endpoint  0,9 20 -20 0 -3

