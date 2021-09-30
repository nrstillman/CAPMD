import sys
sys.path.append('../../build/') # This will be replaced by a called function in later releases
sys.path.append('../../util/') # This will be replaced by a called function in later releases

import json
import numpy as np
import pycapmd as capmd

from tqdm import tqdm # used for printing progress bar

def paramsFromFile(paramObj, fileName):
    paramObjCopy = paramObj
    try:
        with open(fileName) as jsonFile:
            parameters = json.load(jsonFile)
            for attribute in parameters:
                setattr(paramObjCopy, attribute, parameters[attribute])
            return paramObjCopy
    except Exception as e:
        print(e)
        print("Incorrect json format, returning default parameters")
        return paramObj
    
def partPosFromFile(filename):
     array = np.loadtxt(filename, delimiter="\t", skiprows = 1)
     return array
     
def main():

    parameterFile = "parameters.json"
    params = paramsFromFile(capmd.Parameters(), parameterFile)
    
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