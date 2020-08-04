import sys as system
system.path.insert(1,'/home/sh18581/Documents/Dome/CAPMD/build')

import pycapmd
import parameters as p


parameterFile = "parameters.json"
params = p.paramsFromFile(pycapmd.Parameters(), parameterFile)

sim = pycapmd.simulation(params)
sim.initialise()

N = sim.popSize()
NB = sim.getBoundarySize()

nmax=1000
for n in range(nmax):
	if n%100 ==0:
		sim.saveData('vtp')
		sim.saveData('text')
	sim.move()
