import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
import time

res_data = pd.read_csv("test_out.csv")
print(res_data)

# Нормализованные L2 абсолютных погрешностей
#fig = plt.figure(figsize=(15,8))
#plt.plot(res_data["step"], res_data["l2_euler_normalized"])
#plt.plot(res_data["step"], res_data["l2_heun_normalized"])

# L2 абсолютных погрешностей
#fig2 = plt.figure(figsize=(15,8))
#plt.plot(res_data["step"], res_data["l2_euler"])
#plt.plot(res_data["step"], res_data["l2_heun"])


# Нормализованные L2 относительных погрешностей
fig3, ax3 = plt.subplots()
ax3.plot(res_data["step"], res_data["l2_euler_re_normalized"], label = 'euler')
ax3.plot(res_data["step"], res_data["l2_heun_re_normalized"], label = 'heun')
ax3.set_xlabel('h, (с)')
ax3.set_ylabel('Q(h), (б/р)')
ax3.grid(axis = 'both')
ax3.legend(loc='upper left')

# L2 относительных погрешностей
#fig4 = plt.figure(figsize=(15,8))
#plt.plot(res_data["step"], res_data["l2_euler_re"])
#plt.plot(res_data["step"], res_data["l2_heun_re"])

plt.show()
