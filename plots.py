import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import sys

if len(sys.argv) != 2: raise RuntimeError("Missing file argument")
path = sys.argv[1]
filename = path.split('/')[-1]
data = pd.read_csv(path)
minthreads = 2
name = filename.split('.')[0]

if ('Philosophe.csv' in path): minthreads = 1
maxthreads = data['nthreads'].max()
time_frame = pd.DataFrame(data=np.zeros((maxthreads - minthreads + 1, 2)), columns=['mean', 'std'])
for i in time_frame.index:
    sub = data[data['nthreads']==i+minthreads].values
    time_frame.loc[i] = (np.mean(sub[:, 1]), np.std(sub[:, 1]))
fig1 = plt.figure()
threads = [i for i in range(minthreads, maxthreads + 1)]

plt.errorbar(
    threads, time_frame['mean'].values, time_frame['std'], ecolor='black',
    capsize=5.0, fmt='o')
plt.grid(True)
plt.title(('Execution time of \"{}\" depending on the number of threads').format(name),fontsize=10)
plt.xlabel('Number of threads')
plt.ylabel('Execution time (s)')
plt.xlim(0, 66)
plt.savefig('plots/{}.pdf'.format(name))
plt.close()