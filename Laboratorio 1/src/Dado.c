#include <pic14/pic12f675.h>
 
//To compile:
//sdcc -mpic14 -p16f675 blink.c
 
//To program the chip using picp:
//Assuming /dev/ttyUSB0 is the serial port.
 
//Erase the chip:
//picp /dev/ttyUSB0 16f887 -ef
 
//Write the program:
//picp /dev/ttyUSB0 16f887 -wp blink.hex
 
//Write the configuration words (optional):
//picp /dev/ttyUSB0 16f887 -wc 0x2ff4 0x3fff
 
//Doing it all at once: erasing, programming, and reading back config words:
//picp /dev/ttyUSB0 16f887 -ef -wp blink.hex -rc
 
//To program the chip using pk2cmd:
//pk2cmd -M -PPIC16f887 -Fblink.hex

///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Set GP3 como I/O Digital (0) y NO Reset(MCLRE CONFIG[5]) (1)
// Se deshabilita Watch Dog Timer, para evitar que se encicle
typedef unsigned int word ;
word __at 0x2007 __CONFIG = ( _WDT_OFF & _MCLRE_OFF );
 
void delay (unsigned int tiempo);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
 
void main(void)
{
	// Configuración de registros
	TRISIO = 0b00010000; // Poner todos los pines como salidas, menos el GP4 para el botón (entrada)
	ANSEL &= 0xF0; // Si se usa pines de entrada, como Analógico (1) o *Digital (0)* (AN3-AN0 como Digital)
	
	// LED OFF: Todos
	GPIO = 0x00; // Poner pines en bajo
	
    unsigned int time = 1000; // Delay para los LEDs ON
    unsigned int number = 1; // Número de dado
 
    // Loop forever
    while ( 1 )
    {		
			/////////////////////////////////////
			// Pines de los LEDs (Outputs)
			//GP0 -> LED: A y G
			//GP1 -> LED: B y F
			//GP2 -> LED: C y E
			//GP5 -> LED: D

			// Pin del botón (Input)
			//GP4 -> Botón: Se activa en alto (5V)
			/////////////////////////////////////

			/////////////////////////////////////
			// Nombre/Posición de LEDs en el dado

			//		A				B
			//		C		D		E
			//		F				G

			/////////////////////////////////////

			/////////////////////////////////////
			// Sección para generar número random
			// Botón SIN Presionar
			while (GP4 == 0) 
			{	
				if (number == 6) number = 1;
				else number += 1;
			}
			/////////////////////////////////////

			/////////////////////////////////////
			// Botón Presionado
			if (GP4 == 1) {
				
				switch (number)
				{
				case 1:
						// LED ON: D
						GP5 = 1; // D
						break;
				case 2:
						// LED ON: A y G
						GP0 = 1; // A y G
						break;
				case 3:
						// LED ON: A y G, D
						GP0 = 1; // A y G
						GP5 = 1; // D
						break;
				case 4:
						// LED ON: A y G, B y F
						GP0 = 1; // A y G
						GP1 = 1; // B y F
						break;
				case 5:
						// LED ON: A y G, B y F, D
						GP0 = 1; // A y G
						GP1 = 1; // B y F
						GP5 = 1; // D
						break;
				case 6:
						// LED ON: A y G, B y F, C y E
						GP0 = 1; // A y G
						GP1 = 1; // B y F
						GP2 = 1; // C y E
						break;
				
				default:
						// LED OFF: Todos
						GPIO = 0x00; // Poner pines en bajo
						break;

				}

				// Función para que duren los LEDs un tiempo encendidos
				delay(time);

				// LED OFF: Todos
				GPIO = 0x00; // Poner pines en bajo

			} 
			else {
				// LED OFF: Todos
				GPIO = 0x00; // Poner pines en bajo	
			}
			/////////////////////////////////////

    }
 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Función de retardo que define el tiempo en que los LED permanecen ON
void delay(unsigned int tiempo)
{
	unsigned int i;
	unsigned int j;

	for(i=0;i<tiempo;i++)
	  for(j=0;j<1275;j++);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////