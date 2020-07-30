"""
parameter class from json
"""

import json

def paramsFromFile(paramObj, fileName):
    paramObjCopy = paramObj
    try:
        with open(fileName) as jsonFile:
            jsonLoad = json.load(jsonFile)
            parameters = jsonLoad["Parameters"]
            for attribute in parameters:
                setattr(paramObjCopy, attribute, parameters[attribute])
            return paramObjCopy
    except:
        print("Incorrect json format, returning default parameters")
        return paramObj