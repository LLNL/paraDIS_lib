#declare lookatpos = <8.000000,8.000000,6.500000>;
#declare camerapos = <8.000000,8.000000,169.000000>;
#declare viewx = x * 154.000000;
#declare viewy = y * 154.000000;
#include "jaime.inc"
// WriteFan([0, 1, (9, 10, 19, 30, 18, 8)])
// segment connecting nodes 10 and 9 with color 0
make_dislocation(<13.000000,3.000000,3.000000>, <13.000000,13.000000,13.000000>, 0, 0)
// triangle color 1 nodes: [9, 10, 19]
make_triangle(<13.000000,13.000000,13.000000>, <13.000000,3.000000,3.000000>, <11.037010,5.702104,3.739117>, 1)
// segment connecting nodes 19 and 10 with color 0
make_dislocation(<11.037010,5.702104,3.739117>, <13.000000,3.000000,3.000000>, 0, 0)
// triangle color 1 nodes: [9, 19, 30]
make_triangle(<13.000000,13.000000,13.000000>, <11.037010,5.702104,3.739117>, <8.613341,8.439738,4.053079>, 1)
// segment connecting nodes 30 and 19 with color 0
make_dislocation(<8.613341,8.439738,4.053079>, <11.037010,5.702104,3.739117>, 0, 0)
// triangle color 1 nodes: [9, 30, 18]
make_triangle(<13.000000,13.000000,13.000000>, <8.613341,8.439738,4.053079>, <5.747016,10.998620,3.745641>, 1)
// segment connecting nodes 18 and 30 with color 0
make_dislocation(<5.747016,10.998620,3.745641>, <8.613341,8.439738,4.053079>, 0, 0)
// triangle color 1 nodes: [9, 18, 8]
make_triangle(<13.000000,13.000000,13.000000>, <5.747016,10.998620,3.745641>, <3.000000,13.000000,3.000000>, 1)
// segment connecting nodes 8 and 18 with color 0
make_dislocation(<3.000000,13.000000,3.000000>, <5.747016,10.998620,3.745641>, 0, 0)
// segment connecting nodes 9 and 19 with color 0
make_dislocation(<13.000000,13.000000,13.000000>, <11.037010,5.702104,3.739117>, 0, 0)
// WriteFan([0, 1, (6, 10, 17, 16)])
// segment connecting nodes 10 and 6 with color 0
make_dislocation(<13.000000,3.000000,3.000000>, <13.000000,0.000000,0.000000>, 0, 0)
// triangle color 1 nodes: [6, 10, 17]
make_triangle(<13.000000,0.000000,0.000000>, <13.000000,3.000000,3.000000>, <11.073400,4.076123,2.149521>, 1)
// segment connecting nodes 17 and 10 with color 0
make_dislocation(<11.073400,4.076123,2.149521>, <13.000000,3.000000,3.000000>, 0, 0)
// triangle color 1 nodes: [6, 17, 16]
make_triangle(<13.000000,0.000000,0.000000>, <11.073400,4.076123,2.149521>, <8.442955,6.433807,1.876762>, 1)
// segment connecting nodes 16 and 17 with color 0
make_dislocation(<8.442955,6.433807,1.876762>, <11.073400,4.076123,2.149521>, 0, 0)
// segment connecting nodes 6 and 17 with color 0
make_dislocation(<13.000000,0.000000,0.000000>, <11.073400,4.076123,2.149521>, 0, 0)
// WriteFan([0, 1, (5, 8, 15, 16)])
// segment connecting nodes 8 and 5 with color 0
make_dislocation(<3.000000,13.000000,3.000000>, <0.000000,13.000000,0.000000>, 0, 0)
// triangle color 1 nodes: [5, 8, 15]
make_triangle(<0.000000,13.000000,0.000000>, <3.000000,13.000000,3.000000>, <5.543269,9.737926,2.281195>, 1)
// segment connecting nodes 15 and 8 with color 0
make_dislocation(<5.543269,9.737926,2.281195>, <3.000000,13.000000,3.000000>, 0, 0)
// triangle color 1 nodes: [5, 15, 16]
make_triangle(<0.000000,13.000000,0.000000>, <5.543269,9.737926,2.281195>, <8.442955,6.433807,1.876762>, 1)
// segment connecting nodes 16 and 15 with color 0
make_dislocation(<8.442955,6.433807,1.876762>, <5.543269,9.737926,2.281195>, 0, 0)
// segment connecting nodes 5 and 15 with color 0
make_dislocation(<0.000000,13.000000,0.000000>, <5.543269,9.737926,2.281195>, 0, 0)
// WriteFan([0, 1, (16, 6, 5)])
// segment connecting nodes 6 and 16 with color 0
make_dislocation(<13.000000,0.000000,0.000000>, <8.442955,6.433807,1.876762>, 0, 0)
// triangle color 1 nodes: [16, 6, 5]
make_triangle(<8.442955,6.433807,1.876762>, <13.000000,0.000000,0.000000>, <0.000000,13.000000,0.000000>, 1)
// segment connecting nodes 5 and 6 with color 0
make_dislocation(<0.000000,13.000000,0.000000>, <13.000000,0.000000,0.000000>, 0, 0)
// segment connecting nodes 16 and 5 with color 0
make_dislocation(<8.442955,6.433807,1.876762>, <0.000000,13.000000,0.000000>, 0, 0)
// WriteFan([3, 1, (9, 5, 7)])
// segment connecting nodes 5 and 9 with color 3
make_dislocation(<0.000000,13.000000,0.000000>, <13.000000,13.000000,13.000000>, 3, 0)
// triangle color 1 nodes: [9, 5, 7]
make_triangle(<13.000000,13.000000,13.000000>, <0.000000,13.000000,0.000000>, <0.000000,0.000000,13.000000>, 1)
// segment connecting nodes 7 and 5 with color 3
make_dislocation(<0.000000,0.000000,13.000000>, <0.000000,13.000000,0.000000>, 3, 0)
// segment connecting nodes 9 and 7 with color 3
make_dislocation(<13.000000,13.000000,13.000000>, <0.000000,0.000000,13.000000>, 3, 0)
// WriteFan([3, 1, (5, 6, 7)])
// segment connecting nodes 6 and 5 with color 3
make_dislocation(<13.000000,0.000000,0.000000>, <0.000000,13.000000,0.000000>, 3, 0)
// triangle color 1 nodes: [5, 6, 7]
make_triangle(<0.000000,13.000000,0.000000>, <13.000000,0.000000,0.000000>, <0.000000,0.000000,13.000000>, 1)
// segment connecting nodes 7 and 6 with color 3
make_dislocation(<0.000000,0.000000,13.000000>, <13.000000,0.000000,0.000000>, 3, 0)
// segment connecting nodes 5 and 7 with color 3
make_dislocation(<0.000000,13.000000,0.000000>, <0.000000,0.000000,13.000000>, 3, 0)
// WriteFan([3, 1, (9, 6, 7)])
// segment connecting nodes 6 and 9 with color 3
make_dislocation(<13.000000,0.000000,0.000000>, <13.000000,13.000000,13.000000>, 3, 0)
// triangle color 1 nodes: [9, 6, 7]
make_triangle(<13.000000,13.000000,13.000000>, <13.000000,0.000000,0.000000>, <0.000000,0.000000,13.000000>, 1)
// segment connecting nodes 7 and 6 with color 3
make_dislocation(<0.000000,0.000000,13.000000>, <13.000000,0.000000,0.000000>, 3, 0)
// segment connecting nodes 9 and 7 with color 3
make_dislocation(<13.000000,13.000000,13.000000>, <0.000000,0.000000,13.000000>, 3, 0)
// WriteStrip([1, 1, (4, 23, 147, 146, 145, 104, 103, 126, 127, 102, 60, 59, 135, 79, 78, 115, 114, 80, 58, 57, 101, 35, 34, 88, 119, 87, 44, 131, 68, 67, 107, 36, 22, 10), (3, 21, 144, 143, 142, 100, 99, 124, 125, 98, 56, 55, 134, 76, 75, 113, 112, 77, 54, 53, 97, 32, 31, 86, 118, 85, 42, 41, 130, 66, 65, 106, 33, 20, 10)])
// segment connecting nodes 3 and 21 with color 1
make_dislocation(<84.000000,-69.000000,4.000000>, <81.771630,-66.857860,4.086232>, 1, 1)
// segment connecting nodes 21 and 144 with color 1
make_dislocation(<81.771630,-66.857860,4.086232>, <79.564720,-64.696130,4.131410>, 1, 0)
// segment connecting nodes 144 and 143 with color 1
make_dislocation(<79.564720,-64.696130,4.131410>, <77.368190,-62.521690,4.153498>, 1, 0)
// segment connecting nodes 143 and 142 with color 1
make_dislocation(<77.368190,-62.521690,4.153498>, <75.172330,-60.346860,4.174530>, 1, 0)
// segment connecting nodes 142 and 100 with color 1
make_dislocation(<75.172330,-60.346860,4.174530>, <72.980540,-58.166290,4.185752>, 1, 0)
// segment connecting nodes 100 and 99 with color 1
make_dislocation(<72.980540,-58.166290,4.185752>, <70.791220,-55.988020,4.196793>, 1, 0)
// segment connecting nodes 99 and 124 with color 1
make_dislocation(<70.791220,-55.988020,4.196793>, <68.601490,-53.809280,4.207795>, 1, 0)
// segment connecting nodes 124 and 125 with color 1
make_dislocation(<68.601490,-53.809280,4.207795>, <66.409490,-51.628080,4.218591>, 1, 0)
// segment connecting nodes 125 and 98 with color 1
make_dislocation(<66.409490,-51.628080,4.218591>, <64.219600,-49.444510,4.224915>, 1, 0)
// segment connecting nodes 98 and 56 with color 1
make_dislocation(<64.219600,-49.444510,4.224915>, <62.032170,-47.263420,4.231255>, 1, 0)
// segment connecting nodes 56 and 55 with color 1
make_dislocation(<62.032170,-47.263420,4.231255>, <59.844020,-45.081640,4.237618>, 1, 0)
// segment connecting nodes 55 and 134 with color 1
make_dislocation(<59.844020,-45.081640,4.237618>, <57.655730,-42.899700,4.243973>, 1, 0)
// segment connecting nodes 134 and 76 with color 1
make_dislocation(<57.655730,-42.899700,4.243973>, <55.463780,-40.714160,4.250380>, 1, 0)
// segment connecting nodes 76 and 75 with color 1
make_dislocation(<55.463780,-40.714160,4.250380>, <53.277750,-38.534470,4.256725>, 1, 0)
// segment connecting nodes 75 and 113 with color 1
make_dislocation(<53.277750,-38.534470,4.256725>, <51.090260,-36.353380,4.263121>, 1, 0)
// segment connecting nodes 113 and 112 with color 1
make_dislocation(<51.090260,-36.353380,4.263121>, <48.901340,-34.170790,4.269443>, 1, 0)
// segment connecting nodes 112 and 77 with color 1
make_dislocation(<48.901340,-34.170790,4.269443>, <46.715290,-31.988690,4.273403>, 1, 0)
// segment connecting nodes 77 and 54 with color 1
make_dislocation(<46.715290,-31.988690,4.273403>, <44.529360,-29.806780,4.277414>, 1, 0)
// segment connecting nodes 54 and 53 with color 1
make_dislocation(<44.529360,-29.806780,4.277414>, <42.344550,-27.626020,4.281472>, 1, 0)
// segment connecting nodes 53 and 97 with color 1
make_dislocation(<42.344550,-27.626020,4.281472>, <40.161830,-25.447340,4.285509>, 1, 0)
// segment connecting nodes 97 and 32 with color 1
make_dislocation(<40.161830,-25.447340,4.285509>, <37.974550,-23.263930,4.289376>, 1, 0)
// segment connecting nodes 32 and 31 with color 1
make_dislocation(<37.974550,-23.263930,4.289376>, <35.791790,-21.084980,4.293187>, 1, 0)
// segment connecting nodes 31 and 86 with color 1
make_dislocation(<35.791790,-21.084980,4.293187>, <33.604770,-18.901740,4.296972>, 1, 0)
// segment connecting nodes 86 and 118 with color 1
make_dislocation(<33.604770,-18.901740,4.296972>, <31.420910,-16.721610,4.300701>, 1, 0)
// segment connecting nodes 118 and 85 with color 1
make_dislocation(<31.420910,-16.721610,4.300701>, <29.242470,-14.546670,4.304198>, 1, 0)
// segment connecting nodes 85 and 42 with color 1
make_dislocation(<29.242470,-14.546670,4.304198>, <27.062060,-12.369600,4.307535>, 1, 0)
// segment connecting nodes 42 and 41 with color 1
make_dislocation(<27.062060,-12.369600,4.307535>, <24.881620,-10.191850,4.310233>, 1, 0)
// segment connecting nodes 41 and 130 with color 1
make_dislocation(<24.881620,-10.191850,4.310233>, <22.702120,-8.014570,4.312449>, 1, 0)
// segment connecting nodes 130 and 66 with color 1
make_dislocation(<22.702120,-8.014570,4.312449>, <20.533290,-5.847299,4.314003>, 1, 0)
// segment connecting nodes 66 and 65 with color 1
make_dislocation(<20.533290,-5.847299,4.314003>, <18.362850,-3.679453,4.316606>, 1, 0)
// segment connecting nodes 65 and 106 with color 1
make_dislocation(<18.362850,-3.679453,4.316606>, <16.210180,-1.532989,4.322812>, 1, 0)
// segment connecting nodes 106 and 33 with color 1
make_dislocation(<16.210180,-1.532989,4.322812>, <14.093710,0.596654,4.309639>, 1, 0)
// segment connecting nodes 33 and 20 with color 1
make_dislocation(<14.093710,0.596654,4.309639>, <13.347830,1.926476,3.725699>, 1, 0)
// segment connecting nodes 20 and 10 with color 1
make_dislocation(<13.347830,1.926476,3.725699>, <13.000000,3.000000,3.000000>, 1, 0)
// segment connecting nodes 4 and 23 with color 1
make_dislocation(<85.000000,-68.000000,2.000000>, <82.825940,-65.812410,1.986481>, 1, 1)
// segment connecting nodes 23 and 147 with color 1
make_dislocation(<82.825940,-65.812410,1.986481>, <80.651250,-63.625270,1.974015>, 1, 0)
// segment connecting nodes 147 and 146 with color 1
make_dislocation(<80.651250,-63.625270,1.974015>, <78.472810,-61.441570,1.968761>, 1, 0)
// segment connecting nodes 146 and 145 with color 1
make_dislocation(<78.472810,-61.441570,1.968761>, <76.294070,-59.258130,1.964061>, 1, 0)
// segment connecting nodes 145 and 104 with color 1
make_dislocation(<76.294070,-59.258130,1.964061>, <74.113990,-57.076150,1.962163>, 1, 0)
// segment connecting nodes 104 and 103 with color 1
make_dislocation(<74.113990,-57.076150,1.962163>, <71.933910,-54.894270,1.960357>, 1, 0)
// segment connecting nodes 103 and 126 with color 1
make_dislocation(<71.933910,-54.894270,1.960357>, <69.753630,-52.712060,1.958424>, 1, 0)
// segment connecting nodes 126 and 127 with color 1
make_dislocation(<69.753630,-52.712060,1.958424>, <67.573580,-50.530090,1.956504>, 1, 0)
// segment connecting nodes 127 and 102 with color 1
make_dislocation(<67.573580,-50.530090,1.956504>, <65.392660,-48.348510,1.955850>, 1, 0)
// segment connecting nodes 102 and 60 with color 1
make_dislocation(<65.392660,-48.348510,1.955850>, <63.212180,-46.167290,1.955113>, 1, 0)
// segment connecting nodes 60 and 59 with color 1
make_dislocation(<63.212180,-46.167290,1.955113>, <61.031380,-43.985700,1.954325>, 1, 0)
// segment connecting nodes 59 and 135 with color 1
make_dislocation(<61.031380,-43.985700,1.954325>, <58.850560,-41.804100,1.953542>, 1, 0)
// segment connecting nodes 135 and 79 with color 1
make_dislocation(<58.850560,-41.804100,1.953542>, <56.669950,-39.622610,1.952656>, 1, 0)
// segment connecting nodes 79 and 78 with color 1
make_dislocation(<56.669950,-39.622610,1.952656>, <54.489130,-37.440950,1.951818>, 1, 0)
// segment connecting nodes 78 and 115 with color 1
make_dislocation(<54.489130,-37.440950,1.951818>, <52.308570,-35.259450,1.950878>, 1, 0)
// segment connecting nodes 115 and 114 with color 1
make_dislocation(<52.308570,-35.259450,1.950878>, <50.128140,-33.078070,1.949933>, 1, 0)
// segment connecting nodes 114 and 80 with color 1
make_dislocation(<50.128140,-33.078070,1.949933>, <47.950930,-30.900780,1.949852>, 1, 0)
// segment connecting nodes 80 and 58 with color 1
make_dislocation(<47.950930,-30.900780,1.949852>, <45.773600,-28.723300,1.949708>, 1, 0)
// segment connecting nodes 58 and 57 with color 1
make_dislocation(<45.773600,-28.723300,1.949708>, <43.596380,-26.545920,1.949543>, 1, 0)
// segment connecting nodes 57 and 101 with color 1
make_dislocation(<43.596380,-26.545920,1.949543>, <41.419510,-24.368850,1.949337>, 1, 0)
// segment connecting nodes 101 and 35 with color 1
make_dislocation(<41.419510,-24.368850,1.949337>, <39.242280,-22.191370,1.949090>, 1, 0)
// segment connecting nodes 35 and 34 with color 1
make_dislocation(<39.242280,-22.191370,1.949090>, <37.065020,-20.013890,1.948871>, 1, 0)
// segment connecting nodes 34 and 88 with color 1
make_dislocation(<37.065020,-20.013890,1.948871>, <34.887490,-17.836080,1.948589>, 1, 0)
// segment connecting nodes 88 and 119 with color 1
make_dislocation(<34.887490,-17.836080,1.948589>, <32.710270,-15.658540,1.948279>, 1, 0)
// segment connecting nodes 119 and 87 with color 1
make_dislocation(<32.710270,-15.658540,1.948279>, <30.539840,-13.488460,1.948617>, 1, 0)
// segment connecting nodes 87 and 44 with color 1
make_dislocation(<30.539840,-13.488460,1.948617>, <28.369480,-11.318390,1.948909>, 1, 0)
// segment connecting nodes 44 and 131 with color 1
make_dislocation(<28.369480,-11.318390,1.948909>, <24.028650,-6.978623,1.949969>, 1, 0)
// segment connecting nodes 131 and 68 with color 1
make_dislocation(<24.028650,-6.978623,1.949969>, <21.866880,-4.820241,1.953359>, 1, 0)
// segment connecting nodes 68 and 67 with color 1
make_dislocation(<21.866880,-4.820241,1.953359>, <19.703760,-2.664790,1.961031>, 1, 0)
// segment connecting nodes 67 and 107 with color 1
make_dislocation(<19.703760,-2.664790,1.961031>, <17.544230,-0.527203,1.982975>, 1, 0)
// segment connecting nodes 107 and 36 with color 1
make_dislocation(<17.544230,-0.527203,1.982975>, <15.367160,1.595532,2.037307>, 1, 0)
// segment connecting nodes 36 and 22 with color 1
make_dislocation(<15.367160,1.595532,2.037307>, <14.135410,2.356505,2.508083>, 1, 0)
// segment connecting nodes 22 and 10 with color 1
make_dislocation(<14.135410,2.356505,2.508083>, <13.000000,3.000000,3.000000>, 1, 0)
// triangle color 1 nodes: [3, 4, 21]
make_triangle(<84.000000,-69.000000,4.000000>, <85.000000,-68.000000,2.000000>, <81.771630,-66.857860,4.086232>, 1)
// triangle color 1 nodes: [21, 4, 23]
make_triangle(<81.771630,-66.857860,4.086232>, <85.000000,-68.000000,2.000000>, <82.825940,-65.812410,1.986481>, 1)
// triangle color 1 nodes: [21, 23, 144]
make_triangle(<81.771630,-66.857860,4.086232>, <82.825940,-65.812410,1.986481>, <79.564720,-64.696130,4.131410>, 1)
// triangle color 1 nodes: [144, 23, 147]
make_triangle(<79.564720,-64.696130,4.131410>, <82.825940,-65.812410,1.986481>, <80.651250,-63.625270,1.974015>, 1)
// triangle color 1 nodes: [144, 147, 143]
make_triangle(<79.564720,-64.696130,4.131410>, <80.651250,-63.625270,1.974015>, <77.368190,-62.521690,4.153498>, 1)
// triangle color 1 nodes: [143, 147, 146]
make_triangle(<77.368190,-62.521690,4.153498>, <80.651250,-63.625270,1.974015>, <78.472810,-61.441570,1.968761>, 1)
// triangle color 1 nodes: [143, 146, 142]
make_triangle(<77.368190,-62.521690,4.153498>, <78.472810,-61.441570,1.968761>, <75.172330,-60.346860,4.174530>, 1)
// triangle color 1 nodes: [142, 146, 145]
make_triangle(<75.172330,-60.346860,4.174530>, <78.472810,-61.441570,1.968761>, <76.294070,-59.258130,1.964061>, 1)
// triangle color 1 nodes: [142, 145, 100]
make_triangle(<75.172330,-60.346860,4.174530>, <76.294070,-59.258130,1.964061>, <72.980540,-58.166290,4.185752>, 1)
// triangle color 1 nodes: [100, 145, 104]
make_triangle(<72.980540,-58.166290,4.185752>, <76.294070,-59.258130,1.964061>, <74.113990,-57.076150,1.962163>, 1)
// triangle color 1 nodes: [100, 104, 99]
make_triangle(<72.980540,-58.166290,4.185752>, <74.113990,-57.076150,1.962163>, <70.791220,-55.988020,4.196793>, 1)
// triangle color 1 nodes: [99, 104, 103]
make_triangle(<70.791220,-55.988020,4.196793>, <74.113990,-57.076150,1.962163>, <71.933910,-54.894270,1.960357>, 1)
// triangle color 1 nodes: [99, 103, 124]
make_triangle(<70.791220,-55.988020,4.196793>, <71.933910,-54.894270,1.960357>, <68.601490,-53.809280,4.207795>, 1)
// triangle color 1 nodes: [124, 103, 126]
make_triangle(<68.601490,-53.809280,4.207795>, <71.933910,-54.894270,1.960357>, <69.753630,-52.712060,1.958424>, 1)
// triangle color 1 nodes: [124, 126, 125]
make_triangle(<68.601490,-53.809280,4.207795>, <69.753630,-52.712060,1.958424>, <66.409490,-51.628080,4.218591>, 1)
// triangle color 1 nodes: [125, 126, 127]
make_triangle(<66.409490,-51.628080,4.218591>, <69.753630,-52.712060,1.958424>, <67.573580,-50.530090,1.956504>, 1)
// triangle color 1 nodes: [125, 127, 98]
make_triangle(<66.409490,-51.628080,4.218591>, <67.573580,-50.530090,1.956504>, <64.219600,-49.444510,4.224915>, 1)
// triangle color 1 nodes: [98, 127, 102]
make_triangle(<64.219600,-49.444510,4.224915>, <67.573580,-50.530090,1.956504>, <65.392660,-48.348510,1.955850>, 1)
// triangle color 1 nodes: [98, 102, 56]
make_triangle(<64.219600,-49.444510,4.224915>, <65.392660,-48.348510,1.955850>, <62.032170,-47.263420,4.231255>, 1)
// triangle color 1 nodes: [56, 102, 60]
make_triangle(<62.032170,-47.263420,4.231255>, <65.392660,-48.348510,1.955850>, <63.212180,-46.167290,1.955113>, 1)
// triangle color 1 nodes: [56, 60, 55]
make_triangle(<62.032170,-47.263420,4.231255>, <63.212180,-46.167290,1.955113>, <59.844020,-45.081640,4.237618>, 1)
// triangle color 1 nodes: [55, 60, 59]
make_triangle(<59.844020,-45.081640,4.237618>, <63.212180,-46.167290,1.955113>, <61.031380,-43.985700,1.954325>, 1)
// triangle color 1 nodes: [55, 59, 134]
make_triangle(<59.844020,-45.081640,4.237618>, <61.031380,-43.985700,1.954325>, <57.655730,-42.899700,4.243973>, 1)
// triangle color 1 nodes: [134, 59, 135]
make_triangle(<57.655730,-42.899700,4.243973>, <61.031380,-43.985700,1.954325>, <58.850560,-41.804100,1.953542>, 1)
// triangle color 1 nodes: [134, 135, 76]
make_triangle(<57.655730,-42.899700,4.243973>, <58.850560,-41.804100,1.953542>, <55.463780,-40.714160,4.250380>, 1)
// triangle color 1 nodes: [76, 135, 79]
make_triangle(<55.463780,-40.714160,4.250380>, <58.850560,-41.804100,1.953542>, <56.669950,-39.622610,1.952656>, 1)
// triangle color 1 nodes: [76, 79, 75]
make_triangle(<55.463780,-40.714160,4.250380>, <56.669950,-39.622610,1.952656>, <53.277750,-38.534470,4.256725>, 1)
// triangle color 1 nodes: [75, 79, 78]
make_triangle(<53.277750,-38.534470,4.256725>, <56.669950,-39.622610,1.952656>, <54.489130,-37.440950,1.951818>, 1)
// triangle color 1 nodes: [75, 78, 113]
make_triangle(<53.277750,-38.534470,4.256725>, <54.489130,-37.440950,1.951818>, <51.090260,-36.353380,4.263121>, 1)
// triangle color 1 nodes: [113, 78, 115]
make_triangle(<51.090260,-36.353380,4.263121>, <54.489130,-37.440950,1.951818>, <52.308570,-35.259450,1.950878>, 1)
// triangle color 1 nodes: [113, 115, 112]
make_triangle(<51.090260,-36.353380,4.263121>, <52.308570,-35.259450,1.950878>, <48.901340,-34.170790,4.269443>, 1)
// triangle color 1 nodes: [112, 115, 114]
make_triangle(<48.901340,-34.170790,4.269443>, <52.308570,-35.259450,1.950878>, <50.128140,-33.078070,1.949933>, 1)
// triangle color 1 nodes: [112, 114, 77]
make_triangle(<48.901340,-34.170790,4.269443>, <50.128140,-33.078070,1.949933>, <46.715290,-31.988690,4.273403>, 1)
// triangle color 1 nodes: [77, 114, 80]
make_triangle(<46.715290,-31.988690,4.273403>, <50.128140,-33.078070,1.949933>, <47.950930,-30.900780,1.949852>, 1)
// triangle color 1 nodes: [77, 80, 54]
make_triangle(<46.715290,-31.988690,4.273403>, <47.950930,-30.900780,1.949852>, <44.529360,-29.806780,4.277414>, 1)
// triangle color 1 nodes: [54, 80, 58]
make_triangle(<44.529360,-29.806780,4.277414>, <47.950930,-30.900780,1.949852>, <45.773600,-28.723300,1.949708>, 1)
// triangle color 1 nodes: [54, 58, 53]
make_triangle(<44.529360,-29.806780,4.277414>, <45.773600,-28.723300,1.949708>, <42.344550,-27.626020,4.281472>, 1)
// triangle color 1 nodes: [53, 58, 57]
make_triangle(<42.344550,-27.626020,4.281472>, <45.773600,-28.723300,1.949708>, <43.596380,-26.545920,1.949543>, 1)
// triangle color 1 nodes: [53, 57, 97]
make_triangle(<42.344550,-27.626020,4.281472>, <43.596380,-26.545920,1.949543>, <40.161830,-25.447340,4.285509>, 1)
// triangle color 1 nodes: [97, 57, 101]
make_triangle(<40.161830,-25.447340,4.285509>, <43.596380,-26.545920,1.949543>, <41.419510,-24.368850,1.949337>, 1)
// triangle color 1 nodes: [97, 101, 32]
make_triangle(<40.161830,-25.447340,4.285509>, <41.419510,-24.368850,1.949337>, <37.974550,-23.263930,4.289376>, 1)
// triangle color 1 nodes: [32, 101, 35]
make_triangle(<37.974550,-23.263930,4.289376>, <41.419510,-24.368850,1.949337>, <39.242280,-22.191370,1.949090>, 1)
// triangle color 1 nodes: [32, 35, 31]
make_triangle(<37.974550,-23.263930,4.289376>, <39.242280,-22.191370,1.949090>, <35.791790,-21.084980,4.293187>, 1)
// triangle color 1 nodes: [31, 35, 34]
make_triangle(<35.791790,-21.084980,4.293187>, <39.242280,-22.191370,1.949090>, <37.065020,-20.013890,1.948871>, 1)
// triangle color 1 nodes: [31, 34, 86]
make_triangle(<35.791790,-21.084980,4.293187>, <37.065020,-20.013890,1.948871>, <33.604770,-18.901740,4.296972>, 1)
// triangle color 1 nodes: [86, 34, 88]
make_triangle(<33.604770,-18.901740,4.296972>, <37.065020,-20.013890,1.948871>, <34.887490,-17.836080,1.948589>, 1)
// triangle color 1 nodes: [86, 88, 118]
make_triangle(<33.604770,-18.901740,4.296972>, <34.887490,-17.836080,1.948589>, <31.420910,-16.721610,4.300701>, 1)
// triangle color 1 nodes: [118, 88, 119]
make_triangle(<31.420910,-16.721610,4.300701>, <34.887490,-17.836080,1.948589>, <32.710270,-15.658540,1.948279>, 1)
// triangle color 1 nodes: [118, 119, 85]
make_triangle(<31.420910,-16.721610,4.300701>, <32.710270,-15.658540,1.948279>, <29.242470,-14.546670,4.304198>, 1)
// triangle color 1 nodes: [85, 119, 87]
make_triangle(<29.242470,-14.546670,4.304198>, <32.710270,-15.658540,1.948279>, <30.539840,-13.488460,1.948617>, 1)
// triangle color 1 nodes: [85, 87, 42]
make_triangle(<29.242470,-14.546670,4.304198>, <30.539840,-13.488460,1.948617>, <27.062060,-12.369600,4.307535>, 1)
// triangle color 1 nodes: [42, 87, 44]
make_triangle(<27.062060,-12.369600,4.307535>, <30.539840,-13.488460,1.948617>, <28.369480,-11.318390,1.948909>, 1)
// triangle color 1 nodes: [42, 44, 41]
make_triangle(<27.062060,-12.369600,4.307535>, <28.369480,-11.318390,1.948909>, <24.881620,-10.191850,4.310233>, 1)
// triangle color 1 nodes: [41, 44, 131]
make_triangle(<24.881620,-10.191850,4.310233>, <28.369480,-11.318390,1.948909>, <24.028650,-6.978623,1.949969>, 1)
// triangle color 1 nodes: [41, 131, 130]
make_triangle(<24.881620,-10.191850,4.310233>, <24.028650,-6.978623,1.949969>, <22.702120,-8.014570,4.312449>, 1)
// triangle color 1 nodes: [130, 131, 68]
make_triangle(<22.702120,-8.014570,4.312449>, <24.028650,-6.978623,1.949969>, <21.866880,-4.820241,1.953359>, 1)
// triangle color 1 nodes: [130, 68, 66]
make_triangle(<22.702120,-8.014570,4.312449>, <21.866880,-4.820241,1.953359>, <20.533290,-5.847299,4.314003>, 1)
// triangle color 1 nodes: [66, 68, 67]
make_triangle(<20.533290,-5.847299,4.314003>, <21.866880,-4.820241,1.953359>, <19.703760,-2.664790,1.961031>, 1)
// triangle color 1 nodes: [66, 67, 65]
make_triangle(<20.533290,-5.847299,4.314003>, <19.703760,-2.664790,1.961031>, <18.362850,-3.679453,4.316606>, 1)
// triangle color 1 nodes: [65, 67, 107]
make_triangle(<18.362850,-3.679453,4.316606>, <19.703760,-2.664790,1.961031>, <17.544230,-0.527203,1.982975>, 1)
// triangle color 1 nodes: [65, 107, 106]
make_triangle(<18.362850,-3.679453,4.316606>, <17.544230,-0.527203,1.982975>, <16.210180,-1.532989,4.322812>, 1)
// triangle color 1 nodes: [106, 107, 36]
make_triangle(<16.210180,-1.532989,4.322812>, <17.544230,-0.527203,1.982975>, <15.367160,1.595532,2.037307>, 1)
// triangle color 1 nodes: [106, 36, 33]
make_triangle(<16.210180,-1.532989,4.322812>, <15.367160,1.595532,2.037307>, <14.093710,0.596654,4.309639>, 1)
// triangle color 1 nodes: [33, 36, 22]
make_triangle(<14.093710,0.596654,4.309639>, <15.367160,1.595532,2.037307>, <14.135410,2.356505,2.508083>, 1)
// triangle color 1 nodes: [33, 22, 20]
make_triangle(<14.093710,0.596654,4.309639>, <14.135410,2.356505,2.508083>, <13.347830,1.926476,3.725699>, 1)
// triangle color 1 nodes: [20, 22, 10]
make_triangle(<13.347830,1.926476,3.725699>, <14.135410,2.356505,2.508083>, <13.000000,3.000000,3.000000>, 1)
// triangle color 1 nodes: [20, 10, 10]
make_triangle(<13.347830,1.926476,3.725699>, <13.000000,3.000000,3.000000>, <13.000000,3.000000,3.000000>, 1)
// WriteStrip([1, 1, (8, 12, 24, 62, 61, 128, 38, 37, 81, 116, 82, 26, 25, 89, 46, 45, 71, 108, 109, 70, 69, 132, 48, 47, 90, 121, 120, 92, 91, 136, 138, 137, 11, 1), (8, 14, 27, 105, 64, 63, 129, 40, 39, 83, 117, 84, 29, 28, 93, 50, 49, 74, 110, 111, 73, 72, 133, 52, 51, 94, 123, 122, 96, 95, 139, 141, 140, 13, 2)])
// segment connecting nodes 8 and 14 with color 1
make_dislocation(<3.000000,13.000000,3.000000>, <2.528333,14.021660,2.450005>, 1, 1)
// segment connecting nodes 14 and 27 with color 1
make_dislocation(<2.528333,14.021660,2.450005>, <1.609322,15.394620,1.996062>, 1, 0)
// segment connecting nodes 27 and 105 with color 1
make_dislocation(<1.609322,15.394620,1.996062>, <-0.469734,17.501870,1.967864>, 1, 0)
// segment connecting nodes 105 and 64 with color 1
make_dislocation(<-0.469734,17.501870,1.967864>, <-2.593136,19.633760,1.959376>, 1, 0)
// segment connecting nodes 64 and 63 with color 1
make_dislocation(<-2.593136,19.633760,1.959376>, <-4.746472,21.791600,1.954869>, 1, 0)
// segment connecting nodes 63 and 129 with color 1
make_dislocation(<-4.746472,21.791600,1.954869>, <-6.900838,23.949100,1.951735>, 1, 0)
// segment connecting nodes 129 and 40 with color 1
make_dislocation(<-6.900838,23.949100,1.951735>, <-9.071927,26.121800,1.950123>, 1, 0)
// segment connecting nodes 40 and 39 with color 1
make_dislocation(<-9.071927,26.121800,1.950123>, <-11.243480,28.294610,1.948864>, 1, 0)
// segment connecting nodes 39 and 83 with color 1
make_dislocation(<-11.243480,28.294610,1.948864>, <-13.415260,30.466980,1.948277>, 1, 0)
// segment connecting nodes 83 and 117 with color 1
make_dislocation(<-13.415260,30.466980,1.948277>, <-15.586980,32.639160,1.947823>, 1, 0)
// segment connecting nodes 117 and 84 with color 1
make_dislocation(<-15.586980,32.639160,1.947823>, <-17.767260,34.819410,1.947854>, 1, 0)
// segment connecting nodes 84 and 29 with color 1
make_dislocation(<-17.767260,34.819410,1.947854>, <-19.947580,36.999690,1.947893>, 1, 0)
// segment connecting nodes 29 and 28 with color 1
make_dislocation(<-19.947580,36.999690,1.947893>, <-22.127940,39.180050,1.947889>, 1, 0)
// segment connecting nodes 28 and 93 with color 1
make_dislocation(<-22.127940,39.180050,1.947889>, <-24.308070,41.360080,1.947986>, 1, 0)
// segment connecting nodes 93 and 50 with color 1
make_dislocation(<-24.308070,41.360080,1.947986>, <-26.488760,43.540530,1.948230>, 1, 0)
// segment connecting nodes 50 and 49 with color 1
make_dislocation(<-26.488760,43.540530,1.948230>, <-28.668770,45.720310,1.948460>, 1, 0)
// segment connecting nodes 49 and 74 with color 1
make_dislocation(<-28.668770,45.720310,1.948460>, <-30.849500,47.900810,1.948687>, 1, 0)
// segment connecting nodes 74 and 110 with color 1
make_dislocation(<-30.849500,47.900810,1.948687>, <-33.029720,50.080860,1.948861>, 1, 0)
// segment connecting nodes 110 and 111 with color 1
make_dislocation(<-33.029720,50.080860,1.948861>, <-35.214150,52.264270,1.949879>, 1, 0)
// segment connecting nodes 111 and 73 with color 1
make_dislocation(<-35.214150,52.264270,1.949879>, <-37.398580,54.447700,1.950883>, 1, 0)
// segment connecting nodes 73 and 72 with color 1
make_dislocation(<-37.398580,54.447700,1.950883>, <-39.582670,56.630890,1.951783>, 1, 0)
// segment connecting nodes 72 and 133 with color 1
make_dislocation(<-39.582670,56.630890,1.951783>, <-41.767250,58.814510,1.952739>, 1, 0)
// segment connecting nodes 133 and 52 with color 1
make_dislocation(<-41.767250,58.814510,1.952739>, <-43.951880,60.998290,1.953589>, 1, 0)
// segment connecting nodes 52 and 51 with color 1
make_dislocation(<-43.951880,60.998290,1.953589>, <-46.136120,63.181670,1.954454>, 1, 0)
// segment connecting nodes 51 and 94 with color 1
make_dislocation(<-46.136120,63.181670,1.954454>, <-48.321060,65.365780,1.955275>, 1, 0)
// segment connecting nodes 94 and 123 with color 1
make_dislocation(<-48.321060,65.365780,1.955275>, <-50.505420,67.549380,1.956033>, 1, 0)
// segment connecting nodes 123 and 122 with color 1
make_dislocation(<-50.505420,67.549380,1.956033>, <-52.690640,69.732560,1.958084>, 1, 0)
// segment connecting nodes 122 and 96 with color 1
make_dislocation(<-52.690640,69.732560,1.958084>, <-54.875640,71.915500,1.960134>, 1, 0)
// segment connecting nodes 96 and 95 with color 1
make_dislocation(<-54.875640,71.915500,1.960134>, <-57.060680,74.098610,1.962074>, 1, 0)
// segment connecting nodes 95 and 139 with color 1
make_dislocation(<-57.060680,74.098610,1.962074>, <-59.245880,76.281650,1.964232>, 1, 0)
// segment connecting nodes 139 and 141 with color 1
make_dislocation(<-59.245880,76.281650,1.964232>, <-61.432620,78.463340,1.969282>, 1, 0)
// segment connecting nodes 141 and 140 with color 1
make_dislocation(<-61.432620,78.463340,1.969282>, <-63.619530,80.644330,1.975199>, 1, 0)
// segment connecting nodes 140 and 13 with color 1
make_dislocation(<-63.619530,80.644330,1.975199>, <-65.809810,82.822160,1.987657>, 1, 0)
// segment connecting nodes 13 and 2 with color 1
make_dislocation(<-65.809810,82.822160,1.987657>, <-68.000000,85.000000,2.000000>, 1, 0)
// segment connecting nodes 8 and 12 with color 1
make_dislocation(<3.000000,13.000000,3.000000>, <1.960506,13.482830,3.556669>, 1, 1)
// segment connecting nodes 12 and 24 with color 1
make_dislocation(<1.960506,13.482830,3.556669>, <0.136354,14.624590,4.239053>, 1, 0)
// segment connecting nodes 24 and 62 with color 1
make_dislocation(<0.136354,14.624590,4.239053>, <-4.020454,18.725690,4.294769>, 1, 0)
// segment connecting nodes 62 and 61 with color 1
make_dislocation(<-4.020454,18.725690,4.294769>, <-6.160735,20.856260,4.304476>, 1, 0)
// segment connecting nodes 61 and 128 with color 1
make_dislocation(<-6.160735,20.856260,4.304476>, <-8.297923,22.992600,4.305324>, 1, 0)
// segment connecting nodes 128 and 38 with color 1
make_dislocation(<-8.297923,22.992600,4.305324>, <-10.457860,25.154970,4.302899>, 1, 0)
// segment connecting nodes 38 and 37 with color 1
make_dislocation(<-10.457860,25.154970,4.302899>, <-12.618780,27.318390,4.300387>, 1, 0)
// segment connecting nodes 37 and 81 with color 1
make_dislocation(<-12.618780,27.318390,4.300387>, <-14.777650,29.480220,4.297434>, 1, 0)
// segment connecting nodes 81 and 116 with color 1
make_dislocation(<-14.777650,29.480220,4.297434>, <-16.936000,31.641530,4.294470>, 1, 0)
// segment connecting nodes 116 and 82 with color 1
make_dislocation(<-16.936000,31.641530,4.294470>, <-19.106380,33.815660,4.290717>, 1, 0)
// segment connecting nodes 82 and 26 with color 1
make_dislocation(<-19.106380,33.815660,4.290717>, <-21.277800,35.990820,4.286975>, 1, 0)
// segment connecting nodes 26 and 25 with color 1
make_dislocation(<-21.277800,35.990820,4.286975>, <-23.444060,38.160760,4.283302>, 1, 0)
// segment connecting nodes 25 and 89 with color 1
make_dislocation(<-23.444060,38.160760,4.283302>, <-25.615530,40.335930,4.279608>, 1, 0)
// segment connecting nodes 89 and 46 with color 1
make_dislocation(<-25.615530,40.335930,4.279608>, <-27.784950,42.509060,4.275892>, 1, 0)
// segment connecting nodes 46 and 45 with color 1
make_dislocation(<-27.784950,42.509060,4.275892>, <-29.955870,44.683640,4.272232>, 1, 0)
// segment connecting nodes 45 and 71 with color 1
make_dislocation(<-29.955870,44.683640,4.272232>, <-32.126640,46.858130,4.268502>, 1, 0)
// segment connecting nodes 71 and 108 with color 1
make_dislocation(<-32.126640,46.858130,4.268502>, <-34.295280,49.030560,4.264722>, 1, 0)
// segment connecting nodes 108 and 109 with color 1
make_dislocation(<-34.295280,49.030560,4.264722>, <-36.469960,51.211460,4.258501>, 1, 0)
// segment connecting nodes 109 and 70 with color 1
make_dislocation(<-36.469960,51.211460,4.258501>, <-38.644820,53.392550,4.252278>, 1, 0)
// segment connecting nodes 70 and 69 with color 1
make_dislocation(<-38.644820,53.392550,4.252278>, <-40.819060,55.572930,4.246125>, 1, 0)
// segment connecting nodes 69 and 132 with color 1
make_dislocation(<-40.819060,55.572930,4.246125>, <-42.993420,57.753510,4.239909>, 1, 0)
// segment connecting nodes 132 and 48 with color 1
make_dislocation(<-42.993420,57.753510,4.239909>, <-45.165940,59.932130,4.233815>, 1, 0)
// segment connecting nodes 48 and 47 with color 1
make_dislocation(<-45.165940,59.932130,4.233815>, <-47.339360,62.111680,4.227670>, 1, 0)
// segment connecting nodes 47 and 90 with color 1
make_dislocation(<-47.339360,62.111680,4.227670>, <-49.513460,64.291990,4.221465>, 1, 0)
// segment connecting nodes 90 and 121 with color 1
make_dislocation(<-49.513460,64.291990,4.221465>, <-51.690630,66.475600,4.215027>, 1, 0)
// segment connecting nodes 121 and 120 with color 1
make_dislocation(<-51.690630,66.475600,4.215027>, <-53.861100,68.656810,4.204282>, 1, 0)
// segment connecting nodes 120 and 92 with color 1
make_dislocation(<-53.861100,68.656810,4.204282>, <-56.033370,70.839910,4.193458>, 1, 0)
// segment connecting nodes 92 and 91 with color 1
make_dislocation(<-56.033370,70.839910,4.193458>, <-58.205310,73.022730,4.182579>, 1, 0)
// segment connecting nodes 91 and 136 with color 1
make_dislocation(<-58.205310,73.022730,4.182579>, <-60.376290,75.205350,4.170942>, 1, 0)
// segment connecting nodes 136 and 138 with color 1
make_dislocation(<-60.376290,75.205350,4.170942>, <-62.540960,77.391280,4.149677>, 1, 0)
// segment connecting nodes 138 and 137 with color 1
make_dislocation(<-62.540960,77.391280,4.149677>, <-64.709560,79.582780,4.126784>, 1, 0)
// segment connecting nodes 137 and 11 with color 1
make_dislocation(<-64.709560,79.582780,4.126784>, <-66.864470,81.782380,4.082094>, 1, 0)
// segment connecting nodes 11 and 1 with color 1
make_dislocation(<-66.864470,81.782380,4.082094>, <-69.000000,84.000000,4.000000>, 1, 0)
// triangle color 1 nodes: [8, 8, 14]
make_triangle(<3.000000,13.000000,3.000000>, <3.000000,13.000000,3.000000>, <2.528333,14.021660,2.450005>, 1)
// triangle color 1 nodes: [14, 8, 12]
make_triangle(<2.528333,14.021660,2.450005>, <3.000000,13.000000,3.000000>, <1.960506,13.482830,3.556669>, 1)
// triangle color 1 nodes: [14, 12, 27]
make_triangle(<2.528333,14.021660,2.450005>, <1.960506,13.482830,3.556669>, <1.609322,15.394620,1.996062>, 1)
// triangle color 1 nodes: [27, 12, 24]
make_triangle(<1.609322,15.394620,1.996062>, <1.960506,13.482830,3.556669>, <0.136354,14.624590,4.239053>, 1)
// triangle color 1 nodes: [27, 24, 105]
make_triangle(<1.609322,15.394620,1.996062>, <0.136354,14.624590,4.239053>, <-0.469734,17.501870,1.967864>, 1)
// triangle color 1 nodes: [105, 24, 62]
make_triangle(<-0.469734,17.501870,1.967864>, <0.136354,14.624590,4.239053>, <-4.020454,18.725690,4.294769>, 1)
// triangle color 1 nodes: [105, 62, 64]
make_triangle(<-0.469734,17.501870,1.967864>, <-4.020454,18.725690,4.294769>, <-2.593136,19.633760,1.959376>, 1)
// triangle color 1 nodes: [64, 62, 61]
make_triangle(<-2.593136,19.633760,1.959376>, <-4.020454,18.725690,4.294769>, <-6.160735,20.856260,4.304476>, 1)
// triangle color 1 nodes: [64, 61, 63]
make_triangle(<-2.593136,19.633760,1.959376>, <-6.160735,20.856260,4.304476>, <-4.746472,21.791600,1.954869>, 1)
// triangle color 1 nodes: [63, 61, 128]
make_triangle(<-4.746472,21.791600,1.954869>, <-6.160735,20.856260,4.304476>, <-8.297923,22.992600,4.305324>, 1)
// triangle color 1 nodes: [63, 128, 129]
make_triangle(<-4.746472,21.791600,1.954869>, <-8.297923,22.992600,4.305324>, <-6.900838,23.949100,1.951735>, 1)
// triangle color 1 nodes: [129, 128, 38]
make_triangle(<-6.900838,23.949100,1.951735>, <-8.297923,22.992600,4.305324>, <-10.457860,25.154970,4.302899>, 1)
// triangle color 1 nodes: [129, 38, 40]
make_triangle(<-6.900838,23.949100,1.951735>, <-10.457860,25.154970,4.302899>, <-9.071927,26.121800,1.950123>, 1)
// triangle color 1 nodes: [40, 38, 37]
make_triangle(<-9.071927,26.121800,1.950123>, <-10.457860,25.154970,4.302899>, <-12.618780,27.318390,4.300387>, 1)
// triangle color 1 nodes: [40, 37, 39]
make_triangle(<-9.071927,26.121800,1.950123>, <-12.618780,27.318390,4.300387>, <-11.243480,28.294610,1.948864>, 1)
// triangle color 1 nodes: [39, 37, 81]
make_triangle(<-11.243480,28.294610,1.948864>, <-12.618780,27.318390,4.300387>, <-14.777650,29.480220,4.297434>, 1)
// triangle color 1 nodes: [39, 81, 83]
make_triangle(<-11.243480,28.294610,1.948864>, <-14.777650,29.480220,4.297434>, <-13.415260,30.466980,1.948277>, 1)
// triangle color 1 nodes: [83, 81, 116]
make_triangle(<-13.415260,30.466980,1.948277>, <-14.777650,29.480220,4.297434>, <-16.936000,31.641530,4.294470>, 1)
// triangle color 1 nodes: [83, 116, 117]
make_triangle(<-13.415260,30.466980,1.948277>, <-16.936000,31.641530,4.294470>, <-15.586980,32.639160,1.947823>, 1)
// triangle color 1 nodes: [117, 116, 82]
make_triangle(<-15.586980,32.639160,1.947823>, <-16.936000,31.641530,4.294470>, <-19.106380,33.815660,4.290717>, 1)
// triangle color 1 nodes: [117, 82, 84]
make_triangle(<-15.586980,32.639160,1.947823>, <-19.106380,33.815660,4.290717>, <-17.767260,34.819410,1.947854>, 1)
// triangle color 1 nodes: [84, 82, 26]
make_triangle(<-17.767260,34.819410,1.947854>, <-19.106380,33.815660,4.290717>, <-21.277800,35.990820,4.286975>, 1)
// triangle color 1 nodes: [84, 26, 29]
make_triangle(<-17.767260,34.819410,1.947854>, <-21.277800,35.990820,4.286975>, <-19.947580,36.999690,1.947893>, 1)
// triangle color 1 nodes: [29, 26, 25]
make_triangle(<-19.947580,36.999690,1.947893>, <-21.277800,35.990820,4.286975>, <-23.444060,38.160760,4.283302>, 1)
// triangle color 1 nodes: [29, 25, 28]
make_triangle(<-19.947580,36.999690,1.947893>, <-23.444060,38.160760,4.283302>, <-22.127940,39.180050,1.947889>, 1)
// triangle color 1 nodes: [28, 25, 89]
make_triangle(<-22.127940,39.180050,1.947889>, <-23.444060,38.160760,4.283302>, <-25.615530,40.335930,4.279608>, 1)
// triangle color 1 nodes: [28, 89, 93]
make_triangle(<-22.127940,39.180050,1.947889>, <-25.615530,40.335930,4.279608>, <-24.308070,41.360080,1.947986>, 1)
// triangle color 1 nodes: [93, 89, 46]
make_triangle(<-24.308070,41.360080,1.947986>, <-25.615530,40.335930,4.279608>, <-27.784950,42.509060,4.275892>, 1)
// triangle color 1 nodes: [93, 46, 50]
make_triangle(<-24.308070,41.360080,1.947986>, <-27.784950,42.509060,4.275892>, <-26.488760,43.540530,1.948230>, 1)
// triangle color 1 nodes: [50, 46, 45]
make_triangle(<-26.488760,43.540530,1.948230>, <-27.784950,42.509060,4.275892>, <-29.955870,44.683640,4.272232>, 1)
// triangle color 1 nodes: [50, 45, 49]
make_triangle(<-26.488760,43.540530,1.948230>, <-29.955870,44.683640,4.272232>, <-28.668770,45.720310,1.948460>, 1)
// triangle color 1 nodes: [49, 45, 71]
make_triangle(<-28.668770,45.720310,1.948460>, <-29.955870,44.683640,4.272232>, <-32.126640,46.858130,4.268502>, 1)
// triangle color 1 nodes: [49, 71, 74]
make_triangle(<-28.668770,45.720310,1.948460>, <-32.126640,46.858130,4.268502>, <-30.849500,47.900810,1.948687>, 1)
// triangle color 1 nodes: [74, 71, 108]
make_triangle(<-30.849500,47.900810,1.948687>, <-32.126640,46.858130,4.268502>, <-34.295280,49.030560,4.264722>, 1)
// triangle color 1 nodes: [74, 108, 110]
make_triangle(<-30.849500,47.900810,1.948687>, <-34.295280,49.030560,4.264722>, <-33.029720,50.080860,1.948861>, 1)
// triangle color 1 nodes: [110, 108, 109]
make_triangle(<-33.029720,50.080860,1.948861>, <-34.295280,49.030560,4.264722>, <-36.469960,51.211460,4.258501>, 1)
// triangle color 1 nodes: [110, 109, 111]
make_triangle(<-33.029720,50.080860,1.948861>, <-36.469960,51.211460,4.258501>, <-35.214150,52.264270,1.949879>, 1)
// triangle color 1 nodes: [111, 109, 70]
make_triangle(<-35.214150,52.264270,1.949879>, <-36.469960,51.211460,4.258501>, <-38.644820,53.392550,4.252278>, 1)
// triangle color 1 nodes: [111, 70, 73]
make_triangle(<-35.214150,52.264270,1.949879>, <-38.644820,53.392550,4.252278>, <-37.398580,54.447700,1.950883>, 1)
// triangle color 1 nodes: [73, 70, 69]
make_triangle(<-37.398580,54.447700,1.950883>, <-38.644820,53.392550,4.252278>, <-40.819060,55.572930,4.246125>, 1)
// triangle color 1 nodes: [73, 69, 72]
make_triangle(<-37.398580,54.447700,1.950883>, <-40.819060,55.572930,4.246125>, <-39.582670,56.630890,1.951783>, 1)
// triangle color 1 nodes: [72, 69, 132]
make_triangle(<-39.582670,56.630890,1.951783>, <-40.819060,55.572930,4.246125>, <-42.993420,57.753510,4.239909>, 1)
// triangle color 1 nodes: [72, 132, 133]
make_triangle(<-39.582670,56.630890,1.951783>, <-42.993420,57.753510,4.239909>, <-41.767250,58.814510,1.952739>, 1)
// triangle color 1 nodes: [133, 132, 48]
make_triangle(<-41.767250,58.814510,1.952739>, <-42.993420,57.753510,4.239909>, <-45.165940,59.932130,4.233815>, 1)
// triangle color 1 nodes: [133, 48, 52]
make_triangle(<-41.767250,58.814510,1.952739>, <-45.165940,59.932130,4.233815>, <-43.951880,60.998290,1.953589>, 1)
// triangle color 1 nodes: [52, 48, 47]
make_triangle(<-43.951880,60.998290,1.953589>, <-45.165940,59.932130,4.233815>, <-47.339360,62.111680,4.227670>, 1)
// triangle color 1 nodes: [52, 47, 51]
make_triangle(<-43.951880,60.998290,1.953589>, <-47.339360,62.111680,4.227670>, <-46.136120,63.181670,1.954454>, 1)
// triangle color 1 nodes: [51, 47, 90]
make_triangle(<-46.136120,63.181670,1.954454>, <-47.339360,62.111680,4.227670>, <-49.513460,64.291990,4.221465>, 1)
// triangle color 1 nodes: [51, 90, 94]
make_triangle(<-46.136120,63.181670,1.954454>, <-49.513460,64.291990,4.221465>, <-48.321060,65.365780,1.955275>, 1)
// triangle color 1 nodes: [94, 90, 121]
make_triangle(<-48.321060,65.365780,1.955275>, <-49.513460,64.291990,4.221465>, <-51.690630,66.475600,4.215027>, 1)
// triangle color 1 nodes: [94, 121, 123]
make_triangle(<-48.321060,65.365780,1.955275>, <-51.690630,66.475600,4.215027>, <-50.505420,67.549380,1.956033>, 1)
// triangle color 1 nodes: [123, 121, 120]
make_triangle(<-50.505420,67.549380,1.956033>, <-51.690630,66.475600,4.215027>, <-53.861100,68.656810,4.204282>, 1)
// triangle color 1 nodes: [123, 120, 122]
make_triangle(<-50.505420,67.549380,1.956033>, <-53.861100,68.656810,4.204282>, <-52.690640,69.732560,1.958084>, 1)
// triangle color 1 nodes: [122, 120, 92]
make_triangle(<-52.690640,69.732560,1.958084>, <-53.861100,68.656810,4.204282>, <-56.033370,70.839910,4.193458>, 1)
// triangle color 1 nodes: [122, 92, 96]
make_triangle(<-52.690640,69.732560,1.958084>, <-56.033370,70.839910,4.193458>, <-54.875640,71.915500,1.960134>, 1)
// triangle color 1 nodes: [96, 92, 91]
make_triangle(<-54.875640,71.915500,1.960134>, <-56.033370,70.839910,4.193458>, <-58.205310,73.022730,4.182579>, 1)
// triangle color 1 nodes: [96, 91, 95]
make_triangle(<-54.875640,71.915500,1.960134>, <-58.205310,73.022730,4.182579>, <-57.060680,74.098610,1.962074>, 1)
// triangle color 1 nodes: [95, 91, 136]
make_triangle(<-57.060680,74.098610,1.962074>, <-58.205310,73.022730,4.182579>, <-60.376290,75.205350,4.170942>, 1)
// triangle color 1 nodes: [95, 136, 139]
make_triangle(<-57.060680,74.098610,1.962074>, <-60.376290,75.205350,4.170942>, <-59.245880,76.281650,1.964232>, 1)
// triangle color 1 nodes: [139, 136, 138]
make_triangle(<-59.245880,76.281650,1.964232>, <-60.376290,75.205350,4.170942>, <-62.540960,77.391280,4.149677>, 1)
// triangle color 1 nodes: [139, 138, 141]
make_triangle(<-59.245880,76.281650,1.964232>, <-62.540960,77.391280,4.149677>, <-61.432620,78.463340,1.969282>, 1)
// triangle color 1 nodes: [141, 138, 137]
make_triangle(<-61.432620,78.463340,1.969282>, <-62.540960,77.391280,4.149677>, <-64.709560,79.582780,4.126784>, 1)
// triangle color 1 nodes: [141, 137, 140]
make_triangle(<-61.432620,78.463340,1.969282>, <-64.709560,79.582780,4.126784>, <-63.619530,80.644330,1.975199>, 1)
// triangle color 1 nodes: [140, 137, 11]
make_triangle(<-63.619530,80.644330,1.975199>, <-64.709560,79.582780,4.126784>, <-66.864470,81.782380,4.082094>, 1)
// triangle color 1 nodes: [140, 11, 13]
make_triangle(<-63.619530,80.644330,1.975199>, <-66.864470,81.782380,4.082094>, <-65.809810,82.822160,1.987657>, 1)
// triangle color 1 nodes: [13, 11, 1]
make_triangle(<-65.809810,82.822160,1.987657>, <-66.864470,81.782380,4.082094>, <-69.000000,84.000000,4.000000>, 1)
// triangle color 1 nodes: [13, 1, 2]
make_triangle(<-65.809810,82.822160,1.987657>, <-69.000000,84.000000,4.000000>, <-68.000000,85.000000,2.000000>, 1)
// WriteDislocation([1, (10, 19, 30, 18, 8)])
// segment connecting nodes 19 and 10 with color 1
make_dislocation(<11.037010,5.702104,3.739117>, <13.000000,3.000000,3.000000>, 1, 1)
// segment connecting nodes 30 and 19 with color 1
make_dislocation(<8.613341,8.439738,4.053079>, <11.037010,5.702104,3.739117>, 1, 0)
// segment connecting nodes 18 and 30 with color 1
make_dislocation(<5.747016,10.998620,3.745641>, <8.613341,8.439738,4.053079>, 1, 0)
// segment connecting nodes 8 and 18 with color 1
make_dislocation(<3.000000,13.000000,3.000000>, <5.747016,10.998620,3.745641>, 1, 0)
// WriteDislocation([1, (10, 17, 16, 15, 8)])
// segment connecting nodes 17 and 10 with color 1
make_dislocation(<11.073400,4.076123,2.149521>, <13.000000,3.000000,3.000000>, 1, 1)
// segment connecting nodes 16 and 17 with color 1
make_dislocation(<8.442955,6.433807,1.876762>, <11.073400,4.076123,2.149521>, 1, 0)
// segment connecting nodes 15 and 16 with color 1
make_dislocation(<5.543269,9.737926,2.281195>, <8.442955,6.433807,1.876762>, 1, 0)
// segment connecting nodes 8 and 15 with color 1
make_dislocation(<3.000000,13.000000,3.000000>, <5.543269,9.737926,2.281195>, 1, 0)
