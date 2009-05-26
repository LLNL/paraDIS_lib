//#include "jaimeparams.inc"
#include "colors.inc"
#include "textures.inc"
#include "woods.inc"
#include "stones2.inc"
#include "finish.inc"
#include "math.inc"

// ===========================================================
// HUGELY USEFUL MACROS FOR DEBUGGING: 
#macro printvector(vthing,vname)
#debug concat(vname, " is: ", vstr(3,vthing, "," , 0,3), "\n")
#end
#macro axis(axisLocation, axisOrientation, axisSize, axisDiameter, axisColor)
cylinder {axisLocation, axisLocation+axisOrientation*axisSize, axisDiameter pigment { axisColor  } finish { brightfinish }}
#end
// ===========================================================

background {Black}
//===========================================
// global settings
global_settings {
  max_trace_level 50
    }
/*        radiosity {
           brightness 1
        }
  }*/
// ===========================================================

#declare dullfinish = finish { ambient .9 }
#declare brightfinish = finish { phong .3 ambient 0.5 }

// Red sky w/ yellow clouds -- very surreal.
#declare My_Blood_Sky = 
pigment {
    agate
    turbulence 1.0
    color_map {
        [0.0 rgb <0.9, 0.700, 0.0>]
        [0.5 rgb <0.3, 0.200, 0.0>]
        [0.5 rgb <0.6, 0.025, 0.0>]
        [0.6 rgb <0.9, 0.700, 0.0>]
        [0.6 rgb <0.6, 0.025, 0.0>]
    }
    scale 0.2
}
// ===========================================================
// the star of the show: 
#declare tetvert1=<13,0,0>;
#declare tetvert2=<0,0,13>;
#declare tetvert3=<0,13,0>;
#declare tetvert4=<13,13,13>;
#declare tetcenter = (tetvert1 + tetvert2 + tetvert3 + tetvert4)/4.0; 
// ===========================================================


printvector(tetcenter, "tetcenter")
// CAMERA AND LIGHTS
#declare lookatpos = <0.000000,5.000000,7.733467>;
#declare camerapos = <71.0, 10.0,-13.1070155>;
#ifdef (zoom)
#declare lookatpos = <5.000000,5.000000,5>;
#declare camerapos = <32.2, 3.5, -9.2>;
#end
#ifdef (far)
#declare lookatpos = <0.000000,5.000000,7.733467>;
#declare camerapos = <324.0, 80.0, 52.4>;
#end
#ifdef (reverse)
// no far case yet for reverse... 
#declare lookatpos = <15.000000,5.000000,12.733467>;
#declare camerapos =  lookatpos + 3*<8.0, -30, 6.0>;
// #declare camerapos =  1.5*<0, 20, 20>;
#end

#declare camvec = vnormalize(camerapos-lookatpos); 
#declare camperpvec1 = vnormalize(VPerp_To_Vector(camvec));
#declare camperpvec2 = vnormalize(VPerp_To_Plane(camvec, camperpvec1)); 
printvector (camvec, "camvec")
     printvector(camperpvec1, "camperpvec1")
     printvector(camperpvec2, "camperpvec2")

#ifdef (debug)
sphere {lookatpos, 0.5  
    pigment {color Yellow } finish { brightfinish }
}
axis(lookatpos, camperpvec1, 200, 0.05, color Orange)
axis(lookatpos, camperpvec2, 200, 0.05, color Green)
#end

#declare viewx = x * 141.000000;
#declare viewy = y * 141.000000;
camera {
  /*
   orthographic
   up viewy
   right viewx
  */
  // up and right just give the aspect ratio:
  up y
    right 1.25*x
    //right 4*x
    location  camerapos
    look_at lookatpos
    // sky <0,0.2,1>
     sky tetvert4 - tetcenter
    //    angle 35
    angle 45
    }

// ===============================
// for umbra effects
#ifdef(reverse) 
#declare spotlightPos =  camerapos - 30 * camvec + 55 * camperpvec1 - 55 * camperpvec2; 
#declare spotlight_pointat = tetcenter - <0,0,10>; 
#declare umbraLightPos = spotlightPos; 
#else
#declare spotlightPos =  <280,250,10>;
#declare spotlight_pointat = tetcenter - <0,0,10>; 
#declare umbraLightPos =  spotlightPos; //600*camvec+ 40*camperpvec1 + 40*campe
#end
 
#declare umbraLightSize = 200.0; 
light_source {
  umbraLightPos
  color 1.1
    area_light
   umbraLightSize*camperpvec1 umbraLightSize*camperpvec2 
   7 7
  circular
  adaptive 3
  jitter
 #ifdef (reverse)
    //  rotate <-65, 0, 0>
#end
    orient
    } 

