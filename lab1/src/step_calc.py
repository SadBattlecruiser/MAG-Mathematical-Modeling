import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
import time

prog_link = '..\out\main.exe'
config_link = 'config.txt'
out_link = 'out.csv'

time_end = 100.
time_beg = 0.
n_steps_arr = np.arange(51, 11, -2)
# Для каждого количества шагов вычисляем величину шага и нормы
res_data = pd.DataFrame({'n_steps': [0],'step': [0.], 'l2_euler': [0.], 'l2_euler_normalized': [0.],
    'l2_heun': [0.], 'l2_heun_normalized': [0.], 'l2_euler_re' : [0.], 'l2_euler_re_normalized' : [0.],
    'l2_heun_re' : [0.], 'l2_heun_re_normalized' : [0.]})
for n_steps in n_steps_arr:
    # Создаем конфиг
    print('n_steps=' + str(n_steps))
    f = open(config_link, 'w')
    print('open')
    f.write('solv_names=euler,heun\n')
    f.write('gam_type=1\n')
    f.write('gam=0.1\n')
    f.write('T_s=25.\n')
    f.write('T_beg=150.\n')
    f.write('time_beg=' + str(time_beg) +'\n')
    f.write('time_end=' + str(time_end)+ '\n')
    f.write('n_steps=' + str(n_steps) + '\n')
    f.write('out_file_name=out.csv\n')
    f.write('csv_dlm=,\n')
    f.close()
    print('close')
    # Почему-то если не паузить, ему становится плохо. Видно, что-то с чтением и записью
    time.sleep(0.3)
    # Величина шага
    step = (time_end - time_beg) / n_steps
    # Вызываем решатель
    os.startfile(prog_link)
    # Считываем результат
    data = pd.read_csv(out_link, delimiter=',')
    # Вычисляем L2 нормы
    l2_euler = np.linalg.norm((data["analytic"] - data["euler"]).to_numpy(), ord=2)
    l2_euler_normalized = l2_euler / np.sqrt(n_steps)
    l2_heun = np.linalg.norm((data["analytic"] - data["heun"]).to_numpy(), ord=2)
    l2_heun_normalized = l2_heun / np.sqrt(n_steps)
    # Относительные ошибки и их нормы
    data["euler rel_err"] = (data["analytic"] - data["euler"]) / data["analytic"]
    data["heun rel_err"] = (data["analytic"] - data["heun"]) / data["analytic"]
    l2_euler_re = np.linalg.norm(data["euler rel_err"].to_numpy(), ord=2)
    l2_euler_re_normalized = l2_euler_re / np.sqrt(n_steps)
    l2_heun_re = np.linalg.norm(data["heun rel_err"], ord=2)
    l2_heun_re_normalized = l2_heun_re / np.sqrt(n_steps)
    # И докидываем всё в данные
    print(data)
    res_data.loc[len(res_data)] = [n_steps, step, l2_euler, l2_euler_normalized, l2_heun, l2_heun_normalized,
        l2_euler_re, l2_euler_re_normalized, l2_heun_re, l2_heun_re_normalized]
print(res_data)
res_data.to_csv("test_out.csv")

os.remove(config_link)
os.remove(out_link)
