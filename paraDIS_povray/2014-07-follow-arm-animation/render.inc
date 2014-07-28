#include "colors.inc"
#include "textures.inc"
#include "strings.inc"
#include "math.inc"
/*  #include "stereocam.inc" */

/*  ============================================================== */
#macro ColorFromBurgers(burgers)
/* good 11-color map from http:/* colorbrewer2.org   */
  #warning concat("Got burgers value: ",str(burgers,0,0),"\n")
*/
#switch(burgers)
#range (-5,-1)											
rgb <141,211,199>/255.0
#break
#case (0)
rgb <255,255,179>/255.0
#break
#case (10)
rgb <190,186,218>/255.0
#break
#case (11)
rgb <251,128,114>/255.0
#break
#case (12)
rgb <128,177,211>/255.0
#break
#case (13)
rgb <253,180,98>/255.0
#break
#range (20,29)
rgb <179,222,105>/255.0
#break
#range (30,39)
rgb <252,205,229>/255.0
#break
#range (40,49)
rgb <217,217,217>/255.0
#break
#case (50)
rgb <188,128,189>/255.0
#break
#case (60)
rgb <204,235,197>/255.0
#break
#else
#error concat("Got unexpected burgers value: ",str(burgers,0,0),"\n")
rgb <1.0,1.0,1.0>
#break
#end 
#end

/* =========================================================== */
#declare shiny = 1; 
#macro textureFromBurgers(burger)
  texture {
	pigment  { 
	  color ColorFromBurgers(burger)
	}
  }
  #if (shiny)
	/* overdriven green sparkle texture: 
  texture {
  pigment {
  granite       /*  pre-defined in textures.inc */
	/*  turbulence 0.3 */
	color_map {
	  [0.0 color rgbf <1,1,1,1>]	    
	  [0.5 color rgbf <1,1,1,1>]	    
	  [0.5 color rgbf <0,2,0,0>]
	  [1.0 color rgbf <0,2,0,0>]
	}
	scale 1        /*  scale by the same amount in all */
	/*  directions */
  }
	/* normal { bumps 2 scale 0.5 } */
  } */
  #end /*  shiny */
#end /* textureFromBurgers(burger) */ 

/* =========================================================== */
/* =========================================================== */
/* making a segment
/*  armDistance is how far from the arm's first endpoint we are from 0.0-1.0 */
/*  metaarmDistance is analogous but for metaArm from 0.0-1.0 */
/*  ep0 and ep1 are locations (3 element vectors) */
*/

#ifndef (CylinderRadius) 
  #declare CylinderRadius = 60; 
#end

#macro segment4(ep0,ep1,burger, shiny)
cylinder { ep0,ep1, CylinderRadius
  textureFromBurgers(burger)
}
#end /* macro Segment */
  
#macro segment(segid, armid, maid, ep0,ep1,burger)
cylinder { ep0,ep1, CylinderRadius
  textureFromBurgers(burger)
}
  sphere { ep0, CylinderRadius
	textureFromBurgers(burger) 
  }
  sphere { ep1, CylinderRadius
	textureFromBurgers(burger) 
  }
#end /* macro Segment */
  
  
/* cylinder { <7.316932e+03,8.636240e+03,4.983278e+03>, <7.307937e+03,8.640321e+03,4.951756e+03>, radius00 pigment { color cylindercolor00 transmit transmit00 } finish { finish00 } } */
/* cylinder { <loc00,loc01,loc02>, <loc10,loc11,loc12>, myradius 
    pigment  { color makeintcolor(burger) transmit 0 } 
  finish { dullfinish } 
  } */

  
/* ================================================================ */
/* Example:  makeMetaArm(5.0, 5, 12, array[2] { <1,1,2>, <3,4,5> }) */
#macro makeMetaArm(marmID, mArmType, burgers, points)
#declare shiny=0;
  #declare last = dimension_size(points,1); 
  #declare index = 0; 
  #while (index < last - 1)
	/*  #if (last > 0)
  #declare index = 0; */
	#declare current = points[index]; 
	#declare index = index + 1; 
	#declare nextpoint = points[index];
	/* #warning concat("current point: ",VStr(current),", nextpoint: ",VStr(nextpoint),"\n") */ 
	#if (VEq(current,nextpoint))		
	  #warning "WRAP detected\n"
	#else
	  segment4(current, nextpoint, burgers, 0)
	  #warning "segment detected\n"
	#end
  #end
