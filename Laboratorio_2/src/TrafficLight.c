#include <avr/io.h>
#include <avr/interrupt.h>

///////////////////// Variables Globales /////////////////////
// Estados
int estado = 0; // Estado actual

// Contador de 0.250 s
int contador_cuartoDsegundo = 0; // Cuenta cuartos de segundo

// Botón presionado: (0: Falso) / (1: Verdadero)
int button_pressed = 0;
//////////////////////////////////////////////////////////////

int main(void)
{ 

////////////////////////////////////////// SETUP //////////////////////////////////////////
  // Habilita las interrupciones globales
  sei(); 

  // Configuracion del puerto
  // Salidas: 1 (PB4, PB3, PB2, PB1)=(LDPV, LDVD, LDPP, LDPD). Página 70 Datasheet	
  DDRB = 0x1E;

  // Colocar en alto (1) o bajo las salidas (0).  Página 70 Datasheet
  PORTB = 0x00;  
  
  // Configuración: Interrupción externa por Botón B1
  GIMSK |= (1 << INT0 ) ; // Se habilita la interrupcion por INT0: Botón B1. Página 52 Datasheet
  MCUCR = 0x0000; // INT0 se dispara con un nivel bajo: 0V. Página 69 y 51 Datasheet
  
  // Configuración: Prescaler - Registro TCCR0B
  TCCR0B |= 0x05; // TCCR0B[2:0] = 101 // (clkI/O / 1024)^-1: (1Mhz / 1024)^-1 = 1.024ms. Página 85 y 86 Datasheet

  // Configuración: Interrupción interna por Tiempo
  OCR0A = 244;  // Contador para alcanzar 0.249856 s aproximadamente 0.250 s, 8 bits: 0-255 // 1.024 ms * 244 = 0.249856 s. Página 86 Datasheet
  TCCR0A |= 0x00; // TCCR0A[7:6]= [0 0]: Normal port operation. OC0A disconnected // Página 82 Datasheet
  TIMSK |= 0x01;//0x02;//0x01 ; // Se pone en 1 el bit de interrupción OCIE0A ligado a TIFR // Página 87 Datasheet
///////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////// LOOP ///////////////////////////////////////////

  // Luces de semáforo peatonal y vehicular
  while (1) {

    //***************// FMS //***************//
    switch(estado){
    
      // PORTB: [0 0 0 LDPV LDVD LDPP LDPD 0]
    
    //////////////// Estado: 0 ////////////////
      
      case 0:
        
        PORTB = 0x12; // LED ON: LDPV y LDPD
        
        break;

    /////////////// Estado: 1 ////////////////  

      case 1:

      // contador_cuartoDsegundo == 12: 2.998272 s : 3 s
      if (contador_cuartoDsegundo == 12) {

        // Restablecer condiciones iniciales
        button_pressed = 0;
        contador_cuartoDsegundo = 0;

        // Cambio de estado
        estado += 1;  // Pasa al siguiente estado

      }
      else {

        // Cada 0.5 s conmuta el estado del LDPV: Parpadea el LED (XOR)
        if ((contador_cuartoDsegundo % 2) == 0) PORTB ^= (1 << PB4);
      
      }

      break;

    /////////////// Estado: 2 ////////////////

      case 2:
        
        PORTB = 0x0A; // LED ON: LDVD y LDPD
        
        // contador_cuartoDsegundo == 4: 0.999424 s : 1 s
        if (contador_cuartoDsegundo == 4) {
          
          // Restablecer condiciones iniciales
          button_pressed = 0;
          contador_cuartoDsegundo = 0;

          // Cambio de estado
          estado += 1;  // Pasa al siguiente estado

        }

        break;
    
    /////////////// Estado: 3 ////////////////

      case 3:
        
        PORTB = 0x0C; // LED ON: LDVD y LDPP
        
        // contador_cuartoDsegundo == 40: 9.99424 s : 10 s
        if (contador_cuartoDsegundo >= 40) {

          // Restablecer condiciones iniciales
          button_pressed = 0;
          contador_cuartoDsegundo = 0;
          
          // Cambio de estado
          estado += 1;  // Pasa al siguiente estado
        }

        break;

    /////////////// Estado: 4 ////////////////

      case 4:

        // contador_cuartoDsegundo == 12: 2.998272 s : 3 s
        if (contador_cuartoDsegundo == 12) {

          // Restablecer condiciones iniciales
          button_pressed = 0;
          contador_cuartoDsegundo = 0;

          // Cambio de estado
          estado += 1;  // Pasa al siguiente estado

        }
        else {

          // Cada 0.5 s conmuta el estado del LDPP: Parpadea el LED (XOR)
          if ((contador_cuartoDsegundo % 2) == 0) PORTB ^= (1 << PB2); 
        
        }

        break;
      
    /////////////// Estado: 5 ////////////////

      case 5:
      
      PORTB = 0x0A; // LED ON: LDVD y LDPD
      
      // contador_cuartoDsegundo == 4: 0.999424 s : 1 s
      if (contador_cuartoDsegundo == 4) {
        
        // Restablecer condiciones iniciales
        button_pressed = 0;
        contador_cuartoDsegundo = 0;

        // Cambio de estado
        estado = 0;  // Vuelve al estado inicial

      }

      break;

    /////////////// Estado por Defecto ////////////////
      default: 
        
        // Por defecto Estado 0
        estado = 0;

        break;
    ///////////////////////////////////////////////////

    }
    //***************************************//

  }

///////////////////////////////////////////////////////////////////////////////////////////
}

// Subrutina de interrupción Botón B1/B2
ISR(INT0_vect) {
  
  // Solo contemplo válido cuando se oprime el botón en el Estado 0
  if (estado == 0) button_pressed = 1;
  else button_pressed = 0;

}

// Subrutina de interrupción TIMER0
ISR(TIMER0_COMPA_vect) {

  ///////////////////////////// Estado 0 ////////s/////////////////////
  // contador_cuartoDsegundo == 40: 9.99424 s : 10 s
  if ((estado == 0) && (button_pressed == 1) && (contador_cuartoDsegundo >= 40)) {
    // Restablecer condiciones iniciales
    button_pressed = 0;
    contador_cuartoDsegundo = 0;
    // Cambio de estado
    estado += 1;
  }
  ////////////////////////////////////////////////////////////////////
  else contador_cuartoDsegundo += 1;
  ////////////////////////////////////////////////////////////////////

}