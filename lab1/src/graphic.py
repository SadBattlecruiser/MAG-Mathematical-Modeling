import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

file_path = "..\out.csv"
data = pd.read_csv(file_path)

# Графики температур
fig_abs, ax_abs = plt.subplots()
ax_abs.plot(data["time"], data["euler"], label = 'euler')
ax_abs.plot(data["time"], data["heun"], label = 'heun')
ax_abs.plot(data["time"], data["analytic"], label = 'analytic')
ax_abs.set_xlabel('время (с)')
ax_abs.set_ylabel('Температура тела, (град.)')
ax_abs.grid(axis = 'both')
ax_abs.legend(loc='upper left')

# Графики относительных ошибок
data["euler rel_err"] = (data["analytic"] - data["euler"]) / data["analytic"]
data["heun rel_err"] = (data["analytic"] - data["heun"]) / data["analytic"]

fig_err, ax_err = plt.subplots()
ax_err.plot(data["time"], data["euler rel_err"], label = "euler")
ax_err.plot(data["time"], data["heun rel_err"], label = "heun")
ax_err.set_xlabel('время (с)')
ax_err.set_ylabel('Относительная погрешность, (б/р)')
ax_err.grid(axis = 'both')
ax_err.legend(loc='upper left')

plt.show()

print(data)
