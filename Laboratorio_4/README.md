# Laboratorio 4

Este laboratorio consiste en una estación meteorológica usando un Arduino MEGA, una pantalla LCD, sensores de temperatura, humedad, luz, velocidad del viento, lluvia, nivel de batería, potenciómetros, servomotores, LEDs y resistores. Además, se emplea IoT para enlazar el Arduino MEGA con ThingsBoard. Mayor detalle en el documento que se encuentra en la carpeta Informe.

## Crear puertos en Linux

Para crear los puertos para comunicarse desde la PC con el Arduino se debe ejecutar el siguiente comando para ejecutar el script de bash que se encuentra en la carpeta ./src, esto en una terminal que debe dejarse abierta.

```
$ ./PuertosUSART.sh
```

## Crear archivo binario

Incluir las bibliotecas de la carpeta ./libraries en el Arduino IDE.

Abrir el archivo ./src/EstacionM/EstacionM.ino en Arduino IDE.
En el Arduino IDE ir a la pestaña "Sketch/Export compiled Binary". Esto creará el archivo binario .hex en la misma carpeta que el archivo .ino. 

## Cargar firmware al circuito en SIMULIDE

Abrir el circuito ./src/EstacionM.simu en el simulador SIMULIDE. En el Arduino MEGA hacer "Click derecho/Cargar Firmware"

## Ejecutar el circuito con enlace de comunicación a PC & ThingsBoard

Activar el puerto UART en SIMULIDE.

Si se desea iniciar con la comunicación serial desde el inicio, activar el switch correspondiente para poner en alto el pin 8.

Abrir en una terminal el script de Python que se encuentra en la carpeta ./src

```
$ ./ThingsBoard.py
```

Ejecutar el circuito y manipular valores de los sensores para pruebas.

Finalmente, diríjase a la página ThingsBoard del curso en donde se encuentra inscrito y abra el dashboard correspondiente.

Esto hará que se impriman datos en el monitor serial del simulador, en la terminal y en un dashboard en la página de ThingsBoard del curso, se requieren permisos. Si desea puede modificar el archivo ThingsBoard.py para redirigir los datos a su propio dashboard en caso de no tener acceso al utilizado en este ejemplo.

