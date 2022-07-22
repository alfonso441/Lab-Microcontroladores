#!/usr/bin/python3

import serial

ser = serial.Serial('/dev/ttyACM0', 115200)
file = open('./content/giros_estudiante.csv','w+')

print("Conectado a puerto: " + ser.portstr)

line = []

while True:
    for c in ser.read():
        c=chr(c)
        line.append(c)
        if c == '\n':
            print(''.join(line))
            str = ''.join(line)
            file.write(str)

            
            
