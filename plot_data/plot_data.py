import serial as serial
import pandas as pandas
import time
import matplotlib.pyplot as plt
import csv
import math
import os

def main():
    file_idle=open("idle_motor.txt",'r')
    file_shake=open("shake_motor.txt",'r')
    file_idle=open("data0_idle.txt",'r')
    file_shake=open("data1_shake.txt",'r')
    """
    file_side_left=open("side_left.txt",'r')
    file_side_right=open("side_right.txt",'r')
    file_side_away=open("side_away.txt",'r')
    file_side_in=open("side_inwards.txt",'r')
    file_flat_positive=open("flat_positive.txt",'r')
    file_flat_negative=open("flat_negative.txt",'r')
    side_left_raw=file_side_left.readlines()
    side_right_raw=file_side_right.readlines()
    side_away_raw=file_side_away.readlines()
    side_in_raw=file_side_in.readlines()
    flat_positive_raw=file_flat_positive.readlines()
    flat_negative_raw=file_flat_negative.readlines()
    side_right_V=joint_vector(side_right_raw)
    side_left_V=joint_vector(side_left_raw)
    side_in_V=joint_vector(side_in_raw)
    side_away_V=joint_vector(side_away_raw)
    flat_positive_V=joint_vector(flat_positive_raw)
    flat_negative_V=joint_vector(flat_negative_raw)
    plt.figure()
    plt.plot(side_right_V,label="SD")
    plt.plot(side_left_V,label="SL")
    plt.plot(side_away_V,label="SA")
    plt.plot(side_in_V,label="SI")
    plt.plot(flat_positive_V,label="FP")
    plt.plot(flat_negative_V,label="FN")
    plt.legend()
    plt.show()
    """
    idle_data_raw=file_idle.readlines()
    shake_data_raw=file_shake.readlines()
    idle_float,med_idle_x,med_idle_y,med_idle_z=joint_vector(idle_data_raw)
    shake_float,med_shake_x,med_shake_y,med_shake_z=joint_vector(shake_data_raw)
    idle_V=[]
    shake_V=[]
    idle_float_offset=[]
    shake_float_offset=[]
    for line in idle_float:
        new_line=[line[0]-med_idle_x,line[1]-med_idle_y,line[2]-med_idle_z]
        idle_float_offset.append(new_line)
    for line in shake_float:
        new_line=[line[0]-med_idle_x,line[1]-med_idle_y,line[2]-med_idle_z]
        shake_float_offset.append(new_line)
    with open("offset_idle.txt",'w') as idle_file:
        for line in idle_float_offset:
            idle_file.write(" ".join(str(numb) for numb in line))
            idle_file.write('\n')
    with open("offset_shake.txt",'w') as idle_file:
        for line in shake_float_offset:
            idle_file.write(" ".join(str(numb) for numb in line))
            idle_file.write('\n')
    for line in idle_float_offset:
        idle_V.append(math.sqrt(line[0]*line[0]+line[1]*line[1]+line[2]*line[2]))
    for line in shake_float_offset:
        shake_V.append(math.sqrt(line[0]*line[0]+line[1]*line[1]+line[2]*line[2]))
    print(med_idle_x)
    print(med_shake_x)
    print(med_idle_y)
    print(med_shake_y)
    print(med_idle_z)
    print(med_shake_z)
    window_size=int(input("Enter window size:"))
    user_mode=input("Enter desired feature(P,V,E):")
    if user_mode=='E':
        shake_features=energy(shake_V,window_size)
        idle_features=energy(idle_V,window_size)
    elif user_mode=='V':
        shake_features=var(shake_V,window_size)
        idle_features=var(idle_V,window_size)
    else:
        shake_features=p2p(shake_V,window_size)
        idle_features=p2p(idle_V,window_size)
    print_grafs=input("Print plots?(y/n):")
    error=0
    for val in idle_features:
        if val<0.0292377 and val>0.000225819:
            error+=1
    print(100-error/len(idle_features))
    error=0
    for val in shake_features:
        if val>0.0292377 or val<0.000225819:
            error+=1
    print(100-error/len(shake_features))
    plt.figure()
    plt.plot(shake_features,label="shake")
    plt.plot(idle_features,label="idle")
    plt.legend()
    plt.figure()
    plt.hist(shake_features,label='shake')
    plt.hist(idle_features,label='idle')
    plt.legend()
    if print_grafs=='y':
        plt.show()
    
def joint_vector(raw_lines):
    result=[]
    med_x=0
    med_y=0
    med_z=0
    for line in raw_lines:
        str_list=list(line.strip().split(" "))
        try:
            float_list=[float(x) for x in str_list]
            med_x+=float_list[0]
            med_y+=float_list[1]
            med_z+=float_list[2]
            result.append(float_list)
        except:
            continue
    med_x=med_x/len(result)
    med_y=med_y/len(result)
    med_z=med_z/len(result)
    return result,med_x,med_y,med_z

def p2p(data,window_size):
    result=[]
    i=0
    data_window=[]
    for x in data:
        i+=1
        data_window.append(x)
        if i>=window_size:
            i=0
            result.append(max(data_window)-min(data_window))
            data_window=[]
    return result

def var(data,window_size):
    result=[]
    i=0
    data_window=[]
    data_window_2=[]
    for x in data:
        i+=1
        data_window.append(x)
        data_window_2.append(x*x)
        if i>=window_size:
            i=0
            second=(sum(data_window)/window_size)
            print(second*second)
            print((sum(data_window_2)/window_size))
            result.append((sum(data_window_2)/window_size)-(second*second))
            data_window=[]
            data_window_2=[]
    return result

def energy(data,window_size):
    result=[]
    i=0
    data_window=[]
    for x in data:
        i+=1
        data_window.append(x*x)
        if i>=window_size:
            i=0
            result.append(sum(data_window))
            data_window=[]
    return result





if __name__ == "__main__":
    main()

