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
#macro printvector5(vthing,vname)
#debug concat(vname, " is: ", vstr(5,vthing, "," , 0,3), "\n")
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

#declare boundsize = upperbounds - lowerbounds; 

#declare dullfinish = finish { ambient .9 }
#declare brightfinish = finish { phong .3 ambient 0.5 }

// CAMERA AND LIGHTS
#declare lookatpos = <30,-5,-5>; 
//#declare camerapos = 0 + <70,100,-100>;
#declare camerapos = 1.2*<-70,-100,100>;

#ifdef (zoom)
#declare lookatpos = <5.000000,5.000000,5>;
#declare camerapos = <32.2, 3.5, -9.2>;
#end
#ifdef (far)
#declare lookatpos = <0.000000,5.000000,7.733467>;
#declare camerapos = <324.0, 80.0, 52.4>;
#end
#ifdef (reverse)
#debug "reverse\n"
// no far case yet for reverse... 
#declare lookatpos = <15.000000,5.000000,12.733467>;
#declare camerapos =  lookatpos + 3*<8.0, -30, 6.0>;
#ifdef (far)
#debug "far and reverse...\n"
#declare lookatpos = <35.000000,5.000000,0.733467>;
#declare camerapos =  lookatpos + 5*<8.0, -30, 6.0>;
#end
#end
     printvector(camerapos, "camerapos")
     printvector(lookatpos, "lookatpos")
#declare camvec = vnormalize(camerapos-lookatpos); 
#declare camperpvec1 = vnormalize(VPerp_To_Vector(camvec));
#declare camperpvec2 = vnormalize(VPerp_To_Plane(camvec, camperpvec1)); 
printvector (camvec, "camvec")
     printvector(camperpvec1, "camperpvec1")
     printvector(camperpvec2, "camperpvec2")

#ifdef (debug)
sphere {0, 5  
    pigment {color 0.5*Yellow } finish { brightfinish }
}
sphere {lookatpos, 0.5  
    pigment {color Yellow } finish { brightfinish }
}
axis(lookatpos, camperpvec1, 200, 0.5, color 0.5*Orange)
axis(lookatpos, camperpvec2, 200, 0.5, color 0.5*Green)
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
    sky <-1, 0, 0>
    // sky <0,0.2,1>
    //    angle 35
    angle 45
    }

// ===============================
// for umbra effects
#ifdef(reverse) 
#declare spotlightPos =  camerapos - 30 * camvec + 5 * camperpvec1 - 5 * camperpvec2; 
#declare spotlight_pointat = 0; 
#declare umbraLightPos = spotlightPos; 
#else
#declare spotlightPos =   camerapos - 30 * camvec + 55 * camperpvec1 + 55 * camperpvec2; ;
#declare spotlight_pointat = 0 ; 
#declare umbraLightPos =  spotlightPos; //600*camvec+ 40*camperpvec1 + 40*campe
#end
 
