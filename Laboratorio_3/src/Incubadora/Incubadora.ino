/*
 * Incubadora - Lee temperatura, humedad y controla temperatura con PID y Resistor. 
 *              Imprime datos en LCD PCD8544. Comunicación serial con PC + Python.
 *              Arduino UNO.
 *
 * Autor:   Alfonso Castillo Orozco
 * Año:     2022
 * Versión: 1.0
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////
//                                     Bibliotecas                                            //
////////////////////////////////////////////////////////////////////////////////////////////////

#include <PCD8544.h>
#include <PID_v1.h>

////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Variables Globales                                        //
////////////////////////////////////////////////////////////////////////////////////////////////

// *** Pines de Arduino *** //
static const byte setTempPin      = A0; // Temperatura referencia del pin
static const byte sensorPin       = A1; // Temperatura sensada del pin
static const byte HumedadPin      = A2; // Humedad sensada del pin
static const byte PC              = 8;  // Comunicación con la PC: 1-activo / 0-inactivo
static const byte PWM_ResistorPin = 11; // Pin PWM controlar resistencia calentadora
static const byte ledBLUEPin      = 12; // LED Azul, por debajo de 30 °C
static const byte ledREDPin       = 13; // LED Rojo, por encima de 42 °C

// *** PID *** //
// (Setpoint, Input, Output) : (TempS, TempR, HumeR) : (Temp Ref, Temp Leída, Hume Leída) 
double Setpoint, Input, Output;
// Parámetros de sintonización
double Kp=2, Ki=3, Kd=1; 
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// *** LCD *** //
// Nombre de módulo
static PCD8544 lcd;
// Dimensiones de LCD (pixeles)
static const byte LCD_WIDTH = 84;
static const byte LCD_HEIGHT = 48;
// Símbolo de grados
static const byte DEGREES_CHAR = 1;
static const byte degrees_glyph[] = { 0x00, 0x07, 0x05, 0x07, 0x00 };

////////////////////////////////////////////////////////////////////////////////////////////////
//                                           SETUP                                            //
////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {

  
  // *** Inicializa Comunicación Serial *** //
  Serial.begin(9600);

  // *** Inicializa Pantalla LCD y caracter ° *** //
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  lcd.createChar(DEGREES_CHAR, degrees_glyph);

  // *** Configuración de Pines *** //
  pinMode(ledREDPin,  OUTPUT);
  pinMode(ledBLUEPin, OUTPUT);
  pinMode(PC, INPUT);
  
  // Referencia externa para entradas Analógicas: 2.9 V
  analogReference(EXTERNAL);

  // *** Inicializa PID *** //
  myPID.SetMode(AUTOMATIC);
  
}

////////////////////////////////////////////////////////////////////////////////////////////////
//                                           LOOP                                             //
////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  
  // Posición inicial en LCD
  static byte xChart = LCD_WIDTH;

  ////////////////////////////////////////////////////////////////////
  // Lectura de Temperatura de Referencia - Setpoint - (0 - 80) °C
  ////////////////////////////////////////////////////////////////////
  
  Setpoint = analogRead(setTempPin)*(80/1023.0); // Temperatura de referencia
 
  ////////////////////////////////////////////////////////////////////
  // Sensor de Temperatura de la Incubadora - Input - (0 - 100) °C
  ////////////////////////////////////////////////////////////////////
  
  // y = mx + b  (V)
  // x = (y-b)/m (°C)
  double voltage_value_pin = analogRead(sensorPin)*(2.9/1023.0); // y (V)
  double m = (2.9-2.5)/(100-0); // m=V/°C // b=2.5V
  Input = (voltage_value_pin - 2.5)/m; // x (°C)

  ////////////////////////////////////////////////////////////////////
  // PID para ajustar a Temperatura de Referencia con Resistor
  ////////////////////////////////////////////////////////////////////
  
  myPID.Compute();
  double Output_Escalado = round(Output*(Setpoint/80.0));
  analogWrite(PWM_ResistorPin, Output_Escalado);

  ////////////////////////////////////////////////////////////////////
  // Sensor de Humedad de la Incubadora - (0 - 100) %
  ////////////////////////////////////////////////////////////////////
 
  double hum = analogRead(HumedadPin)*(100.0/1023.0);

  ////////////////////////////////////////////////////////////////////
  // LEDs Indicadores de Temperatura fuera de Rango (30 - 42) °C
  ////////////////////////////////////////////////////////////////////

  // *** LED Rojo - Temperatura Alta *** //
  if (Input > 42.0) digitalWrite(ledREDPin, HIGH);
  else digitalWrite(ledREDPin, LOW);

  // *** LED Azul - Temperatura Baja *** //
  if (Input < 30.0) digitalWrite(ledBLUEPin, HIGH);
  else digitalWrite(ledBLUEPin, LOW);

  ////////////////////////////////////////////////////////////////////
  // Impresión de datos de interés en Pantalla LCD
  ////////////////////////////////////////////////////////////////////
  
  // *** LCD - Imprime Temperatura de Referencia - Setpoint *** //
  lcd.setCursor(0, 0);
  lcd.print("TempS: ");
  lcd.print(Setpoint, 1);
  lcd.print(" \001C "); // °C: (0 - 80)

  // *** LCD - Imprime Señal de Control - u *** //
  lcd.setCursor(0, 1);
  lcd.print("u: ");
  lcd.print(Output_Escalado, 1);
  lcd.print(" PWM "); // PWM: (0 - 255)

  // *** LCD - Línea en blanco *** //
  lcd.setCursor(0, 2);
  lcd.print("");
  
  // *** LCD - Imprime Temperatura de la Incubadora - Input *** //
  lcd.setCursor(0, 3);
  lcd.print("TempR: ");
  lcd.print(Input, 1);
  lcd.print(" \001C "); // °C: (0 - 100)

  // *** LCD - Imprime Humedad Relativa de la Incubadora - hum *** //
  lcd.setCursor(0, 4);
  lcd.print("HumeR: ");
  lcd.print(hum, 1);
  lcd.print(" % ");     // %: (0 - 100)

  ////////////////////////////////////////////////////////////////////
  // Comunicación Serial con PC si Switch está en ON
  ////////////////////////////////////////////////////////////////////
  
  if(digitalRead(PC)==1){

    // *** PC - Imprime Temperatura de la Incubadora - Input *** //
    Serial.print("Temperatura: ");
    Serial.print( Input );
    Serial.print(" ");
    Serial.print(char(176));
    Serial.print("C \n");

    // *** PC - Imprime Humedad Relativa de la Incubadora - hum *** //
    Serial.print("Humedad    : ");
    Serial.print( hum );
    Serial.print(" % \n");
    Serial.print(" \n");
    
  }
  
  ////////////////////////////////////////////////////////////////////
  
  delay(500); // Retardo de 0.5 s
  
}

////////////////////////////////////////////////////////////////////////////////////////////////
