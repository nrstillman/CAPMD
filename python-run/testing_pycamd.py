import sys as system
system.path.insert(1,'/home/sh18581/Documents/Dome/CAPMD/build')

import pycapmd
import parameters as p


parameterFile = "parameters.json"
params = p.paramsFromFile(pycapmd.Parameters(), parameterFile)

# setting some of ours here
params.N = 20
params.Lx = 30
params.Ly = 30
params.poly = 0.3
params.maxZ = 6
params.divrate = 0.1
params.deathrate = 0.01
# debug only! fiddle with cutoff of neighbour list
#params.cutoff = 0.5
#params.bc_opt = "periodic"
#params.ntypes = 1
#params.pairstiff = [[1]]
#params.pairatt = [[0]]



sim = pycapmd.simulation(params)
# This repeats the constructor as written
# sim.initialise()

N = sim.popSize()
NB = sim.getBoundarySize()

nmax=10000
steps_between_check = 100
for n in range(nmax):
	if n%100 ==0:
		print('step ' + str(n))
		sim.saveData('vtp')
		sim.saveData('text')
	sim.move()
	# attempt to divide and die
	# steps_between_check*dt*div_rate < 1 for it to make sense mathematically
	if n%steps_between_check ==0:
		sim.populationDynamics(steps_between_check)
