#!/usr/bin/python
import os
from subprocess import call

path = 'examples/'
examples = [f for f in os.listdir(path) if not f.startswith('.')]
example_path = path + examples[0] + '/main.py'

call(["python", example_path])
