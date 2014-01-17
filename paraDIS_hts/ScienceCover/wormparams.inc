#include "colors.inc"
#include "textures.inc"
//#include "stereocam.inc"
#macro makeintcolor(intcolor)
#switch(burger)
#case (0)
color00
#break
#case (1)
color01
#break
#case (2)
color02
#break
#case (3)
color03
#break
#case (4)
color04
#break
#case (5)
color05
#break
#case (6)
color06
#break
#case (7)
color07
#break
#case (8)
color08
#break
#case (9)
color09
#break
#end 
#end

#macro make_endpoint_color(floatvalue,burger)
#local maxval = 500.0; 
#if (floatvalue > maxval) 
#local normalized = 1.0;
#else
#local normalized = floatvalue / maxval;
#end


//#if (burger < 4)
// 0=white to 1=yellow:
//color rgb 1 - <0.3*normalized, 0.3*normalized, normalized>
//#else
// 0=white to 1=dark green:
color rgb 2*(1 - <normalized, 0.6*normalized, normalized>)
  //#end
// 0=red to 1=white: 
//color rgb <1, normalized, normalized>
#end


// ===========================================================
// making a cylinder
#macro make_radius(value)
#declare myradius = 50;
#end 

#macro makecylinder(burger,weight,monstrosity,segdistance, loc00,loc01,loc02, node0monstrosity,node0distance, loc10,loc11,loc12, node1monstrosity,node1distance)
make_radius(weight);
cylinder { <loc00,loc01,loc02>, <loc10,loc11,loc12>, myradius 
    texture {
    pigment  { 
#local cyl_v=<loc10-loc00,loc11-loc01,loc12-loc02>;
      gradient cyl_v
	color_map { 
	[ 0.0 make_endpoint_color(node0distance,burger) ]
	  [ 0.5 make_endpoint_color(segdistance,burger) ]
	  [ 1.0 make_endpoint_color(node1distance,burger) ]
	  }
      scale vlength(cyl_v*1.002)
	translate <loc00, loc01, loc02>-cyl_v*0.001
	}
    /*
     #end
    */  
    // finish { dullfinish } 
  }
#ifdef(sparkle)
#if (burger < 4)
// overdriven green sparkle texture: 
  texture {
       pigment {
        granite       // pre-defined in textures.inc
	  // turbulence 0.3
       color_map {
          [0.0 color rgbf <1,1,1,1>]	    
	     [0.5 color rgbf <1,1,1,1>]	    
	    //[0.6 color rgbf <0.2, 0.1, 0.0, 0>]
	    // [0.7 color rgbf <0.2, 0.1, 0.0, 0>]	   
	    //[0.6 color rgbf <0,2,0,0>]
	    [0.5 color rgbf <0,2,0,0>]
	    [1.0 color rgbf <0,2,0,0>]
	    //	    [0.7 color rgbf <0.4, 0.2, 0.0, 0>]
	    // [1.0 color rgbf <0.4, 0.2, 0.0, 0>]
        }
          scale 1        // scale by the same amount in all
                       // directions
       }
       //normal { bumps 2 scale 0.5 }
  }
#end   // burger < 4
#end // sparkle
}
/* cylinder { <7.316932e+03,8.636240e+03,4.983278e+03>, <7.307937e+03,8.640321e+03,4.951756e+03>, radius00 pigment { color cylindercolor00 transmit transmit00 } finish { finish00 } } */
/* cylinder { <loc00,loc01,loc02>, <loc10,loc11,loc12>, myradius 
    pigment  { color makeintcolor(burger) transmit 0 } 
  finish { dullfinish } 
  } */
#end
#macro makesphere(loc0,loc1,loc2, burger,connect,nodeweight, monstrosity, nodedistance)
make_radius(nodeweight);
sphere { <loc0,loc1,loc2>,  myradius
    pigment {  make_endpoint_color(nodedistance,burger)  } 
  finish { dullfinish } 
}
/* sphere { <loc0,loc1,loc2>, sphere_radius00 
  pigment { color spherecolor00 transmit transmit00 } 
  finish { finish00 } 
  } */
#end

//#declare dodebug = 1; 

#ifdef (dodebug) 
#declare myradius=10; 
#else
#declare myradius=10;
#end

