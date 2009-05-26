#declare lookatpos = <9.085395,8.466035,19.160285>;
#declare camerapos = <9.085395,8.466035,498.167410>;
#declare viewx = x * 151.829210;
#declare viewy = y * 153.067930;
#include "jaime.inc"
// WriteFan([0, 1, (85, 39, 16, 127, 68)])
// segment connecting nodes 39 and 85 with color 0
make_dislocation(<-1.686056,6.182347,20.503710>, <-17.202260,7.307394,34.894860>, 0)
// triangle color 1 nodes: [85, 39, 16]
make_triangle(<-17.202260,7.307394,34.894860>, <-1.686056,6.182347,20.503710>, <0.154583,6.902457,17.942960>, 1)
// segment connecting nodes 16 and 39 with color 0
make_dislocation(<0.154583,6.902457,17.942960>, <-1.686056,6.182347,20.503710>, 0)
// triangle color 1 nodes: [85, 16, 127]
make_triangle(<-17.202260,7.307394,34.894860>, <0.154583,6.902457,17.942960>, <-1.559360,10.504440,16.054850>, 1)
// segment connecting nodes 127 and 16 with color 0
make_dislocation(<-1.559360,10.504440,16.054850>, <0.154583,6.902457,17.942960>, 0)
// triangle color 1 nodes: [85, 127, 68]
make_triangle(<-17.202260,7.307394,34.894860>, <-1.559360,10.504440,16.054850>, <-4.173561,10.833960,18.342230>, 1)
// segment connecting nodes 68 and 127 with color 0
make_dislocation(<-4.173561,10.833960,18.342230>, <-1.559360,10.504440,16.054850>, 0)
// segment connecting nodes 85 and 16 with color 0
make_dislocation(<-17.202260,7.307394,34.894860>, <0.154583,6.902457,17.942960>, 0)
// WriteFan([0, 1, (11, 15, 127, 100, 13)])
// segment connecting nodes 15 and 11 with color 0
make_dislocation(<3.650219,6.594448,14.755330>, <6.169073,5.830927,13.000000>, 0)
// triangle color 1 nodes: [11, 15, 127]
make_triangle(<6.169073,5.830927,13.000000>, <3.650219,6.594448,14.755330>, <-1.559360,10.504440,16.054850>, 1)
// segment connecting nodes 127 and 15 with color 0
make_dislocation(<-1.559360,10.504440,16.054850>, <3.650219,6.594448,14.755330>, 0)
// triangle color 1 nodes: [11, 127, 100]
make_triangle(<6.169073,5.830927,13.000000>, <-1.559360,10.504440,16.054850>, <0.667054,10.975430,13.357470>, 1)
// segment connecting nodes 100 and 127 with color 0
make_dislocation(<0.667054,10.975430,13.357470>, <-1.559360,10.504440,16.054850>, 0)
// triangle color 1 nodes: [11, 100, 13]
make_triangle(<6.169073,5.830927,13.000000>, <0.667054,10.975430,13.357470>, <5.816433,8.602456,10.581110>, 1)
// segment connecting nodes 13 and 100 with color 0
make_dislocation(<5.816433,8.602456,10.581110>, <0.667054,10.975430,13.357470>, 0)
// segment connecting nodes 11 and 127 with color 0
make_dislocation(<6.169073,5.830927,13.000000>, <-1.559360,10.504440,16.054850>, 0)
// WriteFan([0, 1, (15, 16, 127)])
// segment connecting nodes 16 and 15 with color 0
make_dislocation(<0.154583,6.902457,17.942960>, <3.650219,6.594448,14.755330>, 0)
// triangle color 1 nodes: [15, 16, 127]
make_triangle(<3.650219,6.594448,14.755330>, <0.154583,6.902457,17.942960>, <-1.559360,10.504440,16.054850>, 1)
// segment connecting nodes 127 and 16 with color 0
make_dislocation(<-1.559360,10.504440,16.054850>, <0.154583,6.902457,17.942960>, 0)
// segment connecting nodes 15 and 127 with color 0
make_dislocation(<3.650219,6.594448,14.755330>, <-1.559360,10.504440,16.054850>, 0)
// WriteFan([0, 1, (13, 100, 88, 42, 14)])
// segment connecting nodes 100 and 13 with color 0
make_dislocation(<0.667054,10.975430,13.357470>, <5.816433,8.602456,10.581110>, 0)
// triangle color 1 nodes: [13, 100, 88]
make_triangle(<5.816433,8.602456,10.581110>, <0.667054,10.975430,13.357470>, <2.008082,11.553810,11.438070>, 1)
// segment connecting nodes 88 and 100 with color 0
make_dislocation(<2.008082,11.553810,11.438070>, <0.667054,10.975430,13.357470>, 0)
// triangle color 1 nodes: [13, 88, 42]
make_triangle(<5.816433,8.602456,10.581110>, <2.008082,11.553810,11.438070>, <3.976924,12.439270,8.583793>, 1)
// segment connecting nodes 42 and 88 with color 0
make_dislocation(<3.976924,12.439270,8.583793>, <2.008082,11.553810,11.438070>, 0)
// triangle color 1 nodes: [13, 42, 14]
make_triangle(<5.816433,8.602456,10.581110>, <3.976924,12.439270,8.583793>, <6.007930,10.981770,8.010298>, 1)
// segment connecting nodes 14 and 42 with color 0
make_dislocation(<6.007930,10.981770,8.010298>, <3.976924,12.439270,8.583793>, 0)
// segment connecting nodes 13 and 88 with color 0
make_dislocation(<5.816433,8.602456,10.581110>, <2.008082,11.553810,11.438070>, 0)
// WriteFan([0, 1, (31, 14, 42)])
// segment connecting nodes 14 and 31 with color 0
make_dislocation(<6.007930,10.981770,8.010298>, <6.000000,13.000000,6.000000>, 0)
// triangle color 1 nodes: [31, 14, 42]
make_triangle(<6.000000,13.000000,6.000000>, <6.007930,10.981770,8.010298>, <3.976924,12.439270,8.583793>, 1)
// segment connecting nodes 42 and 14 with color 0
make_dislocation(<3.976924,12.439270,8.583793>, <6.007930,10.981770,8.010298>, 0)
// segment connecting nodes 31 and 42 with color 0
make_dislocation(<6.000000,13.000000,6.000000>, <3.976924,12.439270,8.583793>, 0)
// WriteFan([0, 1, (39, 85, 27)])
// segment connecting nodes 85 and 39 with color 0
make_dislocation(<-17.202260,7.307394,34.894860>, <-1.686056,6.182347,20.503710>, 0)
// triangle color 1 nodes: [39, 85, 27]
make_triangle(<-1.686056,6.182347,20.503710>, <-17.202260,7.307394,34.894860>, <-15.133910,4.926839,35.207080>, 1)
// segment connecting nodes 27 and 85 with color 0
make_dislocation(<-15.133910,4.926839,35.207080>, <-17.202260,7.307394,34.894860>, 0)
// segment connecting nodes 39 and 27 with color 0
make_dislocation(<-1.686056,6.182347,20.503710>, <-15.133910,4.926839,35.207080>, 0)
// WriteFan([0, 1, (9, 10, 18, 17, 22, 8)])
// segment connecting nodes 10 and 9 with color 0
make_dislocation(<13.000000,6.000000,6.000000>, <13.000000,13.000000,13.000000>, 0)
// triangle color 1 nodes: [9, 10, 18]
make_triangle(<13.000000,13.000000,13.000000>, <13.000000,6.000000,6.000000>, <12.670320,6.924030,6.594350>, 1)
// segment connecting nodes 18 and 10 with color 0
make_dislocation(<12.670320,6.924030,6.594350>, <13.000000,6.000000,6.000000>, 0)
// triangle color 1 nodes: [9, 18, 17]
make_triangle(<13.000000,13.000000,13.000000>, <12.670320,6.924030,6.594350>, <10.411190,10.110060,7.521242>, 1)
// segment connecting nodes 17 and 18 with color 0
make_dislocation(<10.411190,10.110060,7.521242>, <12.670320,6.924030,6.594350>, 0)
// triangle color 1 nodes: [9, 17, 22]
make_triangle(<13.000000,13.000000,13.000000>, <10.411190,10.110060,7.521242>, <8.697445,11.837410,7.534854>, 1)
// segment connecting nodes 22 and 17 with color 0
make_dislocation(<8.697445,11.837410,7.534854>, <10.411190,10.110060,7.521242>, 0)
// triangle color 1 nodes: [9, 22, 8]
make_triangle(<13.000000,13.000000,13.000000>, <8.697445,11.837410,7.534854>, <7.018861,13.000000,7.018861>, 1)
// segment connecting nodes 8 and 22 with color 0
make_dislocation(<7.018861,13.000000,7.018861>, <8.697445,11.837410,7.534854>, 0)
// segment connecting nodes 9 and 18 with color 0
make_dislocation(<13.000000,13.000000,13.000000>, <12.670320,6.924030,6.594350>, 0)
// WriteFan([0, 1, (6, 10, 21, 20, 19, 31, 5)])
// segment connecting nodes 10 and 6 with color 0
make_dislocation(<13.000000,6.000000,6.000000>, <13.000000,0.000000,0.000000>, 0)
// triangle color 1 nodes: [6, 10, 21]
make_triangle(<13.000000,0.000000,0.000000>, <13.000000,6.000000,6.000000>, <11.906220,5.981126,4.887346>, 1)
// segment connecting nodes 21 and 10 with color 0
make_dislocation(<11.906220,5.981126,4.887346>, <13.000000,6.000000,6.000000>, 0)
// triangle color 1 nodes: [6, 21, 20]
make_triangle(<13.000000,0.000000,0.000000>, <11.906220,5.981126,4.887346>, <9.749181,7.465570,4.214751>, 1)
// segment connecting nodes 20 and 21 with color 0
make_dislocation(<9.749181,7.465570,4.214751>, <11.906220,5.981126,4.887346>, 0)
// triangle color 1 nodes: [6, 20, 19]
make_triangle(<13.000000,0.000000,0.000000>, <9.749181,7.465570,4.214751>, <7.450931,10.173750,4.624686>, 1)
// segment connecting nodes 19 and 20 with color 0
make_dislocation(<7.450931,10.173750,4.624686>, <9.749181,7.465570,4.214751>, 0)
// triangle color 1 nodes: [6, 19, 31]
make_triangle(<13.000000,0.000000,0.000000>, <7.450931,10.173750,4.624686>, <6.000000,13.000000,6.000000>, 1)
// segment connecting nodes 31 and 19 with color 0
make_dislocation(<6.000000,13.000000,6.000000>, <7.450931,10.173750,4.624686>, 0)
// triangle color 1 nodes: [6, 31, 5]
make_triangle(<13.000000,0.000000,0.000000>, <6.000000,13.000000,6.000000>, <0.000000,13.000000,0.000000>, 1)
// segment connecting nodes 5 and 31 with color 0
make_dislocation(<0.000000,13.000000,0.000000>, <6.000000,13.000000,6.000000>, 0)
// segment connecting nodes 6 and 21 with color 0
make_dislocation(<13.000000,0.000000,0.000000>, <11.906220,5.981126,4.887346>, 0)
// WriteFan([3, 1, (9, 5, 7)])
// segment connecting nodes 5 and 9 with color 3
make_dislocation(<0.000000,13.000000,0.000000>, <13.000000,13.000000,13.000000>, 3)
// triangle color 1 nodes: [9, 5, 7]
make_triangle(<13.000000,13.000000,13.000000>, <0.000000,13.000000,0.000000>, <0.000000,0.000000,13.000000>, 1)
// segment connecting nodes 7 and 5 with color 3
make_dislocation(<0.000000,0.000000,13.000000>, <0.000000,13.000000,0.000000>, 3)
// segment connecting nodes 9 and 7 with color 3
make_dislocation(<13.000000,13.000000,13.000000>, <0.000000,0.000000,13.000000>, 3)
// WriteFan([3, 1, (5, 6, 7)])
// segment connecting nodes 6 and 5 with color 3
make_dislocation(<13.000000,0.000000,0.000000>, <0.000000,13.000000,0.000000>, 3)
// triangle color 1 nodes: [5, 6, 7]
make_triangle(<0.000000,13.000000,0.000000>, <13.000000,0.000000,0.000000>, <0.000000,0.000000,13.000000>, 1)
// segment connecting nodes 7 and 6 with color 3
make_dislocation(<0.000000,0.000000,13.000000>, <13.000000,0.000000,0.000000>, 3)
// segment connecting nodes 5 and 7 with color 3
make_dislocation(<0.000000,13.000000,0.000000>, <0.000000,0.000000,13.000000>, 3)
// WriteFan([3, 1, (9, 6, 7)])
// segment connecting nodes 6 and 9 with color 3
make_dislocation(<13.000000,0.000000,0.000000>, <13.000000,13.000000,13.000000>, 3)
// triangle color 1 nodes: [9, 6, 7]
make_triangle(<13.000000,13.000000,13.000000>, <13.000000,0.000000,0.000000>, <0.000000,0.000000,13.000000>, 1)
// segment connecting nodes 7 and 6 with color 3
make_dislocation(<0.000000,0.000000,13.000000>, <13.000000,0.000000,0.000000>, 3)
// segment connecting nodes 9 and 7 with color 3
make_dislocation(<13.000000,13.000000,13.000000>, <0.000000,0.000000,13.000000>, 3)
// WriteStrip([1, 1, (1, 12, 66, 112, 116, 115, 155, 57, 56, 114, 156, 105, 104, 33, 32, 140, 83, 82, 84, 65, 64, 109, 44, 43, 93, 132, 133, 92, 91, 99, 98, 128, 51, 50, 145, 39), (4, 108, 107, 111, 110, 113, 138, 139, 106, 63, 62, 148, 80, 79, 122, 121, 81, 61, 60, 41, 40, 144, 30, 29, 45, 89, 90, 87, 69, 49, 48, 131, 143, 71, 70, 28, 27, 85)])
// segment connecting nodes 4 and 108 with color 1
make_dislocation(<84.000000,-66.000000,7.000000>, <83.328310,-66.976200,8.647893>, 1)
// segment connecting nodes 108 and 107 with color 1
make_dislocation(<83.328310,-66.976200,8.647893>, <80.904150,-67.948960,12.044810>, 1)
// segment connecting nodes 107 and 111 with color 1
make_dislocation(<80.904150,-67.948960,12.044810>, <77.532900,-68.067930,15.535040>, 1)
// segment connecting nodes 111 and 110 with color 1
make_dislocation(<77.532900,-68.067930,15.535040>, <73.951130,-67.502980,18.551850>, 1)
// segment connecting nodes 110 and 113 with color 1
make_dislocation(<73.951130,-67.502980,18.551850>, <70.391850,-66.512370,21.120520>, 1)
// segment connecting nodes 113 and 138 with color 1
make_dislocation(<70.391850,-66.512370,21.120520>, <66.772380,-65.190260,23.417870>, 1)
// segment connecting nodes 138 and 139 with color 1
make_dislocation(<66.772380,-65.190260,23.417870>, <63.074150,-63.581820,25.507670>, 1)
// segment connecting nodes 139 and 106 with color 1
make_dislocation(<63.074150,-63.581820,25.507670>, <59.393260,-61.766640,27.373380>, 1)
// segment connecting nodes 106 and 63 with color 1
make_dislocation(<59.393260,-61.766640,27.373380>, <55.856080,-59.847310,28.991220>, 1)
// segment connecting nodes 63 and 62 with color 1
make_dislocation(<55.856080,-59.847310,28.991220>, <52.125590,-57.661760,30.536170>, 1)
// segment connecting nodes 62 and 148 with color 1
make_dislocation(<52.125590,-57.661760,30.536170>, <48.544900,-55.426460,31.881560>, 1)
// segment connecting nodes 148 and 80 with color 1
make_dislocation(<48.544900,-55.426460,31.881560>, <44.919000,-53.035420,33.116420>, 1)
// segment connecting nodes 80 and 79 with color 1
make_dislocation(<44.919000,-53.035420,33.116420>, <41.845730,-50.907640,34.061900>, 1)
// segment connecting nodes 79 and 122 with color 1
make_dislocation(<41.845730,-50.907640,34.061900>, <38.860820,-48.748370,34.887550>, 1)
// segment connecting nodes 122 and 121 with color 1
make_dislocation(<38.860820,-48.748370,34.887550>, <35.991510,-46.589460,35.597950>, 1)
// segment connecting nodes 121 and 81 with color 1
make_dislocation(<35.991510,-46.589460,35.597950>, <33.188150,-44.402040,36.213890>, 1)
// segment connecting nodes 81 and 61 with color 1
make_dislocation(<33.188150,-44.402040,36.213890>, <30.477750,-42.213690,36.735940>, 1)
// segment connecting nodes 61 and 60 with color 1
make_dislocation(<30.477750,-42.213690,36.735940>, <28.042930,-40.185530,37.142590>, 1)
// segment connecting nodes 60 and 41 with color 1
make_dislocation(<28.042930,-40.185530,37.142590>, <25.834610,-38.294150,37.459530>, 1)
// segment connecting nodes 41 and 40 with color 1
make_dislocation(<25.834610,-38.294150,37.459530>, <23.826370,-36.532440,37.706070>, 1)
// segment connecting nodes 40 and 144 with color 1
make_dislocation(<23.826370,-36.532440,37.706070>, <20.998690,-33.982710,37.984000>, 1)
// segment connecting nodes 144 and 30 with color 1
make_dislocation(<20.998690,-33.982710,37.984000>, <17.856180,-31.049230,38.193040>, 1)
// segment connecting nodes 30 and 29 with color 1
make_dislocation(<17.856180,-31.049230,38.193040>, <15.846210,-29.120410,38.274200>, 1)
// segment connecting nodes 29 and 45 with color 1
make_dislocation(<15.846210,-29.120410,38.274200>, <12.701670,-26.022240,38.320570>, 1)
// segment connecting nodes 45 and 89 with color 1
make_dislocation(<12.701670,-26.022240,38.320570>, <9.325649,-22.567930,38.242280>, 1)
// segment connecting nodes 89 and 90 with color 1
make_dislocation(<9.325649,-22.567930,38.242280>, <6.434226,-19.503980,38.069750>, 1)
// segment connecting nodes 90 and 87 with color 1
make_dislocation(<6.434226,-19.503980,38.069750>, <4.292584,-17.183870,37.891290>, 1)
// segment connecting nodes 87 and 69 with color 1
make_dislocation(<4.292584,-17.183870,37.891290>, <2.201280,-14.883030,37.681750>, 1)
// segment connecting nodes 69 and 49 with color 1
make_dislocation(<2.201280,-14.883030,37.681750>, <-0.254824,-12.149220,37.404040>, 1)
// segment connecting nodes 49 and 48 with color 1
make_dislocation(<-0.254824,-12.149220,37.404040>, <-2.538184,-9.574737,37.112920>, 1)
// segment connecting nodes 48 and 131 with color 1
make_dislocation(<-2.538184,-9.574737,37.112920>, <-4.183297,-7.698255,36.881550>, 1)
// segment connecting nodes 131 and 143 with color 1
make_dislocation(<-4.183297,-7.698255,36.881550>, <-5.820786,-5.821722,36.642510>, 1)
// segment connecting nodes 143 and 71 with color 1
make_dislocation(<-5.820786,-5.821722,36.642510>, <-8.177766,-3.114519,36.292290>, 1)
// segment connecting nodes 71 and 70 with color 1
make_dislocation(<-8.177766,-3.114519,36.292290>, <-11.150050,0.317705,35.832350>, 1)
// segment connecting nodes 70 and 28 with color 1
make_dislocation(<-11.150050,0.317705,35.832350>, <-13.236180,2.733500,35.502680>, 1)
// segment connecting nodes 28 and 27 with color 1
make_dislocation(<-13.236180,2.733500,35.502680>, <-15.133910,4.926839,35.207080>, 1)
// segment connecting nodes 27 and 85 with color 1
make_dislocation(<-15.133910,4.926839,35.207080>, <-17.202260,7.307394,34.894860>, 1)
// segment connecting nodes 1 and 12 with color 1
make_dislocation(<85.000000,-65.000000,5.000000>, <84.021210,-64.903720,5.882507>, 1)
// segment connecting nodes 12 and 66 with color 1
make_dislocation(<84.021210,-64.903720,5.882507>, <80.224960,-63.862030,8.637073>, 1)
// segment connecting nodes 66 and 112 with color 1
make_dislocation(<80.224960,-63.862030,8.637073>, <77.579740,-63.149340,10.569600>, 1)
// segment connecting nodes 112 and 116 with color 1
make_dislocation(<77.579740,-63.149340,10.569600>, <74.733970,-62.328120,12.594150>, 1)
// segment connecting nodes 116 and 115 with color 1
make_dislocation(<74.733970,-62.328120,12.594150>, <72.804930,-61.717450,13.912510>, 1)
// segment connecting nodes 115 and 155 with color 1
make_dislocation(<72.804930,-61.717450,13.912510>, <70.405740,-60.891830,15.486070>, 1)
// segment connecting nodes 155 and 57 with color 1
make_dislocation(<70.405740,-60.891830,15.486070>, <66.956270,-59.562380,17.606090>, 1)
// segment connecting nodes 57 and 56 with color 1
make_dislocation(<66.956270,-59.562380,17.606090>, <63.884030,-58.248510,19.364440>, 1)
// segment connecting nodes 56 and 114 with color 1
make_dislocation(<63.884030,-58.248510,19.364440>, <60.249400,-56.563790,21.314360>, 1)
// segment connecting nodes 114 and 156 with color 1
make_dislocation(<60.249400,-56.563790,21.314360>, <57.085410,-54.964240,22.878780>, 1)
// segment connecting nodes 156 and 105 with color 1
make_dislocation(<57.085410,-54.964240,22.878780>, <54.038860,-53.319610,24.280720>, 1)
// segment connecting nodes 105 and 104 with color 1
make_dislocation(<54.038860,-53.319610,24.280720>, <51.206700,-51.719240,25.512510>, 1)
// segment connecting nodes 104 and 33 with color 1
make_dislocation(<51.206700,-51.719240,25.512510>, <49.821510,-50.904080,26.082540>, 1)
// segment connecting nodes 33 and 32 with color 1
make_dislocation(<49.821510,-50.904080,26.082540>, <46.064900,-48.598520,27.533610>, 1)
// segment connecting nodes 32 and 140 with color 1
make_dislocation(<46.064900,-48.598520,27.533610>, <43.469030,-46.917210,28.448170>, 1)
// segment connecting nodes 140 and 83 with color 1
make_dislocation(<43.469030,-46.917210,28.448170>, <40.213650,-44.715530,29.501880>, 1)
// segment connecting nodes 83 and 82 with color 1
make_dislocation(<40.213650,-44.715530,29.501880>, <36.297990,-41.922330,30.624330>, 1)
// segment connecting nodes 82 and 84 with color 1
make_dislocation(<36.297990,-41.922330,30.624330>, <33.910690,-40.135160,31.224460>, 1)
// segment connecting nodes 84 and 65 with color 1
make_dislocation(<33.910690,-40.135160,31.224460>, <31.210000,-38.034730,31.824730>, 1)
// segment connecting nodes 65 and 64 with color 1
make_dislocation(<31.210000,-38.034730,31.824730>, <28.383080,-35.745080,32.362000>, 1)
// segment connecting nodes 64 and 109 with color 1
make_dislocation(<28.383080,-35.745080,32.362000>, <25.553970,-33.358390,32.804410>, 1)
// segment connecting nodes 109 and 44 with color 1
make_dislocation(<25.553970,-33.358390,32.804410>, <22.696440,-30.847150,33.150710>, 1)
// segment connecting nodes 44 and 43 with color 1
make_dislocation(<22.696440,-30.847150,33.150710>, <19.785680,-28.180870,33.395190>, 1)
// segment connecting nodes 43 and 93 with color 1
make_dislocation(<19.785680,-28.180870,33.395190>, <16.857370,-25.382110,33.524740>, 1)
// segment connecting nodes 93 and 132 with color 1
make_dislocation(<16.857370,-25.382110,33.524740>, <13.813940,-22.339140,33.525200>, 1)
// segment connecting nodes 132 and 133 with color 1
make_dislocation(<13.813940,-22.339140,33.525200>, <10.891020,-19.265820,33.374810>, 1)
// segment connecting nodes 133 and 92 with color 1
make_dislocation(<10.891020,-19.265820,33.374810>, <8.219155,-16.287840,33.068680>, 1)
// segment connecting nodes 92 and 91 with color 1
make_dislocation(<8.219155,-16.287840,33.068680>, <5.360350,-12.863400,32.503040>, 1)
// segment connecting nodes 91 and 99 with color 1
make_dislocation(<5.360350,-12.863400,32.503040>, <3.645867,-10.660260,32.014390>, 1)
// segment connecting nodes 99 and 98 with color 1
make_dislocation(<3.645867,-10.660260,32.014390>, <1.823855,-8.148223,31.324370>, 1)
// segment connecting nodes 98 and 128 with color 1
make_dislocation(<1.823855,-8.148223,31.324370>, <-0.207991,-4.963054,30.171050>, 1)
// segment connecting nodes 128 and 51 with color 1
make_dislocation(<-0.207991,-4.963054,30.171050>, <-1.940544,-1.486137,28.426680>, 1)
// segment connecting nodes 51 and 50 with color 1
make_dislocation(<-1.940544,-1.486137,28.426680>, <-2.882485,1.664093,26.218390>, 1)
// segment connecting nodes 50 and 145 with color 1
make_dislocation(<-2.882485,1.664093,26.218390>, <-2.833417,4.341197,23.492220>, 1)
// segment connecting nodes 145 and 39 with color 1
make_dislocation(<-2.833417,4.341197,23.492220>, <-1.686056,6.182347,20.503710>, 1)
// triangle color 1 nodes: [4, 1, 108]
make_triangle(<84.000000,-66.000000,7.000000>, <85.000000,-65.000000,5.000000>, <83.328310,-66.976200,8.647893>, 1)
// triangle color 1 nodes: [108, 1, 12]
make_triangle(<83.328310,-66.976200,8.647893>, <85.000000,-65.000000,5.000000>, <84.021210,-64.903720,5.882507>, 1)
// triangle color 1 nodes: [108, 12, 107]
make_triangle(<83.328310,-66.976200,8.647893>, <84.021210,-64.903720,5.882507>, <80.904150,-67.948960,12.044810>, 1)
// triangle color 1 nodes: [107, 12, 66]
make_triangle(<80.904150,-67.948960,12.044810>, <84.021210,-64.903720,5.882507>, <80.224960,-63.862030,8.637073>, 1)
// triangle color 1 nodes: [107, 66, 111]
make_triangle(<80.904150,-67.948960,12.044810>, <80.224960,-63.862030,8.637073>, <77.532900,-68.067930,15.535040>, 1)
// triangle color 1 nodes: [111, 66, 112]
make_triangle(<77.532900,-68.067930,15.535040>, <80.224960,-63.862030,8.637073>, <77.579740,-63.149340,10.569600>, 1)
// triangle color 1 nodes: [111, 112, 110]
make_triangle(<77.532900,-68.067930,15.535040>, <77.579740,-63.149340,10.569600>, <73.951130,-67.502980,18.551850>, 1)
// triangle color 1 nodes: [110, 112, 116]
make_triangle(<73.951130,-67.502980,18.551850>, <77.579740,-63.149340,10.569600>, <74.733970,-62.328120,12.594150>, 1)
// triangle color 1 nodes: [110, 116, 113]
make_triangle(<73.951130,-67.502980,18.551850>, <74.733970,-62.328120,12.594150>, <70.391850,-66.512370,21.120520>, 1)
// triangle color 1 nodes: [113, 116, 115]
make_triangle(<70.391850,-66.512370,21.120520>, <74.733970,-62.328120,12.594150>, <72.804930,-61.717450,13.912510>, 1)
// triangle color 1 nodes: [113, 115, 138]
make_triangle(<70.391850,-66.512370,21.120520>, <72.804930,-61.717450,13.912510>, <66.772380,-65.190260,23.417870>, 1)
// triangle color 1 nodes: [138, 115, 155]
make_triangle(<66.772380,-65.190260,23.417870>, <72.804930,-61.717450,13.912510>, <70.405740,-60.891830,15.486070>, 1)
// triangle color 1 nodes: [138, 155, 139]
make_triangle(<66.772380,-65.190260,23.417870>, <70.405740,-60.891830,15.486070>, <63.074150,-63.581820,25.507670>, 1)
// triangle color 1 nodes: [139, 155, 57]
make_triangle(<63.074150,-63.581820,25.507670>, <70.405740,-60.891830,15.486070>, <66.956270,-59.562380,17.606090>, 1)
// triangle color 1 nodes: [139, 57, 106]
make_triangle(<63.074150,-63.581820,25.507670>, <66.956270,-59.562380,17.606090>, <59.393260,-61.766640,27.373380>, 1)
// triangle color 1 nodes: [106, 57, 56]
make_triangle(<59.393260,-61.766640,27.373380>, <66.956270,-59.562380,17.606090>, <63.884030,-58.248510,19.364440>, 1)
// triangle color 1 nodes: [106, 56, 63]
make_triangle(<59.393260,-61.766640,27.373380>, <63.884030,-58.248510,19.364440>, <55.856080,-59.847310,28.991220>, 1)
// triangle color 1 nodes: [63, 56, 114]
make_triangle(<55.856080,-59.847310,28.991220>, <63.884030,-58.248510,19.364440>, <60.249400,-56.563790,21.314360>, 1)
// triangle color 1 nodes: [63, 114, 62]
make_triangle(<55.856080,-59.847310,28.991220>, <60.249400,-56.563790,21.314360>, <52.125590,-57.661760,30.536170>, 1)
// triangle color 1 nodes: [62, 114, 156]
make_triangle(<52.125590,-57.661760,30.536170>, <60.249400,-56.563790,21.314360>, <57.085410,-54.964240,22.878780>, 1)
// triangle color 1 nodes: [62, 156, 148]
make_triangle(<52.125590,-57.661760,30.536170>, <57.085410,-54.964240,22.878780>, <48.544900,-55.426460,31.881560>, 1)
// triangle color 1 nodes: [148, 156, 105]
make_triangle(<48.544900,-55.426460,31.881560>, <57.085410,-54.964240,22.878780>, <54.038860,-53.319610,24.280720>, 1)
// triangle color 1 nodes: [148, 105, 80]
make_triangle(<48.544900,-55.426460,31.881560>, <54.038860,-53.319610,24.280720>, <44.919000,-53.035420,33.116420>, 1)
// triangle color 1 nodes: [80, 105, 104]
make_triangle(<44.919000,-53.035420,33.116420>, <54.038860,-53.319610,24.280720>, <51.206700,-51.719240,25.512510>, 1)
// triangle color 1 nodes: [80, 104, 79]
make_triangle(<44.919000,-53.035420,33.116420>, <51.206700,-51.719240,25.512510>, <41.845730,-50.907640,34.061900>, 1)
// triangle color 1 nodes: [79, 104, 33]
make_triangle(<41.845730,-50.907640,34.061900>, <51.206700,-51.719240,25.512510>, <49.821510,-50.904080,26.082540>, 1)
// triangle color 1 nodes: [79, 33, 122]
make_triangle(<41.845730,-50.907640,34.061900>, <49.821510,-50.904080,26.082540>, <38.860820,-48.748370,34.887550>, 1)
// triangle color 1 nodes: [122, 33, 32]
make_triangle(<38.860820,-48.748370,34.887550>, <49.821510,-50.904080,26.082540>, <46.064900,-48.598520,27.533610>, 1)
// triangle color 1 nodes: [122, 32, 121]
make_triangle(<38.860820,-48.748370,34.887550>, <46.064900,-48.598520,27.533610>, <35.991510,-46.589460,35.597950>, 1)
// triangle color 1 nodes: [121, 32, 140]
make_triangle(<35.991510,-46.589460,35.597950>, <46.064900,-48.598520,27.533610>, <43.469030,-46.917210,28.448170>, 1)
// triangle color 1 nodes: [121, 140, 81]
make_triangle(<35.991510,-46.589460,35.597950>, <43.469030,-46.917210,28.448170>, <33.188150,-44.402040,36.213890>, 1)
// triangle color 1 nodes: [81, 140, 83]
make_triangle(<33.188150,-44.402040,36.213890>, <43.469030,-46.917210,28.448170>, <40.213650,-44.715530,29.501880>, 1)
// triangle color 1 nodes: [81, 83, 61]
make_triangle(<33.188150,-44.402040,36.213890>, <40.213650,-44.715530,29.501880>, <30.477750,-42.213690,36.735940>, 1)
// triangle color 1 nodes: [61, 83, 82]
make_triangle(<30.477750,-42.213690,36.735940>, <40.213650,-44.715530,29.501880>, <36.297990,-41.922330,30.624330>, 1)
// triangle color 1 nodes: [61, 82, 60]
make_triangle(<30.477750,-42.213690,36.735940>, <36.297990,-41.922330,30.624330>, <28.042930,-40.185530,37.142590>, 1)
// triangle color 1 nodes: [60, 82, 84]
make_triangle(<28.042930,-40.185530,37.142590>, <36.297990,-41.922330,30.624330>, <33.910690,-40.135160,31.224460>, 1)
// triangle color 1 nodes: [60, 84, 41]
make_triangle(<28.042930,-40.185530,37.142590>, <33.910690,-40.135160,31.224460>, <25.834610,-38.294150,37.459530>, 1)
// triangle color 1 nodes: [41, 84, 65]
make_triangle(<25.834610,-38.294150,37.459530>, <33.910690,-40.135160,31.224460>, <31.210000,-38.034730,31.824730>, 1)
// triangle color 1 nodes: [41, 65, 40]
make_triangle(<25.834610,-38.294150,37.459530>, <31.210000,-38.034730,31.824730>, <23.826370,-36.532440,37.706070>, 1)
// triangle color 1 nodes: [40, 65, 64]
make_triangle(<23.826370,-36.532440,37.706070>, <31.210000,-38.034730,31.824730>, <28.383080,-35.745080,32.362000>, 1)
// triangle color 1 nodes: [40, 64, 144]
make_triangle(<23.826370,-36.532440,37.706070>, <28.383080,-35.745080,32.362000>, <20.998690,-33.982710,37.984000>, 1)
// triangle color 1 nodes: [144, 64, 109]
make_triangle(<20.998690,-33.982710,37.984000>, <28.383080,-35.745080,32.362000>, <25.553970,-33.358390,32.804410>, 1)
// triangle color 1 nodes: [144, 109, 30]
make_triangle(<20.998690,-33.982710,37.984000>, <25.553970,-33.358390,32.804410>, <17.856180,-31.049230,38.193040>, 1)
// triangle color 1 nodes: [30, 109, 44]
make_triangle(<17.856180,-31.049230,38.193040>, <25.553970,-33.358390,32.804410>, <22.696440,-30.847150,33.150710>, 1)
// triangle color 1 nodes: [30, 44, 29]
make_triangle(<17.856180,-31.049230,38.193040>, <22.696440,-30.847150,33.150710>, <15.846210,-29.120410,38.274200>, 1)
// triangle color 1 nodes: [29, 44, 43]
make_triangle(<15.846210,-29.120410,38.274200>, <22.696440,-30.847150,33.150710>, <19.785680,-28.180870,33.395190>, 1)
// triangle color 1 nodes: [29, 43, 45]
make_triangle(<15.846210,-29.120410,38.274200>, <19.785680,-28.180870,33.395190>, <12.701670,-26.022240,38.320570>, 1)
// triangle color 1 nodes: [45, 43, 93]
make_triangle(<12.701670,-26.022240,38.320570>, <19.785680,-28.180870,33.395190>, <16.857370,-25.382110,33.524740>, 1)
// triangle color 1 nodes: [45, 93, 89]
make_triangle(<12.701670,-26.022240,38.320570>, <16.857370,-25.382110,33.524740>, <9.325649,-22.567930,38.242280>, 1)
// triangle color 1 nodes: [89, 93, 132]
make_triangle(<9.325649,-22.567930,38.242280>, <16.857370,-25.382110,33.524740>, <13.813940,-22.339140,33.525200>, 1)
// triangle color 1 nodes: [89, 132, 90]
make_triangle(<9.325649,-22.567930,38.242280>, <13.813940,-22.339140,33.525200>, <6.434226,-19.503980,38.069750>, 1)
// triangle color 1 nodes: [90, 132, 133]
make_triangle(<6.434226,-19.503980,38.069750>, <13.813940,-22.339140,33.525200>, <10.891020,-19.265820,33.374810>, 1)
// triangle color 1 nodes: [90, 133, 87]
make_triangle(<6.434226,-19.503980,38.069750>, <10.891020,-19.265820,33.374810>, <4.292584,-17.183870,37.891290>, 1)
// triangle color 1 nodes: [87, 133, 92]
make_triangle(<4.292584,-17.183870,37.891290>, <10.891020,-19.265820,33.374810>, <8.219155,-16.287840,33.068680>, 1)
// triangle color 1 nodes: [87, 92, 69]
make_triangle(<4.292584,-17.183870,37.891290>, <8.219155,-16.287840,33.068680>, <2.201280,-14.883030,37.681750>, 1)
// triangle color 1 nodes: [69, 92, 91]
make_triangle(<2.201280,-14.883030,37.681750>, <8.219155,-16.287840,33.068680>, <5.360350,-12.863400,32.503040>, 1)
// triangle color 1 nodes: [69, 91, 49]
make_triangle(<2.201280,-14.883030,37.681750>, <5.360350,-12.863400,32.503040>, <-0.254824,-12.149220,37.404040>, 1)
// triangle color 1 nodes: [49, 91, 99]
make_triangle(<-0.254824,-12.149220,37.404040>, <5.360350,-12.863400,32.503040>, <3.645867,-10.660260,32.014390>, 1)
// triangle color 1 nodes: [49, 99, 48]
make_triangle(<-0.254824,-12.149220,37.404040>, <3.645867,-10.660260,32.014390>, <-2.538184,-9.574737,37.112920>, 1)
// triangle color 1 nodes: [48, 99, 98]
make_triangle(<-2.538184,-9.574737,37.112920>, <3.645867,-10.660260,32.014390>, <1.823855,-8.148223,31.324370>, 1)
// triangle color 1 nodes: [48, 98, 131]
make_triangle(<-2.538184,-9.574737,37.112920>, <1.823855,-8.148223,31.324370>, <-4.183297,-7.698255,36.881550>, 1)
// triangle color 1 nodes: [131, 98, 128]
make_triangle(<-4.183297,-7.698255,36.881550>, <1.823855,-8.148223,31.324370>, <-0.207991,-4.963054,30.171050>, 1)
// triangle color 1 nodes: [131, 128, 143]
make_triangle(<-4.183297,-7.698255,36.881550>, <-0.207991,-4.963054,30.171050>, <-5.820786,-5.821722,36.642510>, 1)
// triangle color 1 nodes: [143, 128, 51]
make_triangle(<-5.820786,-5.821722,36.642510>, <-0.207991,-4.963054,30.171050>, <-1.940544,-1.486137,28.426680>, 1)
// triangle color 1 nodes: [143, 51, 71]
make_triangle(<-5.820786,-5.821722,36.642510>, <-1.940544,-1.486137,28.426680>, <-8.177766,-3.114519,36.292290>, 1)
// triangle color 1 nodes: [71, 51, 50]
make_triangle(<-8.177766,-3.114519,36.292290>, <-1.940544,-1.486137,28.426680>, <-2.882485,1.664093,26.218390>, 1)
// triangle color 1 nodes: [71, 50, 70]
make_triangle(<-8.177766,-3.114519,36.292290>, <-2.882485,1.664093,26.218390>, <-11.150050,0.317705,35.832350>, 1)
// triangle color 1 nodes: [70, 50, 145]
make_triangle(<-11.150050,0.317705,35.832350>, <-2.882485,1.664093,26.218390>, <-2.833417,4.341197,23.492220>, 1)
// triangle color 1 nodes: [70, 145, 28]
make_triangle(<-11.150050,0.317705,35.832350>, <-2.833417,4.341197,23.492220>, <-13.236180,2.733500,35.502680>, 1)
// triangle color 1 nodes: [28, 145, 39]
make_triangle(<-13.236180,2.733500,35.502680>, <-2.833417,4.341197,23.492220>, <-1.686056,6.182347,20.503710>, 1)
// triangle color 1 nodes: [28, 39, 27]
make_triangle(<-13.236180,2.733500,35.502680>, <-1.686056,6.182347,20.503710>, <-15.133910,4.926839,35.207080>, 1)
// WriteStrip([1, 1, (85, 126, 125, 124, 86, 35, 34, 94, 53, 52, 75, 117, 118, 74, 73, 146, 55, 54, 95, 135, 134, 97, 96, 47, 46, 157, 142, 2), (68, 67, 123, 141, 24, 23, 130, 129, 26, 25, 38, 37, 78, 119, 120, 77, 76, 147, 59, 58, 101, 137, 136, 103, 102, 150, 152, 151, 154, 153, 149, 3)])
// segment connecting nodes 68 and 67 with color 1
make_dislocation(<-4.173561,10.833960,18.342230>, <-7.465161,11.977610,20.491560>, 1)
// segment connecting nodes 67 and 123 with color 1
make_dislocation(<-7.465161,11.977610,20.491560>, <-11.383500,13.834040,22.553750>, 1)
// segment connecting nodes 123 and 141 with color 1
make_dislocation(<-11.383500,13.834040,22.553750>, <-14.965010,15.867670,24.100970>, 1)
// segment connecting nodes 141 and 24 with color 1
make_dislocation(<-14.965010,15.867670,24.100970>, <-17.907420,17.751020,25.158880>, 1)
// segment connecting nodes 24 and 23 with color 1
make_dislocation(<-17.907420,17.751020,25.158880>, <-21.531880,20.341770,26.190460>, 1)
// segment connecting nodes 23 and 130 with color 1
make_dislocation(<-21.531880,20.341770,26.190460>, <-23.602190,21.967690,26.634440>, 1)
// segment connecting nodes 130 and 129 with color 1
make_dislocation(<-23.602190,21.967690,26.634440>, <-26.935950,24.793860,27.142050>, 1)
// segment connecting nodes 129 and 26 with color 1
make_dislocation(<-26.935950,24.793860,27.142050>, <-29.936620,27.579040,27.357550>, 1)
// segment connecting nodes 26 and 25 with color 1
make_dislocation(<-29.936620,27.579040,27.357550>, <-31.682290,29.298680,27.383600>, 1)
// segment connecting nodes 25 and 38 with color 1
make_dislocation(<-31.682290,29.298680,27.383600>, <-34.483980,32.199700,27.284280>, 1)
// segment connecting nodes 38 and 37 with color 1
make_dislocation(<-34.483980,32.199700,27.284280>, <-37.422600,35.447510,26.975100>, 1)
// segment connecting nodes 37 and 78 with color 1
make_dislocation(<-37.422600,35.447510,26.975100>, <-39.187510,37.500130,26.687370>, 1)
// segment connecting nodes 78 and 119 with color 1
make_dislocation(<-39.187510,37.500130,26.687370>, <-41.147060,39.864750,26.282310>, 1)
// segment connecting nodes 119 and 120 with color 1
make_dislocation(<-41.147060,39.864750,26.282310>, <-43.189880,42.438620,25.751260>, 1)
// segment connecting nodes 120 and 77 with color 1
make_dislocation(<-43.189880,42.438620,25.751260>, <-45.278500,45.197640,25.080850>, 1)
// segment connecting nodes 77 and 76 with color 1
make_dislocation(<-45.278500,45.197640,25.080850>, <-47.305050,48.010280,24.294770>, 1)
// segment connecting nodes 76 and 147 with color 1
make_dislocation(<-47.305050,48.010280,24.294770>, <-49.227760,50.818650,23.409120>, 1)
// segment connecting nodes 147 and 59 with color 1
make_dislocation(<-49.227760,50.818650,23.409120>, <-51.033430,53.599820,22.433610>, 1)
// segment connecting nodes 59 and 58 with color 1
make_dislocation(<-51.033430,53.599820,22.433610>, <-52.715200,56.338360,21.376830>, 1)
// segment connecting nodes 58 and 101 with color 1
make_dislocation(<-52.715200,56.338360,21.376830>, <-54.272350,59.027740,20.244620>, 1)
// segment connecting nodes 101 and 137 with color 1
make_dislocation(<-54.272350,59.027740,20.244620>, <-55.704340,61.661290,19.043050>, 1)
// segment connecting nodes 137 and 136 with color 1
make_dislocation(<-55.704340,61.661290,19.043050>, <-57.022830,64.255840,17.766990>, 1)
// segment connecting nodes 136 and 103 with color 1
make_dislocation(<-57.022830,64.255840,17.766990>, <-58.227570,66.806920,16.420660>, 1)
// segment connecting nodes 103 and 102 with color 1
make_dislocation(<-58.227570,66.806920,16.420660>, <-59.326440,69.325900,15.000530>, 1)
// segment connecting nodes 102 and 150 with color 1
make_dislocation(<-59.326440,69.325900,15.000530>, <-60.322280,71.808770,13.513510>, 1)
// segment connecting nodes 150 and 152 with color 1
make_dislocation(<-60.322280,71.808770,13.513510>, <-61.235600,74.284290,11.951310>, 1)
// segment connecting nodes 152 and 151 with color 1
make_dislocation(<-61.235600,74.284290,11.951310>, <-61.998530,76.506210,10.492320>, 1)
// segment connecting nodes 151 and 154 with color 1
make_dislocation(<-61.998530,76.506210,10.492320>, <-62.798550,78.928400,8.870158>, 1)
// segment connecting nodes 154 and 153 with color 1
make_dislocation(<-62.798550,78.928400,8.870158>, <-63.544040,81.088580,7.455454>, 1)
// segment connecting nodes 153 and 149 with color 1
make_dislocation(<-63.544040,81.088580,7.455454>, <-64.788520,84.162860,5.625656>, 1)
// segment connecting nodes 149 and 3 with color 1
make_dislocation(<-64.788520,84.162860,5.625656>, <-65.000000,85.000000,5.000000>, 1)
// segment connecting nodes 85 and 126 with color 1
make_dislocation(<-17.202260,7.307394,34.894860>, <-19.572590,10.014460,34.558130>, 1)
// segment connecting nodes 126 and 125 with color 1
make_dislocation(<-19.572590,10.014460,34.558130>, <-21.967340,12.732250,34.235090>, 1)
// segment connecting nodes 125 and 124 with color 1
make_dislocation(<-21.967340,12.732250,34.235090>, <-23.969770,14.995390,33.974380>, 1)
// segment connecting nodes 124 and 86 with color 1
make_dislocation(<-23.969770,14.995390,33.974380>, <-25.923110,17.193310,33.729800>, 1)
// segment connecting nodes 86 and 35 with color 1
make_dislocation(<-25.923110,17.193310,33.729800>, <-28.488450,20.068820,33.419630>, 1)
// segment connecting nodes 35 and 34 with color 1
make_dislocation(<-28.488450,20.068820,33.419630>, <-31.252500,23.169120,33.083380>, 1)
// segment connecting nodes 34 and 94 with color 1
make_dislocation(<-31.252500,23.169120,33.083380>, <-33.822360,26.075840,32.746520>, 1)
// segment connecting nodes 94 and 53 with color 1
make_dislocation(<-33.822360,26.075840,32.746520>, <-36.338400,28.964000,32.374400>, 1)
// segment connecting nodes 53 and 52 with color 1
make_dislocation(<-36.338400,28.964000,32.374400>, <-38.703720,31.736670,31.967050>, 1)
// segment connecting nodes 52 and 75 with color 1
make_dislocation(<-38.703720,31.736670,31.967050>, <-40.961070,34.445730,31.515340>, 1)
// segment connecting nodes 75 and 117 with color 1
make_dislocation(<-40.961070,34.445730,31.515340>, <-43.163860,37.157890,31.005970>, 1)
// segment connecting nodes 117 and 118 with color 1
make_dislocation(<-43.163860,37.157890,31.005970>, <-45.295100,39.861000,30.434100>, 1)
// segment connecting nodes 118 and 74 with color 1
make_dislocation(<-45.295100,39.861000,30.434100>, <-47.406530,42.628140,29.778390>, 1)
// segment connecting nodes 74 and 73 with color 1
make_dislocation(<-47.406530,42.628140,29.778390>, <-49.327600,45.237360,29.090240>, 1)
// segment connecting nodes 73 and 146 with color 1
make_dislocation(<-49.327600,45.237360,29.090240>, <-51.278200,47.990930,28.287270>, 1)
// segment connecting nodes 146 and 55 with color 1
make_dislocation(<-51.278200,47.990930,28.287270>, <-53.249380,50.895170,27.354200>, 1)
// segment connecting nodes 55 and 54 with color 1
make_dislocation(<-53.249380,50.895170,27.354200>, <-55.244290,53.979490,26.264790>, 1)
// segment connecting nodes 54 and 95 with color 1
make_dislocation(<-55.244290,53.979490,26.264790>, <-57.271780,57.293670,24.978110>, 1)
// segment connecting nodes 95 and 135 with color 1
make_dislocation(<-57.271780,57.293670,24.978110>, <-59.323230,60.878440,23.444790>, 1)
// segment connecting nodes 135 and 134 with color 1
make_dislocation(<-59.323230,60.878440,23.444790>, <-61.325580,64.680510,21.645070>, 1)
// segment connecting nodes 134 and 97 with color 1
make_dislocation(<-61.325580,64.680510,21.645070>, <-63.145540,68.524910,19.620630>, 1)
// segment connecting nodes 97 and 96 with color 1
make_dislocation(<-63.145540,68.524910,19.620630>, <-64.694450,72.300130,17.394320>, 1)
// segment connecting nodes 96 and 47 with color 1
make_dislocation(<-64.694450,72.300130,17.394320>, <-65.832250,75.725460,15.106800>, 1)
// segment connecting nodes 47 and 46 with color 1
make_dislocation(<-65.832250,75.725460,15.106800>, <-66.439290,78.153430,13.285860>, 1)
// segment connecting nodes 46 and 157 with color 1
make_dislocation(<-66.439290,78.153430,13.285860>, <-66.829210,80.680640,11.148570>, 1)
// segment connecting nodes 157 and 142 with color 1
make_dislocation(<-66.829210,80.680640,11.148570>, <-66.707730,83.032420,8.675316>, 1)
// segment connecting nodes 142 and 2 with color 1
make_dislocation(<-66.707730,83.032420,8.675316>, <-66.000000,84.000000,7.000000>, 1)
// triangle color 1 nodes: [68, 85, 67]
make_triangle(<-4.173561,10.833960,18.342230>, <-17.202260,7.307394,34.894860>, <-7.465161,11.977610,20.491560>, 1)
// triangle color 1 nodes: [67, 85, 126]
make_triangle(<-7.465161,11.977610,20.491560>, <-17.202260,7.307394,34.894860>, <-19.572590,10.014460,34.558130>, 1)
// triangle color 1 nodes: [67, 126, 123]
make_triangle(<-7.465161,11.977610,20.491560>, <-19.572590,10.014460,34.558130>, <-11.383500,13.834040,22.553750>, 1)
// triangle color 1 nodes: [123, 126, 125]
make_triangle(<-11.383500,13.834040,22.553750>, <-19.572590,10.014460,34.558130>, <-21.967340,12.732250,34.235090>, 1)
// triangle color 1 nodes: [123, 125, 141]
make_triangle(<-11.383500,13.834040,22.553750>, <-21.967340,12.732250,34.235090>, <-14.965010,15.867670,24.100970>, 1)
// triangle color 1 nodes: [141, 125, 124]
make_triangle(<-14.965010,15.867670,24.100970>, <-21.967340,12.732250,34.235090>, <-23.969770,14.995390,33.974380>, 1)
// triangle color 1 nodes: [141, 124, 24]
make_triangle(<-14.965010,15.867670,24.100970>, <-23.969770,14.995390,33.974380>, <-17.907420,17.751020,25.158880>, 1)
// triangle color 1 nodes: [24, 124, 86]
make_triangle(<-17.907420,17.751020,25.158880>, <-23.969770,14.995390,33.974380>, <-25.923110,17.193310,33.729800>, 1)
// triangle color 1 nodes: [24, 86, 23]
make_triangle(<-17.907420,17.751020,25.158880>, <-25.923110,17.193310,33.729800>, <-21.531880,20.341770,26.190460>, 1)
// triangle color 1 nodes: [23, 86, 35]
make_triangle(<-21.531880,20.341770,26.190460>, <-25.923110,17.193310,33.729800>, <-28.488450,20.068820,33.419630>, 1)
// triangle color 1 nodes: [23, 35, 130]
make_triangle(<-21.531880,20.341770,26.190460>, <-28.488450,20.068820,33.419630>, <-23.602190,21.967690,26.634440>, 1)
// triangle color 1 nodes: [130, 35, 34]
make_triangle(<-23.602190,21.967690,26.634440>, <-28.488450,20.068820,33.419630>, <-31.252500,23.169120,33.083380>, 1)
// triangle color 1 nodes: [130, 34, 129]
make_triangle(<-23.602190,21.967690,26.634440>, <-31.252500,23.169120,33.083380>, <-26.935950,24.793860,27.142050>, 1)
// triangle color 1 nodes: [129, 34, 94]
make_triangle(<-26.935950,24.793860,27.142050>, <-31.252500,23.169120,33.083380>, <-33.822360,26.075840,32.746520>, 1)
// triangle color 1 nodes: [129, 94, 26]
make_triangle(<-26.935950,24.793860,27.142050>, <-33.822360,26.075840,32.746520>, <-29.936620,27.579040,27.357550>, 1)
// triangle color 1 nodes: [26, 94, 53]
make_triangle(<-29.936620,27.579040,27.357550>, <-33.822360,26.075840,32.746520>, <-36.338400,28.964000,32.374400>, 1)
// triangle color 1 nodes: [26, 53, 25]
make_triangle(<-29.936620,27.579040,27.357550>, <-36.338400,28.964000,32.374400>, <-31.682290,29.298680,27.383600>, 1)
// triangle color 1 nodes: [25, 53, 52]
make_triangle(<-31.682290,29.298680,27.383600>, <-36.338400,28.964000,32.374400>, <-38.703720,31.736670,31.967050>, 1)
// triangle color 1 nodes: [25, 52, 38]
make_triangle(<-31.682290,29.298680,27.383600>, <-38.703720,31.736670,31.967050>, <-34.483980,32.199700,27.284280>, 1)
// triangle color 1 nodes: [38, 52, 75]
make_triangle(<-34.483980,32.199700,27.284280>, <-38.703720,31.736670,31.967050>, <-40.961070,34.445730,31.515340>, 1)
// triangle color 1 nodes: [38, 75, 37]
make_triangle(<-34.483980,32.199700,27.284280>, <-40.961070,34.445730,31.515340>, <-37.422600,35.447510,26.975100>, 1)
// triangle color 1 nodes: [37, 75, 117]
make_triangle(<-37.422600,35.447510,26.975100>, <-40.961070,34.445730,31.515340>, <-43.163860,37.157890,31.005970>, 1)
// triangle color 1 nodes: [37, 117, 78]
make_triangle(<-37.422600,35.447510,26.975100>, <-43.163860,37.157890,31.005970>, <-39.187510,37.500130,26.687370>, 1)
// triangle color 1 nodes: [78, 117, 118]
make_triangle(<-39.187510,37.500130,26.687370>, <-43.163860,37.157890,31.005970>, <-45.295100,39.861000,30.434100>, 1)
// triangle color 1 nodes: [78, 118, 119]
make_triangle(<-39.187510,37.500130,26.687370>, <-45.295100,39.861000,30.434100>, <-41.147060,39.864750,26.282310>, 1)
// triangle color 1 nodes: [119, 118, 74]
make_triangle(<-41.147060,39.864750,26.282310>, <-45.295100,39.861000,30.434100>, <-47.406530,42.628140,29.778390>, 1)
// triangle color 1 nodes: [119, 74, 120]
make_triangle(<-41.147060,39.864750,26.282310>, <-47.406530,42.628140,29.778390>, <-43.189880,42.438620,25.751260>, 1)
// triangle color 1 nodes: [120, 74, 73]
make_triangle(<-43.189880,42.438620,25.751260>, <-47.406530,42.628140,29.778390>, <-49.327600,45.237360,29.090240>, 1)
// triangle color 1 nodes: [120, 73, 77]
make_triangle(<-43.189880,42.438620,25.751260>, <-49.327600,45.237360,29.090240>, <-45.278500,45.197640,25.080850>, 1)
// triangle color 1 nodes: [77, 73, 146]
make_triangle(<-45.278500,45.197640,25.080850>, <-49.327600,45.237360,29.090240>, <-51.278200,47.990930,28.287270>, 1)
// triangle color 1 nodes: [77, 146, 76]
make_triangle(<-45.278500,45.197640,25.080850>, <-51.278200,47.990930,28.287270>, <-47.305050,48.010280,24.294770>, 1)
// triangle color 1 nodes: [76, 146, 55]
make_triangle(<-47.305050,48.010280,24.294770>, <-51.278200,47.990930,28.287270>, <-53.249380,50.895170,27.354200>, 1)
// triangle color 1 nodes: [76, 55, 147]
make_triangle(<-47.305050,48.010280,24.294770>, <-53.249380,50.895170,27.354200>, <-49.227760,50.818650,23.409120>, 1)
// triangle color 1 nodes: [147, 55, 54]
make_triangle(<-49.227760,50.818650,23.409120>, <-53.249380,50.895170,27.354200>, <-55.244290,53.979490,26.264790>, 1)
// triangle color 1 nodes: [147, 54, 59]
make_triangle(<-49.227760,50.818650,23.409120>, <-55.244290,53.979490,26.264790>, <-51.033430,53.599820,22.433610>, 1)
// triangle color 1 nodes: [59, 54, 95]
make_triangle(<-51.033430,53.599820,22.433610>, <-55.244290,53.979490,26.264790>, <-57.271780,57.293670,24.978110>, 1)
// triangle color 1 nodes: [59, 95, 58]
make_triangle(<-51.033430,53.599820,22.433610>, <-57.271780,57.293670,24.978110>, <-52.715200,56.338360,21.376830>, 1)
// triangle color 1 nodes: [58, 95, 135]
make_triangle(<-52.715200,56.338360,21.376830>, <-57.271780,57.293670,24.978110>, <-59.323230,60.878440,23.444790>, 1)
// triangle color 1 nodes: [58, 135, 101]
make_triangle(<-52.715200,56.338360,21.376830>, <-59.323230,60.878440,23.444790>, <-54.272350,59.027740,20.244620>, 1)
// triangle color 1 nodes: [101, 135, 134]
make_triangle(<-54.272350,59.027740,20.244620>, <-59.323230,60.878440,23.444790>, <-61.325580,64.680510,21.645070>, 1)
// triangle color 1 nodes: [101, 134, 137]
make_triangle(<-54.272350,59.027740,20.244620>, <-61.325580,64.680510,21.645070>, <-55.704340,61.661290,19.043050>, 1)
// triangle color 1 nodes: [137, 134, 97]
make_triangle(<-55.704340,61.661290,19.043050>, <-61.325580,64.680510,21.645070>, <-63.145540,68.524910,19.620630>, 1)
// triangle color 1 nodes: [137, 97, 136]
make_triangle(<-55.704340,61.661290,19.043050>, <-63.145540,68.524910,19.620630>, <-57.022830,64.255840,17.766990>, 1)
// triangle color 1 nodes: [136, 97, 96]
make_triangle(<-57.022830,64.255840,17.766990>, <-63.145540,68.524910,19.620630>, <-64.694450,72.300130,17.394320>, 1)
// triangle color 1 nodes: [136, 96, 103]
make_triangle(<-57.022830,64.255840,17.766990>, <-64.694450,72.300130,17.394320>, <-58.227570,66.806920,16.420660>, 1)
// triangle color 1 nodes: [103, 96, 47]
make_triangle(<-58.227570,66.806920,16.420660>, <-64.694450,72.300130,17.394320>, <-65.832250,75.725460,15.106800>, 1)
// triangle color 1 nodes: [103, 47, 102]
make_triangle(<-58.227570,66.806920,16.420660>, <-65.832250,75.725460,15.106800>, <-59.326440,69.325900,15.000530>, 1)
// triangle color 1 nodes: [102, 47, 46]
make_triangle(<-59.326440,69.325900,15.000530>, <-65.832250,75.725460,15.106800>, <-66.439290,78.153430,13.285860>, 1)
// triangle color 1 nodes: [102, 46, 150]
make_triangle(<-59.326440,69.325900,15.000530>, <-66.439290,78.153430,13.285860>, <-60.322280,71.808770,13.513510>, 1)
// triangle color 1 nodes: [150, 46, 157]
make_triangle(<-60.322280,71.808770,13.513510>, <-66.439290,78.153430,13.285860>, <-66.829210,80.680640,11.148570>, 1)
// triangle color 1 nodes: [150, 157, 152]
make_triangle(<-60.322280,71.808770,13.513510>, <-66.829210,80.680640,11.148570>, <-61.235600,74.284290,11.951310>, 1)
// triangle color 1 nodes: [152, 157, 142]
make_triangle(<-61.235600,74.284290,11.951310>, <-66.829210,80.680640,11.148570>, <-66.707730,83.032420,8.675316>, 1)
// triangle color 1 nodes: [152, 142, 151]
make_triangle(<-61.235600,74.284290,11.951310>, <-66.707730,83.032420,8.675316>, <-61.998530,76.506210,10.492320>, 1)
// triangle color 1 nodes: [151, 142, 2]
make_triangle(<-61.998530,76.506210,10.492320>, <-66.707730,83.032420,8.675316>, <-66.000000,84.000000,7.000000>, 1)
// triangle color 1 nodes: [151, 2, 154]
make_triangle(<-61.998530,76.506210,10.492320>, <-66.000000,84.000000,7.000000>, <-62.798550,78.928400,8.870158>, 1)
// WriteDislocation([1, (39, 16, 15, 11)])
// segment connecting nodes 16 and 39 with color 1
make_dislocation(<0.154583,6.902457,17.942960>, <-1.686056,6.182347,20.503710>, 1)
// segment connecting nodes 15 and 16 with color 1
make_dislocation(<3.650219,6.594448,14.755330>, <0.154583,6.902457,17.942960>, 1)
// segment connecting nodes 11 and 15 with color 1
make_dislocation(<6.169073,5.830927,13.000000>, <3.650219,6.594448,14.755330>, 1)
// WriteDislocation([1, (68, 127, 100, 88, 42, 31)])
// segment connecting nodes 127 and 68 with color 1
make_dislocation(<-1.559360,10.504440,16.054850>, <-4.173561,10.833960,18.342230>, 1)
// segment connecting nodes 100 and 127 with color 1
make_dislocation(<0.667054,10.975430,13.357470>, <-1.559360,10.504440,16.054850>, 1)
// segment connecting nodes 88 and 100 with color 1
make_dislocation(<2.008082,11.553810,11.438070>, <0.667054,10.975430,13.357470>, 1)
// segment connecting nodes 42 and 88 with color 1
make_dislocation(<3.976924,12.439270,8.583793>, <2.008082,11.553810,11.438070>, 1)
// segment connecting nodes 31 and 42 with color 1
make_dislocation(<6.000000,13.000000,6.000000>, <3.976924,12.439270,8.583793>, 1)
// WriteDislocation([1, (11, 13, 14, 31)])
// segment connecting nodes 13 and 11 with color 1
make_dislocation(<5.816433,8.602456,10.581110>, <6.169073,5.830927,13.000000>, 1)
// segment connecting nodes 14 and 13 with color 1
make_dislocation(<6.007930,10.981770,8.010298>, <5.816433,8.602456,10.581110>, 1)
// segment connecting nodes 31 and 14 with color 1
make_dislocation(<6.000000,13.000000,6.000000>, <6.007930,10.981770,8.010298>, 1)
// WriteDislocation([2, (11, 36, 72, 10)])
// segment connecting nodes 36 and 11 with color 2
make_dislocation(<8.692137,6.013532,10.294330>, <6.169073,5.830927,13.000000>, 2)
// segment connecting nodes 72 and 36 with color 2
make_dislocation(<11.284610,6.008515,7.706870>, <8.692137,6.013532,10.294330>, 2)
// segment connecting nodes 10 and 72 with color 2
make_dislocation(<13.000000,6.000000,6.000000>, <11.284610,6.008515,7.706870>, 2)
// WriteDislocation([1, (10, 18, 17, 22, 8, 31)])
// segment connecting nodes 18 and 10 with color 1
make_dislocation(<12.670320,6.924030,6.594350>, <13.000000,6.000000,6.000000>, 1)
// segment connecting nodes 17 and 18 with color 1
make_dislocation(<10.411190,10.110060,7.521242>, <12.670320,6.924030,6.594350>, 1)
// segment connecting nodes 22 and 17 with color 1
make_dislocation(<8.697445,11.837410,7.534854>, <10.411190,10.110060,7.521242>, 1)
// segment connecting nodes 8 and 22 with color 1
make_dislocation(<7.018861,13.000000,7.018861>, <8.697445,11.837410,7.534854>, 1)
// segment connecting nodes 31 and 8 with color 1
make_dislocation(<6.000000,13.000000,6.000000>, <7.018861,13.000000,7.018861>, 1)
// WriteDislocation([1, (10, 21, 20, 19, 31)])
// segment connecting nodes 21 and 10 with color 1
make_dislocation(<11.906220,5.981126,4.887346>, <13.000000,6.000000,6.000000>, 1)
// segment connecting nodes 20 and 21 with color 1
make_dislocation(<9.749181,7.465570,4.214751>, <11.906220,5.981126,4.887346>, 1)
// segment connecting nodes 19 and 20 with color 1
make_dislocation(<7.450931,10.173750,4.624686>, <9.749181,7.465570,4.214751>, 1)
// segment connecting nodes 31 and 19 with color 1
make_dislocation(<6.000000,13.000000,6.000000>, <7.450931,10.173750,4.624686>, 1)
