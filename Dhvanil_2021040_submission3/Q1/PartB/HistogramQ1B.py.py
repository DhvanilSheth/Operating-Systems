import matplotlib.pyplot as plt
import numpy as np

value1 = 13*60 + 37
value2 = 9*60 + 44
value3 = 9*60 + 34

plt.bar(1, value1, color='red')
plt.bar(2, value2, color='green')
plt.bar(3, value3, color='blue')
plt.xlabel("RUN NUMBER ")
plt.ylabel("TIME IN SECONDS ")
plt.show()