light_source {
  spotlightPos
    color 0.5
    spotlight
    falloff 45
    radius 10
    point_at spotlight_pointat
    //tightness 0
    //shadowless
    //radius umbraLightSize

}

#ifdef (debug)
// place a green sphere at the point_at spot for the lights
printvector(spotlightPos, "spotlightPos")
// DUMB!  THIS WILL BLOCK THE SPOTLIGHT!  :-)
     sphere {
  spotlight_pointat, 0.25
    texture {pigment {color Green} finish {ambient 0.5}}
}
     sphere {
       spotlightPos+<1,1,1>, 0.25
    texture {pigment {color Orange} finish {ambient 0.5}}
}
//also a yellow cylinder showing the direction of the lights
#declare spotvec = spotlightPos-spotlight_pointat; 
cylinder {spotlightPos+ <1,1,1>, 2 * spotlight_pointat - spotlightPos , 0.05 pigment { color Yellow  } finish { brightfinish }}
#end

// ===========================================================
// This macro takes any object and casts an "anti-shadow" through it -- see povray docs (Projected_Through)
// Although it's totally awesome to be able to do this, I'm not going to use it.  :-)
#macro ShineLightThrough(thing)
light_source {
  spotlightPos
    color 0.50
    spotlight
    point_at spotlight_pointat
    falloff 15
    radius 10
    tightness 0
    projected_through {thing}

}
#end

// ===========================================================
// DISLOCATIONS
// thin == 0.05, thick = 0.3, medium = 0.10
//#declare dislocationRadius = 0.20; # jaime likes this
#declare dislocationRadius = 0.08; 

#macro make_dislocation(loc0, loc1, disType, sphereBothEnds)
#declare dis_transp =  <0,0,0,0,0>; 

//#if (mod(disType,3) = 1)
#switch (disType)
#case (4)  // new!  
#local pig = pigment {Blue + dis_transp}; 
#break
#case (3)
//#local pig = pigment {color red 0.0 green 0.7 blue 0.9};
#local pig = pigment {Yellow + dis_transp}; 
#break
#case (2)
#local pig = pigment {Red + dis_transp};
#break
#case (1)
//#local pig = pigment {color red 0.0 green 0.6 blue 0.4 };
#local pig = pigment {Green + dis_transp}; 
#break
#case (0)
#local pig = pigment {Orange + dis_transp}; 
#ifndef (debug)
#local donotdraw = 1;
#end
#break
#else
#error concat("Bad disType!  disType is: ", str(disType,0,2), "\n")
#local pig = pigment {Blue + dis_transp}; 
#ifndef (debug)
#local donotdraw = 1;
#end
#break
#end


#ifndef (donotdraw)


#macro metalfinish()
     finish {
        ambient 0.2
        diffuse .2
        specular 0.4
        roughness .3
        reflection {
           0.5
	     metallic
        }
     }
#end
#macro thecyl()
cylinder {
    loc0, loc1, 
#ifdef (debug)
    dislocationRadius*0.7
#else
      dislocationRadius
#end
    open
      texture {pigment {pig} } 
    //  texture {pigment {pig} metalfinish() } 
}
#end // macro thecyl
#macro make_connecting_sphere(loc)
sphere {
  loc, dislocationRadius
#ifdef (debug)
    texture {pigment { Orange} }
#else
    texture {pigment { pig} }
#end
}
#end // macro make_connecting_sphere

#if (sphereBothEnds = 1)
make_connecting_sphere(loc0)
#end
make_connecting_sphere(loc1)

thecyl()
//  ShineLightThrough(thecyl())

#end // ifndef donotdraw
#end  //macro make_dislocation

#macro make_debug_node(node)
#ifdef (debug)
sphere {
  node, dislocationRadius*2
    texture {pigment {Red} }
}
#end
#end // make_debug_node
// ===========================================================
// TRIANGLES 
#macro make_triangle(v0,v1,v2, triType) 
#declare tri_transp =  <0,0,0,0,0.6>; 
//#if (triType = 1)
#switch (triType)
#case(3)
//#local pig = Gold_Metal;
#local col = color Red + tri_transp;
#break
#case(2)
//#local pig = T_Stone27;
#local col = color Green  + tri_transp; 
#break
#case(1)
//#local pig = T_Wood16;
//#local col = color red 0.0 green 0.4 blue 0.6  + tri_transp; 
#local col = color <0.0, 0.4, 0.6>  + tri_transp; 
#break
#case (0)
#ifndef (debug)
#local donotdraw = 1;
#end
#local col = color Yellow + tri_transp; 
#break
#end

