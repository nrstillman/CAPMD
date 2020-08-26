"""
Steps towards simulation
# 1 start up sim with params specified in json
# 2 run simulation with division for X steps
# 3 activate the death rectangle
# 4 run simulaiton with killed cells
# meanwhile saving to vtp file formats
# Export folder of vtp to paraview
"""
import pycapmd as capmd
import parameters as p
import death
import border
import matplotlib.pyplot as plt # required to print the example border detection
from tqdm import tqdm 
import time
parameterFile = "parameters.json"
params = p.paramsFromFile(capmd.Parameters(), parameterFile)
#sim = capmd.simulation(params)
interface = capmd.interface(params)

N = interface.popSize()
NB = interface.getBoundarySize()

# Choosing the opposing corners of the rectangle to be killed
lx = params.Lx
ly = params.Ly
rect = [(-lx/4, -ly), (lx/4,ly)]

nmax = 10052 
death_time = 10050 # <- 50 such that it is far from the death causing holes in popDyn
save_at = 100000 # Will need to change so that we save pop location to laod that into border.py for example
steps_between_check = 100
for n in tqdm(range(nmax)):
    interface.move()
    # attempt to divide and die
    # steps_between_check*dt*div_rate < 1 for it to make sense mathematically
    if (n % steps_between_check == 0):
        interface.populationDynamics(steps_between_check)

    if (n == death_time):

        N = interface.popSize()
        NB = interface.getBoundarySize()
        idxs = capmd.VectorInt(range(NB, N+NB))
        pop_id = interface.getPopulationId(idxs) #<- get id
        pop_pos = interface.getPopulationPosition(idxs) # <- attempts to get position
        [death.kill_cell_in_rect(x, i, rect, interface) for (x,i) in zip(pop_pos, pop_id)] 

        print('Finished killing')
    elif (n >= death_time) and (n <= death_time + 1): # Only after wounding occurs (limit is in place as atm we plot the result)
        N = interface.popSize()
        NB = interface.getBoundarySize()
        idxs = capmd.VectorInt(range(N, N+NB))
        pop_pos = interface.getPopulationPosition(idxs)
        
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
        interface.saveData('vtp')
        # sim.saveData('text')
            
print('Simulation Complete\n\n') 
