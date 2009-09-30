
SHELL=/bin/bash

VISIT_DIR=/usr/gapps/visit/current/$(shell visit -arch)

VTK_INCLUDES=-I$(VISIT_DIR)/include/vtk/ \
-I$(VISIT_DIR)/include/vtk/Common \
-I$(VISIT_DIR)/include/vtk/Filtering \
-I$(VISIT_DIR)/include/vtk/GenericFiltering \
-I$(VISIT_DIR)/include/vtk/Graphics \
-I$(VISIT_DIR)/include/vtk/Hybrid \
-I$(VISIT_DIR)/include/vtk/IO \
-I$(VISIT_DIR)/include/vtk/Imaging \
-I$(VISIT_DIR)/include/vtk/MangleMesaInclude \
-I$(VISIT_DIR)/include/vtk/Rendering \
-I$(VISIT_DIR)/include/vtk/Utilities \
-I$(VISIT_DIR)/include/vtk/vtkstd

VTK_LIBS=-L$(VISIT_DIR)/lib/ -lvtkCommon -lvtkDICOMParser -lvtkFiltering -lvtkGraphics -lvtkHybrid -lvtkIO -lvtkImaging -lvtkRendering -lvtkzlib -lvtkexpat -lvtkjpeg -lvtkpng -lvtktiff -lvtkftgl -lvtkfreetype -lvtksys -lvtkMPEG2Encode -L/usr/nvidia/lib64 -L/usr/lib64 -L/lib64 -lX11 -lGL -lMesaGL -lOSMesa  -lXt -lpthread


all: makeVTKCylinder

makeVTKCylinder: makeVTKCylinder.C
	g++-4.3.2 $< -o $@  -fpermissive -Wno-deprecated -Wl,-rpath,$(VISIT_DIR)/lib $(VTK_INCLUDES) $(VTK_LIBS) 