//#ifndef (donotdraw)
// the #declare statement creates a compiled "thingy", e.g., a "finish identifier", instead of a finish statement.  So later, to use testfinish, you have to say "finish {testfinish}", and to use testtexture, you must say "texture {testtexture}".  #macro does not have this behavior as it is a literal text substitution.

#declare testfinish = finish {ambient 0.7};
#declare testtexture = texture { pigment {color col} finish {ambient 0.7} };

#macro thetriangle()
 triangle {
    v0 v1 v2
      texture {
      pigment{color col}
       finish {ambient 0.4}
       //metalfinish()
    }
}
#end
thetriangle()
//  ShineLightThrough(thetriangle())

        //      finish { ambient 0.7 }
      //finish {specular 3 roughness 0.001 metallic }
      // Shiny

//#end // ifndef donotdraw
   
#end // macro make_triangle
// ===========================================================

// =======================================================
// WALLS AND MEDIA 
//=========================================================
// draw three "walls" behind the data for aesthetics
//#declare backdrop=1;
//#declare makebox=1;

#declare bgorigin = lookatpos - 0.005*camvec;
#ifdef (reverse) 
#declare bgorigin = lookatpos - 30*camvec;
#end
#declare bgextent = 5750;
printvector(bgorigin, "bgorigin")
/* 
 #declare bgvec1 = bgextent * camperpvec1;
 #declare bgvec2 = bgextent * camperpvec2;
 printvector(bgvec1, "bgvec1")
 printvector(bgvec2, "bgvec2")
 printvector(bgvec1+bgvec2, "bgvec1+bgvec2")
 #declare bgthickness = 0.01; 
 #declare bgstart = bgorigin - bgvec1 - bgvec2;
 #declare bgend=   bgorigin + bgvec1 + bgvec2 + 0.00001*camvec;
printvector(bgstart, "bgstart")
     printvector(bgend, "bgend")
*/ 
//#declare     boxx = lookatpos.x + 0.1 * (lookatpos.x - camerapos.x ) ;
box {
  
     // shouldn't have to change rotation:
#ifdef(reverse)
//     rotate <0,260,135>
  bgorigin+<bgextent, 0, bgextent>, bgorigin+<-bgextent, -0.001, -bgextent>
    rotate <0,0,-45>
#else
   bgorigin+<0, bgextent, bgextent>, bgorigin+<-0.001, -bgextent, -bgextent>
     rotate <0,-20,45>
#end
     //bgstart, bgend
    //  rotate -40*camperpvec2
    // bgstart, bgend
 texture  {
    //pigment { My_Blood_Sky }
    //finish { phong 0.4 ambient 0.1}
    //pigment { color <0.01,0.01,0.01>  }
    pigment { color DimGray  }
    finish { dullfinish }
    //normal { bumps .3 scale 5 }
}
}
background { color DimGray }

//=================================================
// media/scattering etc
#ifdef (xxmakebox)
box {
  -20*boundsize, 20*boundsize
    //pigment { color rgbf 0.2 }
  hollow
    interior {
    media {
      absorption 0.03
	//      scattering {1, color rgb<0.000025,0.00005,0.000025> extinction 0}
      //scattering {1, color 0.001 extinction 0.5}
      method 3
	intervals 1
	jitter 1.0
        }
  }
}
#end
#macro wormAxes (axisLocation, axisSize, axisDiameter) 
// Red, White Blue == X, Y, Z
// red/orange is X
axis(axisLocation, x, axisSize, axisDiameter, color Red)
axis(axisLocation, -x, axisSize, axisDiameter, color Orange)
// green/yellow is Y
axis(axisLocation, y, axisSize, axisDiameter, color Green)
axis(axisLocation, -y, axisSize, axisDiameter, color Yellow)
// blue/cyan is Z
axis(axisLocation, z, axisSize, axisDiameter, color Blue)
axis(axisLocation, -z, axisSize, axisDiameter, color Cyan)
#end
#ifdef (debug)
// place a large sphere at the primary axis center, and one along each axis 10 units out for sizing
sphere {0, 0.5  
    pigment {color Red } finish { brightfinish }
}
// Create a grid in space... 
#declare gridspacing = 10.0; 
#declare numgrids = 2;

