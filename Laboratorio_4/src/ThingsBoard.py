#!/usr/bin/python3

 # Bibliotecas #
import serial
import paho.mqtt.client as mqtt
import json
import ssl, socket
import time

# Variables de conexión #
ser = serial.Serial('/tmp/ttyS1', 9600)
line = []
lineTerminal = []
broker = "iot.eie.ucr.ac.cr"
port = 1883
topic = "v1/devices/me/telemetry"
topicreq = "v1/devices/me/attributes/request/1"
username = "RXQR6dkOnLHe2BiG7uGv"
password = ""

############ Callbacks ############
def on_log(client,userdata,level,buf): # Callback para logging
    print( "log: "+buf)

def on_connect(client,userdata,flags,rc): # Callback cuando se conecta
    if rc ==0:
        print( "connected OK" )
    else:
        print( "Bad connection Returned code=" , rc)

def on_disconnect(client,userdata,flags,rc = 0): # Callback cuando se desconecta
    print( "DisConnected result code "+ str( rc ))

def on_message(client,userdata,msg): # Callback cuando recibe mensaje
    topic = msg . topic
    m_decode = str(msg.payload.decode( "utf-8" , "ignore" ))
    print("message received", m_decode)
###################################

# Estableciendo Conexión #
client = mqtt.Client("PyLab4_B41546")
client.on_connect = on_connect
client.on_disconnect = on_disconnect
client.on_message = on_message
print("Connecting to broker",broker)
client.username_pw_set(username,password)
client.connect(broker,port)
client.subscribe(topic)

########## LOOP ##########
while True:
    for c in ser.read():
        
        c=chr(c)
        line.append(c)
        lineTerminal.append(c)
        
        if c == ':':
            line.remove(c)
            str1 = " { \" "+''.join(line)+ " \" : "
            line=[]
            
        if c == '\n':
            line.remove(c)
            #print(''.join(lineTerminal))
            str2 = str1+"\""+''.join(line)+"\"}"
            print(str2)
            client.publish(topic, str2)
            line=[]
            str1=''
            #time.sleep(4)
##########################