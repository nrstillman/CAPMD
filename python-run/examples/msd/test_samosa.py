import pycapmd as capmd
import parameters as p
import matplotlib.pyplot as plt
import sys
import argparse

import read_param_CAPMD
from Dynamics import *

def calculate_msd():
	parameterFile = "parameters.json"

	parser = argparse.ArgumentParser()
	parser.add_argument("-d", "--directory", type=str, default="./",help="input directory")
	#parser.add_argument("-o", "--output", type=str, help="output directory")
	#parser.add_argument("-p", "--prefix", type=str, default="cornea",help="prefix for output file")
	parser.add_argument("-s", "--skip", type=int, default=1100, help="skip this many samples")
	parser.add_argument("-m", "--howmany", type=int, default=30, help="read this many samples")
	parser.add_argument("-t", "--step", type=int, default=4, help="step snapshots with this spacing in flow field")
	parser.add_argument("-a", "--average", type=int, default=5, help="average over these many samples in flow field")

	args = parser.parse_args()

	param = read_param_CAPMD.Param(p.paramsFromFile(capmd.Parameters(), parameterFile))

	sheet = Dynamics(initype="fromCSV",param=param,datapath='output/',multiopt="many")
	skip = 0
	step = 1
	howmany = 500
	sheet.readDataMany("CAPMD",skip,step,howmany,False,readtypes = [1])
	sheet.validate_initialise()
	sheet.getMSD(False)

	plt.show()
	return 0