#end
  
/* ===================================================== */
/* test drawing an arm using a spline supplied from rsxxx.pov */ 
#if (0) 
  #declare c=0;
  #while (c<=1)
	sphere { ParaDIS_MetaArmSplines[0](c) 150 pigment { rgb<c,0,1-c> }}
	#declare c=c+0.0001;
  #end
#end

/* ===================================================== */
/* test drawing an arm using a sweep supplied from rsxxx.pov */ 
#if (0) 
  #declare sweepnum = 0; 
  #while (sweepnum < ParaDIS_NumMetaArms)
	object {
	  ParaDIS_MetaArmSweeps[sweepnum]
	  textureFromBurgers(ParaDIS_MetaArmBurgers[sweepnum])
	}
	#declare sweepnum = sweepnum + 1;
  #end
#end

/* ===================================================== */
/* test drawing arms using segments from rsxxx.pov */ 
#if (0) 
  #declare sweepnum = 0; 
  #while (sweepnum < ParaDIS_NumMetaArms)
	object {
	  ParaDIS_MetaArmSweeps[sweepnum]
	  textureFromBurgers(ParaDIS_MetaArmBurgers[sweepnum])
	}
	#declare sweepnum = sweepnum + 1;
  #end
#end

/* ===================================================== */
/* test drawing an object including two sphere sweeps  */ 
#if (0)
  union {
	sphere_sweep { linear_spline 
	  4 
	  <-8522.228516, -2723.805420, 3733.772705>, 30
	  <-8574.898438, -2820.153076, 3822.468262>, 30
	  <-8659.490234, -2968.449219, 3910.256348>, 30
	  <-8552.521484, -3020.665527, 3989.028076>, 30
	}
	sphere_sweep { linear_spline 
	  5 
	  <-8099.788574, -3396.696533, 4420.531738>, 30
	  <-8021.464844, -3481.914551, 4504.170410>, 30
	  <-7959.227051, -3552.609863, 4569.908203>, 30
	  <-7877.412598, -3651.507568, 4653.834473>, 30
	  <-7799.574707, -3753.822266, 4736.366211>, 30
	}
	pigment { rgb<1,0,0> }
  }
#end

/* ================================================================ */

#declare myfinish = finish { phong .5 }
#declare dullfinish = finish { ambient .0 }
#declare brightfinish = finish { phong .5 ambient 0.5 }	

/* #declare dullfinish = finish { phong 0.2 }   */
/* #declare brightfinish = finish { phong 0.7 }  */

#declare finish00 = dullfinish
#declare finish01 = dullfinish
#declare finish02 = dullfinish
#declare finish03 = dullfinish
#declare finish04 = dullfinish
#declare finish05 = dullfinish
#declare finish06 = dullfinish
#declare finish07 = dullfinish
#declare finish08 = dullfinish


global_settings {
  ambient_light White
    /* max_trace_level 15 */
}

/*  background { color rgb 0.3*<0.8,0.8,1> } */
background { 0.2*(0.333*Yellow + Brown) }


/* ========================================================= */
/* CAMERA CAMERA CAMERA */

/* ========================================================= */
/* WORKING (DUPLICATES MEIJIE'S VIEW) */

#declare ParaDIS_Size = ParaDIS_Bounds[1] - ParaDIS_Bounds[0]; 
#declare ParaDIS_Center = ParaDIS_Bounds[0] + 0.5 * ParaDIS_Size;

#declare L=8750;
#declare LL = 2*L;
#declare LLL = 3.5*L;
#declare zero = 0.01;
#declare near = L/2;
#declare cameraLocSpline =
spline {
  natural_spline
    /*  Zoom in */
    0.000,  <LLL, zero, 0.1>
    0.167,  <LL, zero, 0.1>
    0.333,  <LLL, zero, 0.1> 
    
    /*  Waggle */
    0.417,  <zero, zero, LLL>
    0.500,   <LLL, zero, 0.1> 
    0.583,  <zero, zero, LLL> 
    0.667,  <LLL, zero, near> 
    
    /*  Flythrough */
    0.833, <zero, 0.1, zero>
    1, <0.1, LLL, 0.1>
}

