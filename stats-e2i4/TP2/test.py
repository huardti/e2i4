# %%

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# On charge les données
X = pd.read_excel("Carsmall3.xlsx", sheet_name=0, header=0, index_col=0, engine="openpyxl")

XArray=pd.DataFrame.to_numpy(X)
plt.boxplot(XArray[:,(1,2,3,4,5,6)])
plt.xticks([1, 2, 3, 4, 5, 6], ['Origin', 'Weight', 'MPG' , 'Horsepower' , 'Displacement' , 'Acceleration'])

X.plot
# %%
