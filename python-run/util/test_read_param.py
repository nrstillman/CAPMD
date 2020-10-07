import pycapmd as capmd
import parameters as p

import read_param_CAPMD as read_param


parameterFile = "parameters.json"
params = p.paramsFromFile(capmd.Parameters(), parameterFile)

read_param.Param(params)