#declare umbraLightSize = 200.0; 
light_source {
  umbraLightPos
  color 1.5
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
// CAREFUL!  THIS CAN BLOCK THE SPOTLIGHT!  :-)
     sphere {
  spotlight_pointat, 2.5
    texture {pigment {color Green} finish {ambient 0.5}}
}
/*    sphere {
       spotlightPos+<1,1,1>, 2.5
    texture {pigment {color Orange} finish {ambient 0.5}}
    }*/
//also a yellow cylinder showing the direction of the lights
#declare spotvec = spotlightPos-spotlight_pointat; 
cylinder {spotlightPos+ <1,1,1>, 2 * spotlight_pointat - spotlightPos , 0.5 pigment { color Yellow  } finish { brightfinish }}
#end


#macro make_atom(r,col, loc)
//#warning concat("color is: ", str(col,0,1),"\n")
#declare newcolor = (col - 330)/ 131.0; // normalized between 330 and 461
/* 
 '381', '402' and '405' refer to our line color '3'.
 anything below 381 can be considered as 381.
 A color code of '422' (in the last column of the atomic files) corresponds to our FAN or STRIP color '1'
 '444' and '461' refer to our line color '1'.
 All the other ones represent color gradations in between
*/

#declare linecolor3 = <1.0, 1.0, 0.0, 0.0, 0.0>;
#declare linecolor1 = <0.0, 1.0, 0.0, 0.0, 0.0>; 
#declare stripcolor1 = <0.8, 0.8, 1.0, 0.0, 0.0>; 
#declare striptransp = <0.0, 0.0, 0.0, 0.0, 0.0>; 
#declare thefinish = finish {ambient 0.0}
#declare radi = r/50; 

#if (col > 461)
#declare col = 461;
#end
#if (col < 331)
#declare col = 331; 
#end
#switch (col)
#range (331,405)  // line color 3
  #local blend = linecolor3; 
#break
#range(444,461)  // line color 1
  #local blend = linecolor1; 
#break
#range(422,443) // blend between stripcolor1 and line color 1
//#declare radi = r/20; 
#local frac = (443-col)/(443-422); // closeness to 422
#ifdef(debug)
#warning concat("frac for col ",str(col,0,1)," is ",str(frac,0,3))
#end
#declare thefinish = finish { specular 0.005 ambient 0.0 }
#local blend = (1-frac)*linecolor1 + (frac)*stripcolor1 + striptransp; 
//#local tex =  texture { pigment { color Red + striptransp } }
#break
#range(406,421) // blend between strip color 1 and line color 3
//#declare radi = r/20; 
#local frac = (421-col)/(421-406); // closeness to 406
#local  blend = (1-frac)*stripcolor1 + (frac)*linecolor3 + striptransp; 
//#local tex =  texture { pigment { color Red + Blue + striptransp } }
#break
#else
#error concat("Impossible value for color: ",str(col,0,1),"\n")
#break
#end // switch(col)
  #local tex = texture  {pigment {color linecolor3} }

#declare newloc = loc-gCenter; 

#ifdef(debug)
printvector5(blend, concat("blend for color ",str(col,0,1)))
#end

#local tex = texture { pigment { color blend } finish { thefinish }} 
sphere {
  newloc, radi
    texture {tex}
    //  texture { 
    // pigment { color rgb <newcolor, 1.0-newcolor, 1.0>}
    // finish {brightfinish}
    // }
}
#end // macro make_atom()

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


// =======================================================
// WALLS AND MEDIA 
//=========================================================
// draw three "walls" behind the data for aesthetics
//#declare backdrop=1;
//#declare makebox=1;

#declare bgorigin = lookatpos + <30,0,0>;
#ifdef (reverse) 
#declare bgorigin = lookatpos - 300*camvec;
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
#ifdef(debug)
sphere { bgorigin, 2.5
    texture {pigment {color Magenta} finish {ambient 0.5}} }
#end
//#declare     boxx = lookatpos.x + 0.1 * (lookatpos.x - camerapos.x ) ;
box {
  
     // shouldn't have to change rotation:
#ifdef(reverse)
//     rotate <0,260,135>
  bgorigin+<bgextent, 0, bgextent>, bgorigin+<-bgextent, -0.001, -bgextent>
    rotate <0,0,-35>
#else
   bgorigin+<0, bgextent, bgextent>, bgorigin+<-0.001, -bgextent, -bgextent>
    //rotate <0,0,45>
    // rotate <0,0,45>
#end
     //bgstart, bgend
    //  rotate -40*camperpvec2
    // bgstart, bgend
 texture  {
    //pigment { My_Blood_Sky }
    //finish { phong 0.4 ambient 0.1}
    //pigment { color <0.01,0.01,0.01>  }
    pigment { color DimGray + <0,0,0,0>  }
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
#end //ifdef (xxmakebox)
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
#end // wormAxes

#ifdef (grid)
#warning "dogrid detected\n" 
// Create a grid in space... 
#declare gridspacing = 25.0; 
#declare numgrids = 1;
#declare gridthickness = 0.4; 
#macro dogrid(gridperp, gridx,gridy,gridcolor)
//#warning "dogrid called\n"
#declare i = -numgrids*gridspacing; 
#while (i <= numgrids*gridspacing)
#declare j = -numgrids*gridspacing; 
#while (j <= numgrids*gridspacing)
#declare k = -numgrids * gridspacing;
axis (0 + i*gridx+j*gridy+k*gridperp, gridperp, 2*numgrids*gridspacing, gridthickness, gridcolor)
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
dogrid (x,y,z,color White)
  dogrid (y,z,x, color White)
  dogrid (z,y,x, color White)

#end // if (dogrid)

#ifdef (debug)
// place a large sphere at the primary axis center, and one along each axis 10 units out for sizing
sphere {0, 5  
    pigment {color Red } finish { brightfinish }
}
  wormAxes (0, 100, 0.7)

#ifdef(debug)
  wormAxes(lookatpos, 1000, 1.0)
// let's put a little ball every n units
#declare ballinterval = 25; 
#declare ballr = 3.0; 
  sphere {0 + ballinterval*<1,0,0>, ballr pigment {color Red} }
  sphere {0 + ballinterval*<0,1,0>, ballr pigment {color Green} }
  sphere {0 + ballinterval*<0,0,1>, ballr pigment {color Blue} }
#end 
// also show the lookat point:
sphere { 
  lookatpos, 4
    texture { pigment {color Yellow} finish {ambient 0.1}}
    }
// try to locate the tet vertices:
#macro tetsphere(loc, tetcolor)
sphere { 
  loc, 2
    texture { pigment { tetcolor} finish {ambient 0.1}}
    }
#end
// ===========================================================
// the star of the show: 
#declare tetvert1=<13,0,0>;
#declare tetvert2=<0,0,13>;
#declare tetvert3=<0,13,0>;
#declare tetvert4=<13,13,13>;
#declare tetcenter = (tetvert1 + tetvert2 + tetvert3 + tetvert4)/4.0; 
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

