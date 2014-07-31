#include "colors.inc"
#include "textures.inc"
#include "strings.inc"
#include "math.inc"
#include "finish.inc"
/*  #include "stereocam.inc" */

global_settings {
  ambient_light White
    /* max_trace_level 15 */
  #ifdef (Radiosity)
	radiosity {}
  #end
}

/*  background { color rgb 0.3*<0.8,0.8,1> } */
background { 0.2*(0.333*Yellow + Brown) }


#ifndef (AnimateFuse)
  #declare AnimateFuse = false; 
#end
#ifndef (AnimateGrow)
  #declare AnimateGrow = false; 
#end

#if (AnimateGrow & AnimateFuse)
  #warning "AnimateGrow and AnimateFuse are contradictory.  I will set AnimateGrow to false and use AnimateFuse\n"
#end
#ifndef (GlowRadius) 
  #declare GlowRadius = 1;
#end
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
#declare glowing = false;
#declare afterglow = false; 

#macro textureFromBurgers(burger)
  #if (!AnimateFuse & !AnimateGrow) 
	  texture {	
		pigment  { 
		  color ColorFromBurgers(burger)
		}
		finish { ambient color ColorFromBurgers(burger) * .2 }
	  }	
  #else
	#if (!glowing & !afterglow) 
	  texture {	
		pigment  { 
		  color ColorFromBurgers(burger) * 0.3
		}
	  }
	#end
	#if (glowing)
	  /* #warning "GLOWING!" */
	  texture {	
		pigment  { 
		  color ColorFromBurgers(burger) 
		}
		finish { ambient color ColorFromBurgers(burger) * 3 }
	  }
	  /* overdriven green sparkle texture: */
	  /* texture { pigment { granite  color_map {
  [0.0 color rgbf 1 ]	    
  [0.5 color rgbf 1 ]	    
  [0.5 color rgbf <0,1,0,0>]
  [1.0 color rgbf <0,1,0,0>]
  } scale 1   color White * 0.5 }
  finish { ambient color White }
  /* normal { bumps 2 scale 0.5 } */
	  }
	  */
	#end
	#if (afterglow) 
	  /* #warning "AFTERGLOW!" */
	  texture {	
		pigment  { 
		  color ColorFromBurgers(burger) 
		}
		finish { ambient color ColorFromBurgers(burger)  * 0.4 }
		/* finish { ambient color Red * 0.3 } */
	  }
	#end /* afterglow */ 
  #end /* animating segments */ 
#end /* textureFromBurgers(burger) */ 

/* =========================================================== */
/* =========================================================== */
/* making a segment
/*  armDistance is how far from the arm's first endpoint we are from 0.0-1.0 */
/*  metaarmDistance is analogous but for metaArm from 0.0-1.0 */
/*  ep0 and ep1 are locations (3 element vectors) */
*/

#ifndef (SegmentRadius) 
  #declare SegmentRadius = 60; 
#end

#macro segment4(ep0,ep1,burger, glowing)
  cylinder { ep0,ep1, SegmentRadius
	textureFromBurgers(burger)
  }
#end /* macro Segment */

#ifndef (SegmentDistance) 
  #declare SegmentDistance = 0;
#end
#if (AnimateFuse | AnimateGrow)
  #if (!SegmentDistance) 
	#declare SegmentDistance = ParaDIS_MaxLightTheFuseDistance;
  #end
#else
  #declare SegmentDistance = 0;
#end
#warning concat("SegmentDistance=",str(SegmentDistance,0,0),"\n")
#warning concat("GlowRadius=",str(GlowRadius,0,0),"\n")
#declare segmentsRendered = 0; 
#macro segmentGrowingAnimation(segid, armid, maid, ep0,ep1,burger, dist)  
  #if (SegmentDistance = 0 | ( dist > 0 & dist < SegmentDistance) )
	cylinder { ep0,ep1, SegmentRadius
	  textureFromBurgers(burger) 
	}
	sphere { ep0, SegmentRadius
	  textureFromBurgers(burger)  
	}
	sphere { ep1, SegmentRadius
	  textureFromBurgers(burger)  
	}
	#declare segmentsRendered = segmentsRendered + 1; 
	/* #warning concat("Rendering segment ",str(segmentsRendered,0,0)," with ID ",str(segid,0,0)," and distance ",str(dist,0,0)) */
  #end
#end /* macro Segment */