//#declare myfinish = finish { phong .5 metallic }
//#declare myfinish = finish { phong .5 reflection {0,0.5} }
#declare myfinish = finish { phong .5 }
#declare dullfinish = finish { ambient .0 }
#declare brightfinish = finish { phong .5 ambient 0.5 }
//#declare dullfinish = finish { phong 0.2 } 
//#declare brightfinish = finish { phong 0.7 } 
#declare finish00 = dullfinish
#declare finish01 = dullfinish
#declare finish02 = dullfinish
#declare finish03 = dullfinish
#declare finish04 = dullfinish
#declare finish05 = dullfinish
#declare finish06 = dullfinish
#declare finish07 = dullfinish
#declare finish08 = dullfinish


#ifndef (wormcolor)
#declare wormcolor = 1;
#end

#ifdef (xwormcolor) 
#declare radius00=myradius;
#declare radius01=myradius*1.5;
#declare radius02=myradius*1.5;
#declare radius03=myradius*1.5;
#declare radius04=myradius;
#declare radius05=myradius;
#declare radius06=myradius;
#declare radius07=myradius;
#declare radius08=myradius;
#else
#declare radius00=myradius;
#declare radius01=myradius;
#declare radius02=myradius;
#declare radius03=myradius;
#declare radius04=myradius;
#declare radius05=myradius;
#declare radius06=myradius;
#declare radius07=myradius;
#declare radius08=myradius;
 
#end

#declare sphere_radius00=radius00;
#declare sphere_radius01=radius01;
#declare sphere_radius02=radius02;
#declare sphere_radius03=radius03;
#declare sphere_radius04=radius04;
#declare sphere_radius05=radius05;
#declare sphere_radius06=radius06;
#declare sphere_radius07=radius07;
#declare sphere_radius08=radius08;

#declare TSBcolor00=White ;  
#declare TSBcolor01=DarkPurple ;  
#declare TSBcolor02=Blue ;  
#declare TSBcolor03=Green;
#declare TSBcolor04=Yellow; 
#declare TSBcolor05=Orange;
#declare TSBcolor06=Red; 
#declare TSBcolor07=Cyan;
#declare TSBcolor08=Magenta;

#switch (wormcolor)
 #case ( 0 )
//dark
#declare boxcolor=rgb <0.9,0.9,0.9>;

#declare color00=White ;  
#declare color01=Cyan ;  
#declare color02=Yellow;
#declare color03=Red; 
#declare color04=rgb <0.1,0.1,0.2>;
#declare color05=rgb <0.1,0.2,0.1>; 
#declare color06=rgb <0.2,0.1,0.1>;
#declare color07=rgb <0.1,0.1,0.1>;
#declare color08=Orange;   

#break

#case ( 1 ) 
//green
#declare boxcolor=rgb <0.9,0.9,0.9>;

#declare color00=DarkGreen ;  
#declare color01=Cyan ;  
#declare color02=Yellow;
#declare color03=Red; 
#declare color04=rgb <0.1,0.3,0.0>;
#declare color05=rgb <0.0,0.3,0.1>; 
#declare color06=rgb <0.1,0.3,0.1>;
#declare color07=rgb <0.2,0.4,0.2>;
#declare color08=Orange;   
#break

#case ( 2 ) 
//by monsters -- light --all
#declare boxcolor=rgb <0.3,0.3,0.3>;

//transparency takes FOREVER. 
//#declare color00=rgbf <1,1,1,0.95> ;  
#declare color00=rgb <1,1,1> ;  
#declare color01=Green ;  
#declare color02=Blue;
#declare color03=Red; 
#declare color04=rgb <0.6,0.8,0.8>;
#declare color05=rgb <0.8,0.6,0.8>; 
#declare color06=rgb <0.8,0.8,0.6>;
#declare color07=rgb <0.8,0.8,0.8>;
#declare color08=Orange;   
#break

#case ( 3 ) 
//by monsters -- light -- binary 
#declare boxcolor=rgb <0.3,0.3,0.3>;

//transparency takes FOREVER. 
//#declare color00=rgbf <1,1,1,0.95> ;  
#declare color00=RichBlue;  
#declare color01=Red ;  
#declare color02=Red;
#declare color03=Red; 
#declare color04=Red; 
#declare color05=Red; 
#declare color06=Red; 
#declare color07=Red; 
#declare color08=Orange;   
#break

#case ( 4 ) 
//warm/cold scheme for Vasily
#declare boxcolor=rgb <0.9,0.9,0.9>;

