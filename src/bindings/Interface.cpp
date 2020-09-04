//
// Created by Namid Stillman on 7/21/20.
//

#include "Interface.h"
//
Interface::Interface() : Simulation(){
};

Interface::Interface(Parameters params) : Simulation(params){
};

void Interface::trackCell(int cell_idx){
    std::shared_ptr<Particle> p = Simulation::getParticle(cell_idx);
    p->setType(99);
}

void Interface::killCell(int cell_id){
    Simulation::removeParticle(cell_id);
    std::cout << "Cell with id " << cell_id << " killed" << std::endl;
}

void Interface::killCells(std::vector<int> cell_ids){
    Simulation::removeParticles(cell_ids);
}

void Interface::setCellType(int cell_idx, int new_cell_type){
    std::shared_ptr<Particle> p = Simulation::getParticle(cell_idx);
    p->setType(new_cell_type);
};

void Interface::setCellTypes(std::vector<int> cell_ids, int new_cell_type){
    Simulation::changeParticles(cell_ids, new_cell_type);
};

//void setCellAttrib(int cell_idx, int attribute_value, std::string attribute){};

