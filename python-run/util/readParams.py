"""
Read in files and convert to parameter objecets
"""

import json
import numpy as np

def fromJSON(paramObj, fileName):
    print("enter loop")
    paramObjCopy = paramObj
    try:
        with open(fileName) as jsonFile:
            jsonLoad = json.load(jsonFile)
            print("loaded file")
            parameters = jsonLoad["Parameters"]
            for attribute in parameters:
                setattr(paramObjCopy, attribute, parameters[attribute])
                print("loaded attr: ", attribute)
            print("set attributes")
            return paramObjCopy
    except Exception as e:
        print(e)
        print("Incorrect json format, returning default parameters")
        return paramObj
    
def partPosFromFile(filename):
     array = np.loadtxt(filename, delimiter="\t", skiprows = 1)
     return array
     