#declare color00=DarkGreen;  
#declare color01=Red ;  
#declare color02=Orange;
#declare color03=Yellow; 
#declare color04=Blue; 
#declare color05=Cyan; 
#declare color06=RichBlue; 
#declare color07=NeonBlue; 
#declare color08=Orange;   
#break

#case (5)
//light
#declare boxcolor=rgb <0.3,0.3,0.3>;

#declare color00=White ;  
#declare color01=Green ;  
#declare color02=Blue;
#declare color03=Red; 
#declare color04=rgb <1.0,0.8,0.8>;
#declare color05=rgb <0.8,1.0,0.8>; 
#declare color06=rgb <0.8,0.8,1.0>;
#declare color07=rgb <1,1,1>;
#declare color08=Orange;   
#break

#case (6)
//TSB colors
#declare boxcolor=rgb <0.7,0.7,0.7>;

#declare color00=TSBcolor00;  
#declare color01=TSBcolor01;  
#declare color02=TSBcolor02;  
#declare color03=TSBcolor03;
#declare color04=TSBcolor04; 
#declare color05=TSBcolor05;
#declare color06=TSBcolor06; 
#declare color07=TSBcolor07;
#declare color08=TSBcolor08;
#break

#case ( 7 ) 
//monster in the forest 2005-01-31
//#declare boxcolor=VeryDarkBrown;
#declare boxcolor=color red 0.30 green 0.16 blue 0.14; 
#declare color00=DarkGreen ;  
#declare color01=DarkPurple ;  
#declare color02=Blue ;  
#declare color03=Green;
#declare color04=Yellow; 
#declare color05=Orange;
#declare color06=Red; 
#declare color07=Cyan;
#declare color08=Magenta;
#break

#case ( 8 ) 
//monster in the forest 2005-02-07 with light wall
//#declare boxcolor=VeryDarkBrown;
#declare boxcolor=color Gray70; 
#declare color00=DarkGreen ;  
#declare color01=DarkPurple ;  
#declare color02=Blue ;  
#declare color03=Green;
#declare color04=Yellow; 
#declare color05=Orange;
#declare color06=Red; 
#declare color07=Cyan;
#declare color08=Magenta;
#break


#end


#declare spherecolor00=color00;
#declare spherecolor01=color01;
#declare spherecolor02=color02;
#declare spherecolor03=color03;
#declare spherecolor04=color04;
#declare spherecolor05=color05;
#declare spherecolor06=color06;
#declare spherecolor07=color07;
#declare spherecolor08=color08;

#declare cylindercolor00=spherecolor00;
#declare cylindercolor01=spherecolor01;
#declare cylindercolor02=spherecolor02;
#declare cylindercolor03=spherecolor03;
#declare cylindercolor04=spherecolor04;
#declare cylindercolor05=spherecolor05;
#declare cylindercolor06=spherecolor06;
#declare cylindercolor07=spherecolor07;
#declare cylindercolor08=spherecolor08;

#declare mytransmit=0;
#declare transmit00=mytransmit;
#declare transmit01=mytransmit;
#declare transmit02=mytransmit;
#declare transmit03=mytransmit;
#declare transmit04=mytransmit;
#declare transmit05=mytransmit;
#declare transmit06=mytransmit;
#declare transmit07=mytransmit;
#declare transmit08=mytransmit;

global_settings {
  ambient_light White
    //max_trace_level 15
}

//  background { color rgb 0.3*<0.8,0.8,1> }
background { 0.2*(0.333*Yellow + Brown) }




//=========================================================
//CAMERA CAMERA CAMERA

//=========================================================
//WORKING (DUPLICATES MEIJIE'S VIEW)

// 1m1b2 stuff:
//Camera (0.1612442284822464, -0.38573053479194641, -0.62498784065246582, -0.73600053787231445, 0.54413539171218872, -0.40276536345481873, 3.6171700954437256, 0.42538520693778992, 0.0, 0.087782956659793854)
//#declare cameraLookAt = <650,-1500,-8200>;
//#declare cameraPos = cameraLookAt + <2000, -4000,-600>;
/*
// 1m2b stuff:
//Camera (-0.039836626499891281, 0.013592414557933807, -0.43755528330802917, -0.52702057361602783, 0.83568418025970459, 0.15453577041625977, 3.4207336902618408, 0.050154335796833038, 0.0, 0.39219698309898376)
#declare LookAt = <-507, 167,-8553>;
#declare cameraPos = <-853, 291, -9378>;
#declare cameraAngle = 23;
*/

