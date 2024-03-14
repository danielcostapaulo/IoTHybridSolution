import serial as serial
import pandas as pandas
import time
import matplotlib.pyplot as plt
import csv

def main():
    ser=serial.Serial('COM5',timeout=1)
    stop=time.time()+30
    file_number=0
    filename="data"+str(file_number)+".txt"
    file=open(filename,'w')
    print("gana start")
    while time.time()<stop:
        while ser.inWaiting():
            line=ser.readline().strip().decode('utf-8').split("    ")
            if line[0]=='c':
                file_number+=1
                filename="data"+str(file_number)+".txt"
                file=open(filename,'w')
                print("Changed file")
            for val in line:
                file.write(val)
                file.write(" ")
            file.write("\n")




if __name__ == "__main__":
    main()

