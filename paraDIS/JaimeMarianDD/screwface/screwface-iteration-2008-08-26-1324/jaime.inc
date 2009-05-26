//#include "jaimeparams.inc"
#include "colors.inc"
#include "textures.inc"
#include "woods.inc"
#include "stones2.inc"
#include "finish.inc"
#include "math.inc"

// ===========================================================
// HUGELY USEFUL MACRO FOR DEBUGGING: 
#macro printvector(vthing,vname)
#debug concat(vname, " is: ", vstr(3,vthing, "," , 0,3), "\n")
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

#declare dullfinish = finish { ambient .0 }
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
// CAMERA AND LIGHTS
#declare lookatpos = <0.000000,5.000000,7.733467>;
#declare camerapos = 0.1*<710.0, 100.0,-131.070155>;
#ifdef (zoom)
#declare lookatpos = <5.000000,5.000000,5>;
#declare camerapos = 0.07*<460.0, 50.0,-131.070155>;
#end
#ifdef (far)
#declare lookatpos = <0.000000,5.000000,7.733467>;
#declare camerapos = 0.4*<810.0, 200.0, 131.070155>;
#end

#declare viewx = x * 141.000000;
#declare viewy = y * 141.000000;
camera {
  //orthographic
  // up and right just give the aspect ratio:
  up y
    right 1.25*x
    //right 4*x
    location  camerapos
    look_at lookatpos
    //  up viewy
    //right viewx
    //    sky <0,0.2,1>
    sky <0,0.5,1>
    angle 35
    }

#declare camvec = vnormalize(camerapos-lookatpos); 
#declare camperpvec1 = vnormalize(VPerp_To_Vector(camvec));
#declare camperpvec2 = vnormalize(VPerp_To_Plane(camvec, camperpvec1)); 
printvector (camvec, "camvec")
     printvector(camperpvec1, "camperpvec1")
     printvector(camperpvec2, "camperpvec2")
// ===============================
// for umbra effects
 
//#declare spotlightPos =  camerapos + 10*camperpvec1; 
#declare spotlightPos =  <280,250,10>;
#declare spotlight_pointat = <1,5,7>; 
#declare umbraLightPos =  spotlightPos; //600*camvec+ 40*camperpvec1 + 40*camperpvec2; 
#declare umbraLightSize = 1500.0; 
light_source {
  umbraLightPos
  color 1.1
    area_light
   umbraLightSize*camperpvec1 umbraLightSize*camperpvec2 
    7 7
  circular
  adaptive 3
  jitter
  orient
    } 


light_source {
  spotlightPos
    color 0.50
    spotlight
    point_at spotlight_pointat
    falloff 15
    radius 10
    tightness 0
    //shadowless
    //radius umbraLightSize

}
#ifdef (debug)
printvector(spotlightPos, "spotlightPos")
     sphere {
  spotlight_pointat, 2
    texture {pigment {color Green} finish {ambient 0.5}}
}
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


printvector(camerapos, "camerapos")

#declare bgextent = 1000;
#declare bgvec1 = bgextent * camperpvec1;
#declare bgvec2 = bgextent * camperpvec2;
printvector(bgvec1, "bgvec1")
     printvector(bgvec2, "bgvec2")
#declare bgthickness = 0.01; 
#declare bgstart = bgorigin - bgvec1 - bgvec2;
#declare bgend=   bgorigin + bgvec1 + bgvec2 + 0.00001*camvec;
printvector(bgorigin, "bgorigin")
     printvector(camvec,"camvec")
printvector(bgstart, "bgstart")
     printvector(bgend, "bgend")
#declare     boxx = -15;
box {
  <boxx, bgextent, bgextent>, <boxx-0.001, -bgextent, -bgextent>
    rotate <0,0,40>
 texture  {
    //pigment { My_Blood_Sky }
    //finish { phong 0.4 ambient 0.1}
    //pigment { color <0.01,0.01,0.01>  }
    pigment { color DimGray  }
    //normal { bumps .3 scale 5 }
}
}
#ifdef (debug)
sphere {
  <0,0,boxx>, 2.0
    pigment {color Red}
}
#end
//=================================================
// media/scattering etc
#ifdef (makebox)
box {
  -10*boundsize, 10*boundsize
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
cylinder {axisLocation, axisLocation+x*axisSize, axisDiameter pigment {color Red  } finish { brightfinish }}
cylinder {axisLocation-x*axisSize, axisLocation, axisDiameter pigment {color Orange } finish { brightfinish }}
// white/black is Y
cylinder {axisLocation, axisLocation+y*axisSize, axisDiameter pigment {color White} finish { brightfinish }}
cylinder {axisLocation-y*axisSize, axisLocation, axisDiameter pigment {color red 0.02 green 0.02 blue 0.02 } finish { brightfinish }}
// blue/cyan is Z
cylinder {axisLocation, axisLocation+z*axisSize, axisDiameter pigment {color Blue } finish { brightfinish }}
cylinder {axisLocation-z*axisSize, axisLocation, axisDiameter pigment {color Cyan } finish { brightfinish }}
#end
#ifdef (debug)
// place a large sphere at the primary axis center, and one along each axis 10 units out for sizing
/*sphere {cameraLookAt, 500   
    pigment {color Orange } finish { brightfinish }
    }*/
sphere {0, 0.5  
    pigment {color Red } finish { brightfinish }
}
sphere {x*10, 0.5  
    pigment {color Gray } finish { brightfinish }
}
sphere {y*10, 0.5  
    pigment {color Gray } finish { brightfinish }
}
sphere {z*10, 0.5  
    pigment {color Gray } finish { brightfinish }
}
wormAxes (0, 100, 0.1)

// also show the lookat point:
sphere { 
  lookatpos, 2
    texture { pigment {color Yellow} finish {ambient 0.1}}
    }
wormAxes (lookatpos, 10, 0.1)
#end // ifdef (debug)