/*
#ifdef (lightX) 
#ifdef (lightY)
#ifdef (lightZ)
#declare lightset = 1;
#declare lightPos = LookAt+100000*<lightX, lightY, lightZ>;
#end
#end
#end
#ifndef (lightset)
#declare lightPos = LookAt + 100000*<1,-1,-1>;
#end
*/
#declare L=8750;
#declare LL = 2*L;
#declare LLL = 3*L;
#declare zero = 0.01;
#declare near = L/2;
#declare cameraLocSpline =
spline {
  natural_spline
    // Zoom in
    0.000,  <LLL, zero, 0.1>
    0.167,  <LL, zero, 0.1>
    0.333,  <LLL, zero, 0.1> 
    
    // Waggle
    0.417,  <zero, zero, LLL>
    0.500,   <LLL, zero, 0.1> 
    0.583,  <zero, zero, LLL> 
    0.667,  <LLL, zero, near> 
    
    // Flythrough
    0.833, <zero, 0.1, zero>
    1, <0.1, LLL, 0.1>
}

#declare cameraLookatSpline =
spline {
    natural_spline
    // Zoom in
    0.000,  <zero, zero, 0>
    // Waggle
    0.583,  <zero, zero, 0>
    0.667,  <zero, zero, 0>
    // Flythrough
    0.833,  -L
    1.000,  <zero, zero, 0>
}

#ifndef (cameraSpline)
  #declare cameraSpline = 0.0; 
#end

// DEBUGGING BY SHOWING THE CAMERA PATH FOR THE MOVIE
#ifdef (camPath)
// Draw the camera path, camera vectors, and look at points.
#declare cameraPos = <zero, zero, 2*ff>; // view from Z
#declare cameraPos = <zero, 2*ff, zero>; // view from Y
#declasre cameraPos = <zero, 2*ff, zero>; //  view from X
#declare cameraPos = 2*ff; //  view from ZYX corner
#declare cameraLookAt = zero;
#declare c=0;
#while (c<=1)
sphere { cameraLocSpline(c) 500 pigment { rgb<c,1,1> }}
    #declare c=c+0.005;
#end
/*
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
*/
#else // not (camPath)
   #declare cameraPos = cameraLocSpline(cameraSpline);
   #declare cameraLookAt = cameraLookatSpline(cameraSpline);
//#declare cameraPos = 1.8*L ;
//#declare cameraLookAt = <0,10,0>;
#end

#macro wormAxes (axisLocation, axisSize, axisDiameter) 
// red/cyan is X
cylinder {axisLocation, axisLocation+x*axisSize, axisDiameter pigment {color Red  } finish { brightfinish }}
cylinder {axisLocation-x*axisSize, axisLocation, axisDiameter pigment {color White-Red } finish { brightfinish }}
// white/black is Y
cylinder {axisLocation, axisLocation+y*axisSize, axisDiameter pigment {color White} finish { brightfinish }}
cylinder {axisLocation-y*axisSize, axisLocation, axisDiameter pigment {color Black} finish { brightfinish }}
// blue/yellow is Z
cylinder {axisLocation, axisLocation+z*axisSize, axisDiameter pigment {color Blue } finish { brightfinish }}
cylinder {axisLocation-z*axisSize, axisLocation, axisDiameter pigment {color White-Blue } finish { brightfinish }}
#end
#ifdef (axes)
// place a large sphere at the primary axis center
/*sphere {cameraLookAt, 500   
    pigment {color Orange } finish { brightfinish }
    }*/
sphere {zero, 500   
    pigment {color Red } finish { brightfinish }
}
wormAxes (zero, L, 100)
// make secondary axes
#declare zz=0;
#while (zz <= L/2)
#declare yy=0;
#while (yy <= L/2)
#declare xx=0; 
#while (xx <= L/2) 
     wormAxes(<xx,yy,zz>,L/2, 100)
#declare xx=xx+L/2;
#end
#declare yy=yy+L/2;
#end
#declare zz=zz+L/2;
#end

#end //ifdef(axes)


//get our bearings by putting an axis marker at the look-at point
//wormAxes (<zero,zero,zero>, L, 500)

