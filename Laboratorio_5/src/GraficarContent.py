#!/usr/bin/python3
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

#######################################
# Graficando: Estacionario
#######################################
print("Cargando archivo...")
filename1 = "estacionario.csv"

df1 = pd.read_csv("./content/" + filename1)

index1 = range(1, len(df1['gX']) + 1)

plt.rcParams["figure.figsize"] = (20,10)

print("Graficando datos...")
plot1 = plt.figure(1)
plt.plot(index1, df1['gX'], 'g.', label='x', linestyle='solid', marker=',')
plt.plot(index1, df1['gY'], 'b.', label='y', linestyle='solid', marker=',')
plt.plot(index1, df1['gZ'], 'r.', label='z', linestyle='solid', marker=',')
plt.title("Giroscopio: Estacionario")
plt.xlabel("Muestras #")
plt.ylabel("Giroscopio (deg/sec)")
plt.legend()
#######################################

#######################################
# Graficando: Flex
#######################################
print("Cargando archivo...")
filename2 = "flex.csv"

df2 = pd.read_csv("./content/" + filename2)

index2 = range(1, len(df2['gX']) + 1)

plt.rcParams["figure.figsize"] = (20,10)

print("Graficando datos...")
plot2 = plt.figure(2)
plt.plot(index2, df2['gX'], 'g.', label='x', linestyle='solid', marker=',')
plt.plot(index2, df2['gY'], 'b.', label='y', linestyle='solid', marker=',')
plt.plot(index2, df2['gZ'], 'r.', label='z', linestyle='solid', marker=',')
plt.title("Giroscopio: Flex")
plt.xlabel("Muestras #")
plt.ylabel("Giroscopio (deg/sec)")
plt.legend()
#######################################

#######################################
# Graficando: Giros
#######################################
print("Cargando archivo...")
filename3 = "giros.csv"

df3 = pd.read_csv("./content/" + filename3)

index3 = range(1, len(df3['gX']) + 1)

plt.rcParams["figure.figsize"] = (20,10)

print("Graficando datos...")
plot3 = plt.figure(3)
plt.plot(index3, df3['gX'], 'g.', label='x', linestyle='solid', marker=',')
plt.plot(index3, df3['gY'], 'b.', label='y', linestyle='solid', marker=',')
plt.plot(index3, df3['gZ'], 'r.', label='z', linestyle='solid', marker=',')
plt.title("Giroscopio: Giros")
plt.xlabel("Muestras #")
plt.ylabel("Giroscopio (deg/sec)")
plt.legend()
#######################################
plt.show()
#######################################