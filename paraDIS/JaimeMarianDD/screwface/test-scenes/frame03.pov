#declare lookatpos = <11.000000,11.000000,7.733467>;
#declare camerapos = <11.000000,11.000000,201.070155>;
#declare viewx = x * 141.000000;
#declare viewy = y * 141.000000;
#include "jaime.inc"
// WriteFan([3, 1, (6, 9, 5)])
// segment connecting nodes 9 and 6 with color 3
make_dislocation(<13.000000,13.000000,13.000000>, <13.000000,0.000000,0.000000>, 3)
// triangle color 1 nodes: [6, 9, 5]
make_triangle(<13.000000,0.000000,0.000000>, <13.000000,13.000000,13.000000>, <0.000000,13.000000,0.000000>, 1)
// segment connecting nodes 5 and 9 with color 3
make_dislocation(<0.000000,13.000000,0.000000>, <13.000000,13.000000,13.000000>, 3)
// segment connecting nodes 6 and 5 with color 3
make_dislocation(<13.000000,0.000000,0.000000>, <0.000000,13.000000,0.000000>, 3)
// WriteFan([3, 1, (9, 5, 7)])
// segment connecting nodes 5 and 9 with color 3
make_dislocation(<0.000000,13.000000,0.000000>, <13.000000,13.000000,13.000000>, 3)
// triangle color 1 nodes: [9, 5, 7]
make_triangle(<13.000000,13.000000,13.000000>, <0.000000,13.000000,0.000000>, <0.000000,0.000000,13.000000>, 1)
// segment connecting nodes 7 and 5 with color 3
make_dislocation(<0.000000,0.000000,13.000000>, <0.000000,13.000000,0.000000>, 3)
// segment connecting nodes 9 and 7 with color 3
make_dislocation(<13.000000,13.000000,13.000000>, <0.000000,0.000000,13.000000>, 3)
// WriteFan([3, 1, (6, 5, 7)])
// segment connecting nodes 5 and 6 with color 3
make_dislocation(<0.000000,13.000000,0.000000>, <13.000000,0.000000,0.000000>, 3)
// triangle color 1 nodes: [6, 5, 7]
make_triangle(<13.000000,0.000000,0.000000>, <0.000000,13.000000,0.000000>, <0.000000,0.000000,13.000000>, 1)
// segment connecting nodes 7 and 5 with color 3
make_dislocation(<0.000000,0.000000,13.000000>, <0.000000,13.000000,0.000000>, 3)
// segment connecting nodes 6 and 7 with color 3
make_dislocation(<13.000000,0.000000,0.000000>, <0.000000,0.000000,13.000000>, 3)
// WriteFan([3, 1, (9, 6, 7)])
// segment connecting nodes 6 and 9 with color 3
make_dislocation(<13.000000,0.000000,0.000000>, <13.000000,13.000000,13.000000>, 3)
// triangle color 1 nodes: [9, 6, 7]
make_triangle(<13.000000,13.000000,13.000000>, <13.000000,0.000000,0.000000>, <0.000000,0.000000,13.000000>, 1)
// segment connecting nodes 7 and 6 with color 3
make_dislocation(<0.000000,0.000000,13.000000>, <13.000000,0.000000,0.000000>, 3)
// segment connecting nodes 9 and 7 with color 3
make_dislocation(<13.000000,13.000000,13.000000>, <0.000000,0.000000,13.000000>, 3)
// WriteStrip([1, 1, (1, 17, 59, 64, 67, 66, 65, 39, 38, 54, 55, 37, 23, 22, 44, 43, 81, 78, 31, 10), (3, 61, 30, 29, 14, 13, 33, 50, 49, 51, 32, 19, 18, 60, 46, 45, 48, 73, 77, 76, 80, 10)])
// segment connecting nodes 3 and 61 with color 1
make_dislocation(<80.500000,-59.500000,4.000000>, <78.843082,-59.095994,5.252912>, 1)
// segment connecting nodes 61 and 30 with color 1
make_dislocation(<78.843082,-59.095994,5.252912>, <76.146409,-57.930977,6.784568>, 1)
// segment connecting nodes 30 and 29 with color 1
make_dislocation(<76.146409,-57.930977,6.784568>, <72.297127,-55.790103,8.492976>, 1)
// segment connecting nodes 29 and 14 with color 1
make_dislocation(<72.297127,-55.790103,8.492976>, <67.454349,-52.658333,10.203984>, 1)
// segment connecting nodes 14 and 13 with color 1
make_dislocation(<67.454349,-52.658333,10.203984>, <62.132218,-48.848148,11.715930>, 1)
// segment connecting nodes 13 and 33 with color 1
make_dislocation(<62.132218,-48.848148,11.715930>, <56.982028,-44.882643,12.900616>, 1)
// segment connecting nodes 33 and 50 with color 1
make_dislocation(<56.982028,-44.882643,12.900616>, <52.165156,-40.962375,13.797218>, 1)
// segment connecting nodes 50 and 49 with color 1
make_dislocation(<52.165156,-40.962375,13.797218>, <47.319014,-36.825739,14.506725>, 1)
// segment connecting nodes 49 and 51 with color 1
make_dislocation(<47.319014,-36.825739,14.506725>, <42.328819,-32.366027,15.037208>, 1)
// segment connecting nodes 51 and 32 with color 1
make_dislocation(<42.328819,-32.366027,15.037208>, <37.385688,-27.743971,15.358283>, 1)
// segment connecting nodes 32 and 19 with color 1
make_dislocation(<37.385688,-27.743971,15.358283>, <32.635464,-23.102399,15.466935>, 1)
// segment connecting nodes 19 and 18 with color 1
make_dislocation(<32.635464,-23.102399,15.466935>, <28.079894,-18.454719,15.374825>, 1)
// segment connecting nodes 18 and 60 with color 1
make_dislocation(<28.079894,-18.454719,15.374825>, <23.871691,-13.973919,15.102228>, 1)
// segment connecting nodes 60 and 46 with color 1
make_dislocation(<23.871691,-13.973919,15.102228>, <20.759551,-10.538188,14.778637>, 1)
// segment connecting nodes 46 and 45 with color 1
make_dislocation(<20.759551,-10.538188,14.778637>, <16.686587,-5.868847,14.182260>, 1)
// segment connecting nodes 45 and 48 with color 1
make_dislocation(<16.686587,-5.868847,14.182260>, <12.750521,-0.981389,13.230868>, 1)
// segment connecting nodes 48 and 73 with color 1
make_dislocation(<12.750521,-0.981389,13.230868>, <11.088160,1.626124,12.285716>, 1)
// segment connecting nodes 73 and 77 with color 1
make_dislocation(<11.088160,1.626124,12.285716>, <10.373554,3.426921,11.199526>, 1)
// segment connecting nodes 77 and 76 with color 1
make_dislocation(<10.373554,3.426921,11.199526>, <10.219787,5.028183,9.752030>, 1)
// segment connecting nodes 76 and 80 with color 1
make_dislocation(<10.219787,5.028183,9.752030>, <10.556617,5.968106,8.475277>, 1)
// segment connecting nodes 80 and 10 with color 1
make_dislocation(<10.556617,5.968106,8.475277>, <13.000000,6.081574,5.918426>, 1)
// segment connecting nodes 1 and 17 with color 1
make_dislocation(<81.500000,-58.500000,2.000000>, <75.909827,-54.592631,3.682804>, 1)
// segment connecting nodes 17 and 59 with color 1
make_dislocation(<75.909827,-54.592631,3.682804>, <71.347908,-51.375395,5.027487>, 1)
// segment connecting nodes 59 and 64 with color 1
make_dislocation(<71.347908,-51.375395,5.027487>, <68.138073,-49.073353,5.935280>, 1)
// segment connecting nodes 64 and 67 with color 1
make_dislocation(<68.138073,-49.073353,5.935280>, <64.820299,-46.631740,6.811441>, 1)
// segment connecting nodes 67 and 66 with color 1
make_dislocation(<64.820299,-46.631740,6.811441>, <61.363627,-44.009957,7.646330>, 1)
// segment connecting nodes 66 and 65 with color 1
make_dislocation(<61.363627,-44.009957,7.646330>, <57.771416,-41.194103,8.422687>, 1)
// segment connecting nodes 65 and 39 with color 1
make_dislocation(<57.771416,-41.194103,8.422687>, <54.055152,-38.178169,9.123017>, 1)
// segment connecting nodes 39 and 38 with color 1
make_dislocation(<54.055152,-38.178169,9.123017>, <50.239767,-34.968843,9.729076>, 1)
// segment connecting nodes 38 and 54 with color 1
make_dislocation(<50.239767,-34.968843,9.729076>, <46.358552,-31.582640,10.224088>, 1)
// segment connecting nodes 54 and 55 with color 1
make_dislocation(<46.358552,-31.582640,10.224088>, <42.455536,-28.049145,10.593609>, 1)
// segment connecting nodes 55 and 37 with color 1
make_dislocation(<42.455536,-28.049145,10.593609>, <38.584096,-24.411506,10.827410>, 1)
// segment connecting nodes 37 and 23 with color 1
make_dislocation(<38.584096,-24.411506,10.827410>, <34.789598,-20.711091,10.921493>, 1)
// segment connecting nodes 23 and 22 with color 1
make_dislocation(<34.789598,-20.711091,10.921493>, <31.103273,-16.980165,10.876892>, 1)
// segment connecting nodes 22 and 44 with color 1
make_dislocation(<31.103273,-16.980165,10.876892>, <27.526755,-13.223793,10.697039>, 1)
// segment connecting nodes 44 and 43 with color 1
make_dislocation(<27.526755,-13.223793,10.697039>, <23.784018,-9.137776,10.353757>, 1)
// segment connecting nodes 43 and 81 with color 1
make_dislocation(<23.784018,-9.137776,10.353757>, <21.150156,-6.133355,9.983200>, 1)
// segment connecting nodes 81 and 78 with color 1
make_dislocation(<21.150156,-6.133355,9.983200>, <18.357718,-2.907782,9.550064>, 1)
// segment connecting nodes 78 and 31 with color 1
make_dislocation(<18.357718,-2.907782,9.550064>, <14.178741,2.559030,8.262230>, 1)
// segment connecting nodes 31 and 10 with color 1
make_dislocation(<14.178741,2.559030,8.262230>, <13.000000,6.081574,5.918426>, 1)
// triangle color 1 nodes: [3, 1, 61]
make_triangle(<80.500000,-59.500000,4.000000>, <81.500000,-58.500000,2.000000>, <78.843082,-59.095994,5.252912>, 1)
// triangle color 1 nodes: [61, 1, 17]
make_triangle(<78.843082,-59.095994,5.252912>, <81.500000,-58.500000,2.000000>, <75.909827,-54.592631,3.682804>, 1)
// triangle color 1 nodes: [61, 17, 30]
make_triangle(<78.843082,-59.095994,5.252912>, <75.909827,-54.592631,3.682804>, <76.146409,-57.930977,6.784568>, 1)
// triangle color 1 nodes: [30, 17, 59]
make_triangle(<76.146409,-57.930977,6.784568>, <75.909827,-54.592631,3.682804>, <71.347908,-51.375395,5.027487>, 1)
// triangle color 1 nodes: [30, 59, 29]
make_triangle(<76.146409,-57.930977,6.784568>, <71.347908,-51.375395,5.027487>, <72.297127,-55.790103,8.492976>, 1)
// triangle color 1 nodes: [29, 59, 64]
make_triangle(<72.297127,-55.790103,8.492976>, <71.347908,-51.375395,5.027487>, <68.138073,-49.073353,5.935280>, 1)
// triangle color 1 nodes: [29, 64, 14]
make_triangle(<72.297127,-55.790103,8.492976>, <68.138073,-49.073353,5.935280>, <67.454349,-52.658333,10.203984>, 1)
// triangle color 1 nodes: [14, 64, 67]
make_triangle(<67.454349,-52.658333,10.203984>, <68.138073,-49.073353,5.935280>, <64.820299,-46.631740,6.811441>, 1)
// triangle color 1 nodes: [14, 67, 13]
make_triangle(<67.454349,-52.658333,10.203984>, <64.820299,-46.631740,6.811441>, <62.132218,-48.848148,11.715930>, 1)
// triangle color 1 nodes: [13, 67, 66]
make_triangle(<62.132218,-48.848148,11.715930>, <64.820299,-46.631740,6.811441>, <61.363627,-44.009957,7.646330>, 1)
// triangle color 1 nodes: [13, 66, 33]
make_triangle(<62.132218,-48.848148,11.715930>, <61.363627,-44.009957,7.646330>, <56.982028,-44.882643,12.900616>, 1)
// triangle color 1 nodes: [33, 66, 65]
make_triangle(<56.982028,-44.882643,12.900616>, <61.363627,-44.009957,7.646330>, <57.771416,-41.194103,8.422687>, 1)
// triangle color 1 nodes: [33, 65, 50]
make_triangle(<56.982028,-44.882643,12.900616>, <57.771416,-41.194103,8.422687>, <52.165156,-40.962375,13.797218>, 1)
// triangle color 1 nodes: [50, 65, 39]
make_triangle(<52.165156,-40.962375,13.797218>, <57.771416,-41.194103,8.422687>, <54.055152,-38.178169,9.123017>, 1)
// triangle color 1 nodes: [50, 39, 49]
make_triangle(<52.165156,-40.962375,13.797218>, <54.055152,-38.178169,9.123017>, <47.319014,-36.825739,14.506725>, 1)
// triangle color 1 nodes: [49, 39, 38]
make_triangle(<47.319014,-36.825739,14.506725>, <54.055152,-38.178169,9.123017>, <50.239767,-34.968843,9.729076>, 1)
// triangle color 1 nodes: [49, 38, 51]
make_triangle(<47.319014,-36.825739,14.506725>, <50.239767,-34.968843,9.729076>, <42.328819,-32.366027,15.037208>, 1)
// triangle color 1 nodes: [51, 38, 54]
make_triangle(<42.328819,-32.366027,15.037208>, <50.239767,-34.968843,9.729076>, <46.358552,-31.582640,10.224088>, 1)
// triangle color 1 nodes: [51, 54, 32]
make_triangle(<42.328819,-32.366027,15.037208>, <46.358552,-31.582640,10.224088>, <37.385688,-27.743971,15.358283>, 1)
// triangle color 1 nodes: [32, 54, 55]
make_triangle(<37.385688,-27.743971,15.358283>, <46.358552,-31.582640,10.224088>, <42.455536,-28.049145,10.593609>, 1)
// triangle color 1 nodes: [32, 55, 19]
make_triangle(<37.385688,-27.743971,15.358283>, <42.455536,-28.049145,10.593609>, <32.635464,-23.102399,15.466935>, 1)
// triangle color 1 nodes: [19, 55, 37]
make_triangle(<32.635464,-23.102399,15.466935>, <42.455536,-28.049145,10.593609>, <38.584096,-24.411506,10.827410>, 1)
// triangle color 1 nodes: [19, 37, 18]
make_triangle(<32.635464,-23.102399,15.466935>, <38.584096,-24.411506,10.827410>, <28.079894,-18.454719,15.374825>, 1)
// triangle color 1 nodes: [18, 37, 23]
make_triangle(<28.079894,-18.454719,15.374825>, <38.584096,-24.411506,10.827410>, <34.789598,-20.711091,10.921493>, 1)
// triangle color 1 nodes: [18, 23, 60]
make_triangle(<28.079894,-18.454719,15.374825>, <34.789598,-20.711091,10.921493>, <23.871691,-13.973919,15.102228>, 1)
// triangle color 1 nodes: [60, 23, 22]
make_triangle(<23.871691,-13.973919,15.102228>, <34.789598,-20.711091,10.921493>, <31.103273,-16.980165,10.876892>, 1)
// triangle color 1 nodes: [60, 22, 46]
make_triangle(<23.871691,-13.973919,15.102228>, <31.103273,-16.980165,10.876892>, <20.759551,-10.538188,14.778637>, 1)
// triangle color 1 nodes: [46, 22, 44]
make_triangle(<20.759551,-10.538188,14.778637>, <31.103273,-16.980165,10.876892>, <27.526755,-13.223793,10.697039>, 1)
// triangle color 1 nodes: [46, 44, 45]
make_triangle(<20.759551,-10.538188,14.778637>, <27.526755,-13.223793,10.697039>, <16.686587,-5.868847,14.182260>, 1)
// triangle color 1 nodes: [45, 44, 43]
make_triangle(<16.686587,-5.868847,14.182260>, <27.526755,-13.223793,10.697039>, <23.784018,-9.137776,10.353757>, 1)
// triangle color 1 nodes: [45, 43, 48]
make_triangle(<16.686587,-5.868847,14.182260>, <23.784018,-9.137776,10.353757>, <12.750521,-0.981389,13.230868>, 1)
// triangle color 1 nodes: [48, 43, 81]
make_triangle(<12.750521,-0.981389,13.230868>, <23.784018,-9.137776,10.353757>, <21.150156,-6.133355,9.983200>, 1)
// triangle color 1 nodes: [48, 81, 73]
make_triangle(<12.750521,-0.981389,13.230868>, <21.150156,-6.133355,9.983200>, <11.088160,1.626124,12.285716>, 1)
// triangle color 1 nodes: [73, 81, 78]
make_triangle(<11.088160,1.626124,12.285716>, <21.150156,-6.133355,9.983200>, <18.357718,-2.907782,9.550064>, 1)
// triangle color 1 nodes: [73, 78, 77]
make_triangle(<11.088160,1.626124,12.285716>, <18.357718,-2.907782,9.550064>, <10.373554,3.426921,11.199526>, 1)
// triangle color 1 nodes: [77, 78, 31]
make_triangle(<10.373554,3.426921,11.199526>, <18.357718,-2.907782,9.550064>, <14.178741,2.559030,8.262230>, 1)
// triangle color 1 nodes: [77, 31, 76]
make_triangle(<10.373554,3.426921,11.199526>, <14.178741,2.559030,8.262230>, <10.219787,5.028183,9.752030>, 1)
// triangle color 1 nodes: [76, 31, 10]
make_triangle(<10.219787,5.028183,9.752030>, <14.178741,2.559030,8.262230>, <13.000000,6.081574,5.918426>, 1)
// triangle color 1 nodes: [76, 10, 80]
make_triangle(<10.219787,5.028183,9.752030>, <13.000000,6.081574,5.918426>, <10.556617,5.968106,8.475277>, 1)
// WriteStrip([1, 1, (8, 47, 58, 69, 71, 62, 25, 24, 40, 57, 56, 42, 41, 68, 74, 75, 16, 15, 2), (8, 72, 79, 83, 27, 26, 21, 20, 34, 53, 52, 36, 35, 63, 82, 12, 11, 70, 4)])
// segment connecting nodes 8 and 72 with color 1
make_dislocation(<6.202430,13.000000,5.797570>, <5.923954,11.558352,7.517694>, 1)
// segment connecting nodes 72 and 79 with color 1
make_dislocation(<5.923954,11.558352,7.517694>, <4.098338,12.441058,8.460604>, 1)
// segment connecting nodes 79 and 83 with color 1
make_dislocation(<4.098338,12.441058,8.460604>, <-0.679116,15.612156,10.066959>, 1)
// segment connecting nodes 83 and 27 with color 1
make_dislocation(<-0.679116,15.612156,10.066959>, <-3.857531,17.768545,11.088986>, 1)
// segment connecting nodes 27 and 26 with color 1
make_dislocation(<-3.857531,17.768545,11.088986>, <-7.538765,20.532225,12.006540>, 1)
// segment connecting nodes 26 and 21 with color 1
make_dislocation(<-7.538765,20.532225,12.006540>, <-11.773516,23.996350,12.777166>, 1)
// segment connecting nodes 21 and 20 with color 1
make_dislocation(<-11.773516,23.996350,12.777166>, <-16.655189,28.286767,13.368422>, 1)
// segment connecting nodes 20 and 34 with color 1
make_dislocation(<-16.655189,28.286767,13.368422>, <-21.976651,33.269313,13.707337>, 1)
// segment connecting nodes 34 and 53 with color 1
make_dislocation(<-21.976651,33.269313,13.707337>, <-27.473287,38.726437,13.746850>, 1)
// segment connecting nodes 53 and 52 with color 1
make_dislocation(<-27.473287,38.726437,13.746850>, <-32.933634,44.463001,13.470632>, 1)
// segment connecting nodes 52 and 36 with color 1
make_dislocation(<-32.933634,44.463001,13.470632>, <-38.133470,50.241706,12.891764>, 1)
// segment connecting nodes 36 and 35 with color 1
make_dislocation(<-38.133470,50.241706,12.891764>, <-42.586799,55.470018,12.116781>, 1)
// segment connecting nodes 35 and 63 with color 1
make_dislocation(<-42.586799,55.470018,12.116781>, <-45.924310,59.581813,11.342497>, 1)
// segment connecting nodes 63 and 82 with color 1
make_dislocation(<-45.924310,59.581813,11.342497>, <-49.365070,64.026980,10.338090>, 1)
// segment connecting nodes 82 and 12 with color 1
make_dislocation(<-49.365070,64.026980,10.338090>, <-53.317547,69.495703,8.821844>, 1)
// segment connecting nodes 12 and 11 with color 1
make_dislocation(<-53.317547,69.495703,8.821844>, <-56.697612,74.713842,6.983770>, 1)
// segment connecting nodes 11 and 70 with color 1
make_dislocation(<-56.697612,74.713842,6.983770>, <-58.696458,78.403338,5.293120>, 1)
// segment connecting nodes 70 and 4 with color 1
make_dislocation(<-58.696458,78.403338,5.293120>, <-59.500000,80.500000,4.000000>, 1)
// segment connecting nodes 8 and 47 with color 1
make_dislocation(<6.202430,13.000000,5.797570>, <4.246452,15.162205,5.591343>, 1)
// segment connecting nodes 47 and 58 with color 1
make_dislocation(<4.246452,15.162205,5.591343>, <0.135254,18.585349,6.279398>, 1)
// segment connecting nodes 58 and 69 with color 1
make_dislocation(<0.135254,18.585349,6.279398>, <-4.667707,22.350936,7.316771>, 1)
// segment connecting nodes 69 and 71 with color 1
make_dislocation(<-4.667707,22.350936,7.316771>, <-8.286498,25.319478,7.967020>, 1)
// segment connecting nodes 71 and 62 with color 1
make_dislocation(<-8.286498,25.319478,7.967020>, <-11.935316,28.446098,8.489218>, 1)
// segment connecting nodes 62 and 25 with color 1
make_dislocation(<-11.935316,28.446098,8.489218>, <-15.647538,31.767971,8.879567>, 1)
// segment connecting nodes 25 and 24 with color 1
make_dislocation(<-15.647538,31.767971,8.879567>, <-19.401301,35.272763,9.128539>, 1)
// segment connecting nodes 24 and 40 with color 1
make_dislocation(<-19.401301,35.272763,9.128539>, <-23.180908,38.951919,9.228989>, 1)
// segment connecting nodes 40 and 57 with color 1
make_dislocation(<-23.180908,38.951919,9.228989>, <-26.957906,42.783252,9.174655>, 1)
// segment connecting nodes 57 and 56 with color 1
make_dislocation(<-26.957906,42.783252,9.174655>, <-30.678751,46.714960,8.963791>, 1)
// segment connecting nodes 56 and 42 with color 1
make_dislocation(<-30.678751,46.714960,8.963791>, <-34.315347,50.716629,8.598718>, 1)
// segment connecting nodes 42 and 41 with color 1
make_dislocation(<-34.315347,50.716629,8.598718>, <-37.848737,54.765644,8.083093>, 1)
// segment connecting nodes 41 and 68 with color 1
make_dislocation(<-37.848737,54.765644,8.083093>, <-41.240923,58.813421,7.427502>, 1)
// segment connecting nodes 68 and 74 with color 1
make_dislocation(<-41.240923,58.813421,7.427502>, <-44.499189,62.859887,6.639302>, 1)
// segment connecting nodes 74 and 75 with color 1
make_dislocation(<-44.499189,62.859887,6.639302>, <-47.666788,66.948278,5.718511>, 1)
// segment connecting nodes 75 and 16 with color 1
make_dislocation(<-47.666788,66.948278,5.718511>, <-50.757325,71.076864,4.680461>, 1)
// segment connecting nodes 16 and 15 with color 1
make_dislocation(<-50.757325,71.076864,4.680461>, <-54.556150,76.238839,3.317311>, 1)
// segment connecting nodes 15 and 2 with color 1
make_dislocation(<-54.556150,76.238839,3.317311>, <-58.500000,81.500000,2.000000>, 1)
// triangle color 1 nodes: [8, 8, 72]
make_triangle(<6.202430,13.000000,5.797570>, <6.202430,13.000000,5.797570>, <5.923954,11.558352,7.517694>, 1)
// triangle color 1 nodes: [72, 8, 47]
make_triangle(<5.923954,11.558352,7.517694>, <6.202430,13.000000,5.797570>, <4.246452,15.162205,5.591343>, 1)
// triangle color 1 nodes: [72, 47, 79]
make_triangle(<5.923954,11.558352,7.517694>, <4.246452,15.162205,5.591343>, <4.098338,12.441058,8.460604>, 1)
// triangle color 1 nodes: [79, 47, 58]
make_triangle(<4.098338,12.441058,8.460604>, <4.246452,15.162205,5.591343>, <0.135254,18.585349,6.279398>, 1)
// triangle color 1 nodes: [79, 58, 83]
make_triangle(<4.098338,12.441058,8.460604>, <0.135254,18.585349,6.279398>, <-0.679116,15.612156,10.066959>, 1)
// triangle color 1 nodes: [83, 58, 69]
make_triangle(<-0.679116,15.612156,10.066959>, <0.135254,18.585349,6.279398>, <-4.667707,22.350936,7.316771>, 1)
// triangle color 1 nodes: [83, 69, 27]
make_triangle(<-0.679116,15.612156,10.066959>, <-4.667707,22.350936,7.316771>, <-3.857531,17.768545,11.088986>, 1)
// triangle color 1 nodes: [27, 69, 71]
make_triangle(<-3.857531,17.768545,11.088986>, <-4.667707,22.350936,7.316771>, <-8.286498,25.319478,7.967020>, 1)
// triangle color 1 nodes: [27, 71, 26]
make_triangle(<-3.857531,17.768545,11.088986>, <-8.286498,25.319478,7.967020>, <-7.538765,20.532225,12.006540>, 1)
// triangle color 1 nodes: [26, 71, 62]
make_triangle(<-7.538765,20.532225,12.006540>, <-8.286498,25.319478,7.967020>, <-11.935316,28.446098,8.489218>, 1)
// triangle color 1 nodes: [26, 62, 21]
make_triangle(<-7.538765,20.532225,12.006540>, <-11.935316,28.446098,8.489218>, <-11.773516,23.996350,12.777166>, 1)
// triangle color 1 nodes: [21, 62, 25]
make_triangle(<-11.773516,23.996350,12.777166>, <-11.935316,28.446098,8.489218>, <-15.647538,31.767971,8.879567>, 1)
// triangle color 1 nodes: [21, 25, 20]
make_triangle(<-11.773516,23.996350,12.777166>, <-15.647538,31.767971,8.879567>, <-16.655189,28.286767,13.368422>, 1)
// triangle color 1 nodes: [20, 25, 24]
make_triangle(<-16.655189,28.286767,13.368422>, <-15.647538,31.767971,8.879567>, <-19.401301,35.272763,9.128539>, 1)
// triangle color 1 nodes: [20, 24, 34]
make_triangle(<-16.655189,28.286767,13.368422>, <-19.401301,35.272763,9.128539>, <-21.976651,33.269313,13.707337>, 1)
// triangle color 1 nodes: [34, 24, 40]
make_triangle(<-21.976651,33.269313,13.707337>, <-19.401301,35.272763,9.128539>, <-23.180908,38.951919,9.228989>, 1)
// triangle color 1 nodes: [34, 40, 53]
make_triangle(<-21.976651,33.269313,13.707337>, <-23.180908,38.951919,9.228989>, <-27.473287,38.726437,13.746850>, 1)
// triangle color 1 nodes: [53, 40, 57]
make_triangle(<-27.473287,38.726437,13.746850>, <-23.180908,38.951919,9.228989>, <-26.957906,42.783252,9.174655>, 1)
// triangle color 1 nodes: [53, 57, 52]
make_triangle(<-27.473287,38.726437,13.746850>, <-26.957906,42.783252,9.174655>, <-32.933634,44.463001,13.470632>, 1)
// triangle color 1 nodes: [52, 57, 56]
make_triangle(<-32.933634,44.463001,13.470632>, <-26.957906,42.783252,9.174655>, <-30.678751,46.714960,8.963791>, 1)
// triangle color 1 nodes: [52, 56, 36]
make_triangle(<-32.933634,44.463001,13.470632>, <-30.678751,46.714960,8.963791>, <-38.133470,50.241706,12.891764>, 1)
// triangle color 1 nodes: [36, 56, 42]
make_triangle(<-38.133470,50.241706,12.891764>, <-30.678751,46.714960,8.963791>, <-34.315347,50.716629,8.598718>, 1)
// triangle color 1 nodes: [36, 42, 35]
make_triangle(<-38.133470,50.241706,12.891764>, <-34.315347,50.716629,8.598718>, <-42.586799,55.470018,12.116781>, 1)
// triangle color 1 nodes: [35, 42, 41]
make_triangle(<-42.586799,55.470018,12.116781>, <-34.315347,50.716629,8.598718>, <-37.848737,54.765644,8.083093>, 1)
// triangle color 1 nodes: [35, 41, 63]
make_triangle(<-42.586799,55.470018,12.116781>, <-37.848737,54.765644,8.083093>, <-45.924310,59.581813,11.342497>, 1)
// triangle color 1 nodes: [63, 41, 68]
make_triangle(<-45.924310,59.581813,11.342497>, <-37.848737,54.765644,8.083093>, <-41.240923,58.813421,7.427502>, 1)
// triangle color 1 nodes: [63, 68, 82]
make_triangle(<-45.924310,59.581813,11.342497>, <-41.240923,58.813421,7.427502>, <-49.365070,64.026980,10.338090>, 1)
// triangle color 1 nodes: [82, 68, 74]
make_triangle(<-49.365070,64.026980,10.338090>, <-41.240923,58.813421,7.427502>, <-44.499189,62.859887,6.639302>, 1)
// triangle color 1 nodes: [82, 74, 12]
make_triangle(<-49.365070,64.026980,10.338090>, <-44.499189,62.859887,6.639302>, <-53.317547,69.495703,8.821844>, 1)
// triangle color 1 nodes: [12, 74, 75]
make_triangle(<-53.317547,69.495703,8.821844>, <-44.499189,62.859887,6.639302>, <-47.666788,66.948278,5.718511>, 1)
// triangle color 1 nodes: [12, 75, 11]
make_triangle(<-53.317547,69.495703,8.821844>, <-47.666788,66.948278,5.718511>, <-56.697612,74.713842,6.983770>, 1)
// triangle color 1 nodes: [11, 75, 16]
make_triangle(<-56.697612,74.713842,6.983770>, <-47.666788,66.948278,5.718511>, <-50.757325,71.076864,4.680461>, 1)
// triangle color 1 nodes: [11, 16, 70]
make_triangle(<-56.697612,74.713842,6.983770>, <-50.757325,71.076864,4.680461>, <-58.696458,78.403338,5.293120>, 1)
// triangle color 1 nodes: [70, 16, 15]
make_triangle(<-58.696458,78.403338,5.293120>, <-50.757325,71.076864,4.680461>, <-54.556150,76.238839,3.317311>, 1)
// triangle color 1 nodes: [70, 15, 4]
make_triangle(<-58.696458,78.403338,5.293120>, <-54.556150,76.238839,3.317311>, <-59.500000,80.500000,4.000000>, 1)
// triangle color 1 nodes: [4, 15, 2]
make_triangle(<-59.500000,80.500000,4.000000>, <-54.556150,76.238839,3.317311>, <-58.500000,81.500000,2.000000>, 1)
// WriteDislocation([2, (10, 28, 8)])
// segment connecting nodes 28 and 10 with color 2
make_dislocation(<7.470906,11.533905,5.995189>, <13.000000,6.081574,5.918426>, 2)
// segment connecting nodes 8 and 28 with color 2
make_dislocation(<6.202430,13.000000,5.797570>, <7.470906,11.533905,5.995189>, 2)
