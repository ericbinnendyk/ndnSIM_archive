import numpy as np
from matplotlib import pyplot as plt

# This script plots the frequency of interests per second over time.

f1 = open(input("Enter the name of file 1:"), 'r')
l1 = [0 if x == '\n' else int(x.strip()) for x in f1][:-1]
l1 = np.array([sum(l1[i:i + 9]) for i in range(len(l1) - 9)])
f1.close()

f2 = open(input("Enter the name of file 2:"), 'r')
l2 = [0 if x == '\n' else int(x.strip()) for x in f2][:-1]
l2 = np.array([sum(l2[i:i + 9]) for i in range(len(l2) - 9)])
f2.close()

x = np.arange(1, 1 + len(l1)/10, step=1/10)

'''plt.title("Interest frequency with and without IFA")
plt.xlabel("Time since start of attack (seconds)")
plt.ylabel("Number of interests per second arriving at producer")
plt.plot(x, l1)
plt.plot(x, l2)
plt.legend(["/prefix/typeIII interests without IFA", "/prefix/typeIII interests with IFA"])
plt.show()
'''

plt.title("Malicious interest frequency with token bucket")
plt.xlabel("Time since start of attack (seconds)")
plt.ylabel("Number of interests per second arriving at producer")
plt.plot(x, l1)
plt.plot(x, l2)
plt.legend(["/prefix/forged interests without mitigation", "/prefix/forged interests with mitigation"])
plt.show()
