#include <stdlib.h>     /* atoi */
#include <iostream>
#include "Output.h"
//
//
Output::Output(Simulation *_ptrSim){

    ptrSim = _ptrSim;
}

//! Dump meshes into VTP output
void Output::vtp(int step, int finalstep)
{
    std::string file_name = "vtp/"+ptrSim->getFileName()+"_" + std::to_string(step) + ".vtp";
    std::string pvdfilename = "vtp/"+ptrSim->getFileName()+ ".pvd";

    //polydata for particle attributes
    vtkSmartPointer<vtkPolyData> polydata =  vtkSmartPointer<vtkPolyData>::New();

    //point data for particle positions
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    int N = ptrSim->popSize();

    vtkSmartPointer<vtkIntArray> ids =  vtkSmartPointer<vtkIntArray>::New();
    ids->SetName("Id");
    ids->SetNumberOfComponents(1);

    vtkSmartPointer<vtkIntArray> types =  vtkSmartPointer<vtkIntArray>::New();
    types->SetName("Type");
    types->SetNumberOfComponents(1);

    vtkSmartPointer<vtkDoubleArray> radii =  vtkSmartPointer<vtkDoubleArray>::New();
    radii->SetName("Radius");
    radii->SetNumberOfComponents(1);

    vtkSmartPointer<vtkIntArray> numneigh =  vtkSmartPointer<vtkIntArray>::New();
    radii->SetName("NumNeigh");
    radii->SetNumberOfComponents(1);

    vtkSmartPointer<vtkDoubleArray> force =  vtkSmartPointer<vtkDoubleArray>::New();
    force->SetName("Force");
    force->SetNumberOfComponents(3);

    for (int i = 0; i < N; i++)
    {
        Particle p = ptrSim->getParticle(i);

        points->InsertNextPoint (p.position[0], p.position[1], 0.0);

        double f[3] = {p.force[0], p.force[1], 0};

        // Get the data
        ids->InsertNextValue(p.getId());
        types->InsertNextValue(p.getType());
        radii->InsertNextValue(p.getRadius());
        numneigh -> InsertNextValue(p.getNumNeigh());

        force->InsertNextTuple(f);

        // Set the additional polydata
        polydata->GetPointData()->AddArray(ids);
        polydata->GetPointData()->AddArray(types);
        polydata->GetPointData()->AddArray(radii);
        polydata->GetPointData()->AddArray(numneigh);

        polydata->GetPointData()->AddArray(force);

        // Set the data to points
        polydata->SetPoints(points);


    }

    // Write the file
    vtkSmartPointer<vtkXMLPolyDataWriter> writer =
            vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer->SetFileName(file_name.c_str());
    writer->SetInputData(polydata);
    writer->Write();

    // create .pvd (is this best way?) <- move to separate method either way
    ofstream pvdfile;
    pvdfile.open(pvdfilename.c_str(), std::ios_base::app);
    if (step == 0){
        pvdfile << "<?xml version=\"1.0\"?>" << std::endl;
        pvdfile << "<VTKFile type=\"Collection\" version=\"0.1\"" << std::endl;
        pvdfile << "\t byte_order=\"LittleEndian\"" << std::endl;
        pvdfile << "\t compressor=\"vtkZLibDataCompressor\">" << std::endl;
        pvdfile << "<Collection>" << std::endl;
    }
    pvdfile << "\t<DataSet timestep=\"" << step << "\" group=\"\" part=\"0\" \n \t\tfile=\""<< file_name<< "\"/>" << endl;

    if (step == finalstep) {
        pvdfile << "</Collection>\n          </VTKFile>" << std::endl;
    }
    pvdfile.close();

}

