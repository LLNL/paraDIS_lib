#declare lookatpos = <8.000000,8.000000,6.500000>;
#declare camerapos = <8.000000,8.000000,169.000000>;
#declare viewx = x * 154.000000;
#declare viewy = y * 154.000000;
#include "jaime.inc"
// WriteFan([0, 1, (10, 7, 8)])
// segment connecting nodes 7 and 10 with color 0
make_dislocation(<0.000000,13.000000,0.000000>, <13.000000,13.000000,13.000000>, 0, 0)
// triangle color 1 nodes: [10, 7, 8]
make_triangle(<13.000000,13.000000,13.000000>, <0.000000,13.000000,0.000000>, <0.000000,0.000000,13.000000>, 1)
// segment connecting nodes 8 and 7 with color 0
make_dislocation(<0.000000,0.000000,13.000000>, <0.000000,13.000000,0.000000>, 0, 0)
// segment connecting nodes 10 and 8 with color 0
make_dislocation(<13.000000,13.000000,13.000000>, <0.000000,0.000000,13.000000>, 0, 0)
// WriteFan([0, 1, (8, 11, 38, 17, 16, 7)])
// segment connecting nodes 11 and 8 with color 0
make_dislocation(<10.181070,0.000000,2.818928>, <0.000000,0.000000,13.000000>, 0, 0)
// triangle color 1 nodes: [8, 11, 38]
make_triangle(<0.000000,0.000000,13.000000>, <10.181070,0.000000,2.818928>, <8.120429,2.933554,1.946017>, 1)
// segment connecting nodes 38 and 11 with color 0
make_dislocation(<8.120429,2.933554,1.946017>, <10.181070,0.000000,2.818928>, 0, 0)
// triangle color 1 nodes: [8, 38, 17]
make_triangle(<0.000000,0.000000,13.000000>, <8.120429,2.933554,1.946017>, <6.164875,5.324576,1.510549>, 1)
// segment connecting nodes 17 and 38 with color 0
make_dislocation(<6.164875,5.324576,1.510549>, <8.120429,2.933554,1.946017>, 0, 0)
// triangle color 1 nodes: [8, 17, 16]
make_triangle(<0.000000,0.000000,13.000000>, <6.164875,5.324576,1.510549>, <3.130793,9.869207,0.000000>, 1)
// segment connecting nodes 16 and 17 with color 0
make_dislocation(<3.130793,9.869207,0.000000>, <6.164875,5.324576,1.510549>, 0, 0)
// triangle color 1 nodes: [8, 16, 7]
make_triangle(<0.000000,0.000000,13.000000>, <3.130793,9.869207,0.000000>, <0.000000,13.000000,0.000000>, 1)
// segment connecting nodes 7 and 16 with color 0
make_dislocation(<0.000000,13.000000,0.000000>, <3.130793,9.869207,0.000000>, 0, 0)
// segment connecting nodes 8 and 38 with color 0
make_dislocation(<0.000000,0.000000,13.000000>, <8.120429,2.933554,1.946017>, 0, 0)
// WriteFan([0, 1, (10, 12, 20, 35, 19, 9)])
// segment connecting nodes 12 and 10 with color 0
make_dislocation(<13.000000,3.000000,3.000000>, <13.000000,13.000000,13.000000>, 0, 0)
// triangle color 1 nodes: [10, 12, 20]
make_triangle(<13.000000,13.000000,13.000000>, <13.000000,3.000000,3.000000>, <10.299910,6.717279,4.017185>, 1)
// segment connecting nodes 20 and 12 with color 0
make_dislocation(<10.299910,6.717279,4.017185>, <13.000000,3.000000,3.000000>, 0, 0)
// triangle color 1 nodes: [10, 20, 35]
make_triangle(<13.000000,13.000000,13.000000>, <10.299910,6.717279,4.017185>, <6.771436,10.844250,4.615684>, 1)
// segment connecting nodes 35 and 20 with color 0
make_dislocation(<6.771436,10.844250,4.615684>, <10.299910,6.717279,4.017185>, 0, 0)
// triangle color 1 nodes: [10, 35, 19]
make_triangle(<13.000000,13.000000,13.000000>, <6.771436,10.844250,4.615684>, <4.518838,12.498000,4.016833>, 1)
// segment connecting nodes 19 and 35 with color 0
make_dislocation(<4.518838,12.498000,4.016833>, <6.771436,10.844250,4.615684>, 0, 0)
// triangle color 1 nodes: [10, 19, 9]
make_triangle(<13.000000,13.000000,13.000000>, <4.518838,12.498000,4.016833>, <3.000000,13.000000,3.000000>, 1)
// segment connecting nodes 9 and 19 with color 0
make_dislocation(<3.000000,13.000000,3.000000>, <4.518838,12.498000,4.016833>, 0, 0)
// segment connecting nodes 10 and 20 with color 0
make_dislocation(<13.000000,13.000000,13.000000>, <10.299910,6.717279,4.017185>, 0, 0)
// WriteFan([0, 1, (10, 12, 11, 8)])
// segment connecting nodes 12 and 10 with color 0
make_dislocation(<13.000000,3.000000,3.000000>, <13.000000,13.000000,13.000000>, 0, 0)
// triangle color 1 nodes: [10, 12, 11]
make_triangle(<13.000000,13.000000,13.000000>, <13.000000,3.000000,3.000000>, <10.181070,0.000000,2.818928>, 1)
// segment connecting nodes 11 and 12 with color 0
make_dislocation(<10.181070,0.000000,2.818928>, <13.000000,3.000000,3.000000>, 0, 0)
// triangle color 1 nodes: [10, 11, 8]
make_triangle(<13.000000,13.000000,13.000000>, <10.181070,0.000000,2.818928>, <0.000000,0.000000,13.000000>, 1)
// segment connecting nodes 8 and 11 with color 0
make_dislocation(<0.000000,0.000000,13.000000>, <10.181070,0.000000,2.818928>, 0, 0)
// segment connecting nodes 10 and 11 with color 0
make_dislocation(<13.000000,13.000000,13.000000>, <10.181070,0.000000,2.818928>, 0, 0)
// WriteStrip([1, 1, (12, 21, 109, 69, 70, 46, 47, 88, 120, 89, 36, 37, 100, 58, 59, 80, 114, 115, 78, 79, 135, 60, 61, 101, 127, 126, 102, 103, 33, 34, 141, 22, 3), (12, 23, 71, 131, 132, 48, 49, 90, 121, 91, 39, 40, 104, 62, 63, 83, 116, 117, 81, 82, 136, 64, 65, 105, 129, 128, 106, 107, 25, 26, 142, 24, 4)])
// segment connecting nodes 12 and 23 with color 1
make_dislocation(<13.000000,3.000000,3.000000>, <16.283340,-0.464138,3.180796>, 1, 1)
// segment connecting nodes 23 and 71 with color 1
make_dislocation(<16.283340,-0.464138,3.180796>, <18.278850,-2.738776,3.459928>, 1, 0)
// segment connecting nodes 71 and 131 with color 1
make_dislocation(<18.278850,-2.738776,3.459928>, <20.274350,-5.013414,3.739060>, 1, 0)
// segment connecting nodes 131 and 132 with color 1
make_dislocation(<20.274350,-5.013414,3.739060>, <22.946780,-8.036062,4.089281>, 1, 0)
// segment connecting nodes 132 and 48 with color 1
make_dislocation(<22.946780,-8.036062,4.089281>, <25.020360,-10.333900,4.313533>, 1, 0)
// segment connecting nodes 48 and 49 with color 1
make_dislocation(<25.020360,-10.333900,4.313533>, <27.091550,-12.586510,4.494953>, 1, 0)
// segment connecting nodes 49 and 90 with color 1
make_dislocation(<27.091550,-12.586510,4.494953>, <29.179380,-14.819480,4.640103>, 1, 0)
// segment connecting nodes 90 and 121 with color 1
make_dislocation(<29.179380,-14.819480,4.640103>, <31.294340,-17.049420,4.755077>, 1, 0)
// segment connecting nodes 121 and 91 with color 1
make_dislocation(<31.294340,-17.049420,4.755077>, <33.417280,-19.261510,4.844233>, 1, 0)
// segment connecting nodes 91 and 39 with color 1
make_dislocation(<33.417280,-19.261510,4.844233>, <35.571020,-21.483990,4.912973>, 1, 0)
// segment connecting nodes 39 and 40 with color 1
make_dislocation(<35.571020,-21.483990,4.912973>, <37.725040,-23.689170,4.964132>, 1, 0)
// segment connecting nodes 40 and 104 with color 1
make_dislocation(<37.725040,-23.689170,4.964132>, <39.889630,-25.890600,5.000979>, 1, 0)
// segment connecting nodes 104 and 62 with color 1
make_dislocation(<39.889630,-25.890600,5.000979>, <42.057910,-28.083560,5.025651>, 1, 0)
// segment connecting nodes 62 and 63 with color 1
make_dislocation(<42.057910,-28.083560,5.025651>, <44.230470,-30.270300,5.039834>, 1, 0)
// segment connecting nodes 63 and 83 with color 1
make_dislocation(<44.230470,-30.270300,5.039834>, <46.407300,-32.451750,5.044449>, 1, 0)
// segment connecting nodes 83 and 116 with color 1
make_dislocation(<46.407300,-32.451750,5.044449>, <48.580670,-34.620720,5.040052>, 1, 0)
// segment connecting nodes 116 and 117 with color 1
make_dislocation(<48.580670,-34.620720,5.040052>, <50.767610,-36.794100,5.026492>, 1, 0)
// segment connecting nodes 117 and 81 with color 1
make_dislocation(<50.767610,-36.794100,5.026492>, <52.958700,-38.961910,5.003210>, 1, 0)
// segment connecting nodes 81 and 82 with color 1
make_dislocation(<52.958700,-38.961910,5.003210>, <55.163360,-41.132430,4.969068>, 1, 0)
// segment connecting nodes 82 and 136 with color 1
make_dislocation(<55.163360,-41.132430,4.969068>, <57.371860,-43.294380,4.922514>, 1, 0)
// segment connecting nodes 136 and 64 with color 1
make_dislocation(<57.371860,-43.294380,4.922514>, <59.581920,-45.443370,4.861450>, 1, 0)
// segment connecting nodes 64 and 65 with color 1
make_dislocation(<59.581920,-45.443370,4.861450>, <61.805330,-47.587970,4.782637>, 1, 0)
// segment connecting nodes 65 and 105 with color 1
make_dislocation(<61.805330,-47.587970,4.782637>, <64.015420,-49.699120,4.683700>, 1, 0)
// segment connecting nodes 105 and 129 with color 1
make_dislocation(<64.015420,-49.699120,4.683700>, <66.255410,-51.814230,4.558822>, 1, 0)
// segment connecting nodes 129 and 128 with color 1
make_dislocation(<66.255410,-51.814230,4.558822>, <68.508550,-53.912320,4.403761>, 1, 0)
// segment connecting nodes 128 and 106 with color 1
make_dislocation(<68.508550,-53.912320,4.403761>, <70.755040,-55.969960,4.214915>, 1, 0)
// segment connecting nodes 106 and 107 with color 1
make_dislocation(<70.755040,-55.969960,4.214915>, <73.005200,-57.992280,3.987078>, 1, 0)
// segment connecting nodes 107 and 25 with color 1
make_dislocation(<73.005200,-57.992280,3.987078>, <75.265890,-59.981380,3.715484>, 1, 0)
// segment connecting nodes 25 and 26 with color 1
make_dislocation(<75.265890,-59.981380,3.715484>, <77.553340,-61.948180,3.394847>, 1, 0)
// segment connecting nodes 26 and 142 with color 1
make_dislocation(<77.553340,-61.948180,3.394847>, <80.012670,-64.014400,3.001730>, 1, 0)
// segment connecting nodes 142 and 24 with color 1
make_dislocation(<80.012670,-64.014400,3.001730>, <83.847340,-67.166660,2.319322>, 1, 0)
// segment connecting nodes 24 and 4 with color 1
make_dislocation(<83.847340,-67.166660,2.319322>, <85.000000,-68.000000,2.000000>, 1, 0)
// segment connecting nodes 12 and 21 with color 1
make_dislocation(<13.000000,3.000000,3.000000>, <13.000090,1.349817,4.650092>, 1, 1)
// segment connecting nodes 21 and 109 with color 1
make_dislocation(<13.000090,1.349817,4.650092>, <15.019950,-2.208582,6.188637>, 1, 0)
// segment connecting nodes 109 and 69 with color 1
make_dislocation(<15.019950,-2.208582,6.188637>, <18.335380,-6.715294,7.379917>, 1, 0)
// segment connecting nodes 69 and 70 with color 1
make_dislocation(<18.335380,-6.715294,7.379917>, <20.973310,-9.969300,7.995991>, 1, 0)
// segment connecting nodes 70 and 46 with color 1
make_dislocation(<20.973310,-9.969300,7.995991>, <23.026830,-12.374820,8.347989>, 1, 0)
// segment connecting nodes 46 and 47 with color 1
make_dislocation(<23.026830,-12.374820,8.347989>, <24.985490,-14.592950,8.607465>, 1, 0)
// segment connecting nodes 47 and 88 with color 1
make_dislocation(<24.985490,-14.592950,8.607465>, <27.016050,-16.831830,8.815784>, 1, 0)
// segment connecting nodes 88 and 120 with color 1
make_dislocation(<27.016050,-16.831830,8.815784>, <29.080160,-19.059930,8.979773>, 1, 0)
// segment connecting nodes 120 and 89 with color 1
make_dislocation(<29.080160,-19.059930,8.979773>, <31.206730,-21.316700,9.109976>, 1, 0)
// segment connecting nodes 89 and 36 with color 1
make_dislocation(<31.206730,-21.316700,9.109976>, <33.351100,-23.561350,9.210258>, 1, 0)
// segment connecting nodes 36 and 37 with color 1
make_dislocation(<33.351100,-23.561350,9.210258>, <35.462810,-25.747970,9.285162>, 1, 0)
// segment connecting nodes 37 and 100 with color 1
make_dislocation(<35.462810,-25.747970,9.285162>, <37.629950,-27.972150,9.342199>, 1, 0)
// segment connecting nodes 100 and 58 with color 1
make_dislocation(<37.629950,-27.972150,9.342199>, <39.811180,-30.194320,9.383138>, 1, 0)
// segment connecting nodes 58 and 59 with color 1
make_dislocation(<39.811180,-30.194320,9.383138>, <41.982070,-32.392370,9.410302>, 1, 0)
// segment connecting nodes 59 and 80 with color 1
make_dislocation(<41.982070,-32.392370,9.410302>, <44.155110,-34.580880,9.425772>, 1, 0)
// segment connecting nodes 80 and 114 with color 1
make_dislocation(<44.155110,-34.580880,9.425772>, <46.329980,-36.760640,9.430663>, 1, 0)
// segment connecting nodes 114 and 115 with color 1
make_dislocation(<46.329980,-36.760640,9.430663>, <48.509850,-38.935420,9.425578>, 1, 0)
// segment connecting nodes 115 and 78 with color 1
make_dislocation(<48.509850,-38.935420,9.425578>, <50.707730,-41.117970,9.410242>, 1, 0)
// segment connecting nodes 78 and 79 with color 1
make_dislocation(<50.707730,-41.117970,9.410242>, <52.909510,-43.293430,9.383923>, 1, 0)
// segment connecting nodes 79 and 135 with color 1
make_dislocation(<52.909510,-43.293430,9.383923>, <55.121500,-45.466560,9.345064>, 1, 0)
// segment connecting nodes 135 and 60 with color 1
make_dislocation(<55.121500,-45.466560,9.345064>, <57.351510,-47.642640,9.291133>, 1, 0)
// segment connecting nodes 60 and 61 with color 1
make_dislocation(<57.351510,-47.642640,9.291133>, <59.557740,-49.777680,9.219949>, 1, 0)
// segment connecting nodes 61 and 101 with color 1
make_dislocation(<59.557740,-49.777680,9.219949>, <61.803760,-51.928760,9.125008>, 1, 0)
// segment connecting nodes 101 and 127 with color 1
make_dislocation(<61.803760,-51.928760,9.125008>, <64.054600,-54.055540,9.000934>, 1, 0)
// segment connecting nodes 127 and 126 with color 1
make_dislocation(<64.054600,-54.055540,9.000934>, <66.321010,-56.159710,8.838693>, 1, 0)
// segment connecting nodes 126 and 102 with color 1
make_dislocation(<66.321010,-56.159710,8.838693>, <68.591040,-58.219020,8.627980>, 1, 0)
// segment connecting nodes 102 and 103 with color 1
make_dislocation(<68.591040,-58.219020,8.627980>, <70.903560,-60.253330,8.349779>, 1, 0)
// segment connecting nodes 103 and 33 with color 1
make_dislocation(<70.903560,-60.253330,8.349779>, <73.334170,-62.303070,7.968903>, 1, 0)
// segment connecting nodes 33 and 34 with color 1
make_dislocation(<73.334170,-62.303070,7.968903>, <76.119770,-64.506650,7.386881>, 1, 0)
// segment connecting nodes 34 and 141 with color 1
make_dislocation(<76.119770,-64.506650,7.386881>, <79.212970,-66.696360,6.483388>, 1, 0)
// segment connecting nodes 141 and 22 with color 1
make_dislocation(<79.212970,-66.696360,6.483388>, <82.048550,-68.321080,5.272517>, 1, 0)
// segment connecting nodes 22 and 3 with color 1
make_dislocation(<82.048550,-68.321080,5.272517>, <84.000000,-69.000000,4.000000>, 1, 0)
// triangle color 1 nodes: [12, 12, 23]
make_triangle(<13.000000,3.000000,3.000000>, <13.000000,3.000000,3.000000>, <16.283340,-0.464138,3.180796>, 1)
// triangle color 1 nodes: [23, 12, 21]
make_triangle(<16.283340,-0.464138,3.180796>, <13.000000,3.000000,3.000000>, <13.000090,1.349817,4.650092>, 1)
// triangle color 1 nodes: [23, 21, 71]
make_triangle(<16.283340,-0.464138,3.180796>, <13.000090,1.349817,4.650092>, <18.278850,-2.738776,3.459928>, 1)
// triangle color 1 nodes: [71, 21, 109]
make_triangle(<18.278850,-2.738776,3.459928>, <13.000090,1.349817,4.650092>, <15.019950,-2.208582,6.188637>, 1)
// triangle color 1 nodes: [71, 109, 131]
make_triangle(<18.278850,-2.738776,3.459928>, <15.019950,-2.208582,6.188637>, <20.274350,-5.013414,3.739060>, 1)
// triangle color 1 nodes: [131, 109, 69]
make_triangle(<20.274350,-5.013414,3.739060>, <15.019950,-2.208582,6.188637>, <18.335380,-6.715294,7.379917>, 1)
// triangle color 1 nodes: [131, 69, 132]
make_triangle(<20.274350,-5.013414,3.739060>, <18.335380,-6.715294,7.379917>, <22.946780,-8.036062,4.089281>, 1)
// triangle color 1 nodes: [132, 69, 70]
make_triangle(<22.946780,-8.036062,4.089281>, <18.335380,-6.715294,7.379917>, <20.973310,-9.969300,7.995991>, 1)
// triangle color 1 nodes: [132, 70, 48]
make_triangle(<22.946780,-8.036062,4.089281>, <20.973310,-9.969300,7.995991>, <25.020360,-10.333900,4.313533>, 1)
// triangle color 1 nodes: [48, 70, 46]
make_triangle(<25.020360,-10.333900,4.313533>, <20.973310,-9.969300,7.995991>, <23.026830,-12.374820,8.347989>, 1)
// triangle color 1 nodes: [48, 46, 49]
make_triangle(<25.020360,-10.333900,4.313533>, <23.026830,-12.374820,8.347989>, <27.091550,-12.586510,4.494953>, 1)
// triangle color 1 nodes: [49, 46, 47]
make_triangle(<27.091550,-12.586510,4.494953>, <23.026830,-12.374820,8.347989>, <24.985490,-14.592950,8.607465>, 1)
// triangle color 1 nodes: [49, 47, 90]
make_triangle(<27.091550,-12.586510,4.494953>, <24.985490,-14.592950,8.607465>, <29.179380,-14.819480,4.640103>, 1)
// triangle color 1 nodes: [90, 47, 88]
make_triangle(<29.179380,-14.819480,4.640103>, <24.985490,-14.592950,8.607465>, <27.016050,-16.831830,8.815784>, 1)
// triangle color 1 nodes: [90, 88, 121]
make_triangle(<29.179380,-14.819480,4.640103>, <27.016050,-16.831830,8.815784>, <31.294340,-17.049420,4.755077>, 1)
// triangle color 1 nodes: [121, 88, 120]
make_triangle(<31.294340,-17.049420,4.755077>, <27.016050,-16.831830,8.815784>, <29.080160,-19.059930,8.979773>, 1)
// triangle color 1 nodes: [121, 120, 91]
make_triangle(<31.294340,-17.049420,4.755077>, <29.080160,-19.059930,8.979773>, <33.417280,-19.261510,4.844233>, 1)
// triangle color 1 nodes: [91, 120, 89]
make_triangle(<33.417280,-19.261510,4.844233>, <29.080160,-19.059930,8.979773>, <31.206730,-21.316700,9.109976>, 1)
// triangle color 1 nodes: [91, 89, 39]
make_triangle(<33.417280,-19.261510,4.844233>, <31.206730,-21.316700,9.109976>, <35.571020,-21.483990,4.912973>, 1)
// triangle color 1 nodes: [39, 89, 36]
make_triangle(<35.571020,-21.483990,4.912973>, <31.206730,-21.316700,9.109976>, <33.351100,-23.561350,9.210258>, 1)
// triangle color 1 nodes: [39, 36, 40]
make_triangle(<35.571020,-21.483990,4.912973>, <33.351100,-23.561350,9.210258>, <37.725040,-23.689170,4.964132>, 1)
// triangle color 1 nodes: [40, 36, 37]
make_triangle(<37.725040,-23.689170,4.964132>, <33.351100,-23.561350,9.210258>, <35.462810,-25.747970,9.285162>, 1)
// triangle color 1 nodes: [40, 37, 104]
make_triangle(<37.725040,-23.689170,4.964132>, <35.462810,-25.747970,9.285162>, <39.889630,-25.890600,5.000979>, 1)
// triangle color 1 nodes: [104, 37, 100]
make_triangle(<39.889630,-25.890600,5.000979>, <35.462810,-25.747970,9.285162>, <37.629950,-27.972150,9.342199>, 1)
// triangle color 1 nodes: [104, 100, 62]
make_triangle(<39.889630,-25.890600,5.000979>, <37.629950,-27.972150,9.342199>, <42.057910,-28.083560,5.025651>, 1)
// triangle color 1 nodes: [62, 100, 58]
make_triangle(<42.057910,-28.083560,5.025651>, <37.629950,-27.972150,9.342199>, <39.811180,-30.194320,9.383138>, 1)
// triangle color 1 nodes: [62, 58, 63]
make_triangle(<42.057910,-28.083560,5.025651>, <39.811180,-30.194320,9.383138>, <44.230470,-30.270300,5.039834>, 1)
// triangle color 1 nodes: [63, 58, 59]
make_triangle(<44.230470,-30.270300,5.039834>, <39.811180,-30.194320,9.383138>, <41.982070,-32.392370,9.410302>, 1)
// triangle color 1 nodes: [63, 59, 83]
make_triangle(<44.230470,-30.270300,5.039834>, <41.982070,-32.392370,9.410302>, <46.407300,-32.451750,5.044449>, 1)
// triangle color 1 nodes: [83, 59, 80]
make_triangle(<46.407300,-32.451750,5.044449>, <41.982070,-32.392370,9.410302>, <44.155110,-34.580880,9.425772>, 1)
// triangle color 1 nodes: [83, 80, 116]
make_triangle(<46.407300,-32.451750,5.044449>, <44.155110,-34.580880,9.425772>, <48.580670,-34.620720,5.040052>, 1)
// triangle color 1 nodes: [116, 80, 114]
make_triangle(<48.580670,-34.620720,5.040052>, <44.155110,-34.580880,9.425772>, <46.329980,-36.760640,9.430663>, 1)
// triangle color 1 nodes: [116, 114, 117]
make_triangle(<48.580670,-34.620720,5.040052>, <46.329980,-36.760640,9.430663>, <50.767610,-36.794100,5.026492>, 1)
// triangle color 1 nodes: [117, 114, 115]
make_triangle(<50.767610,-36.794100,5.026492>, <46.329980,-36.760640,9.430663>, <48.509850,-38.935420,9.425578>, 1)
// triangle color 1 nodes: [117, 115, 81]
make_triangle(<50.767610,-36.794100,5.026492>, <48.509850,-38.935420,9.425578>, <52.958700,-38.961910,5.003210>, 1)
// triangle color 1 nodes: [81, 115, 78]
make_triangle(<52.958700,-38.961910,5.003210>, <48.509850,-38.935420,9.425578>, <50.707730,-41.117970,9.410242>, 1)
// triangle color 1 nodes: [81, 78, 82]
make_triangle(<52.958700,-38.961910,5.003210>, <50.707730,-41.117970,9.410242>, <55.163360,-41.132430,4.969068>, 1)
// triangle color 1 nodes: [82, 78, 79]
make_triangle(<55.163360,-41.132430,4.969068>, <50.707730,-41.117970,9.410242>, <52.909510,-43.293430,9.383923>, 1)
// triangle color 1 nodes: [82, 79, 136]
make_triangle(<55.163360,-41.132430,4.969068>, <52.909510,-43.293430,9.383923>, <57.371860,-43.294380,4.922514>, 1)
// triangle color 1 nodes: [136, 79, 135]
make_triangle(<57.371860,-43.294380,4.922514>, <52.909510,-43.293430,9.383923>, <55.121500,-45.466560,9.345064>, 1)
// triangle color 1 nodes: [136, 135, 64]
make_triangle(<57.371860,-43.294380,4.922514>, <55.121500,-45.466560,9.345064>, <59.581920,-45.443370,4.861450>, 1)
// triangle color 1 nodes: [64, 135, 60]
make_triangle(<59.581920,-45.443370,4.861450>, <55.121500,-45.466560,9.345064>, <57.351510,-47.642640,9.291133>, 1)
// triangle color 1 nodes: [64, 60, 65]
make_triangle(<59.581920,-45.443370,4.861450>, <57.351510,-47.642640,9.291133>, <61.805330,-47.587970,4.782637>, 1)
// triangle color 1 nodes: [65, 60, 61]
make_triangle(<61.805330,-47.587970,4.782637>, <57.351510,-47.642640,9.291133>, <59.557740,-49.777680,9.219949>, 1)
// triangle color 1 nodes: [65, 61, 105]
make_triangle(<61.805330,-47.587970,4.782637>, <59.557740,-49.777680,9.219949>, <64.015420,-49.699120,4.683700>, 1)
// triangle color 1 nodes: [105, 61, 101]
make_triangle(<64.015420,-49.699120,4.683700>, <59.557740,-49.777680,9.219949>, <61.803760,-51.928760,9.125008>, 1)
// triangle color 1 nodes: [105, 101, 129]
make_triangle(<64.015420,-49.699120,4.683700>, <61.803760,-51.928760,9.125008>, <66.255410,-51.814230,4.558822>, 1)
// triangle color 1 nodes: [129, 101, 127]
make_triangle(<66.255410,-51.814230,4.558822>, <61.803760,-51.928760,9.125008>, <64.054600,-54.055540,9.000934>, 1)
// triangle color 1 nodes: [129, 127, 128]
make_triangle(<66.255410,-51.814230,4.558822>, <64.054600,-54.055540,9.000934>, <68.508550,-53.912320,4.403761>, 1)
// triangle color 1 nodes: [128, 127, 126]
make_triangle(<68.508550,-53.912320,4.403761>, <64.054600,-54.055540,9.000934>, <66.321010,-56.159710,8.838693>, 1)
// triangle color 1 nodes: [128, 126, 106]
make_triangle(<68.508550,-53.912320,4.403761>, <66.321010,-56.159710,8.838693>, <70.755040,-55.969960,4.214915>, 1)
// triangle color 1 nodes: [106, 126, 102]
make_triangle(<70.755040,-55.969960,4.214915>, <66.321010,-56.159710,8.838693>, <68.591040,-58.219020,8.627980>, 1)
// triangle color 1 nodes: [106, 102, 107]
make_triangle(<70.755040,-55.969960,4.214915>, <68.591040,-58.219020,8.627980>, <73.005200,-57.992280,3.987078>, 1)
// triangle color 1 nodes: [107, 102, 103]
make_triangle(<73.005200,-57.992280,3.987078>, <68.591040,-58.219020,8.627980>, <70.903560,-60.253330,8.349779>, 1)
// triangle color 1 nodes: [107, 103, 25]
make_triangle(<73.005200,-57.992280,3.987078>, <70.903560,-60.253330,8.349779>, <75.265890,-59.981380,3.715484>, 1)
// triangle color 1 nodes: [25, 103, 33]
make_triangle(<75.265890,-59.981380,3.715484>, <70.903560,-60.253330,8.349779>, <73.334170,-62.303070,7.968903>, 1)
// triangle color 1 nodes: [25, 33, 26]
make_triangle(<75.265890,-59.981380,3.715484>, <73.334170,-62.303070,7.968903>, <77.553340,-61.948180,3.394847>, 1)
// triangle color 1 nodes: [26, 33, 34]
make_triangle(<77.553340,-61.948180,3.394847>, <73.334170,-62.303070,7.968903>, <76.119770,-64.506650,7.386881>, 1)
// triangle color 1 nodes: [26, 34, 142]
make_triangle(<77.553340,-61.948180,3.394847>, <76.119770,-64.506650,7.386881>, <80.012670,-64.014400,3.001730>, 1)
// triangle color 1 nodes: [142, 34, 141]
make_triangle(<80.012670,-64.014400,3.001730>, <76.119770,-64.506650,7.386881>, <79.212970,-66.696360,6.483388>, 1)
// triangle color 1 nodes: [142, 141, 24]
make_triangle(<80.012670,-64.014400,3.001730>, <79.212970,-66.696360,6.483388>, <83.847340,-67.166660,2.319322>, 1)
// triangle color 1 nodes: [24, 141, 22]
make_triangle(<83.847340,-67.166660,2.319322>, <79.212970,-66.696360,6.483388>, <82.048550,-68.321080,5.272517>, 1)
// triangle color 1 nodes: [24, 22, 4]
make_triangle(<83.847340,-67.166660,2.319322>, <82.048550,-68.321080,5.272517>, <85.000000,-68.000000,2.000000>, 1)
// triangle color 1 nodes: [4, 22, 3]
make_triangle(<85.000000,-68.000000,2.000000>, <82.048550,-68.321080,5.272517>, <84.000000,-69.000000,4.000000>, 1)
// WriteStrip([1, 1, (9, 14, 27, 67, 66, 130, 43, 42, 84, 118, 85, 29, 28, 92, 51, 50, 74, 110, 111, 73, 72, 133, 53, 52, 93, 123, 122, 95, 94, 137, 138, 30, 13, 1), (9, 108, 6, 5, 68, 45, 44, 86, 119, 87, 32, 31, 96, 55, 54, 77, 112, 113, 76, 75, 134, 57, 56, 97, 125, 124, 99, 98, 139, 140, 41, 15, 2)])
// segment connecting nodes 9 and 14 with color 1
make_dislocation(<3.000000,13.000000,3.000000>, <2.011143,12.918580,4.070279>, 1, 1)
// segment connecting nodes 14 and 27 with color 1
make_dislocation(<2.011143,12.918580,4.070279>, <-1.165296,13.998390,6.166907>, 1, 0)
// segment connecting nodes 27 and 67 with color 1
make_dislocation(<-1.165296,13.998390,6.166907>, <-3.919068,15.606330,7.312743>, 1, 0)
// segment connecting nodes 67 and 66 with color 1
make_dislocation(<-3.919068,15.606330,7.312743>, <-7.053962,17.944840,8.109121>, 1, 0)
// segment connecting nodes 66 and 130 with color 1
make_dislocation(<-7.053962,17.944840,8.109121>, <-10.160430,20.563570,8.596863>, 1, 0)
// segment connecting nodes 130 and 43 with color 1
make_dislocation(<-10.160430,20.563570,8.596863>, <-12.660560,22.797580,8.862980>, 1, 0)
// segment connecting nodes 43 and 42 with color 1
make_dislocation(<-12.660560,22.797580,8.862980>, <-14.946270,24.905300,9.040968>, 1, 0)
// segment connecting nodes 42 and 84 with color 1
make_dislocation(<-14.946270,24.905300,9.040968>, <-17.173560,27.004110,9.169452>, 1, 0)
// segment connecting nodes 84 and 118 with color 1
make_dislocation(<-17.173560,27.004110,9.169452>, <-19.379990,29.116210,9.263780>, 1, 0)
// segment connecting nodes 118 and 85 with color 1
make_dislocation(<-19.379990,29.116210,9.263780>, <-21.610460,31.276910,9.333554>, 1, 0)
// segment connecting nodes 85 and 29 with color 1
make_dislocation(<-21.610460,31.276910,9.333554>, <-23.809030,33.426110,9.382921>, 1, 0)
// segment connecting nodes 29 and 28 with color 1
make_dislocation(<-23.809030,33.426110,9.382921>, <-26.003270,35.586460,9.416807>, 1, 0)
// segment connecting nodes 28 and 92 with color 1
make_dislocation(<-26.003270,35.586460,9.416807>, <-28.182540,37.744500,9.438041>, 1, 0)
// segment connecting nodes 92 and 51 with color 1
make_dislocation(<-28.182540,37.744500,9.438041>, <-30.367160,39.918510,9.448656>, 1, 0)
// segment connecting nodes 51 and 50 with color 1
make_dislocation(<-30.367160,39.918510,9.448656>, <-32.555740,42.106080,9.449666>, 1, 0)
// segment connecting nodes 50 and 74 with color 1
make_dislocation(<-32.555740,42.106080,9.449666>, <-34.698980,44.257190,9.441788>, 1, 0)
// segment connecting nodes 74 and 110 with color 1
make_dislocation(<-34.698980,44.257190,9.441788>, <-36.856240,46.431360,9.424887>, 1, 0)
// segment connecting nodes 110 and 111 with color 1
make_dislocation(<-36.856240,46.431360,9.424887>, <-39.023160,48.624950,9.398216>, 1, 0)
// segment connecting nodes 111 and 73 with color 1
make_dislocation(<-39.023160,48.624950,9.398216>, <-41.195520,50.835210,9.360312>, 1, 0)
// segment connecting nodes 73 and 72 with color 1
make_dislocation(<-41.195520,50.835210,9.360312>, <-43.377660,53.068560,9.309102>, 1, 0)
// segment connecting nodes 72 and 133 with color 1
make_dislocation(<-43.377660,53.068560,9.309102>, <-45.500580,55.256490,9.244086>, 1, 0)
// segment connecting nodes 133 and 53 with color 1
make_dislocation(<-45.500580,55.256490,9.244086>, <-47.643620,57.483480,9.160133>, 1, 0)
// segment connecting nodes 53 and 52 with color 1
make_dislocation(<-47.643620,57.483480,9.160133>, <-49.766500,59.712340,9.054166>, 1, 0)
// segment connecting nodes 52 and 93 with color 1
make_dislocation(<-49.766500,59.712340,9.054166>, <-51.863590,61.941910,8.921678>, 1, 0)
// segment connecting nodes 93 and 123 with color 1
make_dislocation(<-51.863590,61.941910,8.921678>, <-53.935910,64.179530,8.756383>, 1, 0)
// segment connecting nodes 123 and 122 with color 1
make_dislocation(<-53.935910,64.179530,8.756383>, <-55.968460,66.416170,8.552289>, 1, 0)
// segment connecting nodes 122 and 95 with color 1
make_dislocation(<-55.968460,66.416170,8.552289>, <-57.964720,68.664830,8.299889>, 1, 0)
// segment connecting nodes 95 and 94 with color 1
make_dislocation(<-57.964720,68.664830,8.299889>, <-59.886060,70.892400,7.993664>, 1, 0)
// segment connecting nodes 94 and 137 with color 1
make_dislocation(<-59.886060,70.892400,7.993664>, <-61.732650,73.110520,7.622127>, 1, 0)
// segment connecting nodes 137 and 138 with color 1
make_dislocation(<-61.732650,73.110520,7.622127>, <-63.585660,75.439350,7.146298>, 1, 0)
// segment connecting nodes 138 and 30 with color 1
make_dislocation(<-63.585660,75.439350,7.146298>, <-65.700460,78.283030,6.417434>, 1, 0)
// segment connecting nodes 30 and 13 with color 1
make_dislocation(<-65.700460,78.283030,6.417434>, <-67.849620,81.574060,5.275556>, 1, 0)
// segment connecting nodes 13 and 1 with color 1
make_dislocation(<-67.849620,81.574060,5.275556>, <-69.000000,84.000000,4.000000>, 1, 0)
// segment connecting nodes 9 and 108 with color 1
make_dislocation(<3.000000,13.000000,3.000000>, <1.660134,14.595980,2.743888>, 1, 1)
// segment connecting nodes 108 and 6 with color 1
make_dislocation(<1.660134,14.595980,2.743888>, <-0.267623,16.270100,2.997525>, 1, 0)
// segment connecting nodes 6 and 5 with color 1
make_dislocation(<-0.267623,16.270100,2.997525>, <-3.966081,19.327290,3.638787>, 1, 0)
// segment connecting nodes 5 and 68 with color 1
make_dislocation(<-3.966081,19.327290,3.638787>, <-8.053893,22.908090,4.145806>, 1, 0)
// segment connecting nodes 68 and 45 with color 1
make_dislocation(<-8.053893,22.908090,4.145806>, <-10.347000,24.977870,4.369128>, 1, 0)
// segment connecting nodes 45 and 44 with color 1
make_dislocation(<-10.347000,24.977870,4.369128>, <-12.574300,27.029920,4.544380>, 1, 0)
// segment connecting nodes 44 and 86 with color 1
make_dislocation(<-12.574300,27.029920,4.544380>, <-14.799850,29.116070,4.683780>, 1, 0)
// segment connecting nodes 86 and 119 with color 1
make_dislocation(<-14.799850,29.116070,4.683780>, <-17.015530,31.222780,4.792754>, 1, 0)
// segment connecting nodes 119 and 87 with color 1
make_dislocation(<-17.015530,31.222780,4.792754>, <-19.223040,33.346250,4.876793>, 1, 0)
// segment connecting nodes 87 and 32 with color 1
make_dislocation(<-19.223040,33.346250,4.876793>, <-21.450910,35.509900,4.941017>, 1, 0)
// segment connecting nodes 32 and 31 with color 1
make_dislocation(<-21.450910,35.509900,4.941017>, <-23.652200,37.664400,4.987797>, 1, 0)
// segment connecting nodes 31 and 96 with color 1
make_dislocation(<-23.652200,37.664400,4.987797>, <-25.845060,39.824310,5.020743>, 1, 0)
// segment connecting nodes 96 and 55 with color 1
make_dislocation(<-25.845060,39.824310,5.020743>, <-28.045860,42.003700,5.042160>, 1, 0)
// segment connecting nodes 55 and 54 with color 1
make_dislocation(<-28.045860,42.003700,5.042160>, <-30.228630,44.175180,5.053451>, 1, 0)
// segment connecting nodes 54 and 77 with color 1
make_dislocation(<-30.228630,44.175180,5.053451>, <-32.406730,46.351010,5.055723>, 1, 0)
// segment connecting nodes 77 and 112 with color 1
make_dislocation(<-32.406730,46.351010,5.055723>, <-34.592200,48.542720,5.049477>, 1, 0)
// segment connecting nodes 112 and 113 with color 1
make_dislocation(<-34.592200,48.542720,5.049477>, <-36.755620,50.720890,5.034730>, 1, 0)
// segment connecting nodes 113 and 76 with color 1
make_dislocation(<-36.755620,50.720890,5.034730>, <-38.921160,52.910160,5.010995>, 1, 0)
// segment connecting nodes 76 and 75 with color 1
make_dislocation(<-38.921160,52.910160,5.010995>, <-41.100900,55.124130,4.976767>, 1, 0)
// segment connecting nodes 75 and 134 with color 1
make_dislocation(<-41.100900,55.124130,4.976767>, <-43.268330,57.337560,4.930767>, 1, 0)
// segment connecting nodes 134 and 57 with color 1
make_dislocation(<-43.268330,57.337560,4.930767>, <-45.425590,59.554850,4.870748>, 1, 0)
// segment connecting nodes 57 and 56 with color 1
make_dislocation(<-45.425590,59.554850,4.870748>, <-47.572170,61.778560,4.793609>, 1, 0)
// segment connecting nodes 56 and 97 with color 1
make_dislocation(<-47.572170,61.778560,4.793609>, <-49.703350,64.007590,4.695760>, 1, 0)
// segment connecting nodes 97 and 125 with color 1
make_dislocation(<-49.703350,64.007590,4.695760>, <-51.826190,66.254210,4.571988>, 1, 0)
// segment connecting nodes 125 and 124 with color 1
make_dislocation(<-51.826190,66.254210,4.571988>, <-53.922630,68.505170,4.417461>, 1, 0)
// segment connecting nodes 124 and 99 with color 1
make_dislocation(<-53.922630,68.505170,4.417461>, <-55.991540,70.765470,4.226062>, 1, 0)
// segment connecting nodes 99 and 98 with color 1
make_dislocation(<-55.991540,70.765470,4.226062>, <-58.032780,73.041450,3.991336>, 1, 0)
// segment connecting nodes 98 and 139 with color 1
make_dislocation(<-58.032780,73.041450,3.991336>, <-60.020790,75.309780,3.711019>, 1, 0)
// segment connecting nodes 139 and 140 with color 1
make_dislocation(<-60.020790,75.309780,3.711019>, <-62.002660,77.626560,3.376095>, 1, 0)
// segment connecting nodes 140 and 41 with color 1
make_dislocation(<-62.002660,77.626560,3.376095>, <-64.068650,80.096050,2.972598>, 1, 0)
// segment connecting nodes 41 and 15 with color 1
make_dislocation(<-64.068650,80.096050,2.972598>, <-67.010640,83.667600,2.343028>, 1, 0)
// segment connecting nodes 15 and 2 with color 1
make_dislocation(<-67.010640,83.667600,2.343028>, <-68.000000,85.000000,2.000000>, 1, 0)
// triangle color 1 nodes: [9, 9, 14]
make_triangle(<3.000000,13.000000,3.000000>, <3.000000,13.000000,3.000000>, <2.011143,12.918580,4.070279>, 1)
// triangle color 1 nodes: [14, 9, 108]
make_triangle(<2.011143,12.918580,4.070279>, <3.000000,13.000000,3.000000>, <1.660134,14.595980,2.743888>, 1)
// triangle color 1 nodes: [14, 108, 27]
make_triangle(<2.011143,12.918580,4.070279>, <1.660134,14.595980,2.743888>, <-1.165296,13.998390,6.166907>, 1)
// triangle color 1 nodes: [27, 108, 6]
make_triangle(<-1.165296,13.998390,6.166907>, <1.660134,14.595980,2.743888>, <-0.267623,16.270100,2.997525>, 1)
// triangle color 1 nodes: [27, 6, 67]
make_triangle(<-1.165296,13.998390,6.166907>, <-0.267623,16.270100,2.997525>, <-3.919068,15.606330,7.312743>, 1)
// triangle color 1 nodes: [67, 6, 5]
make_triangle(<-3.919068,15.606330,7.312743>, <-0.267623,16.270100,2.997525>, <-3.966081,19.327290,3.638787>, 1)
// triangle color 1 nodes: [67, 5, 66]
make_triangle(<-3.919068,15.606330,7.312743>, <-3.966081,19.327290,3.638787>, <-7.053962,17.944840,8.109121>, 1)
// triangle color 1 nodes: [66, 5, 68]
make_triangle(<-7.053962,17.944840,8.109121>, <-3.966081,19.327290,3.638787>, <-8.053893,22.908090,4.145806>, 1)
// triangle color 1 nodes: [66, 68, 130]
make_triangle(<-7.053962,17.944840,8.109121>, <-8.053893,22.908090,4.145806>, <-10.160430,20.563570,8.596863>, 1)
// triangle color 1 nodes: [130, 68, 45]
make_triangle(<-10.160430,20.563570,8.596863>, <-8.053893,22.908090,4.145806>, <-10.347000,24.977870,4.369128>, 1)
// triangle color 1 nodes: [130, 45, 43]
make_triangle(<-10.160430,20.563570,8.596863>, <-10.347000,24.977870,4.369128>, <-12.660560,22.797580,8.862980>, 1)
// triangle color 1 nodes: [43, 45, 44]
make_triangle(<-12.660560,22.797580,8.862980>, <-10.347000,24.977870,4.369128>, <-12.574300,27.029920,4.544380>, 1)
// triangle color 1 nodes: [43, 44, 42]
make_triangle(<-12.660560,22.797580,8.862980>, <-12.574300,27.029920,4.544380>, <-14.946270,24.905300,9.040968>, 1)
// triangle color 1 nodes: [42, 44, 86]
make_triangle(<-14.946270,24.905300,9.040968>, <-12.574300,27.029920,4.544380>, <-14.799850,29.116070,4.683780>, 1)
// triangle color 1 nodes: [42, 86, 84]
make_triangle(<-14.946270,24.905300,9.040968>, <-14.799850,29.116070,4.683780>, <-17.173560,27.004110,9.169452>, 1)
// triangle color 1 nodes: [84, 86, 119]
make_triangle(<-17.173560,27.004110,9.169452>, <-14.799850,29.116070,4.683780>, <-17.015530,31.222780,4.792754>, 1)
// triangle color 1 nodes: [84, 119, 118]
make_triangle(<-17.173560,27.004110,9.169452>, <-17.015530,31.222780,4.792754>, <-19.379990,29.116210,9.263780>, 1)
// triangle color 1 nodes: [118, 119, 87]
make_triangle(<-19.379990,29.116210,9.263780>, <-17.015530,31.222780,4.792754>, <-19.223040,33.346250,4.876793>, 1)
// triangle color 1 nodes: [118, 87, 85]
make_triangle(<-19.379990,29.116210,9.263780>, <-19.223040,33.346250,4.876793>, <-21.610460,31.276910,9.333554>, 1)
// triangle color 1 nodes: [85, 87, 32]
make_triangle(<-21.610460,31.276910,9.333554>, <-19.223040,33.346250,4.876793>, <-21.450910,35.509900,4.941017>, 1)
// triangle color 1 nodes: [85, 32, 29]
make_triangle(<-21.610460,31.276910,9.333554>, <-21.450910,35.509900,4.941017>, <-23.809030,33.426110,9.382921>, 1)
// triangle color 1 nodes: [29, 32, 31]
make_triangle(<-23.809030,33.426110,9.382921>, <-21.450910,35.509900,4.941017>, <-23.652200,37.664400,4.987797>, 1)
// triangle color 1 nodes: [29, 31, 28]
make_triangle(<-23.809030,33.426110,9.382921>, <-23.652200,37.664400,4.987797>, <-26.003270,35.586460,9.416807>, 1)
// triangle color 1 nodes: [28, 31, 96]
make_triangle(<-26.003270,35.586460,9.416807>, <-23.652200,37.664400,4.987797>, <-25.845060,39.824310,5.020743>, 1)
// triangle color 1 nodes: [28, 96, 92]
make_triangle(<-26.003270,35.586460,9.416807>, <-25.845060,39.824310,5.020743>, <-28.182540,37.744500,9.438041>, 1)
// triangle color 1 nodes: [92, 96, 55]
make_triangle(<-28.182540,37.744500,9.438041>, <-25.845060,39.824310,5.020743>, <-28.045860,42.003700,5.042160>, 1)
// triangle color 1 nodes: [92, 55, 51]
make_triangle(<-28.182540,37.744500,9.438041>, <-28.045860,42.003700,5.042160>, <-30.367160,39.918510,9.448656>, 1)
// triangle color 1 nodes: [51, 55, 54]
make_triangle(<-30.367160,39.918510,9.448656>, <-28.045860,42.003700,5.042160>, <-30.228630,44.175180,5.053451>, 1)
// triangle color 1 nodes: [51, 54, 50]
make_triangle(<-30.367160,39.918510,9.448656>, <-30.228630,44.175180,5.053451>, <-32.555740,42.106080,9.449666>, 1)
// triangle color 1 nodes: [50, 54, 77]
make_triangle(<-32.555740,42.106080,9.449666>, <-30.228630,44.175180,5.053451>, <-32.406730,46.351010,5.055723>, 1)
// triangle color 1 nodes: [50, 77, 74]
make_triangle(<-32.555740,42.106080,9.449666>, <-32.406730,46.351010,5.055723>, <-34.698980,44.257190,9.441788>, 1)
// triangle color 1 nodes: [74, 77, 112]
make_triangle(<-34.698980,44.257190,9.441788>, <-32.406730,46.351010,5.055723>, <-34.592200,48.542720,5.049477>, 1)
// triangle color 1 nodes: [74, 112, 110]
make_triangle(<-34.698980,44.257190,9.441788>, <-34.592200,48.542720,5.049477>, <-36.856240,46.431360,9.424887>, 1)
// triangle color 1 nodes: [110, 112, 113]
make_triangle(<-36.856240,46.431360,9.424887>, <-34.592200,48.542720,5.049477>, <-36.755620,50.720890,5.034730>, 1)
// triangle color 1 nodes: [110, 113, 111]
make_triangle(<-36.856240,46.431360,9.424887>, <-36.755620,50.720890,5.034730>, <-39.023160,48.624950,9.398216>, 1)
// triangle color 1 nodes: [111, 113, 76]
make_triangle(<-39.023160,48.624950,9.398216>, <-36.755620,50.720890,5.034730>, <-38.921160,52.910160,5.010995>, 1)
// triangle color 1 nodes: [111, 76, 73]
make_triangle(<-39.023160,48.624950,9.398216>, <-38.921160,52.910160,5.010995>, <-41.195520,50.835210,9.360312>, 1)
// triangle color 1 nodes: [73, 76, 75]
make_triangle(<-41.195520,50.835210,9.360312>, <-38.921160,52.910160,5.010995>, <-41.100900,55.124130,4.976767>, 1)
// triangle color 1 nodes: [73, 75, 72]
make_triangle(<-41.195520,50.835210,9.360312>, <-41.100900,55.124130,4.976767>, <-43.377660,53.068560,9.309102>, 1)
// triangle color 1 nodes: [72, 75, 134]
make_triangle(<-43.377660,53.068560,9.309102>, <-41.100900,55.124130,4.976767>, <-43.268330,57.337560,4.930767>, 1)
// triangle color 1 nodes: [72, 134, 133]
make_triangle(<-43.377660,53.068560,9.309102>, <-43.268330,57.337560,4.930767>, <-45.500580,55.256490,9.244086>, 1)
// triangle color 1 nodes: [133, 134, 57]
make_triangle(<-45.500580,55.256490,9.244086>, <-43.268330,57.337560,4.930767>, <-45.425590,59.554850,4.870748>, 1)
// triangle color 1 nodes: [133, 57, 53]
make_triangle(<-45.500580,55.256490,9.244086>, <-45.425590,59.554850,4.870748>, <-47.643620,57.483480,9.160133>, 1)
// triangle color 1 nodes: [53, 57, 56]
make_triangle(<-47.643620,57.483480,9.160133>, <-45.425590,59.554850,4.870748>, <-47.572170,61.778560,4.793609>, 1)
// triangle color 1 nodes: [53, 56, 52]
make_triangle(<-47.643620,57.483480,9.160133>, <-47.572170,61.778560,4.793609>, <-49.766500,59.712340,9.054166>, 1)
// triangle color 1 nodes: [52, 56, 97]
make_triangle(<-49.766500,59.712340,9.054166>, <-47.572170,61.778560,4.793609>, <-49.703350,64.007590,4.695760>, 1)
// triangle color 1 nodes: [52, 97, 93]
make_triangle(<-49.766500,59.712340,9.054166>, <-49.703350,64.007590,4.695760>, <-51.863590,61.941910,8.921678>, 1)
// triangle color 1 nodes: [93, 97, 125]
make_triangle(<-51.863590,61.941910,8.921678>, <-49.703350,64.007590,4.695760>, <-51.826190,66.254210,4.571988>, 1)
// triangle color 1 nodes: [93, 125, 123]
make_triangle(<-51.863590,61.941910,8.921678>, <-51.826190,66.254210,4.571988>, <-53.935910,64.179530,8.756383>, 1)
// triangle color 1 nodes: [123, 125, 124]
make_triangle(<-53.935910,64.179530,8.756383>, <-51.826190,66.254210,4.571988>, <-53.922630,68.505170,4.417461>, 1)
// triangle color 1 nodes: [123, 124, 122]
make_triangle(<-53.935910,64.179530,8.756383>, <-53.922630,68.505170,4.417461>, <-55.968460,66.416170,8.552289>, 1)
// triangle color 1 nodes: [122, 124, 99]
make_triangle(<-55.968460,66.416170,8.552289>, <-53.922630,68.505170,4.417461>, <-55.991540,70.765470,4.226062>, 1)
// triangle color 1 nodes: [122, 99, 95]
make_triangle(<-55.968460,66.416170,8.552289>, <-55.991540,70.765470,4.226062>, <-57.964720,68.664830,8.299889>, 1)
// triangle color 1 nodes: [95, 99, 98]
make_triangle(<-57.964720,68.664830,8.299889>, <-55.991540,70.765470,4.226062>, <-58.032780,73.041450,3.991336>, 1)
// triangle color 1 nodes: [95, 98, 94]
make_triangle(<-57.964720,68.664830,8.299889>, <-58.032780,73.041450,3.991336>, <-59.886060,70.892400,7.993664>, 1)
// triangle color 1 nodes: [94, 98, 139]
make_triangle(<-59.886060,70.892400,7.993664>, <-58.032780,73.041450,3.991336>, <-60.020790,75.309780,3.711019>, 1)
// triangle color 1 nodes: [94, 139, 137]
make_triangle(<-59.886060,70.892400,7.993664>, <-60.020790,75.309780,3.711019>, <-61.732650,73.110520,7.622127>, 1)
// triangle color 1 nodes: [137, 139, 140]
make_triangle(<-61.732650,73.110520,7.622127>, <-60.020790,75.309780,3.711019>, <-62.002660,77.626560,3.376095>, 1)
// triangle color 1 nodes: [137, 140, 138]
make_triangle(<-61.732650,73.110520,7.622127>, <-62.002660,77.626560,3.376095>, <-63.585660,75.439350,7.146298>, 1)
// triangle color 1 nodes: [138, 140, 41]
make_triangle(<-63.585660,75.439350,7.146298>, <-62.002660,77.626560,3.376095>, <-64.068650,80.096050,2.972598>, 1)
// triangle color 1 nodes: [138, 41, 30]
make_triangle(<-63.585660,75.439350,7.146298>, <-64.068650,80.096050,2.972598>, <-65.700460,78.283030,6.417434>, 1)
// triangle color 1 nodes: [30, 41, 15]
make_triangle(<-65.700460,78.283030,6.417434>, <-64.068650,80.096050,2.972598>, <-67.010640,83.667600,2.343028>, 1)
// triangle color 1 nodes: [30, 15, 13]
make_triangle(<-65.700460,78.283030,6.417434>, <-67.010640,83.667600,2.343028>, <-67.849620,81.574060,5.275556>, 1)
// triangle color 1 nodes: [13, 15, 2]
make_triangle(<-67.849620,81.574060,5.275556>, <-67.010640,83.667600,2.343028>, <-68.000000,85.000000,2.000000>, 1)
// triangle color 1 nodes: [13, 2, 1]
make_triangle(<-67.849620,81.574060,5.275556>, <-68.000000,85.000000,2.000000>, <-69.000000,84.000000,4.000000>, 1)
// WriteDislocation([1, (12, 20, 35, 19, 9, 18, 16, 17, 38, 11, 12)])
// segment connecting nodes 20 and 12 with color 1
make_dislocation(<10.299910,6.717279,4.017185>, <13.000000,3.000000,3.000000>, 1, 1)
// segment connecting nodes 35 and 20 with color 1
make_dislocation(<6.771436,10.844250,4.615684>, <10.299910,6.717279,4.017185>, 1, 0)
// segment connecting nodes 19 and 35 with color 1
make_dislocation(<4.518838,12.498000,4.016833>, <6.771436,10.844250,4.615684>, 1, 0)
// segment connecting nodes 9 and 19 with color 1
make_dislocation(<3.000000,13.000000,3.000000>, <4.518838,12.498000,4.016833>, 1, 0)
// segment connecting nodes 18 and 9 with color 1
make_dislocation(<2.611213,12.362740,1.973952>, <3.000000,13.000000,3.000000>, 1, 0)
// segment connecting nodes 16 and 18 with color 1
make_dislocation(<3.130793,9.869207,0.000000>, <2.611213,12.362740,1.973952>, 1, 0)
// segment connecting nodes 17 and 16 with color 1
make_dislocation(<6.164875,5.324576,1.510549>, <3.130793,9.869207,0.000000>, 1, 0)
// segment connecting nodes 38 and 17 with color 1
make_dislocation(<8.120429,2.933554,1.946017>, <6.164875,5.324576,1.510549>, 1, 0)
// segment connecting nodes 11 and 38 with color 1
make_dislocation(<10.181070,0.000000,2.818928>, <8.120429,2.933554,1.946017>, 1, 0)
// segment connecting nodes 12 and 11 with color 1
make_dislocation(<13.000000,3.000000,3.000000>, <10.181070,0.000000,2.818928>, 1, 0)
// WriteDislocation([3, (16, 7, 8, 11)])
// segment connecting nodes 7 and 16 with color 3
make_dislocation(<0.000000,13.000000,0.000000>, <3.130793,9.869207,0.000000>, 3, 1)
// segment connecting nodes 8 and 7 with color 3
make_dislocation(<0.000000,0.000000,13.000000>, <0.000000,13.000000,0.000000>, 3, 0)
// segment connecting nodes 11 and 8 with color 3
make_dislocation(<10.181070,0.000000,2.818928>, <0.000000,0.000000,13.000000>, 3, 0)
// WriteDislocation([3, (12, 10, 7)])
// segment connecting nodes 10 and 12 with color 3
make_dislocation(<13.000000,13.000000,13.000000>, <13.000000,3.000000,3.000000>, 3, 1)
// segment connecting nodes 7 and 10 with color 3
make_dislocation(<0.000000,13.000000,0.000000>, <13.000000,13.000000,13.000000>, 3, 0)
// WriteDislocation([3, (10, 8)])
// segment connecting nodes 8 and 10 with color 3
make_dislocation(<0.000000,0.000000,13.000000>, <13.000000,13.000000,13.000000>, 3, 1)
