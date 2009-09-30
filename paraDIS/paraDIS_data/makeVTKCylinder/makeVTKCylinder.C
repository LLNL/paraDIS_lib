
#define VTK_STREAMS_FWD_ONLY
#include <vtkCylinderSource.h>
#include <vtkPolyDataWriter.h>
#include <vtkPointData.h>
#include <vtkFloatArray.h>


int
main(int argc, char *argv[])
{
   // cylinder parameters
   double radius = 8000.;
   double h = 16000.;

   // Create the cylinder
   vtkCylinderSource *c = vtkCylinderSource::New();
   c->SetCapping(1);
   c->SetRadius(radius);
   c->SetResolution(360); // 1 face per degree
   c->SetCenter(0.,0.,0.);
   c->SetHeight(h);

   // Could add transform code here

   // Add a variable to it so there's something to easily make transparent
   // in a Pseudocolor plot.
   c->Update();
   vtkPolyData *pd = c->GetOutput();
   vtkFloatArray *one = vtkFloatArray::New();
   one->SetNumberOfTuples(pd->GetNumberOfPoints());
   for(int i = 0; i < pd->GetNumberOfPoints(); ++i)
       one->SetTuple1(i, 1.);
   one->SetName("one");
   pd->GetPointData()->AddArray(one);

   // Write out the cylinder
   vtkPolyDataWriter *w = vtkPolyDataWriter::New();
   w->SetInput(pd);
   w->SetFileName("cylinder.vtk");
   w->Update();

   c->Delete();
   w->Delete();

   return 0;
}
