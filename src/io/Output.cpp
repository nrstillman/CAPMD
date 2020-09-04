#include <stdlib.h>     /* atoi */
#include <iostream>
#include "Output.h"

Output::Output(Parameters params, int boundarysize, std::vector<std::shared_ptr<Particle>> _particles){
        file_name = params.filename;
        output_folder = params.outputfolder;
        N = params.N;
        NB = boundarysize;
        particles = _particles;
}

void Output::update(Parameters params, int boundarysize, std::vector<std::shared_ptr<Particle>> _particles){
    file_name = params.filename;
    output_folder = params.outputfolder;
    N = params.N;
    NB = boundarysize;
    particles = _particles;
}

void Output::log(int timestep){
    auto current_time = std::chrono::steady_clock::now();
    elapsed += std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_log).count()/1000.;
    if (isnan(elapsed)){elapsed = 0;}

    std::cout << "---------" << std::endl;
    std::cout << "timestep: " << timestep << std::endl;
    std::cout << "# of cells: " << particles.size() << std::endl;
    std::cout << "Since last log: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(current_time-last_log).count()/1000.
              << " seconds\n";
    std::cout << "Total runtime: "
              << elapsed
              << " seconds\n";
    last_log = current_time;
}

void Output::savePopulation(int timestep)
{
	std::string outputfile = output_folder + file_name+"_" + std::to_string(timestep) + ".dat";
    std::filebuf fb;
    fb.open (outputfile, std::ofstream::out | std::ofstream::trunc); //< currently deleting txt - use this for appending: std::ios::app);
    std::ostream out(&fb);
    out << particles.size();
    out << '\n';

    for (auto p : particles) {
        out << (*p);
    }
    fb.close();
}

//! Dump meshes into VTP output
void Output::vtp(int timestep)
{
    std::string outputfile = output_folder + file_name+"_" + std::to_string(timestep) + ".vtp";
    //std::string pvdfilename = output_folder + file_name+ ".pvd";

    //polydata for particle attributes
    vtkSmartPointer<vtkPolyData> polydata =  vtkSmartPointer<vtkPolyData>::New();

    //point data for particle positions
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

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
    numneigh->SetName("NumNeigh");
    numneigh->SetNumberOfComponents(1);

    vtkSmartPointer<vtkIntArray> numcontact =  vtkSmartPointer<vtkIntArray>::New();
    numcontact->SetName("NumContact");
    numcontact->SetNumberOfComponents(1);

    vtkSmartPointer<vtkDoubleArray> force =  vtkSmartPointer<vtkDoubleArray>::New();
    force->SetName("Force");
    force->SetNumberOfComponents(3);

    vtkSmartPointer<vtkDoubleArray> vel =  vtkSmartPointer<vtkDoubleArray>::New();
    vel->SetName("Velocity");
    vel->SetNumberOfComponents(3);

	vtkSmartPointer<vtkDoubleArray> forceact =  vtkSmartPointer<vtkDoubleArray>::New();
	forceact->SetName("Active Force");
    forceact->SetNumberOfComponents(3);

    for(auto p = particles.begin(); p != particles.end(); p++){
        points->InsertNextPoint ((*p)->getPosition()[0], (*p)->getPosition()[1], 0.0);

        double f[3] = {(*p)->getForce()[0], (*p)->getForce()[1], 0};
        double v[3] = {(*p)->getVel()[0], (*p)->getVel()[1], 0};
		double fact[3] = {cos((*p)->getTheta()), sin((*p)->getTheta()), 0};

        // Get the data
        ids->InsertNextValue((*p)->getId());
        types->InsertNextValue((*p)->getType());
        radii->InsertNextValue((*p)->getRadius());
        numneigh -> InsertNextValue((*p)->getNumNeigh());
        numcontact -> InsertNextValue((*p)->getZ());

        force->InsertNextTuple(f);
        vel->InsertNextTuple(v);
		forceact->InsertNextTuple(fact);
	}

	// Set the additional polydata
	polydata->GetPointData()->AddArray(ids);
	polydata->GetPointData()->AddArray(types);
	polydata->GetPointData()->AddArray(radii);
	polydata->GetPointData()->AddArray(numneigh);
	polydata->GetPointData()->AddArray(numcontact);

	polydata->GetPointData()->AddArray(force);
    polydata->GetPointData()->AddArray(vel);
	polydata->GetPointData()->AddArray(forceact);

	// Set the data to points
	polydata->SetPoints(points);

    // Write the file
    vtkSmartPointer<vtkXMLPolyDataWriter> writer =
            vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer->SetFileName(outputfile.c_str());
    writer->SetInputData(polydata);
    writer->Write();

//     // create .pvd (is this best way?) <- move to separate method either way
//     ofstream pvdfile;
//     if (t == 0){
//         pvdfile.open(pvdfilename.c_str());
//         pvdfile << "<?xml version=\"1.0\"?>" << std::endl;
//         pvdfile << "<VTKFile type=\"Collection\" version=\"0.1\"" << std::endl;
//         pvdfile << "\t byte_order=\"LittleEndian\"" << std::endl;
//         pvdfile << "\t compressor=\"vtkZLibDataCompressor\">" << std::endl;
//         pvdfile << "<Collection>" << std::endl;
//     }
//     else{
//         pvdfile.open(pvdfilename.c_str(), std::ios_base::app);
//     }
//     pvdfile << "\t<DataSet timestep=\"" << t << "\" group=\"\" part=\"0\" \n"
//                " \t\tfile=\""<< file_name+"_" + std::to_string(t) + ".vtp"<< "\"/>" << endl;
// 
//     if (t == finalstep) {
//         pvdfile << "</Collection>\n          </VTKFile>" << std::endl;
//     }
//     pvdfile.close();


}