#macro segmentFuseAnimation(segid, armid, maid, ep0,ep1,burger, dist)  
  #declare glowing = 0;
  #declare afterglow = 0; /* the "afterglow" */
  /* #warning concat("dist is ",str(dist,0,0)," and SegmentDistance is ",str(SegmentDistance,0,0)) */
  #if (dist > 0 & dist > (SegmentDistance - GlowRadius) & dist < (SegmentDistance + GlowRadius) )
	#declare glowing=1;
	#local MyRadius = SegmentRadius; 
	/* #warning concat("GLOWING: fuse color segment ",str(segmentsRendered,0,0)," with ID ",str(segid,0,0)," and distance ",str(dist,0,0)) */
  #else
	#local MyRadius = SegmentRadius; 
	#if ( dist > 0 & dist <= SegmentDistance - GlowRadius )
	/* #warning concat("AFTERGLOW: fuse color segment ",str(segmentsRendered,0,0)," with ID ",str(segid,0,0)," and distance ",str(dist,0,0))  */
	  #declare afterglow = 1;
	#end
  #end
  cylinder { ep0,ep1, MyRadius
	textureFromBurgers(burger)
  }
  sphere { ep0, MyRadius
	textureFromBurgers(burger) 
  }
  sphere { ep1, MyRadius
	textureFromBurgers(burger) 
  }
  #declare segmentsRendered = segmentsRendered + 1; 
  /* #warning concat("Rendering segment ",str(segmentsRendered,0,0)," with ID ",str(segid,0,0)," and distance ",str(dist,0,0)) */
#end /* macro Segment */
  

#macro segment(segid, armid, maid, ep0,ep1,burger, dist)  
  #if (AnimateGrow)
	segmentGrowingAnimation(segid, armid, maid, ep0,ep1,burger, dist)  	
  #else
	#if (AnimateFuse)
	  segmentFuseAnimation(segid, armid, maid, ep0,ep1,burger, dist)  
	#else
	  segmentFuseAnimation(segid, armid, maid, ep0,ep1,burger, 0)
	#end
  #end
#end
/* cylinder { <7.316932e+03,8.636240e+03,4.983278e+03>, <7.307937e+03,8.640321e+03,4.951756e+03>, radius00 pigment { color cylindercolor00 transmit transmit00 } finish { finish00 } } */
/* cylinder { <loc00,loc01,loc02>, <loc10,loc11,loc12>, myradius 
    pigment  { color makeintcolor(burger) transmit 0 } 
  finish { dullfinish } 
  } */

  
/* ================================================================ */
/* Example:  makeMetaArm(5.0, 5, 12, array[2] { <1,1,2>, <3,4,5> }) */
#macro makeMetaArm(marmID, mArmType, burgers, points)
#declare glowing=0;
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



/* ========================================================= */
/* CAMERA CAMERA CAMERA */

/* ========================================================= */
/* WORKING (DUPLICATES MEIJIE'S VIEW) */

#declare ParaDIS_Size = ParaDIS_Bounds[1] - ParaDIS_Bounds[0]; 
#declare ParaDIS_Center = ParaDIS_Bounds[0] + 0.5 * ParaDIS_Size;

#declare zero = 0.01;
#declare near = ParaDIS_Bounds[1].x/2;
#declare cameraLocSpline =
spline {
  natural_spline
    /*  Zoom in */
    0.000,  <3*ParaDIS_Bounds[1].x, ParaDIS_Center.y, 0.1>
    0.167,  <2*ParaDIS_Bounds[1].x, ParaDIS_Center.y, 0.1>
    0.333,  <3*ParaDIS_Bounds[1].x, ParaDIS_Center.y, 0.1> 
    
    /*  Waggle */
    0.417,  <ParaDIS_Center.x,  ParaDIS_Center.y, 3*ParaDIS_Bounds[1].x>
    0.500,   <3*ParaDIS_Bounds[1].x, ParaDIS_Center.y, 0.1> 
    0.583,  <ParaDIS_Center.x,  ParaDIS_Center.y, 3*ParaDIS_Bounds[1].x> 
    0.667,  <3*ParaDIS_Bounds[1].x, ParaDIS_Center.y, near> 
    
    /*  Flythrough */
    0.833, <ParaDIS_Center.x,  0.1, ParaDIS_Center.z>
    1, <0.1, 3*ParaDIS_Bounds[1].x, 0.1>
}

#declare cameraLookatSpline =
spline {
    natural_spline
    /*  Zoom in */
    0.000,  <ParaDIS_Center.x,  ParaDIS_Center.y, 0>
    /*  Waggle */
    0.583,  <ParaDIS_Center.x,  ParaDIS_Center.y, 0>
    0.667,  <ParaDIS_Center.x,  ParaDIS_Center.y, 0>
    /*  Flythrough */
    0.833,  -ParaDIS_Bounds[1].x
    1.000,  <ParaDIS_Center.x,  ParaDIS_Center.y, 0>
}

/* ========================================================== */
#ifndef (cameraSpline)
  #declare cameraSpline = 0.0; 
#end
/*  DEBUGGING BY SHOWING THE CAMERA PATH FOR THE MOVIE */
#ifdef (camPath)
  /*  Draw the camera path, camera vectors, and look at points. */
  #declare cameraPos = <ParaDIS_Center.x,  ParaDIS_Center.y, 2*ff>; /*  view from Z */
  #declare cameraPos = <ParaDIS_Center.x,  2*ff, ParaDIS_Center.z>; /*  view from Y */
  #declasre cameraPos = <ParaDIS_Center.x,  2*ff, ParaDIS_Center.z>; /*   view from X */
  #declare cameraPos = 2*ff; /*   view from ZYX corner */
  #declare cameraLookAt = ParaDIS_Center;
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

