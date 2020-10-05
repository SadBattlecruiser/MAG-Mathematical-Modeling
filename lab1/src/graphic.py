import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

file_path = "..\out.csv"
data = pd.read_csv(file_path)

# Графики температур
fig_abs = plt.figure(figsize=(15,8))
#fig_abs.suptitle('Графики температур', fontsize=16)
plt.plot(data["time"], data["analytic"])
plt.plot(data["time"], data["euler"])
plt.plot(data["time"], data["heun"])
fig_abs.legend(loc='upper left')
#plt.show()
# Графики относительных ошибок
data["euler rel_err"] = (data["analytic"] - data["euler"]) / data["analytic"]
data["heun rel_err"] = (data["analytic"] - data["heun"]) / data["analytic"]
fig_err = plt.figure(figsize=(15,8))
#fig_abs.suptitle('Графики относительных погрешностей', fontsize=16)
plt.plot(data["time"], data["euler rel_err"])
plt.plot(data["time"], data["heun rel_err"])
fig_abs.legend(loc='upper left')
plt.show()

print(data)