camera {     
  //  orthographic
  location cameraPos
    look_at cameraLookAt
    up <0,0,1>
    //right -x*1.33
    //sky <-1,0,0>
    //angle cameraAngle 
    //up <0,-1,0>
    }

 //"camera light" ("behind" the camera)
#declare cameraLightPos = cameraPos + 0.1 * (cameraPos - cameraLookAt);
light_source {
    cameraPos
      color 1
  //rotate -rot03
  //rotate -rot02
  //rotate -rot01
  //rotate y*(1-cameraSpline)*90
  //shadowless
}

// "remote light" 
//#declare lightPos = cameraLookAt+<2000,-5000,50000>;
#declare remoteLightPos = (<-L,-L,0>);
light_source {
  remoteLightPos
  color 5*White
    area_light
  L*<1, 0, 0> L*<0, 0, 1> 40 40
  circular
  adaptive 3
  jitter
  orient
}

/*camera {     
#declare debugCamera = <000,000,12000>;
 orthographic
  location debugCamera
   look_at LookAt
    //right -x*1.33
    //sky <-1,0,0>
    //up <0,-1,0>
    }
*/
/*light_source {
   debugCamera
   color White
*/
     /*
 area_light
 <L/5, 0, 0> <0, 0, L/5> 40 40
 circular
 adaptive 3
 jitter
 orient
     */
/*
}
     */

//camera:
#declare sphereSize = 100;
/*sphere {
  cameraPos, sphereSize
    pigment {     color Blue } finish { brightfinish }
    }
*/
//which way is up to the camera?
//cylinder {cameraPos, cameraPos+y*axisSize, axisSize/15 pigment { color Blue  } finish { brightfinish }}

//light:
/*sphere {
  lightPos, sphereSize
    pigment {  color White } finish { brightfinish }
    }
*/
//look-at:
/*sphere {
  LookAt, sphereSize
    pigment {     color Red }    }
*/


#ifdef (box)
box {
  -10*L, 10*L
      pigment { color rgbf .2 }
#ifdef (media)
  hollow
    interior {
    media {
    //green
      scattering {1, color rgb 0.00001*<0.5, 1.0, 0.5> extinction 0}

      //  gray
      //scattering {1, color rgb<0.000025,0.000025,0.000025> extinction 0}
      method 3
	intervals 1
	jitter 1.0
        }
  }
#end
}
#end

//taken right out of TSB: 
/*
 #declare cameraPos = < 3455.9, -8267.48, -13395.4 > ;
 camera {     
 location cameraPos
 look_at LookAt
 angle 0.087/3.1415*360
 //added to switch handedness:
 right -x*1.33
 sky <0,-1,0>
 }
*/
//=========================================================



// draw three "walls" behind the data for aesthetics
#declare dL=L/1000; 
#declare BackDrop = union {
    box { <-L-dL,-L,-L>,<-L,L,L> }
    box { <-L,-L-dL,-L>,<L,-L,L> }
    box { <-L,-L,-L-dL>,<L,L,-L> }
    
    //pigment {color rgb<1,1,0.5>} 
    //pigment {checker rgb<1,1,0.5> rgb<0.9,0.9,0.8> transmit 0 scale 2000} 
    /*
    pigment {
        hexagon rgb<.8,1,.8> rgb<.8,.8,1> Gray50 scale 2000
        rotate <45,45,0>
    } */
// pigment { boxcolor }
pigment /*{ color 0.5* (0.2*Blue + 0.8* Yellow + Tan) }*/
  { rgb 0.7 }
    normal { bumps 0.1 scale 500 }
    finish { phong 0 metallic } 
};
#ifdef (backdrop)
BackDrop
#end

/*
#declare SlicePlane = union {
    box { <dL,-L,-L>,<0,L,L> }
    box { <dL,-L,-L>,<0,L,L> rotate y*90}
    box { <dL,-L,-L>,<0,L,L> rotate z*90}
    pigment {color rgb<0.7,1,1> transmit 0.5} 
    finish { phong 1 metallic } 
};
*/
 
#declare num=1; 
   
// this is provided to give you something to set AdditionItems01 to if needed
#declare invisibleSphere = sphere {
<1000000,0,0>, 1
   pigment { color <0,0,0> }
};

#declare AdditionItems01 = invisibleSphere; 

#ifdef (fog)
fog {
    //distance 55000
    distance 30000  // For stereo
    color Gray30
}
#end
