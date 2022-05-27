# Laboratorio 3

Este laboratorio consiste en una incubadora usando un Arduino UNO, una pantalla LCD, sensores de temperatura, humedad, potenciómetro, un calentador, PID, LEDs y resistores. Mayor detalle en el documento que se encuentra en la carpeta Informe.

## Crear puertos en Linux

Para crear los puertos para comunicarse desde la PC con el Arduino se debe ejecutar el siguiente comando en una terminal y dejarla abierta.

`$ socat PTY,link=/tmp/ttyS0,raw,echo=0 PTY,link=/tmp/ttyS1,raw,echo=0`

## Crear archivo binario

Incluir las bibliotecas de la carpeta ./libraries en el Arduino IDE.

Abrir el archivo ./src/Incubadora/Incubadora.ino en Arduino IDE.
En el Arduino IDE ir a la pestaña "Sketch/Export compiled Binary". Esto creará el archivo binario .hex en la misma carpeta que el archivo .ino. 

## Cargar firmware al circuito en SIMULIDE

Abrir el circuito ./src/Incubadora.simu en el simulador SIMULIDE. En el Arduino UNO hacer "Click derecho/Cargar Firmware"

## Ejecutar el circuito con enlace de comunicación a PC

Activar el puerto UART en SIMULIDE.

Si se desea iniciar con la comunicación serial desde el inicio, activar el switch correspondiente para poner en alto el pin 8.

Abrir en una terminal el script de Python ./src/data_saver.py

Finalmente, ejecutar el circuito y manipular valores de los sensores para pruebas.

Esto hará que se impriman datos en el monitor serial del simulador, en la terminal y en un archivo .csv que estará disponible al finalizar el script de Python, siempre y cuando el switch esté activado, de lo contrario no se imprimirá ningún dato en los lugares antes mencionados.

Puede abrir el archivo .csv en una Hoja de cálculo de Excel o LibreOffice.
