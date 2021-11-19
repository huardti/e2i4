import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# On charge les données
X = pd.read_excel("Exercices1.xlsx",sheet_name=0,header=0,index_col=0)
# Pour le TP1 on travaillera sur les Array
XArray=pd.DataFrame.to_numpy(X)