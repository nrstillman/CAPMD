import os
import sys
import json 
import time
import subprocess

import numpy as np
import pandas as pd
import multiprocessing as mp

import calc_summ_stats as sumstats

def sample_params(params, NSamples=8):
    s = []
    for p in  params:
        s.append(np.random.uniform(0,1,NSamples)*p)
        
    return np.array(s)

def update_config(params,values,configfile,n):
    dicts = {}
    print(f"params = {values}")
    for i,p in enumerate(params):
            dicts[p] = values[i]

    with open(configfile, 'r') as j:
         config = json.loads(j.read())

    for key, value in dicts.items():
        try:
            config[key][-1][-1] = value
        except: 
            config[key][-1] = value

    config['filename'] = f"wh_{n}"

    with open(f"../include/config/config{n}.json", 'w') as f:
        json.dump(config, f, indent=4)

    return config['outputfolder'] + config['filename']

def capmd_run(r):

    config_template = "../include/config/wound_temp.json"
    configfile = f'config{r[0]}.json'
    params = ["pairatt","factive","tau"]

    # update json with parameter values
    filename = update_config(params, list(r[1]),config_template,r[0])
    
    # run capmd
    command = "./capmd "  + f'{configfile}'
    subprocess.call(command, shell=True)
    time.sleep(5)

    calc_summ_stats
    output = sumstats.read_all_data("../" + filename[:] + "_*.dat")
    edges = sumstats.calc_edge_speed(output)
    rho = sumstats.calc_final_density(output)

    save parameter values in dataframe
    with open(f'./analysis/sumstats.txt', 'a') as f:
        print(f'{r[0]} \t {edges[0]} \t {edges[1]} \t {rho}', file=f)

    run capmd
    command = "rm -f ../abcout/*"
    subprocess.call(command, shell=True)
    return 0

def main():
    print("Number of processors: ", mp.cpu_count())
    pool = mp.Pool(mp.cpu_count())

    with open(f'./analysis/sumstats.txt', 'w') as f:
        print(f'n \t lhs_edge_speed \t rhs_edge_speed \t rho', file=f)

    # make folder specific to sample run
    if not os.path.isdir(f'./analysis'):
        os.mkdir(f'./analysis')

    configfolder = "../include/config/"
    configfile = 'wound_temp.json'

    # parameter values of interest
    params = ["pairatt","factive","tau"]
    eps = [0.02]
    v0 = [0.1]
    tau = [400]
    param = [eps, v0, tau]

    # create uniform priors from parameters 
    s = np.array(sample_params(param).T)
    # save for later
    df = pd.DataFrame(s, columns=params)
    df.to_csv('params.csv')

    # multiprocessing
    pool.map(capmd_run, df.iterrows())
    print("Batch complete")

if __name__ == "__main__":

    main()