#macro dogrid(gridperp, gridx,gridy,gridcolor)
#declare i = -numgrids*gridspacing; 
#while (i <= numgrids*gridspacing)
#declare j = -numgrids*gridspacing; 
#while (j <= numgrids*gridspacing)
#declare k = -numgrids * gridspacing;
axis (i*gridx+j*gridy+k*gridperp, gridperp, 2*numgrids*gridspacing, 0.05, gridcolor)
#while (k <= numgrids*gridspacing)
sphere {i*gridx+j*gridy+k*gridperp, 0.2
    pigment {color Gray } finish { brightfinish }
}
#declare k = k+gridspacing;
#end
#declare j = j+gridspacing;
#end
#declare i = i+gridspacing;
#end
#end // dogrid
/*dogrid (x,y,z,color White)
  dogrid (y,z,x, color White)
  dogrid (z,y,x, color White)
*/ 
  wormAxes (0, 100, 0.07)

// also show the lookat point:
sphere { 
  lookatpos, 0.75
    texture { pigment {color Yellow} finish {ambient 0.1}}
    }
// try to locate the tet vertices:
#declare lapradius=0.6;
#macro tetsphere(loc, tetcolor)
sphere { 
  loc, lapradius
    texture { pigment { tetcolor} finish {ambient 0.1}}
    }
#end
tetsphere(tetvert1, color Red)
  tetsphere(tetvert2, color Yellow)
  tetsphere(tetvert3, color Green)
  tetsphere(tetvert4, color Blue)
// now the tet center is:
  tetsphere(tetcenter, color White)
// draw the scene bounds if provided:
#ifdef (lowerbounds)
#if (0) // changed my mind
#macro makecyl(p1,p2)
  cylinder {p1, p2, 0.05 pigment {color White} finish {brightfinish}}
#end
#debug "drawing bounding box\n"
makecyl(lowerbounds, <lowerbounds.x, lowerbounds.y, upperbounds.z>)
makecyl(<lowerbounds.x, lowerbounds.y, upperbounds.z>, <upperbounds.x, lowerbounds.y, upperbounds.z>)
makecyl(<lowerbounds.x, lowerbounds.y, upperbounds.z>, <lowerbounds.x, upperbounds.y, upperbounds.z>)
makecyl(lowerbounds, <lowerbounds.x, upperbounds.y, lowerbounds.z>)
makecyl(<lowerbounds.x, upperbounds.y, lowerbounds.z>, <upperbounds.x, upperbounds.y, lowerbounds.z>)
makecyl(<lowerbounds.x, upperbounds.y, lowerbounds.z>, <lowerbounds.x, upperbounds.y, upperbounds.z>)
makecyl(lowerbounds, <upperbounds.x, lowerbounds.y, lowerbounds.z>)
makecyl(<upperbounds.x, lowerbounds.y, lowerbounds.z>, <upperbounds.x, upperbounds.y, lowerbounds.z>)
makecyl(<upperbounds.x, lowerbounds.y, lowerbounds.z>, <upperbounds.x, lowerbounds.y, upperbounds.z>)

makecyl(upperbounds, <upperbounds.x, upperbounds.y, lowerbounds.z>)
makecyl(<upperbounds.x, upperbounds.y, lowerbounds.z>, <lowerbounds.x, upperbounds.y, lowerbounds.z>)
makecyl(<upperbounds.x, upperbounds.y, lowerbounds.z>, <upperbounds.x, lowerbounds.y, lowerbounds.z>)
makecyl(upperbounds, <upperbounds.x, lowerbounds.y, upperbounds.z>)
makecyl(<upperbounds.x, lowerbounds.y, upperbounds.z>, <lowerbounds.x, lowerbounds.y, upperbounds.z>)
makecyl(<upperbounds.x, lowerbounds.y, upperbounds.z>, <upperbounds.x, lowerbounds.y, lowerbounds.z>)
makecyl(upperbounds, <lowerbounds.x, upperbounds.y, upperbounds.z>)
makecyl(<lowerbounds.x, upperbounds.y, upperbounds.z>, <lowerbounds.x, lowerbounds.y, upperbounds.z>)
makecyl(<lowerbounds.x, upperbounds.y, upperbounds.z>, <lowerbounds.x, upperbounds.y, lowerbounds.z>)
box {
  lowerbounds, upperbounds
    pigment { color rgbf 0.9 }
  hollow
    interior {
    media {
      absorption 0.0000001
	//      scattering {1, color rgb<0.000025,0.00005,0.000025> extinction 0}
      //scattering {1, color 0.001 extinction 0.5}
      method 3
	intervals 1
	jitter 1.0
        }
  }
}
#end
#printvector(lowerbounds, "lowerbounds")
#printvector(upperbounds, "upperbounds")
#else
#debug "NOT drawing bounding box\n"
#end // lowerbounds
#end // ifdef (debug)

