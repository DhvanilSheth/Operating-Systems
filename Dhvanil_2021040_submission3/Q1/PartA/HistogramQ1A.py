import numpy as np
import matplotlib.pyplot as plt
  
N = 5
nums = np.arange(N) 
width = 0.25

SCHED_OTHER = []
SCHED_RR = []
SCHED_FIFO = []

with open('cache_values.txt') as f:
    for line in f:
        SCHED_OTHER.append(int(line.split()[0]))
        SCHED_RR.append(int(line.split()[1]))
        SCHED_FIFO.append(int(line.split()[2]))
  
bar1 = plt.bar(nums, SCHED_OTHER, width, color = 'r')
bar2 = plt.bar(nums+width, SCHED_RR, width, color='g')
bar3 = plt.bar(nums+width*2, SCHED_FIFO, width, color = 'b')
  
plt.xlabel("Priorites")
plt.ylabel("Time in ms")
plt.title("Histogram")
  
plt.xticks(nums+width,['0-50-50', '1-60-60', '2-70-70', '3-80-80', '4-90-90'])
plt.legend( (bar1, bar2, bar3), ('SCHED_OTHER', 'SCHED_RR', 'SCHED_FIFO') )
plt.show()