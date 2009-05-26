#declare lookatpos = <8.000000,8.000000,6.500000>;
#declare camerapos = <8.000000,8.000000,169.000000>;
#declare viewx = x * 154.000000;
#declare viewy = y * 154.000000;
#include "jaime.inc"
// WriteFan([0, 1, (10, 11, 20, 31, 19, 9)])
// segment connecting nodes 11 and 10 with color 0
make_dislocation(<13.000000,3.000000,3.000000>, <13.000000,13.000000,13.000000>, 0, 0)
// triangle color 1 nodes: [10, 11, 20]
make_triangle(<13.000000,13.000000,13.000000>, <13.000000,3.000000,3.000000>, <11.442970,6.112888,4.555856>, 1)
// segment connecting nodes 20 and 11 with color 0
make_dislocation(<11.442970,6.112888,4.555856>, <13.000000,3.000000,3.000000>, 0, 0)
// triangle color 1 nodes: [10, 20, 31]
make_triangle(<13.000000,13.000000,13.000000>, <11.442970,6.112888,4.555856>, <8.044220,10.355830,5.400051>, 1)
// segment connecting nodes 31 and 20 with color 0
make_dislocation(<8.044220,10.355830,5.400051>, <11.442970,6.112888,4.555856>, 0, 0)
// triangle color 1 nodes: [10, 31, 19]
make_triangle(<13.000000,13.000000,13.000000>, <8.044220,10.355830,5.400051>, <5.032478,12.576440,4.608920>, 1)
// segment connecting nodes 19 and 31 with color 0
make_dislocation(<5.032478,12.576440,4.608920>, <8.044220,10.355830,5.400051>, 0, 0)
// triangle color 1 nodes: [10, 19, 9]
make_triangle(<13.000000,13.000000,13.000000>, <5.032478,12.576440,4.608920>, <3.000000,13.000000,3.000000>, 1)
// segment connecting nodes 9 and 19 with color 0
make_dislocation(<3.000000,13.000000,3.000000>, <5.032478,12.576440,4.608920>, 0, 0)
// segment connecting nodes 10 and 20 with color 0
make_dislocation(<13.000000,13.000000,13.000000>, <11.442970,6.112888,4.555856>, 0, 0)
// WriteFan([0, 1, (7, 15, 17, 9)])
// segment connecting nodes 15 and 7 with color 0
make_dislocation(<4.515128,8.484872,0.000000>, <0.000000,13.000000,0.000000>, 0, 0)
// triangle color 1 nodes: [7, 15, 17]
make_triangle(<0.000000,13.000000,0.000000>, <4.515128,8.484872,0.000000>, <3.369101,10.861120,1.230219>, 1)
// segment connecting nodes 17 and 15 with color 0
make_dislocation(<3.369101,10.861120,1.230219>, <4.515128,8.484872,0.000000>, 0, 0)
// triangle color 1 nodes: [7, 17, 9]
make_triangle(<0.000000,13.000000,0.000000>, <3.369101,10.861120,1.230219>, <3.000000,13.000000,3.000000>, 1)
// segment connecting nodes 9 and 17 with color 0
make_dislocation(<3.000000,13.000000,3.000000>, <3.369101,10.861120,1.230219>, 0, 0)
// segment connecting nodes 7 and 17 with color 0
make_dislocation(<0.000000,13.000000,0.000000>, <3.369101,10.861120,1.230219>, 0, 0)
// WriteFan([0, 1, (18, 6, 5)])
// segment connecting nodes 6 and 18 with color 0
make_dislocation(<13.000000,0.000000,0.000000>, <13.000000,1.618558,1.618558>, 0, 0)
// triangle color 1 nodes: [18, 6, 5]
make_triangle(<13.000000,1.618558,1.618558>, <13.000000,0.000000,0.000000>, <11.312600,1.687398,0.000000>, 1)
// segment connecting nodes 5 and 6 with color 0
make_dislocation(<11.312600,1.687398,0.000000>, <13.000000,0.000000,0.000000>, 0, 0)
// segment connecting nodes 18 and 5 with color 0
make_dislocation(<13.000000,1.618558,1.618558>, <11.312600,1.687398,0.000000>, 0, 0)
// WriteFan([0, 1, (10, 6, 8)])
// segment connecting nodes 6 and 10 with color 0
make_dislocation(<13.000000,0.000000,0.000000>, <13.000000,13.000000,13.000000>, 0, 0)
// triangle color 1 nodes: [10, 6, 8]
make_triangle(<13.000000,13.000000,13.000000>, <13.000000,0.000000,0.000000>, <0.000000,0.000000,13.000000>, 1)
// segment connecting nodes 8 and 6 with color 0
make_dislocation(<0.000000,0.000000,13.000000>, <13.000000,0.000000,0.000000>, 0, 0)
// segment connecting nodes 10 and 8 with color 0
make_dislocation(<13.000000,13.000000,13.000000>, <0.000000,0.000000,13.000000>, 0, 0)
// WriteFan([0, 1, (8, 6, 7)])
// segment connecting nodes 6 and 8 with color 0
make_dislocation(<13.000000,0.000000,0.000000>, <0.000000,0.000000,13.000000>, 0, 0)
// triangle color 1 nodes: [8, 6, 7]
make_triangle(<0.000000,0.000000,13.000000>, <13.000000,0.000000,0.000000>, <0.000000,13.000000,0.000000>, 1)
// segment connecting nodes 7 and 6 with color 0
make_dislocation(<0.000000,13.000000,0.000000>, <13.000000,0.000000,0.000000>, 0, 0)
// segment connecting nodes 8 and 7 with color 0
make_dislocation(<0.000000,0.000000,13.000000>, <0.000000,13.000000,0.000000>, 0, 0)
// WriteFan([3, 1, (10, 8, 7)])
// segment connecting nodes 8 and 10 with color 3
make_dislocation(<0.000000,0.000000,13.000000>, <13.000000,13.000000,13.000000>, 3, 0)
// triangle color 1 nodes: [10, 8, 7]
make_triangle(<13.000000,13.000000,13.000000>, <0.000000,0.000000,13.000000>, <0.000000,13.000000,0.000000>, 1)
// segment connecting nodes 7 and 8 with color 3
make_dislocation(<0.000000,13.000000,0.000000>, <0.000000,0.000000,13.000000>, 3, 0)
// segment connecting nodes 10 and 7 with color 3
make_dislocation(<13.000000,13.000000,13.000000>, <0.000000,13.000000,0.000000>, 3, 0)
// WriteStrip([1, 1, (11, 21, 105, 106, 65, 66, 41, 42, 84, 118, 85, 32, 33, 96, 53, 53, 76, 112, 113, 74, 75, 133, 55, 56, 97, 125, 124, 98, 99, 139, 140, 141, 22, 3), (11, 23, 67, 130, 43, 44, 86, 119, 87, 34, 35, 100, 57, 58, 79, 114, 115, 77, 78, 134, 59, 60, 101, 127, 126, 102, 103, 142, 143, 144, 24, 4)])
// segment connecting nodes 11 and 21 with color 1
make_dislocation(<13.000000,3.000000,3.000000>, <12.610490,2.032845,4.356669>, 1, 1)
// segment connecting nodes 21 and 105 with color 1
make_dislocation(<12.610490,2.032845,4.356669>, <13.140630,0.547818,5.311552>, 1, 0)
// segment connecting nodes 105 and 106 with color 1
make_dislocation(<13.140630,0.547818,5.311552>, <15.225080,-2.327782,6.102701>, 1, 0)
// segment connecting nodes 106 and 65 with color 1
make_dislocation(<15.225080,-2.327782,6.102701>, <17.773630,-5.473034,6.699403>, 1, 0)
// segment connecting nodes 65 and 66 with color 1
make_dislocation(<17.773630,-5.473034,6.699403>, <21.311560,-9.537230,7.225666>, 1, 0)
// segment connecting nodes 66 and 41 with color 1
make_dislocation(<21.311560,-9.537230,7.225666>, <23.391530,-11.824960,7.433435>, 1, 0)
// segment connecting nodes 41 and 42 with color 1
make_dislocation(<23.391530,-11.824960,7.433435>, <25.442560,-14.025310,7.582751>, 1, 0)
// segment connecting nodes 42 and 84 with color 1
make_dislocation(<25.442560,-14.025310,7.582751>, <27.555700,-16.250790,7.695092>, 1, 0)
// segment connecting nodes 84 and 118 with color 1
make_dislocation(<27.555700,-16.250790,7.695092>, <29.673090,-18.451240,7.778149>, 1, 0)
// segment connecting nodes 118 and 85 with color 1
make_dislocation(<29.673090,-18.451240,7.778149>, <31.845790,-20.686740,7.840953>, 1, 0)
// segment connecting nodes 85 and 32 with color 1
make_dislocation(<31.845790,-20.686740,7.840953>, <34.018380,-22.905600,7.887223>, 1, 0)
// segment connecting nodes 32 and 33 with color 1
make_dislocation(<34.018380,-22.905600,7.887223>, <36.160590,-25.081660,7.921071>, 1, 0)
// segment connecting nodes 33 and 96 with color 1
make_dislocation(<36.160590,-25.081660,7.921071>, <38.334360,-27.279620,7.945262>, 1, 0)
// segment connecting nodes 96 and 53 with color 1
make_dislocation(<38.334360,-27.279620,7.945262>, <40.518550,-29.479290,7.960733>, 1, 0)
// segment connecting nodes 53 and 76 with color 1
make_dislocation(<40.518550,-29.479290,7.960733>, <44.875830,-33.851360,7.975532>, 1, 0)
// segment connecting nodes 76 and 112 with color 1
make_dislocation(<44.875830,-33.851360,7.975532>, <47.061290,-36.037700,7.976415>, 1, 0)
// segment connecting nodes 112 and 113 with color 1
make_dislocation(<47.061290,-36.037700,7.976415>, <49.239250,-38.212390,7.973136>, 1, 0)
// segment connecting nodes 113 and 74 with color 1
make_dislocation(<49.239250,-38.212390,7.973136>, <51.426350,-40.391930,7.965571>, 1, 0)
// segment connecting nodes 74 and 75 with color 1
make_dislocation(<51.426350,-40.391930,7.965571>, <53.624260,-42.577560,7.953305>, 1, 0)
// segment connecting nodes 75 and 133 with color 1
make_dislocation(<53.624260,-42.577560,7.953305>, <55.816300,-44.751880,7.935589>, 1, 0)
// segment connecting nodes 133 and 55 with color 1
make_dislocation(<55.816300,-44.751880,7.935589>, <58.024350,-46.935290,7.910936>, 1, 0)
// segment connecting nodes 55 and 56 with color 1
make_dislocation(<58.024350,-46.935290,7.910936>, <60.208950,-49.086730,7.877782>, 1, 0)
// segment connecting nodes 56 and 97 with color 1
make_dislocation(<60.208950,-49.086730,7.877782>, <62.432260,-51.264460,7.832204>, 1, 0)
// segment connecting nodes 97 and 125 with color 1
make_dislocation(<62.432260,-51.264460,7.832204>, <64.655890,-53.425950,7.770056>, 1, 0)
// segment connecting nodes 125 and 124 with color 1
make_dislocation(<64.655890,-53.425950,7.770056>, <66.879250,-55.564100,7.684857>, 1, 0)
// segment connecting nodes 124 and 98 with color 1
make_dislocation(<66.879250,-55.564100,7.684857>, <69.111320,-57.677950,7.566633>, 1, 0)
// segment connecting nodes 98 and 99 with color 1
make_dislocation(<69.111320,-57.677950,7.566633>, <71.358460,-59.759190,7.400727>, 1, 0)
// segment connecting nodes 99 and 139 with color 1
make_dislocation(<71.358460,-59.759190,7.400727>, <73.652940,-61.815870,7.162929>, 1, 0)
// segment connecting nodes 139 and 140 with color 1
make_dislocation(<73.652940,-61.815870,7.162929>, <76.132020,-63.929600,6.797574>, 1, 0)
// segment connecting nodes 140 and 141 with color 1
make_dislocation(<76.132020,-63.929600,6.797574>, <79.023650,-66.188700,6.165046>, 1, 0)
// segment connecting nodes 141 and 22 with color 1
make_dislocation(<79.023650,-66.188700,6.165046>, <81.877430,-68.062220,5.184792>, 1, 0)
// segment connecting nodes 22 and 3 with color 1
make_dislocation(<81.877430,-68.062220,5.184792>, <84.000000,-69.000000,4.000000>, 1, 0)
// segment connecting nodes 11 and 23 with color 1
make_dislocation(<13.000000,3.000000,3.000000>, <17.139200,-1.158784,3.019586>, 1, 1)
// segment connecting nodes 23 and 67 with color 1
make_dislocation(<17.139200,-1.158784,3.019586>, <20.803570,-4.949935,3.146367>, 1, 0)
// segment connecting nodes 67 and 130 with color 1
make_dislocation(<20.803570,-4.949935,3.146367>, <23.332130,-7.645620,3.313487>, 1, 0)
// segment connecting nodes 130 and 43 with color 1
make_dislocation(<23.332130,-7.645620,3.313487>, <25.456000,-9.874533,3.418537>, 1, 0)
// segment connecting nodes 43 and 44 with color 1
make_dislocation(<25.456000,-9.874533,3.418537>, <27.590220,-12.091770,3.501551>, 1, 0)
// segment connecting nodes 44 and 86 with color 1
make_dislocation(<27.590220,-12.091770,3.501551>, <29.721740,-14.290110,3.568364>, 1, 0)
// segment connecting nodes 86 and 119 with color 1
make_dislocation(<29.721740,-14.290110,3.568364>, <31.873290,-16.493520,3.620228>, 1, 0)
// segment connecting nodes 119 and 87 with color 1
make_dislocation(<31.873290,-16.493520,3.620228>, <34.028210,-18.687720,3.659507>, 1, 0)
// segment connecting nodes 87 and 34 with color 1
make_dislocation(<34.028210,-18.687720,3.659507>, <36.197170,-20.886020,3.688853>, 1, 0)
// segment connecting nodes 34 and 35 with color 1
make_dislocation(<36.197170,-20.886020,3.688853>, <38.357980,-23.068140,3.710163>, 1, 0)
// segment connecting nodes 35 and 100 with color 1
make_dislocation(<38.357980,-23.068140,3.710163>, <40.524420,-25.249640,3.725217>, 1, 0)
// segment connecting nodes 100 and 57 with color 1
make_dislocation(<40.524420,-25.249640,3.725217>, <42.702290,-27.437520,3.735234>, 1, 0)
// segment connecting nodes 57 and 58 with color 1
make_dislocation(<42.702290,-27.437520,3.735234>, <44.881150,-29.622060,3.740913>, 1, 0)
// segment connecting nodes 58 and 79 with color 1
make_dislocation(<44.881150,-29.622060,3.740913>, <47.060430,-31.803180,3.742759>, 1, 0)
// segment connecting nodes 79 and 114 with color 1
make_dislocation(<47.060430,-31.803180,3.742759>, <49.232300,-33.973410,3.741115>, 1, 0)
// segment connecting nodes 114 and 115 with color 1
make_dislocation(<49.232300,-33.973410,3.741115>, <51.415650,-36.151620,3.735970>, 1, 0)
// segment connecting nodes 115 and 77 with color 1
make_dislocation(<51.415650,-36.151620,3.735970>, <53.604000,-38.331070,3.727072>, 1, 0)
// segment connecting nodes 77 and 78 with color 1
make_dislocation(<53.604000,-38.331070,3.727072>, <55.788910,-40.502880,3.713977>, 1, 0)
// segment connecting nodes 78 and 134 with color 1
make_dislocation(<55.788910,-40.502880,3.713977>, <57.977620,-42.673420,3.695807>, 1, 0)
// segment connecting nodes 134 and 59 with color 1
make_dislocation(<57.977620,-42.673420,3.695807>, <60.167430,-44.838810,3.671385>, 1, 0)
// segment connecting nodes 59 and 60 with color 1
make_dislocation(<60.167430,-44.838810,3.671385>, <62.362170,-47.001030,3.638852>, 1, 0)
// segment connecting nodes 60 and 101 with color 1
make_dislocation(<62.362170,-47.001030,3.638852>, <64.564350,-49.160170,3.595817>, 1, 0)
// segment connecting nodes 101 and 127 with color 1
make_dislocation(<64.564350,-49.160170,3.595817>, <66.774620,-51.313850,3.539234>, 1, 0)
// segment connecting nodes 127 and 126 with color 1
make_dislocation(<66.774620,-51.313850,3.539234>, <68.997380,-53.462450,3.465071>, 1, 0)
// segment connecting nodes 126 and 102 with color 1
make_dislocation(<68.997380,-53.462450,3.465071>, <71.213330,-55.582650,3.369319>, 1, 0)
// segment connecting nodes 102 and 103 with color 1
make_dislocation(<71.213330,-55.582650,3.369319>, <73.437200,-57.683590,3.246381>, 1, 0)
// segment connecting nodes 103 and 142 with color 1
make_dislocation(<73.437200,-57.683590,3.246381>, <75.668910,-59.759780,3.090876>, 1, 0)
// segment connecting nodes 142 and 143 with color 1
make_dislocation(<75.668910,-59.759780,3.090876>, <77.922120,-61.819280,2.897163>, 1, 0)
// segment connecting nodes 143 and 144 with color 1
make_dislocation(<77.922120,-61.819280,2.897163>, <80.299830,-63.952250,2.652421>, 1, 0)
// segment connecting nodes 144 and 24 with color 1
make_dislocation(<80.299830,-63.952250,2.652421>, <83.889690,-67.124080,2.234402>, 1, 0)
// segment connecting nodes 24 and 4 with color 1
make_dislocation(<83.889690,-67.124080,2.234402>, <85.000000,-68.000000,2.000000>, 1, 0)
// triangle color 1 nodes: [11, 11, 21]
make_triangle(<13.000000,3.000000,3.000000>, <13.000000,3.000000,3.000000>, <12.610490,2.032845,4.356669>, 1)
// triangle color 1 nodes: [21, 11, 23]
make_triangle(<12.610490,2.032845,4.356669>, <13.000000,3.000000,3.000000>, <17.139200,-1.158784,3.019586>, 1)
// triangle color 1 nodes: [21, 23, 105]
make_triangle(<12.610490,2.032845,4.356669>, <17.139200,-1.158784,3.019586>, <13.140630,0.547818,5.311552>, 1)
// triangle color 1 nodes: [105, 23, 67]
make_triangle(<13.140630,0.547818,5.311552>, <17.139200,-1.158784,3.019586>, <20.803570,-4.949935,3.146367>, 1)
// triangle color 1 nodes: [105, 67, 106]
make_triangle(<13.140630,0.547818,5.311552>, <20.803570,-4.949935,3.146367>, <15.225080,-2.327782,6.102701>, 1)
// triangle color 1 nodes: [106, 67, 130]
make_triangle(<15.225080,-2.327782,6.102701>, <20.803570,-4.949935,3.146367>, <23.332130,-7.645620,3.313487>, 1)
// triangle color 1 nodes: [106, 130, 65]
make_triangle(<15.225080,-2.327782,6.102701>, <23.332130,-7.645620,3.313487>, <17.773630,-5.473034,6.699403>, 1)
// triangle color 1 nodes: [65, 130, 43]
make_triangle(<17.773630,-5.473034,6.699403>, <23.332130,-7.645620,3.313487>, <25.456000,-9.874533,3.418537>, 1)
// triangle color 1 nodes: [65, 43, 66]
make_triangle(<17.773630,-5.473034,6.699403>, <25.456000,-9.874533,3.418537>, <21.311560,-9.537230,7.225666>, 1)
// triangle color 1 nodes: [66, 43, 44]
make_triangle(<21.311560,-9.537230,7.225666>, <25.456000,-9.874533,3.418537>, <27.590220,-12.091770,3.501551>, 1)
// triangle color 1 nodes: [66, 44, 41]
make_triangle(<21.311560,-9.537230,7.225666>, <27.590220,-12.091770,3.501551>, <23.391530,-11.824960,7.433435>, 1)
// triangle color 1 nodes: [41, 44, 86]
make_triangle(<23.391530,-11.824960,7.433435>, <27.590220,-12.091770,3.501551>, <29.721740,-14.290110,3.568364>, 1)
// triangle color 1 nodes: [41, 86, 42]
make_triangle(<23.391530,-11.824960,7.433435>, <29.721740,-14.290110,3.568364>, <25.442560,-14.025310,7.582751>, 1)
// triangle color 1 nodes: [42, 86, 119]
make_triangle(<25.442560,-14.025310,7.582751>, <29.721740,-14.290110,3.568364>, <31.873290,-16.493520,3.620228>, 1)
// triangle color 1 nodes: [42, 119, 84]
make_triangle(<25.442560,-14.025310,7.582751>, <31.873290,-16.493520,3.620228>, <27.555700,-16.250790,7.695092>, 1)
// triangle color 1 nodes: [84, 119, 87]
make_triangle(<27.555700,-16.250790,7.695092>, <31.873290,-16.493520,3.620228>, <34.028210,-18.687720,3.659507>, 1)
// triangle color 1 nodes: [84, 87, 118]
make_triangle(<27.555700,-16.250790,7.695092>, <34.028210,-18.687720,3.659507>, <29.673090,-18.451240,7.778149>, 1)
// triangle color 1 nodes: [118, 87, 34]
make_triangle(<29.673090,-18.451240,7.778149>, <34.028210,-18.687720,3.659507>, <36.197170,-20.886020,3.688853>, 1)
// triangle color 1 nodes: [118, 34, 85]
make_triangle(<29.673090,-18.451240,7.778149>, <36.197170,-20.886020,3.688853>, <31.845790,-20.686740,7.840953>, 1)
// triangle color 1 nodes: [85, 34, 35]
make_triangle(<31.845790,-20.686740,7.840953>, <36.197170,-20.886020,3.688853>, <38.357980,-23.068140,3.710163>, 1)
// triangle color 1 nodes: [85, 35, 32]
make_triangle(<31.845790,-20.686740,7.840953>, <38.357980,-23.068140,3.710163>, <34.018380,-22.905600,7.887223>, 1)
// triangle color 1 nodes: [32, 35, 100]
make_triangle(<34.018380,-22.905600,7.887223>, <38.357980,-23.068140,3.710163>, <40.524420,-25.249640,3.725217>, 1)
// triangle color 1 nodes: [32, 100, 33]
make_triangle(<34.018380,-22.905600,7.887223>, <40.524420,-25.249640,3.725217>, <36.160590,-25.081660,7.921071>, 1)
// triangle color 1 nodes: [33, 100, 57]
make_triangle(<36.160590,-25.081660,7.921071>, <40.524420,-25.249640,3.725217>, <42.702290,-27.437520,3.735234>, 1)
// triangle color 1 nodes: [33, 57, 96]
make_triangle(<36.160590,-25.081660,7.921071>, <42.702290,-27.437520,3.735234>, <38.334360,-27.279620,7.945262>, 1)
// triangle color 1 nodes: [96, 57, 58]
make_triangle(<38.334360,-27.279620,7.945262>, <42.702290,-27.437520,3.735234>, <44.881150,-29.622060,3.740913>, 1)
// triangle color 1 nodes: [96, 58, 53]
make_triangle(<38.334360,-27.279620,7.945262>, <44.881150,-29.622060,3.740913>, <40.518550,-29.479290,7.960733>, 1)
// triangle color 1 nodes: [53, 58, 79]
make_triangle(<40.518550,-29.479290,7.960733>, <44.881150,-29.622060,3.740913>, <47.060430,-31.803180,3.742759>, 1)
// triangle color 1 nodes: [53, 79, 53]
make_triangle(<40.518550,-29.479290,7.960733>, <47.060430,-31.803180,3.742759>, <40.518550,-29.479290,7.960733>, 1)
// triangle color 1 nodes: [53, 79, 114]
make_triangle(<40.518550,-29.479290,7.960733>, <47.060430,-31.803180,3.742759>, <49.232300,-33.973410,3.741115>, 1)
// triangle color 1 nodes: [53, 114, 76]
make_triangle(<40.518550,-29.479290,7.960733>, <49.232300,-33.973410,3.741115>, <44.875830,-33.851360,7.975532>, 1)
// triangle color 1 nodes: [76, 114, 115]
make_triangle(<44.875830,-33.851360,7.975532>, <49.232300,-33.973410,3.741115>, <51.415650,-36.151620,3.735970>, 1)
// triangle color 1 nodes: [76, 115, 112]
make_triangle(<44.875830,-33.851360,7.975532>, <51.415650,-36.151620,3.735970>, <47.061290,-36.037700,7.976415>, 1)
// triangle color 1 nodes: [112, 115, 77]
make_triangle(<47.061290,-36.037700,7.976415>, <51.415650,-36.151620,3.735970>, <53.604000,-38.331070,3.727072>, 1)
// triangle color 1 nodes: [112, 77, 113]
make_triangle(<47.061290,-36.037700,7.976415>, <53.604000,-38.331070,3.727072>, <49.239250,-38.212390,7.973136>, 1)
// triangle color 1 nodes: [113, 77, 78]
make_triangle(<49.239250,-38.212390,7.973136>, <53.604000,-38.331070,3.727072>, <55.788910,-40.502880,3.713977>, 1)
// triangle color 1 nodes: [113, 78, 74]
make_triangle(<49.239250,-38.212390,7.973136>, <55.788910,-40.502880,3.713977>, <51.426350,-40.391930,7.965571>, 1)
// triangle color 1 nodes: [74, 78, 134]
make_triangle(<51.426350,-40.391930,7.965571>, <55.788910,-40.502880,3.713977>, <57.977620,-42.673420,3.695807>, 1)
// triangle color 1 nodes: [74, 134, 75]
make_triangle(<51.426350,-40.391930,7.965571>, <57.977620,-42.673420,3.695807>, <53.624260,-42.577560,7.953305>, 1)
// triangle color 1 nodes: [75, 134, 59]
make_triangle(<53.624260,-42.577560,7.953305>, <57.977620,-42.673420,3.695807>, <60.167430,-44.838810,3.671385>, 1)
// triangle color 1 nodes: [75, 59, 133]
make_triangle(<53.624260,-42.577560,7.953305>, <60.167430,-44.838810,3.671385>, <55.816300,-44.751880,7.935589>, 1)
// triangle color 1 nodes: [133, 59, 60]
make_triangle(<55.816300,-44.751880,7.935589>, <60.167430,-44.838810,3.671385>, <62.362170,-47.001030,3.638852>, 1)
// triangle color 1 nodes: [133, 60, 55]
make_triangle(<55.816300,-44.751880,7.935589>, <62.362170,-47.001030,3.638852>, <58.024350,-46.935290,7.910936>, 1)
// triangle color 1 nodes: [55, 60, 101]
make_triangle(<58.024350,-46.935290,7.910936>, <62.362170,-47.001030,3.638852>, <64.564350,-49.160170,3.595817>, 1)
// triangle color 1 nodes: [55, 101, 56]
make_triangle(<58.024350,-46.935290,7.910936>, <64.564350,-49.160170,3.595817>, <60.208950,-49.086730,7.877782>, 1)
// triangle color 1 nodes: [56, 101, 127]
make_triangle(<60.208950,-49.086730,7.877782>, <64.564350,-49.160170,3.595817>, <66.774620,-51.313850,3.539234>, 1)
// triangle color 1 nodes: [56, 127, 97]
make_triangle(<60.208950,-49.086730,7.877782>, <66.774620,-51.313850,3.539234>, <62.432260,-51.264460,7.832204>, 1)
// triangle color 1 nodes: [97, 127, 126]
make_triangle(<62.432260,-51.264460,7.832204>, <66.774620,-51.313850,3.539234>, <68.997380,-53.462450,3.465071>, 1)
// triangle color 1 nodes: [97, 126, 125]
make_triangle(<62.432260,-51.264460,7.832204>, <68.997380,-53.462450,3.465071>, <64.655890,-53.425950,7.770056>, 1)
// triangle color 1 nodes: [125, 126, 102]
make_triangle(<64.655890,-53.425950,7.770056>, <68.997380,-53.462450,3.465071>, <71.213330,-55.582650,3.369319>, 1)
// triangle color 1 nodes: [125, 102, 124]
make_triangle(<64.655890,-53.425950,7.770056>, <71.213330,-55.582650,3.369319>, <66.879250,-55.564100,7.684857>, 1)
// triangle color 1 nodes: [124, 102, 103]
make_triangle(<66.879250,-55.564100,7.684857>, <71.213330,-55.582650,3.369319>, <73.437200,-57.683590,3.246381>, 1)
// triangle color 1 nodes: [124, 103, 98]
make_triangle(<66.879250,-55.564100,7.684857>, <73.437200,-57.683590,3.246381>, <69.111320,-57.677950,7.566633>, 1)
// triangle color 1 nodes: [98, 103, 142]
make_triangle(<69.111320,-57.677950,7.566633>, <73.437200,-57.683590,3.246381>, <75.668910,-59.759780,3.090876>, 1)
// triangle color 1 nodes: [98, 142, 99]
make_triangle(<69.111320,-57.677950,7.566633>, <75.668910,-59.759780,3.090876>, <71.358460,-59.759190,7.400727>, 1)
// triangle color 1 nodes: [99, 142, 143]
make_triangle(<71.358460,-59.759190,7.400727>, <75.668910,-59.759780,3.090876>, <77.922120,-61.819280,2.897163>, 1)
// triangle color 1 nodes: [99, 143, 139]
make_triangle(<71.358460,-59.759190,7.400727>, <77.922120,-61.819280,2.897163>, <73.652940,-61.815870,7.162929>, 1)
// triangle color 1 nodes: [139, 143, 144]
make_triangle(<73.652940,-61.815870,7.162929>, <77.922120,-61.819280,2.897163>, <80.299830,-63.952250,2.652421>, 1)
// triangle color 1 nodes: [139, 144, 140]
make_triangle(<73.652940,-61.815870,7.162929>, <80.299830,-63.952250,2.652421>, <76.132020,-63.929600,6.797574>, 1)
// triangle color 1 nodes: [140, 144, 24]
make_triangle(<76.132020,-63.929600,6.797574>, <80.299830,-63.952250,2.652421>, <83.889690,-67.124080,2.234402>, 1)
// triangle color 1 nodes: [140, 24, 141]
make_triangle(<76.132020,-63.929600,6.797574>, <83.889690,-67.124080,2.234402>, <79.023650,-66.188700,6.165046>, 1)
// triangle color 1 nodes: [141, 24, 4]
make_triangle(<79.023650,-66.188700,6.165046>, <83.889690,-67.124080,2.234402>, <85.000000,-68.000000,2.000000>, 1)
// triangle color 1 nodes: [141, 4, 22]
make_triangle(<79.023650,-66.188700,6.165046>, <85.000000,-68.000000,2.000000>, <81.877430,-68.062220,5.184792>, 1)
// WriteStrip([1, 1, (9, 13, 25, 62, 61, 128, 38, 37, 80, 116, 81, 27, 26, 88, 46, 45, 70, 108, 109, 69, 68, 131, 48, 47, 89, 121, 120, 91, 90, 135, 136, 28, 12, 1), (9, 104, 107, 64, 63, 129, 40, 39, 82, 117, 83, 30, 29, 92, 50, 49, 73, 110, 111, 72, 71, 132, 52, 51, 93, 123, 122, 95, 94, 137, 138, 36, 14, 2)])
// segment connecting nodes 9 and 104 with color 1
make_dislocation(<3.000000,13.000000,3.000000>, <1.541273,14.877980,2.580749>, 1, 1)
// segment connecting nodes 104 and 107 with color 1
make_dislocation(<1.541273,14.877980,2.580749>, <-0.854749,17.073720,2.781031>, 1, 0)
// segment connecting nodes 107 and 64 with color 1
make_dislocation(<-0.854749,17.073720,2.781031>, <-3.182248,19.156500,3.025747>, 1, 0)
// segment connecting nodes 64 and 63 with color 1
make_dislocation(<-3.182248,19.156500,3.025747>, <-5.455904,21.232760,3.223150>, 1, 0)
// segment connecting nodes 63 and 129 with color 1
make_dislocation(<-5.455904,21.232760,3.223150>, <-7.655975,23.285670,3.370309>, 1, 0)
// segment connecting nodes 129 and 40 with color 1
make_dislocation(<-7.655975,23.285670,3.370309>, <-9.861250,25.377360,3.483889>, 1, 0)
// segment connecting nodes 40 and 39 with color 1
make_dislocation(<-9.861250,25.377360,3.483889>, <-12.066630,27.497560,3.569063>, 1, 0)
// segment connecting nodes 39 and 82 with color 1
make_dislocation(<-12.066630,27.497560,3.569063>, <-14.262180,29.630950,3.631225>, 1, 0)
// segment connecting nodes 82 and 117 with color 1
make_dislocation(<-14.262180,29.630950,3.631225>, <-16.455270,31.779150,3.676112>, 1, 0)
// segment connecting nodes 117 and 83 with color 1
make_dislocation(<-16.455270,31.779150,3.676112>, <-18.647950,33.940010,3.707935>, 1, 0)
// segment connecting nodes 83 and 30 with color 1
make_dislocation(<-18.647950,33.940010,3.707935>, <-20.841290,36.111310,3.729986>, 1, 0)
// segment connecting nodes 30 and 29 with color 1
make_dislocation(<-20.841290,36.111310,3.729986>, <-23.027210,38.282570,3.744637>, 1, 0)
// segment connecting nodes 29 and 92 with color 1
make_dislocation(<-23.027210,38.282570,3.744637>, <-25.211320,40.457610,3.753713>, 1, 0)
// segment connecting nodes 92 and 50 with color 1
make_dislocation(<-25.211320,40.457610,3.753713>, <-27.396260,42.637690,3.758567>, 1, 0)
// segment connecting nodes 50 and 49 with color 1
make_dislocation(<-27.396260,42.637690,3.758567>, <-29.578570,44.818740,3.759833>, 1, 0)
// segment connecting nodes 49 and 73 with color 1
make_dislocation(<-29.578570,44.818740,3.759833>, <-31.755700,46.997870,3.757830>, 1, 0)
// segment connecting nodes 73 and 110 with color 1
make_dislocation(<-31.755700,46.997870,3.757830>, <-33.938190,49.185390,3.752800>, 1, 0)
// segment connecting nodes 110 and 111 with color 1
make_dislocation(<-33.938190,49.185390,3.752800>, <-36.110600,51.365800,3.744801>, 1, 0)
// segment connecting nodes 111 and 72 with color 1
make_dislocation(<-36.110600,51.365800,3.744801>, <-38.288400,53.554890,3.733505>, 1, 0)
// segment connecting nodes 72 and 71 with color 1
make_dislocation(<-38.288400,53.554890,3.733505>, <-40.469560,55.751250,3.718310>, 1, 0)
// segment connecting nodes 71 and 132 with color 1
make_dislocation(<-40.469560,55.751250,3.718310>, <-42.641590,57.943200,3.698393>, 1, 0)
// segment connecting nodes 132 and 52 with color 1
make_dislocation(<-42.641590,57.943200,3.698393>, <-44.810800,60.138420,3.672383>, 1, 0)
// segment connecting nodes 52 and 51 with color 1
make_dislocation(<-44.810800,60.138420,3.672383>, <-46.978820,62.340300,3.638520>, 1, 0)
// segment connecting nodes 51 and 93 with color 1
make_dislocation(<-46.978820,62.340300,3.638520>, <-49.143420,64.548990,3.594436>, 1, 0)
// segment connecting nodes 93 and 123 with color 1
make_dislocation(<-49.143420,64.548990,3.594436>, <-51.299220,66.762260,3.536965>, 1, 0)
// segment connecting nodes 123 and 122 with color 1
make_dislocation(<-51.299220,66.762260,3.536965>, <-53.445670,68.983530,3.462150>, 1, 0)
// segment connecting nodes 122 and 95 with color 1
make_dislocation(<-53.445670,68.983530,3.462150>, <-55.571270,71.206050,3.365215>, 1, 0)
// segment connecting nodes 95 and 94 with color 1
make_dislocation(<-55.571270,71.206050,3.365215>, <-57.687170,73.447500,3.239667>, 1, 0)
// segment connecting nodes 94 and 137 with color 1
make_dislocation(<-57.687170,73.447500,3.239667>, <-59.762200,75.680650,3.081551>, 1, 0)
// segment connecting nodes 137 and 138 with color 1
make_dislocation(<-59.762200,75.680650,3.081551>, <-61.834570,77.950910,2.883662>, 1, 0)
// segment connecting nodes 138 and 36 with color 1
make_dislocation(<-61.834570,77.950910,2.883662>, <-63.958100,80.318240,2.639867>, 1, 0)
// segment connecting nodes 36 and 14 with color 1
make_dislocation(<-63.958100,80.318240,2.639867>, <-66.807300,83.532820,2.274477>, 1, 0)
// segment connecting nodes 14 and 2 with color 1
make_dislocation(<-66.807300,83.532820,2.274477>, <-68.000000,85.000000,2.000000>, 1, 0)
// segment connecting nodes 9 and 13 with color 1
make_dislocation(<3.000000,13.000000,3.000000>, <1.803960,13.094460,4.101578>, 1, 1)
// segment connecting nodes 13 and 25 with color 1
make_dislocation(<1.803960,13.094460,4.101578>, <-1.399186,14.509810,5.889376>, 1, 0)
// segment connecting nodes 25 and 62 with color 1
make_dislocation(<-1.399186,14.509810,5.889376>, <-3.960868,16.235910,6.724956>, 1, 0)
// segment connecting nodes 62 and 61 with color 1
make_dislocation(<-3.960868,16.235910,6.724956>, <-7.100946,18.819230,7.281711>, 1, 0)
// segment connecting nodes 61 and 128 with color 1
make_dislocation(<-7.100946,18.819230,7.281711>, <-9.845094,21.284760,7.560339>, 1, 0)
// segment connecting nodes 128 and 38 with color 1
make_dislocation(<-9.845094,21.284760,7.560339>, <-12.131360,23.421310,7.710049>, 1, 0)
// segment connecting nodes 38 and 37 with color 1
make_dislocation(<-12.131360,23.421310,7.710049>, <-14.362460,25.553600,7.808863>, 1, 0)
// segment connecting nodes 37 and 80 with color 1
make_dislocation(<-14.362460,25.553600,7.808863>, <-16.551430,27.676530,7.874908>, 1, 0)
// segment connecting nodes 80 and 116 with color 1
make_dislocation(<-16.551430,27.676530,7.874908>, <-18.737250,29.817410,7.919845>, 1, 0)
// segment connecting nodes 116 and 81 with color 1
make_dislocation(<-18.737250,29.817410,7.919845>, <-20.933090,31.982790,7.950298>, 1, 0)
// segment connecting nodes 81 and 27 with color 1
make_dislocation(<-20.933090,31.982790,7.950298>, <-23.111690,34.141430,7.970254>, 1, 0)
// segment connecting nodes 27 and 26 with color 1
make_dislocation(<-23.111690,34.141430,7.970254>, <-25.285560,36.302740,7.982819>, 1, 0)
// segment connecting nodes 26 and 88 with color 1
make_dislocation(<-25.285560,36.302740,7.982819>, <-27.460000,38.470190,7.989805>, 1, 0)
// segment connecting nodes 88 and 46 with color 1
make_dislocation(<-27.460000,38.470190,7.989805>, <-29.641410,40.649110,7.992305>, 1, 0)
// segment connecting nodes 46 and 45 with color 1
make_dislocation(<-29.641410,40.649110,7.992305>, <-31.827150,42.836260,7.990892>, 1, 0)
// segment connecting nodes 45 and 70 with color 1
make_dislocation(<-31.827150,42.836260,7.990892>, <-33.976810,44.990850,7.985963>, 1, 0)
// segment connecting nodes 70 and 108 with color 1
make_dislocation(<-33.976810,44.990850,7.985963>, <-36.141280,47.163830,7.977456>, 1, 0)
// segment connecting nodes 108 and 109 with color 1
make_dislocation(<-36.141280,47.163830,7.977456>, <-38.305760,49.340800,7.964961>, 1, 0)
// segment connecting nodes 109 and 69 with color 1
make_dislocation(<-38.305760,49.340800,7.964961>, <-40.484170,51.536360,7.947813>, 1, 0)
// segment connecting nodes 69 and 68 with color 1
make_dislocation(<-40.484170,51.536360,7.947813>, <-42.665640,53.740680,7.924958>, 1, 0)
// segment connecting nodes 68 and 131 with color 1
make_dislocation(<-42.665640,53.740680,7.924958>, <-44.812250,55.917000,7.895252>, 1, 0)
// segment connecting nodes 131 and 48 with color 1
make_dislocation(<-44.812250,55.917000,7.895252>, <-46.985710,58.129970,7.855743>, 1, 0)
// segment connecting nodes 48 and 47 with color 1
make_dislocation(<-46.985710,58.129970,7.855743>, <-49.130930,60.326690,7.804244>, 1, 0)
// segment connecting nodes 47 and 89 with color 1
make_dislocation(<-49.130930,60.326690,7.804244>, <-51.277530,62.541540,7.735984>, 1, 0)
// segment connecting nodes 89 and 121 with color 1
make_dislocation(<-51.277530,62.541540,7.735984>, <-53.391570,64.745140,7.646433>, 1, 0)
// segment connecting nodes 121 and 120 with color 1
make_dislocation(<-53.391570,64.745140,7.646433>, <-55.490750,66.962970,7.527785>, 1, 0)
// segment connecting nodes 120 and 91 with color 1
make_dislocation(<-55.490750,66.962970,7.527785>, <-57.564480,69.193760,7.370726>, 1, 0)
// segment connecting nodes 91 and 90 with color 1
make_dislocation(<-57.564480,69.193760,7.370726>, <-59.572020,71.405560,7.166463>, 1, 0)
// segment connecting nodes 90 and 135 with color 1
make_dislocation(<-59.572020,71.405560,7.166463>, <-61.494470,73.590750,6.903721>, 1, 0)
// segment connecting nodes 135 and 136 with color 1
make_dislocation(<-61.494470,73.590750,6.903721>, <-63.336770,75.771880,6.564891>, 1, 0)
// segment connecting nodes 136 and 28 with color 1
make_dislocation(<-63.336770,75.771880,6.564891>, <-65.340350,78.285550,6.054793>, 1, 0)
// segment connecting nodes 28 and 12 with color 1
make_dislocation(<-65.340350,78.285550,6.054793>, <-67.569360,81.399800,5.169560>, 1, 0)
// segment connecting nodes 12 and 1 with color 1
make_dislocation(<-67.569360,81.399800,5.169560>, <-69.000000,84.000000,4.000000>, 1, 0)
// triangle color 1 nodes: [9, 9, 104]
make_triangle(<3.000000,13.000000,3.000000>, <3.000000,13.000000,3.000000>, <1.541273,14.877980,2.580749>, 1)
// triangle color 1 nodes: [104, 9, 13]
make_triangle(<1.541273,14.877980,2.580749>, <3.000000,13.000000,3.000000>, <1.803960,13.094460,4.101578>, 1)
// triangle color 1 nodes: [104, 13, 107]
make_triangle(<1.541273,14.877980,2.580749>, <1.803960,13.094460,4.101578>, <-0.854749,17.073720,2.781031>, 1)
// triangle color 1 nodes: [107, 13, 25]
make_triangle(<-0.854749,17.073720,2.781031>, <1.803960,13.094460,4.101578>, <-1.399186,14.509810,5.889376>, 1)
// triangle color 1 nodes: [107, 25, 64]
make_triangle(<-0.854749,17.073720,2.781031>, <-1.399186,14.509810,5.889376>, <-3.182248,19.156500,3.025747>, 1)
// triangle color 1 nodes: [64, 25, 62]
make_triangle(<-3.182248,19.156500,3.025747>, <-1.399186,14.509810,5.889376>, <-3.960868,16.235910,6.724956>, 1)
// triangle color 1 nodes: [64, 62, 63]
make_triangle(<-3.182248,19.156500,3.025747>, <-3.960868,16.235910,6.724956>, <-5.455904,21.232760,3.223150>, 1)
// triangle color 1 nodes: [63, 62, 61]
make_triangle(<-5.455904,21.232760,3.223150>, <-3.960868,16.235910,6.724956>, <-7.100946,18.819230,7.281711>, 1)
// triangle color 1 nodes: [63, 61, 129]
make_triangle(<-5.455904,21.232760,3.223150>, <-7.100946,18.819230,7.281711>, <-7.655975,23.285670,3.370309>, 1)
// triangle color 1 nodes: [129, 61, 128]
make_triangle(<-7.655975,23.285670,3.370309>, <-7.100946,18.819230,7.281711>, <-9.845094,21.284760,7.560339>, 1)
// triangle color 1 nodes: [129, 128, 40]
make_triangle(<-7.655975,23.285670,3.370309>, <-9.845094,21.284760,7.560339>, <-9.861250,25.377360,3.483889>, 1)
// triangle color 1 nodes: [40, 128, 38]
make_triangle(<-9.861250,25.377360,3.483889>, <-9.845094,21.284760,7.560339>, <-12.131360,23.421310,7.710049>, 1)
// triangle color 1 nodes: [40, 38, 39]
make_triangle(<-9.861250,25.377360,3.483889>, <-12.131360,23.421310,7.710049>, <-12.066630,27.497560,3.569063>, 1)
// triangle color 1 nodes: [39, 38, 37]
make_triangle(<-12.066630,27.497560,3.569063>, <-12.131360,23.421310,7.710049>, <-14.362460,25.553600,7.808863>, 1)
// triangle color 1 nodes: [39, 37, 82]
make_triangle(<-12.066630,27.497560,3.569063>, <-14.362460,25.553600,7.808863>, <-14.262180,29.630950,3.631225>, 1)
// triangle color 1 nodes: [82, 37, 80]
make_triangle(<-14.262180,29.630950,3.631225>, <-14.362460,25.553600,7.808863>, <-16.551430,27.676530,7.874908>, 1)
// triangle color 1 nodes: [82, 80, 117]
make_triangle(<-14.262180,29.630950,3.631225>, <-16.551430,27.676530,7.874908>, <-16.455270,31.779150,3.676112>, 1)
// triangle color 1 nodes: [117, 80, 116]
make_triangle(<-16.455270,31.779150,3.676112>, <-16.551430,27.676530,7.874908>, <-18.737250,29.817410,7.919845>, 1)
// triangle color 1 nodes: [117, 116, 83]
make_triangle(<-16.455270,31.779150,3.676112>, <-18.737250,29.817410,7.919845>, <-18.647950,33.940010,3.707935>, 1)
// triangle color 1 nodes: [83, 116, 81]
make_triangle(<-18.647950,33.940010,3.707935>, <-18.737250,29.817410,7.919845>, <-20.933090,31.982790,7.950298>, 1)
// triangle color 1 nodes: [83, 81, 30]
make_triangle(<-18.647950,33.940010,3.707935>, <-20.933090,31.982790,7.950298>, <-20.841290,36.111310,3.729986>, 1)
// triangle color 1 nodes: [30, 81, 27]
make_triangle(<-20.841290,36.111310,3.729986>, <-20.933090,31.982790,7.950298>, <-23.111690,34.141430,7.970254>, 1)
// triangle color 1 nodes: [30, 27, 29]
make_triangle(<-20.841290,36.111310,3.729986>, <-23.111690,34.141430,7.970254>, <-23.027210,38.282570,3.744637>, 1)
// triangle color 1 nodes: [29, 27, 26]
make_triangle(<-23.027210,38.282570,3.744637>, <-23.111690,34.141430,7.970254>, <-25.285560,36.302740,7.982819>, 1)
// triangle color 1 nodes: [29, 26, 92]
make_triangle(<-23.027210,38.282570,3.744637>, <-25.285560,36.302740,7.982819>, <-25.211320,40.457610,3.753713>, 1)
// triangle color 1 nodes: [92, 26, 88]
make_triangle(<-25.211320,40.457610,3.753713>, <-25.285560,36.302740,7.982819>, <-27.460000,38.470190,7.989805>, 1)
// triangle color 1 nodes: [92, 88, 50]
make_triangle(<-25.211320,40.457610,3.753713>, <-27.460000,38.470190,7.989805>, <-27.396260,42.637690,3.758567>, 1)
// triangle color 1 nodes: [50, 88, 46]
make_triangle(<-27.396260,42.637690,3.758567>, <-27.460000,38.470190,7.989805>, <-29.641410,40.649110,7.992305>, 1)
// triangle color 1 nodes: [50, 46, 49]
make_triangle(<-27.396260,42.637690,3.758567>, <-29.641410,40.649110,7.992305>, <-29.578570,44.818740,3.759833>, 1)
// triangle color 1 nodes: [49, 46, 45]
make_triangle(<-29.578570,44.818740,3.759833>, <-29.641410,40.649110,7.992305>, <-31.827150,42.836260,7.990892>, 1)
// triangle color 1 nodes: [49, 45, 73]
make_triangle(<-29.578570,44.818740,3.759833>, <-31.827150,42.836260,7.990892>, <-31.755700,46.997870,3.757830>, 1)
// triangle color 1 nodes: [73, 45, 70]
make_triangle(<-31.755700,46.997870,3.757830>, <-31.827150,42.836260,7.990892>, <-33.976810,44.990850,7.985963>, 1)
// triangle color 1 nodes: [73, 70, 110]
make_triangle(<-31.755700,46.997870,3.757830>, <-33.976810,44.990850,7.985963>, <-33.938190,49.185390,3.752800>, 1)
// triangle color 1 nodes: [110, 70, 108]
make_triangle(<-33.938190,49.185390,3.752800>, <-33.976810,44.990850,7.985963>, <-36.141280,47.163830,7.977456>, 1)
// triangle color 1 nodes: [110, 108, 111]
make_triangle(<-33.938190,49.185390,3.752800>, <-36.141280,47.163830,7.977456>, <-36.110600,51.365800,3.744801>, 1)
// triangle color 1 nodes: [111, 108, 109]
make_triangle(<-36.110600,51.365800,3.744801>, <-36.141280,47.163830,7.977456>, <-38.305760,49.340800,7.964961>, 1)
// triangle color 1 nodes: [111, 109, 72]
make_triangle(<-36.110600,51.365800,3.744801>, <-38.305760,49.340800,7.964961>, <-38.288400,53.554890,3.733505>, 1)
// triangle color 1 nodes: [72, 109, 69]
make_triangle(<-38.288400,53.554890,3.733505>, <-38.305760,49.340800,7.964961>, <-40.484170,51.536360,7.947813>, 1)
// triangle color 1 nodes: [72, 69, 71]
make_triangle(<-38.288400,53.554890,3.733505>, <-40.484170,51.536360,7.947813>, <-40.469560,55.751250,3.718310>, 1)
// triangle color 1 nodes: [71, 69, 68]
make_triangle(<-40.469560,55.751250,3.718310>, <-40.484170,51.536360,7.947813>, <-42.665640,53.740680,7.924958>, 1)
// triangle color 1 nodes: [71, 68, 132]
make_triangle(<-40.469560,55.751250,3.718310>, <-42.665640,53.740680,7.924958>, <-42.641590,57.943200,3.698393>, 1)
// triangle color 1 nodes: [132, 68, 131]
make_triangle(<-42.641590,57.943200,3.698393>, <-42.665640,53.740680,7.924958>, <-44.812250,55.917000,7.895252>, 1)
// triangle color 1 nodes: [132, 131, 52]
make_triangle(<-42.641590,57.943200,3.698393>, <-44.812250,55.917000,7.895252>, <-44.810800,60.138420,3.672383>, 1)
// triangle color 1 nodes: [52, 131, 48]
make_triangle(<-44.810800,60.138420,3.672383>, <-44.812250,55.917000,7.895252>, <-46.985710,58.129970,7.855743>, 1)
// triangle color 1 nodes: [52, 48, 51]
make_triangle(<-44.810800,60.138420,3.672383>, <-46.985710,58.129970,7.855743>, <-46.978820,62.340300,3.638520>, 1)
// triangle color 1 nodes: [51, 48, 47]
make_triangle(<-46.978820,62.340300,3.638520>, <-46.985710,58.129970,7.855743>, <-49.130930,60.326690,7.804244>, 1)
// triangle color 1 nodes: [51, 47, 93]
make_triangle(<-46.978820,62.340300,3.638520>, <-49.130930,60.326690,7.804244>, <-49.143420,64.548990,3.594436>, 1)
// triangle color 1 nodes: [93, 47, 89]
make_triangle(<-49.143420,64.548990,3.594436>, <-49.130930,60.326690,7.804244>, <-51.277530,62.541540,7.735984>, 1)
// triangle color 1 nodes: [93, 89, 123]
make_triangle(<-49.143420,64.548990,3.594436>, <-51.277530,62.541540,7.735984>, <-51.299220,66.762260,3.536965>, 1)
// triangle color 1 nodes: [123, 89, 121]
make_triangle(<-51.299220,66.762260,3.536965>, <-51.277530,62.541540,7.735984>, <-53.391570,64.745140,7.646433>, 1)
// triangle color 1 nodes: [123, 121, 122]
make_triangle(<-51.299220,66.762260,3.536965>, <-53.391570,64.745140,7.646433>, <-53.445670,68.983530,3.462150>, 1)
// triangle color 1 nodes: [122, 121, 120]
make_triangle(<-53.445670,68.983530,3.462150>, <-53.391570,64.745140,7.646433>, <-55.490750,66.962970,7.527785>, 1)
// triangle color 1 nodes: [122, 120, 95]
make_triangle(<-53.445670,68.983530,3.462150>, <-55.490750,66.962970,7.527785>, <-55.571270,71.206050,3.365215>, 1)
// triangle color 1 nodes: [95, 120, 91]
make_triangle(<-55.571270,71.206050,3.365215>, <-55.490750,66.962970,7.527785>, <-57.564480,69.193760,7.370726>, 1)
// triangle color 1 nodes: [95, 91, 94]
make_triangle(<-55.571270,71.206050,3.365215>, <-57.564480,69.193760,7.370726>, <-57.687170,73.447500,3.239667>, 1)
// triangle color 1 nodes: [94, 91, 90]
make_triangle(<-57.687170,73.447500,3.239667>, <-57.564480,69.193760,7.370726>, <-59.572020,71.405560,7.166463>, 1)
// triangle color 1 nodes: [94, 90, 137]
make_triangle(<-57.687170,73.447500,3.239667>, <-59.572020,71.405560,7.166463>, <-59.762200,75.680650,3.081551>, 1)
// triangle color 1 nodes: [137, 90, 135]
make_triangle(<-59.762200,75.680650,3.081551>, <-59.572020,71.405560,7.166463>, <-61.494470,73.590750,6.903721>, 1)
// triangle color 1 nodes: [137, 135, 138]
make_triangle(<-59.762200,75.680650,3.081551>, <-61.494470,73.590750,6.903721>, <-61.834570,77.950910,2.883662>, 1)
// triangle color 1 nodes: [138, 135, 136]
make_triangle(<-61.834570,77.950910,2.883662>, <-61.494470,73.590750,6.903721>, <-63.336770,75.771880,6.564891>, 1)
// triangle color 1 nodes: [138, 136, 36]
make_triangle(<-61.834570,77.950910,2.883662>, <-63.336770,75.771880,6.564891>, <-63.958100,80.318240,2.639867>, 1)
// triangle color 1 nodes: [36, 136, 28]
make_triangle(<-63.958100,80.318240,2.639867>, <-63.336770,75.771880,6.564891>, <-65.340350,78.285550,6.054793>, 1)
// triangle color 1 nodes: [36, 28, 14]
make_triangle(<-63.958100,80.318240,2.639867>, <-65.340350,78.285550,6.054793>, <-66.807300,83.532820,2.274477>, 1)
// triangle color 1 nodes: [14, 28, 12]
make_triangle(<-66.807300,83.532820,2.274477>, <-65.340350,78.285550,6.054793>, <-67.569360,81.399800,5.169560>, 1)
// triangle color 1 nodes: [14, 12, 2]
make_triangle(<-66.807300,83.532820,2.274477>, <-67.569360,81.399800,5.169560>, <-68.000000,85.000000,2.000000>, 1)
// triangle color 1 nodes: [2, 12, 1]
make_triangle(<-68.000000,85.000000,2.000000>, <-67.569360,81.399800,5.169560>, <-69.000000,84.000000,4.000000>, 1)
// WriteDislocation([1, (18, 11, 20, 31, 19, 9, 17, 15, 16, 18)])
// segment connecting nodes 11 and 18 with color 1
make_dislocation(<13.000000,3.000000,3.000000>, <13.000000,1.618558,1.618558>, 1, 1)
// segment connecting nodes 20 and 11 with color 1
make_dislocation(<11.442970,6.112888,4.555856>, <13.000000,3.000000,3.000000>, 1, 0)
// segment connecting nodes 31 and 20 with color 1
make_dislocation(<8.044220,10.355830,5.400051>, <11.442970,6.112888,4.555856>, 1, 0)
// segment connecting nodes 19 and 31 with color 1
make_dislocation(<5.032478,12.576440,4.608920>, <8.044220,10.355830,5.400051>, 1, 0)
// segment connecting nodes 9 and 19 with color 1
make_dislocation(<3.000000,13.000000,3.000000>, <5.032478,12.576440,4.608920>, 1, 0)
// segment connecting nodes 17 and 9 with color 1
make_dislocation(<3.369101,10.861120,1.230219>, <3.000000,13.000000,3.000000>, 1, 0)
// segment connecting nodes 15 and 17 with color 1
make_dislocation(<4.515128,8.484872,0.000000>, <3.369101,10.861120,1.230219>, 1, 0)
// segment connecting nodes 16 and 15 with color 1
make_dislocation(<7.961043,4.995044,0.043913>, <4.515128,8.484872,0.000000>, 1, 0)
// segment connecting nodes 18 and 16 with color 1
make_dislocation(<13.000000,1.618558,1.618558>, <7.961043,4.995044,0.043913>, 1, 0)
// WriteDislocation([3, (16, 5, 6, 8, 7, 15)])
// segment connecting nodes 5 and 16 with color 3
make_dislocation(<11.312600,1.687398,0.000000>, <7.961043,4.995044,0.043913>, 3, 1)
// segment connecting nodes 6 and 5 with color 3
make_dislocation(<13.000000,0.000000,0.000000>, <11.312600,1.687398,0.000000>, 3, 0)
// segment connecting nodes 8 and 6 with color 3
make_dislocation(<0.000000,0.000000,13.000000>, <13.000000,0.000000,0.000000>, 3, 0)
// segment connecting nodes 7 and 8 with color 3
make_dislocation(<0.000000,13.000000,0.000000>, <0.000000,0.000000,13.000000>, 3, 0)
// segment connecting nodes 15 and 7 with color 3
make_dislocation(<4.515128,8.484872,0.000000>, <0.000000,13.000000,0.000000>, 3, 0)
// WriteDislocation([3, (6, 18)])
// segment connecting nodes 18 and 6 with color 3
make_dislocation(<13.000000,1.618558,1.618558>, <13.000000,0.000000,0.000000>, 3, 1)
// WriteDislocation([3, (11, 10)])
// segment connecting nodes 10 and 11 with color 3
make_dislocation(<13.000000,13.000000,13.000000>, <13.000000,3.000000,3.000000>, 3, 1)
