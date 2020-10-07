import sys
sys.path.append('../../build/') # This will be replaced by a called function in later releases
sys.path.append('../../util/') # This will be replaced by a called function in later releases

import readParams
import pycapmd as capmd

from tqdm import tqdm # used for printing progress bar

def main():

    parameterFile = "parameters.json"
    params = readParams.fromJSON(capmd.Parameters(), parameterFile)

    # setting some of ours here
    params.N = 20
    params.Lx = 30
    params.Ly = 30
    params.poly = 0.3
    params.maxZ = 6
    params.divrate = 0
    params.deathrate = 0

    sim = capmd.interface(params)

    N = sim.popSize()
    NB = sim.getBoundarySize()

    nmax = params.t_final

    save_at = params.output_time 
    steps_between_check = params.popdynfreq 
    Nzaplist = 50

    for n in tqdm(range(nmax)):

        sim.move()

        if (n % save_at == 0):
            sim.saveData('vtp')
            sim.saveData('text')
                
    print('Simulation Complete\n\n') 


if __name__ == "__main__":

    main()