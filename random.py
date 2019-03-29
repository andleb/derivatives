# -*- coding: utf-8 -*-
"""
Created on Fri Mar 29 06:37:35 2019

@author: Andrej Leban
"""

import copy
import collections as coll
import itertools as it
import functools as ft
import operator as op
import re

import sortedcontainers as sc
from blist import blist

import numpy as np
import pandas as pd
import scipy as sp


import csv

a = np.array(list(map(float, list(csv.reader(open("uniforms")))[0])))
plt.hist(a)

plt.figure()
b = np.array(list(map(float, list(csv.reader(open("gaussians")))[0])))
plt.hist(b, bins=100, density=True)