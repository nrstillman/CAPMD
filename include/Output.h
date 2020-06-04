#ifndef CAPMD_OUTPUT_H
#define CAPMD_OUTPUT_H

#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPointData.h>

#include "Simulation.h"
#include "Parameters.h"

struct Output {

private:
    Simulation *ptrSim;           // Pointer to the simulation

public:
        /// Constructor
//        Output(){};
        Output(Simulation *);

        /// Destructor
//        ~Output();

        void vtp(int, int);
};
#endif //CAPMD_OUTPUT_H