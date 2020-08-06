from tqdm import tqdm
#import matplotlib.pyplot as plt
import numpy as np
import Debug.pycapmd as pycapmd # this can be changed to build.pycapmd when availiable
import parameters as p
import plotter

resolution = (500, 500)
parameterFile = "parameters.json"
params = p.paramsFromFile(pycapmd.Parameters(), parameterFile)

sim = pycapmd.simulation(params) # interface
sim.initialise()

p = sim.getParticle(0)
p.getPosition()

N = sim.popSize()
NB = sim.getBoundarySize()
a = range(NB, N+NB)
part_ids = pycapmd.VectorInt(a)

N = sim.popSize()
#plotter.testDrawWindow("TEST")
print("[INFO] - Animation Started")
end = False

rect = None
for t in tqdm(range(1000)):
    #break
    if int(t)%100 == 0:
        sim.populationDynamics(100)
        rect = np.random.randint(0,resolution[0],(2,2)) # generates a random rect of the size of the screen
    num_particles = sim.popSize()
    index = list(range(NB, num_particles + NB))
    #part_ids = []
    #for idx in index:
        #part_ids.append(sim.getParticle(idx))
        #print(sim.getParticle(idx))
    #print(part_ids)
    part_ids = pycapmd.VectorInt(index)
    sim.move(t) # n is current time step. weird
    part_pos = sim.getPopulationPosition(part_ids)
    part_rad = sim.getPopulationRadius(part_ids)
    kill_list = plotter.animPlot(part_pos, part_rad, index, params, resolution, rect)
    #print(kill_list)
    plotter.checkPause("p")
    end = plotter.cleanExit("q")
    if end:
        break
print("[INFO] - Animation End")
plotter.endAnimation()