from serial import Serial
import time
import os

def main():
    os.chdir(r'Data')
    ser=Serial('COM8',baudrate=115200)
    stop=time.time()+10000
    file_number=0
    filename="data"+str(file_number)+".txt"
    file=open(filename,'w')
    print("gana start")
    line=""
    while time.time()<stop:
        if ser.in_waiting!=0:
            try:
                line_binary=ser.read(ser.in_waiting)
                line+=line_binary.strip().decode('utf-8')
                print(line)
                if line_binary[-1]==10:
                    if line[0]=='c':
                        file.close()
                        file_number+=1
                        filename="data"+str(file_number)+".txt"
                        file=open(filename,'w')
                        print("Changed file")
                    if(line[0]=='*'):
                        for val in line:
                            if val!='*':
                                file.write(val)
                        file.write("\n")
                    line=""
            except:
                print("Couldent decode line")
    file.close()




if __name__ == "__main__":
    main()

