//
// Created by Namid Stillman on 7/21/20.

#include <string>
#include "Simulation.h"

#ifndef CAPMD_DO_H
#define CAPMD_DO_H


class Interface : public Simulation{

    private:
    std::unique_ptr<Simulation> sim;

    public:
        Interface();
        Interface(Parameters);

        void trackCell(int);
        void killCell(int);
        void killCells(std::vector<int>);
        void setCellType(int, int);
        void setCellTypes(std::vector<int>, int);

//        void setCellAttrib(int, int, std::string attribute); <- this will be evolution of set cell type

//        void killROI(int);
//        void setROIAttrib(int, int, std::string attribute);
};


#endif //CAPMD_DO_H
