#!/usr/bin/env python3
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy
import chigger
import mooseutils

T_exact = mooseutils.VectorPostprocessorReader('1d_analytical_out_T_exact_*.csv')
T_sim = mooseutils.VectorPostprocessorReader('1d_analytical_out_T_simulation_*.csv')
x = T_exact['x']

fig = plt.figure(dpi=300)
fig.subplots_adjust(right=0.85)
ax = fig.add_subplot(111, xlabel='x [m]', ylabel='T [K]', ylim=[300,350])
l_sim = ax.plot(x,[0]*len(x), 'ob', label='simulation', markersize=3, markeredgewidth=0.5, markerfacecolor=[1]*3, markevery=5)[0]
l_exact = ax.plot(x,[0]*len(x), '-k', label='exact', linewidth=1)[0]
plt.legend(loc='upper left')
ax.grid(color=[0.8]*3)

ax2 = ax.twinx()
ax2.set(ylabel="Absolute error [K]")
ax2.yaxis.set_major_locator(ticker.LinearLocator(numticks=6))
ax2.ticklabel_format(axis='y', style='scientific', scilimits=(0,0))
l_err = ax2.plot(x,[0]*len(x), '-r', label='error')[0]
plt.legend(loc='upper center')

ax_pos = None
n = len(T_exact.times())
for i, t in enumerate(T_exact.times()):
    ax.set(title='time = {:3d} sec.'.format(t))
    print('t = {} of {}\r'.format(t, n), end='')
    T_exact.update(time=t)
    T_sim.update(time=t)
    l_exact.set_ydata(T_exact['T_exact'])
    l_sim.set_ydata(T_sim['T'])
    l_err.set_ydata(numpy.array(T_exact['T_exact']) - numpy.array(T_sim['T']))

    ax2.relim()
    ax2.autoscale_view()
    fig.savefig("1d_analytical_{:04d}.png".format(i))

# Leave this commented out, don't want to worry about setting up ffmpeg on test machines
# chigger.utils.img2mov('1d_analytical_*.png', '1d_analytical.mp4', duration=20, overwrite=True)