/* ========================================================== */
#macro MakeAxes (axisLocation, axisLength, axisDiameter) 
  /*  red/cyan is X */
  cylinder {axisLocation, axisLocation+x*axisLength, axisDiameter pigment {color Red  } finish { brightfinish }}
  cylinder {axisLocation-x*axisLength, axisLocation, axisDiameter pigment {color White-Red } finish { brightfinish }}
  /*  green/opposite is Y */
  cylinder {axisLocation, axisLocation+y*axisLength, axisDiameter pigment {color Green} finish { brightfinish }}
  cylinder {axisLocation-y*axisLength, axisLocation, axisDiameter pigment {color White-Green} finish { brightfinish }}
  /*  blue/yellow is Z */
  cylinder {axisLocation, axisLocation+z*axisLength, axisDiameter pigment {color Blue } finish { brightfinish }}
  cylinder {axisLocation-z*axisLength, axisLocation, axisDiameter pigment {color White-Blue } finish { brightfinish }}
#end

/* ========================================================== */
#ifdef (DoAxes)
  #if (DoAxes)
	/*  place a large sphere at the primary axis center */
	/*sphere {cameraLookAt, 500   
  pigment {color Orange } finish { brightfinish }
  }*/
	sphere {ParaDIS_Center, 300   
	  pigment {color Red } finish { brightfinish }
	}
	MakeAxes (ParaDIS_Center, ParaDIS_Size.x/2.0, 100)
  #end
#end /* ifdef(DoAxes) */


/* get our bearings by putting an axis marker at the look-at point */
/* wormAxes (<ParaDIS_Center.x, ParaDIS_Center.y, ParaDIS_Center.z>, L, 500) */

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
      color 0.5
  /* rotate -rot03 */
  /* rotate -rot02 */
  /* rotate -rot01 */
  /* rotate y*(1-cameraSpline)*90 */
  /* shadowless */
}

/*  "remote light"  */
/* #declare lightPos = cameraLookAt+<2000,-5000,50000>; */
#declare remoteLightPos = (<ParaDIS_Bounds[0].x, ParaDIS_Bounds[0].y, 0>);
light_source {
  remoteLightPos
  color 2*White
    area_light
  <ParaDIS_Bounds[1].x, 0, 0>, <0, 0, ParaDIS_Bounds[1].z> 40 40
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
/* cylinder {cameraPos, cameraPos+y*axisLength, axisLength/15 pigment { color Blue  } finish { brightfinish }} */

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
#ifndef (DoBounds)   
  #declare DoBounds = 0; 
#end
#if (DoBounds)
  setbounds(ParaDIS_Bounds[0].x, ParaDIS_Bounds[1].x, ParaDIS_Bounds[0].y, ParaDIS_Bounds[1].y, ParaDIS_Bounds[0].z, ParaDIS_Bounds[1].z)
#end

/* #declare domedia = 1;  */
#ifdef (domedia)
  box {
	10*ParaDIS_Bounds[0], 10*ParaDIS_Bounds[1]
	pigment { color rgbf .2 }
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
  }
#end


/* ========================================================= */


/*  draw five "walls" behind the data for aesthetics */
#ifdef (Backdrop)
  #if (Backdrop) 
	object {
	  merge {
		box { ParaDIS_Bounds[0] - 0.0001 * ParaDIS_Size.x * x, 	  
		  ParaDIS_Bounds[0] + <0, ParaDIS_Size.y, ParaDIS_Size.z> }
		box { ParaDIS_Bounds[0] - 0.0001 * ParaDIS_Size.y * y, 
		  ParaDIS_Bounds[0] + <ParaDIS_Size.x, 0, ParaDIS_Size.z> }
		box { ParaDIS_Bounds[0] - 0.0001 * ParaDIS_Size.z * z, 
		  ParaDIS_Bounds[0] + <ParaDIS_Size.x, ParaDIS_Size.y, 0> }
		
		box { ParaDIS_Bounds[1] + 0.0001 * ParaDIS_Size.x * x, 
		  ParaDIS_Bounds[1] - <0, ParaDIS_Size.y, ParaDIS_Size.z> }
		box { ParaDIS_Bounds[1] + 0.0001 * ParaDIS_Size.y * y, 
		  ParaDIS_Bounds[1] - <ParaDIS_Size.y, 0, ParaDIS_Size.z> }
		pigment 
		{ rgb 0.7 }
		normal { bumps 0.1 scale 500 }
		finish { phong 0 metallic }
		
	  }
	  scale 2
	}
  #end
#end
   
/*  this is provided to give you something to set AdditionItems01 to if needed */
#declare invisibleSphere = sphere {
<1000000,0,0>, 1
   pigment { color <0,0,0> }
};

#declare AdditionItems01 = invisibleSphere; 

#ifdef (dofog)
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

