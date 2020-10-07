import sys
sys.path.append('../../build/') # This will be replaced by a called function in later releases
sys.path.append('../../util/') # This will be replaced by a called function in later releases

import pycapmd as capmd
import readParams

import killCells as kc
import findBorder as border

import matplotlib.pyplot as plt # required to print the example border detection
from tqdm import tqdm 
import time

def main():

    parameterFile = "parameters.json"
    params = readParams.fromJSON(capmd.Parameters(), parameterFile)
    #sim = capmd.simulation(params)
    sim = capmd.interface(params)

    N = sim.popSize()
    NB = sim.getBoundarySize()

    # Choosing the opposing corners of the rectangle to be killed
    lx = params.Lx
    ly = params.Ly
    wound = [(-lx/4, -ly), (lx/4,ly)]

    nmax = 10052 
    death_time = 10050 # <- 50 such that it is far from the death causing holes in popDyn
    save_at = params.output_time
    steps_between_check = 100
    for n in tqdm(range(nmax)):
        sim.move()

        if (n % steps_between_check == 0):
            sim.populationDynamics(steps_between_check)

        if (n == death_time):

            N = sim.popSize()
            NB = sim.getBoundarySize()
            idxs = capmd.VectorInt(range(NB, N+NB))
            pop_id = sim.getPopulationId(idxs) #<- get id
            pop_pos = sim.getPopulationPosition(idxs) # <-  get position
            # [kc.kill_cell_in_rect(x, i, rect, sim) for (x,i) in zip(pop_pos, pop_id)] 
            print(kc.select_cells(pop_pos, pop_id, wound))
            killList = capmd.VectorInt(kc.select_cells(pop_pos, pop_id, wound))
            sim.killCells(killList);

            print('Finished killing')

        elif (n >= death_time) and (n <= death_time + 1): # Only after wounding occurs (limit is in place as atm we plot the result)
            N = sim.popSize()
            NB = sim.getBoundarySize()
            idxs = capmd.VectorInt(range(N, N+NB))
            pop_pos = sim.getPopulationPosition(idxs)
            
            edges = border.alpha_shape(pop_pos, alpha = 3)
            plt.figure()
            plt.axis('equal')
            xs = []
            ys = []
            # separate xs and ys since pop_pos[:,0] and pop_pos[:,1] dont agree since its a list of tuples
            for pos in pop_pos:
                xs.append(pos[0])
                ys.append(pos[1])
            plt.plot(xs, ys, '.')
            for i, j in edges:
                print("edges: ",i,j)
                plt.plot([xs[i],xs[j]], [ys[i],ys[j]])
            plt.show()
        
        if(n % save_at == 0):
            sim.saveData('vtp')
            sim.saveData('text')
                
    print('Simulation Complete\n\n') 
    return 0

if __name__ == "__main__":

    main()