#declare cameraLookatSpline =
spline {
    natural_spline
    /*  Zoom in */
    0.000,  <zero, zero, 0>
    /*  Waggle */
    0.583,  <zero, zero, 0>
    0.667,  <zero, zero, 0>
    /*  Flythrough */
    0.833,  -L
    1.000,  <zero, zero, 0>
}

#ifndef (cameraSpline)
  #declare cameraSpline = 0.0; 
#end
/*  DEBUGGING BY SHOWING THE CAMERA PATH FOR THE MOVIE */
#ifdef (camPath)
/*  Draw the camera path, camera vectors, and look at points. */
#declare cameraPos = <zero, zero, 2*ff>; /*  view from Z */
#declare cameraPos = <zero, 2*ff, zero>; /*  view from Y */
#declasre cameraPos = <zero, 2*ff, zero>; /*   view from X */
#declare cameraPos = 2*ff; /*   view from ZYX corner */
#declare cameraLookAt = zero;
#declare c=0;
#while (c<=1)
sphere { cameraLocSpline(c) 500 pigment { rgb<c,1,1> }}
    #declare c=c+0.005;
#end
/*
#declare c=0;
#while (c<=1)
    // sphere { cameraLocSpline(c) 1100 pigment { Red }} 
    #declare p = cameraLocSpline(c);
    #declare l = cameraLookatSpline(c);
    #declare d=l-p;
    #declare d=vnormalize(d);
    cylinder { p p+d*20000 1100 pigment { Red*c } }
    #declare c=c+0.10;
    sphere { l 1100 pigment { Green*c }}
#end
*/
#else /*  not (camPath) */
   #declare cameraPos = cameraLocSpline(cameraSpline);
   #declare cameraLookAt = cameraLookatSpline(cameraSpline);

  #declare cameraPos = ParaDIS_Center + <0,0,1.6*ParaDIS_Size.z> ;
  #declare cameraLookAt = ParaDIS_Center; 
#end

#macro wormAxes (axisLocation, axisSize, axisDiameter) 
/*  red/cyan is X */
cylinder {axisLocation, axisLocation+x*axisSize, axisDiameter pigment {color Red  } finish { brightfinish }}
cylinder {axisLocation-x*axisSize, axisLocation, axisDiameter pigment {color White-Red } finish { brightfinish }}
/*  white/black is Y */
cylinder {axisLocation, axisLocation+y*axisSize, axisDiameter pigment {color White} finish { brightfinish }}
cylinder {axisLocation-y*axisSize, axisLocation, axisDiameter pigment {color Black} finish { brightfinish }}
/*  blue/yellow is Z */
cylinder {axisLocation, axisLocation+z*axisSize, axisDiameter pigment {color Blue } finish { brightfinish }}
cylinder {axisLocation-z*axisSize, axisLocation, axisDiameter pigment {color White-Blue } finish { brightfinish }}
#end
#ifdef (axes)
/*  place a large sphere at the primary axis center */
/*sphere {cameraLookAt, 500   
    pigment {color Orange } finish { brightfinish }
    }*/
sphere {zero, 500   
    pigment {color Red } finish { brightfinish }
}
wormAxes (zero, L, 100)
/*  make secondary axes */
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

#end /* ifdef(axes) */


/* get our bearings by putting an axis marker at the look-at point */
/* wormAxes (<zero,zero,zero>, L, 500) */

camera {     
  /*   orthographic */
  location cameraPos
    look_at cameraLookAt
    up <0,0,1>
    /* right -x*1.33 */
    /* sky <-1,0,0> */
    /* angle cameraAngle  */
    /* up <0,-1,0> */
    }

 /* "camera light" ("behind" the camera) */
#declare cameraLightPos = cameraPos + 0.1 * (cameraPos - cameraLookAt);
light_source {
    cameraPos
      color 1
  /* rotate -rot03 */
  /* rotate -rot02 */
  /* rotate -rot01 */
  /* rotate y*(1-cameraSpline)*90 */
  /* shadowless */
}

/*  "remote light"  */
/* #declare lightPos = cameraLookAt+<2000,-5000,50000>; */
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

/* camera {     
#declare debugCamera = <000,000,12000>;
 orthographic
  location debugCamera
   look_at LookAt
    // right -x*1.33 
    // sky <-1,0,0> 
    // up <0,-1,0> 
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

/* camera: */
#declare sphereSize = 100;
/*sphere {
  cameraPos, sphereSize
    pigment {     color Blue } finish { brightfinish }
    }
*/
/* which way is up to the camera? */
/* cylinder {cameraPos, cameraPos+y*axisSize, axisSize/15 pigment { color Blue  } finish { brightfinish }} */

