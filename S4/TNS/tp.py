import numpy as np
import matplotlib.pyplot as plt
import scipy as sc

Fe = 100 # Hz
duree = 10 # sec

t = np.arange(1/Fe, duree, 1/Fe)

f0 = 20 # Hz

m = np.cos(2*np.pi*f0*t)

T = 0.25 # sec
D = 2 # sec

x = np.zeros(t.size)

for i in range(0, int(duree/D-T)+1):
    x[int(i*Fe*D+1) : int(i*Fe*D+T*Fe)] = 1

s = m * x

plt.figure()

plt.subplot(3,1,1)
plt.plot(t,m)
plt.ylabel("figure 1a")

plt.subplot(3,1,2)
plt.plot(t,x)
plt.ylabel("figure 1b")

plt.subplot(3,1,3)
plt.plot(t,s)
plt.ylabel("figure 1c")

plt.show()


M = 4096
S = sc.fft(s, M)
f = np.arange(0, M)/M*Fe

plt.figure()
plt.plot(f,np.abs(S))
plt.ylabel('figure 2a')
plt.show()


#qestion de l'exam => vaut il mieux prendre beaucoup de pour sur une periode (sur echantillonage)
#                       OU
#                     2 points (shannon) mais beaucoup de periode

