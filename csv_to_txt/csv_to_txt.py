import os
import numpy as np
import pandas as pd

os.chdir(r'C:\Users\danip\OneDrive\Desktop\IST-vibration\Data\corrected_new_motor')
df=pd.read_csv('main_df.csv')
all_states=df['logger_id'].unique()
print(all_states)
i=0
for state_n in all_states:
    x_values=df.query('logger_id==@state_n')['acc_x']
    y_values=df.query('logger_id==@state_n')['acc_y']
    z_values=df.query('logger_id==@state_n')['acc_z']
    file_name="state"+str(state_n)+".txt"
    fp=open(file_name,'w')
    print(x_values)
    for index in range(x_values.size):
        index=index+i
        fp.write(str(x_values[index])+" "+str(y_values[index])+" "+str(z_values[index])+"\n")
    fp.close()
    i=index+1
