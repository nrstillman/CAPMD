#ifndef CAPMD_OUTPUT_H
#define CAPMD_OUTPUT_H

#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPointData.h>

#include "Parameters.h"
#include "Particle.h"

class Output{

public:

    /// Constructor
    Output(Parameters, int, std::vector<std::shared_ptr<Particle>>);

    std::string file_name;
    std::string output_folder;
    int N;
    int NB;
    std::vector<std::shared_ptr<Particle>> particles;


    void vtp(int, int);
};
#endif //CAPMD_OUTPUT_H