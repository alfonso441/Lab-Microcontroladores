#!/usr/bin/python3

import serial

ser = serial.Serial('/tmp/ttyS1', 9600)
file = open('Incubadora_Data.csv','w+')

print("Conectado a puerto: " + ser.portstr)

line = []

while True:
    for c in ser.read():
        c=chr(c)
        print(c)
        line.append(c)
        if c == '\n':
            print(''.join(line))
            str = ''.join(line)
            file.write(str)

            
            
