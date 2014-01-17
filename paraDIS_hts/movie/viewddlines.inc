#include "colors.inc"
#include "stereocam.inc"

//#declare finish01 = finish { phong .5 metallic }
//#declare finish02 = finish { phong .5 metallic }
#declare finish01 = finish { phong .5 }
#declare finish02 = finish { phong .5 }
//#declare finish01 = finish { phong .5 reflection {0,0.5} }
//#declare finish02 = finish { phong .5 reflection {0,0.5} }

#declare myradius=100;
#declare highlightcolor=Red;
#declare mytransmit=0;

#declare radius00=myradius;
#declare radius01=myradius;
#declare radius02=myradius;
#declare radius03=myradius;
#declare radius04=myradius;
#declare sphere_radius01 = 100; 
#declare sphere_radius02 = 100; 
 
#declare color00=Pink;
#declare color01=White;
#declare color02=rgb<1,.4,.4>;
#declare color03=LightBlue;
#declare color04=Cyan;    

#declare transmit00=mytransmit;
#declare transmit01=mytransmit;
#declare transmit02=mytransmit;
#declare transmit03=0;
#declare transmit04=0;

global_settings {
  ambient_light White
  max_trace_level 15
}

background { White }

  
// 10mic box
#declare L=17500;
// 5mic box 
//#declare L=8750;
#declare dL=300; 
#declare BoxFrame = union {
    box { <L+dL,-L,-L>,<L,L,L> }
    box { <-L,-L-dL,-L>,<L,-L,L> }
    box { <-L,-L,L+dL>,<L,L,L> }
      /*
    union {
      cylinder {
        <0,0,0>, <1000000, 0, 0>, 1000
        pigment { color rgb<1,0,0> }
        no_shadow
        hollow
      }
      cylinder {
        <0,0,0>, <0, 1000000, 0>, 1000
        pigment { color rgb<0,1,0> }
        no_shadow
        hollow
      }
      cylinder {
        <0,0,0>, <0, 0, 1000000>, 1000
        pigment { color rgb<0,0,1> }
        no_shadow
        hollow
      }
      sphere {
        <-L, 0, 0> 1000
        pigment { color White }
      }
    }
      */
    
    //pigment {color rgb<1,1,0.5>} 
    //pigment {checker rgb<1,1,0.5> rgb<0.9,0.9,0.8> transmit 0 scale 2000} 
    /*
    pigment {
        hexagon rgb<.8,1,.8> rgb<.8,.8,1> Gray50 scale 2000
        rotate <45,45,0>
    } */
    pigment { color rgb<0.8,0.8,1> }
    normal { bumps 0.1 scale 500 }
    finish { phong 0 metallic } 
};
#declare SlicePlane = union {
    box { <dL,-L,-L>,<0,L,L> }
    box { <dL,-L,-L>,<0,L,L> rotate y*90}
    box { <dL,-L,-L>,<0,L,L> rotate z*90}
    pigment {color rgb<0.7,1,1> transmit 0.5} 
    finish { phong 1 metallic } 
};

///union {
///  }
///  BoxFrame
///  //SlicePlane         
///  no_shadow                
///  //rotate y*-45 rotate x*-20
///  rotate y*-40 rotate x*-20
///  //rotate y*-60 rotate x*-30
///}  
 
#declare num=1; 
    
//view 3d
#declare AdditionItems01 = BoxFrame; 
#declare rot01 = -y*40;
#declare rot02 = -x*20;
#declare rot03 = z*0;

//view slice
///#declare AdditionItems01 = sphere{<0,0,0>,0};
// view 100 #declare rot01 = y*90;
// view 001
//#declare rot01 = y*0;
//#declare rot02 = x*0;
//#declare rot03 = z*0;

//camera {     
//  orthographic
//  location <0, 0, -50000> //entire view
//  look_at  <-0, 0, 0>
//}

#declare ff = 35355;
#declare cameraLocSpline =
spline {
    natural_spline
    // Zoom in
    0.000,  <-100000, L, -100000>
    0.167,  <-50000, L, -50000>
    0.333,  <-50000, L/5, 0>

    // Waggle
    0.417,  <-ff, L/5, -ff>
    0.500,  <0, L/5, -50000>
    0.583,  <-ff, L/5, -ff>
    0.667,  <-50000, L/5, 0>

    // Flythrough
    0.833, <-10000, L, -10000>
    1, <0, L/5, -50000>
}

#declare cameraLookatSpline =
spline {
    natural_spline
    0.000,  <0, 0, 0>
    0.583,  <0, 0, 0>
    0.667,  <0, 0, 0>
    0.833,  <10000, -L/5, 10000>
    1.000,  <0, 0, 0>
}

#ifdef (camPath)
// Draw the camera path, camera vectors, and look at points.
#declare c=0;
#while (c<=1)
    sphere { cameraLocSpline(c) 1000 pigment { rgb<c,c,c> }}
    #declare c=c+0.001;
#end

#declare c=0;
#while (c<=1)
    //sphere { cameraLocSpline(c) 1100 pigment { Red }}
    #declare p = cameraLocSpline(c);
    #declare l = cameraLookatSpline(c);
    #declare d=l-p;
    #declare d=vnormalize(d);
    cylinder { p p+d*20000 1100 pigment { Red*c } }
    #declare c=c+0.10;
    sphere { l 1100 pigment { Green*c }}
#end
#end

//#declare cameraPos = <0, -L/5, -50000>;
#ifdef (camPath)
    #declare cameraPos = <-10000, 126666, -10000>; // top view
    #declare cameraLookat = <-10000, 0, -10000+1>;
#else
    #declare cameraPos = cameraLocSpline(clock);
    #declare cameraLookat = cameraLookatSpline(clock);
#end

#ifdef (stereo)
// Stereo camera
camera {
  FullStereoCam(cameraPos, cameraLookat, side, 1000, 60, 50000)
  //rotate -rot03
  //rotate -rot02
  //rotate -rot01
  //rotate y*(1-clock)*90
}
#else
// Normal camera
camera {     
  location cameraPos
  look_at cameraLookat
  //rotate -rot03
  //rotate -rot02
  //rotate -rot01
  //rotate y*(1-clock)*90
}
#end

light_source {
  cameraPos color 1
  //rotate -rot03
  //rotate -rot02
  //rotate -rot01
  //rotate y*(1-clock)*90
  shadowless
}

#ifdef (Shadows)
#declare light_pos = <-100000, 100000, -10000>;
#declare n = 0;
#while (n<1)
//#declare light_pos = <-L*2, 80000, -40000>;
light_source {
  light_pos
  color White/10
  /*
  area_light
  <20000, 0, 0> <0, 20000, 0> 5 5
  circular
  adaptive 2
  orient
  shadowless
  */
  rotate -y*90
  rotate y*90*n
}
#declare n = n+.1;
#end
#end

fog {
    //distance 55000
    distance 200000  // For stereo
    color Black
}