/* light: */
/*sphere {
  lightPos, sphereSize
    pigment {  color White } finish { brightfinish }
    }
*/
/* look-at: */
/*sphere {
  LookAt, sphereSize
    pigment {     color Red }    }
*/

#declare boxcolor=rgb <0.9,0.9,0.9>;
#ifndef (BoundsRadius)
  #declare BoundsRadius = 200;
#end
#macro setbounds(x0, x1, y0, y1, z0, z1)  
  #declare boundsTexture = texture { pigment { color boxcolor } }
  
  cylinder { <x0,y0,z0>, <x1,y0,z0>, BoundsRadius texture { boundsTexture } }
  cylinder { <x0,y0,z1>, <x1,y0,z1>, BoundsRadius texture { boundsTexture } }
  cylinder { <x0,y1,z0>, <x1,y1,z0>, BoundsRadius texture { boundsTexture } }
  cylinder { <x0,y1,z1>, <x1,y1,z1>, BoundsRadius texture { boundsTexture } }
  
  cylinder { <x0,y0,z0>, <x0,y1,z0>, BoundsRadius texture { boundsTexture } }
  cylinder { <x0,y0,z1>, <x0,y1,z1>, BoundsRadius texture { boundsTexture } }
  cylinder { <x1,y0,z0>, <x1,y1,z0>, BoundsRadius texture { boundsTexture } }
  cylinder { <x1,y0,z1>, <x1,y1,z1>, BoundsRadius texture { boundsTexture } }
  
  cylinder { <x0,y0,z0>, <x0,y0,z1>, BoundsRadius texture { boundsTexture } }
  cylinder { <x0,y1,z0>, <x0,y1,z1>, BoundsRadius texture { boundsTexture } }
  cylinder { <x1,y0,z0>, <x1,y0,z1>, BoundsRadius texture { boundsTexture } }
  cylinder { <x1,y1,z0>, <x1,y1,z1>, BoundsRadius texture { boundsTexture } }
#end
setbounds(ParaDIS_Bounds[0].x, ParaDIS_Bounds[1].x, ParaDIS_Bounds[0].y, ParaDIS_Bounds[1].y, ParaDIS_Bounds[0].z, ParaDIS_Bounds[1].z)

#ifdef (box)
box {
  -10*L, 10*L
      pigment { color rgbf .2 }
#ifdef (media)
  hollow
    interior {
    media {
    /* green */
      scattering {1, color rgb 0.00001*<0.5, 1.0, 0.5> extinction 0}

      /*   gray */
      /* scattering {1, color rgb<0.000025,0.000025,0.000025> extinction 0} */
      method 3
	intervals 1
	jitter 1.0
        }
  }
#end
}
#end

/* taken right out of TSB:  */
/*
 #declare cameraPos = < 3455.9, -8267.48, -13395.4 > ;
 camera {     
 location cameraPos
 look_at LookAt
 angle 0.087/3.1415*360
 // added to switch handedness:
 right -x*1.33
 sky <0,-1,0>
 }
*/
/* ========================================================= */



/*  draw three "walls" behind the data for aesthetics */
#declare dL=L/1000; 
#declare BackDrop = union {
    box { <-L-dL,-L,-L>,<-L,L,L> }
    box { <-L,-L-dL,-L>,<L,-L,L> }
    box { <-L,-L,-L-dL>,<L,L,-L> }
    
    /* pigment {color rgb<1,1,0.5>}  */
    /* pigment {checker rgb<1,1,0.5> rgb<0.9,0.9,0.8> transmit 0 scale 2000}  */
    /*
    pigment {
        hexagon rgb<.8,1,.8> rgb<.8,.8,1> Gray50 scale 2000
        rotate <45,45,0>
    } */
/*  pigment { boxcolor } */
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
   
/*  this is provided to give you something to set AdditionItems01 to if needed */
#declare invisibleSphere = sphere {
<1000000,0,0>, 1
   pigment { color <0,0,0> }
};

#declare AdditionItems01 = invisibleSphere; 

#ifdef (fog)
fog {
    /* distance 55000 */
    distance 30000  /*  For stereo */
    color Gray30
}
#end


/* 
 =================================================================
 End render.inc file contents
 =================================================================
*/

