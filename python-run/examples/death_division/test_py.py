import sys
sys.path.append('../../build/') # This will be replaced by a called function in later releases
sys.path.append('../../util/') # This will be replaced by a called function in later releases

import readParams
import pycapmd as capmd

from tqdm import tqdm # used for printing progress bar

parameterFile = "parameters.json"
params = readParams.paramsFromFile(capmd.Parameters(), parameterFile)

sim = capmd.interface(params)

N = sim.popSize()
NB = sim.getBoundarySize()

lx = params.Lx
ly = params.Ly
nmax = params.t_final

save_at = params.output_time 
steps_between_check = params.popdynfreq
Nzaplist = 50

sim.move()