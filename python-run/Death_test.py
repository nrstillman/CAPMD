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
from tqdm import tqdm 
import time
parameterFile = "parameters.json"
params = p.paramsFromFile(capmd.Parameters(), parameterFile)
sim = capmd.simulation(params)
interface = capmd.interface(params)

N = sim.popSize()
NB = sim.getBoundarySize()

# Choosing the opposing corners of the rectangle to be killed
lx = params.Lx
ly = params.Ly
rect = [(-lx/4, -ly/4), (lx/4,ly/4)]

nmax=10000
death_time = int(nmax/2)
save_at = 500
steps_between_check = 500
for n in tqdm(range(nmax)):
    interface.move()
    # attempt to divide and die
    # steps_between_check*dt*div_rate < 1 for it to make sense mathematically
    if (n % steps_between_check == 0):
        interface.populationDynamics(steps_between_check)

    if (n == death_time):
        N = sim.popSize()
        NB = sim.getBoundarySize()
        idxs = capmd.VectorInt(range(NB, N+NB))
        # pop_id = sim.getPopulationId(idxs) #<- attempts to get id
        # Alternative ID grab
        pop_id = []
        pop_pos = []
        # pop_pos = sim.getPopulationPosition(idxs) # <- attempts to get id
        for idx in idxs[0:50]:
            part = sim.getParticle(idx)
            part_id = part.getId()
            pop_id.append(part_id)
            part_pos = part.getPosition()#            part_pos = pop_pos[idx - NB]
            pop_pos.append(part_pos)
        
        [death.kill_cell_in_rect(x, i, rect, interface) for (x,i) in zip(pop_pos, pop_id)] 
        print('finished killing')
        """
        for (part_id, part_pos) in zip(alt_pop_id, pop_pos):
            # Test Collision
            # Kill collided - all in combined function in death module
            death.kill_cell_in_rect(part_pos, part_id, rect, interface)
        """
    if(n % save_at == 0):
        interface.saveData('vtp')
        # sim.saveData('text')
            
print('Simulation Complete\n\n') 
    #     