from tqdm import tqdm
#import matplotlib.pyplot as plt
#import numpy as np
import Debug.pycapmd as pycapmd # this can be changed to build.pycapmd when availiable
import parameters as p
import plotter

resolution = (500, 500)
parameterFile = "parameters.json"
params = p.paramsFromFile(pycapmd.Parameters(), parameterFile)

sim = pycapmd.simulation(params)
sim.initialise()

p = sim.getParticle(0)
p.getPosition()

N = sim.popSize()
NB = sim.getBoundarySize()
a = range(NB, N+NB)
parts = pycapmd.VectorInt(a)

N = sim.popSize()
#plotter.testDrawWindow("TEST")
print("[INFO] - Animation Started")
end = False
for t in tqdm(range(2000)):
    #break
    if int(t)%100 == 0:
        sim.populationDynamics(100)
    sim.move(t) # n is current time step. weird
    part_pos = sim.getPopulationPosition(parts)
    part_rad = sim.getPopulationRadius(parts)
    plotter.animPlot(part_pos, part_rad, params, resolution)
    plotter.checkPause("p")
    end = plotter.cleanExit("q")
    if end:
        break
print("[INFO] - Animation End")
plotter.endAnimation()