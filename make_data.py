from sklearn.datasets import *
import numpy as np
import pandas as pd
from matplotlib import pyplot
#used to make blobs to check the model for correctness. Experiment part I
X,y = make_blobs(n_samples = 1000, n_features=2, centers=2)
df = pd.DataFrame(dict(x=X[:,0], y=X[:,1], label=y))
colors = {0:'red', 1:'blue'}
fig, ax = pyplot.subplots()
grouped = df.groupby('label')
for key, group in grouped:
    group.plot(ax=ax, kind='scatter', x='x', y='y', label=key, color=colors[key])
pyplot.show()

df.to_csv("data.csv")