"""
Steps towards simulation
# 1 start up sim with params specified in json
# 2 run simulation with division for X steps
# 3 activate the death rectangle
# 4 run simulaiton with killed cells
# meanwhile saving to vtp file formats
# Export folder of vtp to paraview
"""
import Debug.pycapmd as capmd
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

    sim.move()
    # attempt to divide and die
    # steps_between_check*dt*div_rate < 1 for it to make sense mathematically
    if(n%steps_between_check == 0):
        sim.populationDynamics(steps_between_check)
    if(n == death_time):
        time.sleep(1)
        N = sim.popSize()
        NB = sim.getBoundarySize()
        idxs = capmd.VectorInt(range(NB, N+NB))
        pop_id = sim.getPopulationId(idxs)
        # Alternative ID grab
        #alt_pop_id = []
        pop_pos = sim.getPopulationPosition(idxs)
        for idx in idxs[0:50]:
            part = sim.getParticle(idx)
            part_id = part.getId()
            #alt_pop_id.append(part_id)
            part_pos = pop_pos[idx - NB]
            
            death.kill_cell_in_rect(part_pos, part_id, rect, interface)
            time.sleep(.01)
        """
        for (part_id, part_pos) in zip(alt_pop_id, pop_pos):
            # Test Collision
            # Kill collided - all in combined function in death module
            death.kill_cell_in_rect(part_pos, part_id, rect, interface)
        """
    if(n%save_at == 0):
        sim.saveData('vtp')
        sim.saveData('text')
            
            
        