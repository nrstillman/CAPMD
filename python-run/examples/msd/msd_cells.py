"""
Steps towards simulation
# 1 start up sim with params specified in json
# 2 run simulation with moving particles
# 3 activate the zapping sequence
# 4 continue simulation run with new cell types
# Output is saved as vtp files for paraview
"""
import pycapmd as capmd
import parameters as p

from tqdm import tqdm # used for printing progress bar

from numpy import random # used to randomly choose cells

parameterFile = "parameters_MSD.json"
params = p.paramsFromFile(capmd.Parameters(), parameterFile)

sim = capmd.interface(params)

N = sim.popSize()
NB = sim.getBoundarySize()

# Choosing the opposing corners of the rectangle to be killed
lx = params.Lx
ly = params.Ly

nmax = params.t_final
# zap_time = 1000
save_at = params.output_time 
# steps_between_check = 100 # <- used for checking death/division
Nzaplist = 50

for n in tqdm(range(nmax)):

    sim.move()

    if(n % save_at == 0):
        sim.saveData('vtp')
        sim.saveData('text')
            
print('Simulation Complete\n\n') 
