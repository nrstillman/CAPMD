import sys
sys.path.append('../../build/') # This will be replaced by a called function in later releases
sys.path.append('../../util/') # This will be replaced by a called function in later releases

import readParams
import pycapmd as capmd

from tqdm import tqdm # used for printing progress bar

def main():

	parameterFile = "parameters.json"
	params = readParams.fromJSON(capmd.Parameters(), parameterFile)

	sim = capmd.interface(params)

	N = sim.popSize()
	NB = sim.getBoundarySize()

	lx = params.Lx
	ly = params.Ly
	nmax = params.t_final

	save_at = params.output_time 
	steps_between_check = params.popdynfreq
	Nzaplist = 50

	for n in tqdm(range(nmax)):

	    sim.move()

        if (n % steps_between_check == 0):
            sim.populationDynamics(steps_between_check)

	    if (n % save_at == 0):
	        sim.saveData('vtp')
	        sim.saveData('text')
	            
	print('Simulation Complete\n\n') 

if __name__ == "__main__":

    main()