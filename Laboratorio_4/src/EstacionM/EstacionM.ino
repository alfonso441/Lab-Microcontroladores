/*
 * Estación Meteorológica
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
#include <Servo.h>
#include <LowPower.h>

////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Variables Globales                                        //
////////////////////////////////////////////////////////////////////////////////////////////////

// *** Pines de Arduino *** //
static const byte BateriaPin      = A0; // Batería sensada
static const byte TemperaturaPin  = A1; // Temperatura sensada
static const byte HumedadPin      = A2; // Humedad sensada
static const byte LuzPin          = A3; // Luz deseada
static const byte VelocidadPin    = A4; // Velocidad Viento sensada
static const byte PPVerticalPin   = A5; // Posición Vertical de Panel sensada
static const byte PPHorizontalPin = A6; // Posición Horizontal de Panel sensada
static const byte SwitchPantalla  = 2;  // Comunicación por Pantalla LCD
static const byte SwitchUSART     = 8;  // Comunicación por USART
static const byte LluviaPin       = 9;  // Lluvia sensada
static const byte ledUSARTPin     = 12; // LED Verde - Comunicación USART
static const byte ledBateriaPin   = 13; // LED Rojo - Batería Baja

// Servomotores
Servo ServoPPVertical;
Servo ServoPPHorizontal;

// The dimensions of the LCD (in pixels)...
static const byte LCD_WIDTH = 84;
static const byte LCD_HEIGHT = 48;

// The number of lines for the temperature chart...
static const byte CHART_HEIGHT = 5;

// A custom "degrees" symbol...
static const byte DEGREES_CHAR = 1;
static const byte degrees_glyph[] = { 0x00, 0x07, 0x05, 0x07, 0x00 };

static PCD8544 lcd;

////////////////////////////////////////////////////////////////////////////////////////////////
//                                           SETUP                                            //
////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {

  // *** Inicializa Comunicación Serial *** //
  Serial.begin(9600);

  // *** Inicializa Pantalla LCD *** //
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  // Register the custom symbol...
  lcd.createChar(DEGREES_CHAR, degrees_glyph);
  
  // *** Configuración de Pines *** //
  pinMode(SwitchPantalla, INPUT);
  pinMode(SwitchUSART, INPUT);
  pinMode(LluviaPin, INPUT);
  pinMode(ledUSARTPin,  OUTPUT);
  pinMode(ledBateriaPin, OUTPUT);

  // *** Configuración Servomotores *** //
  ServoPPVertical.attach(10);
  ServoPPHorizontal.attach(11);

  // The internal 1.1V reference provides for better
  // resolution from the LM35, and is also more stable
  // when powered from either a battery or USB...
  //analogReference(INTERNAL);
  analogReference(EXTERNAL);
  //analogReference(DEFAULT);
  
}

////////////////////////////////////////////////////////////////////////////////////////////////
//                                           LOOP                                             //
////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {

  
  // Start beyond the edge of the screen...
  static byte xChart = LCD_WIDTH;

  ////////////////////////////////////////////////////////////////////
  // Sensor de Temperatura - (0 - 100) °C
  ////////////////////////////////////////////////////////////////////
  
  // Read the temperature (in celsius)...
  // y = mx + b (V)
  // x = (y-b)/m
  double voltage_value_pin = analogRead(TemperaturaPin)*(3/1023.0);// y
  double m = (3-2.6)/(100-0);// m=V/°C // b=2.6V
  double TemperaturaValue = (voltage_value_pin - 2.6)/m;

  ////////////////////////////////////////////////////////////////////
  // Sensor de Humedad - (0 - 100) %
  ////////////////////////////////////////////////////////////////////
 
  double HumedadValue = analogRead(HumedadPin)*(100.0/1023.0);

  ////////////////////////////////////////////////////////////////////
  // Sensor de Luz - (1 - 999) Lux
  ////////////////////////////////////////////////////////////////////

  double LuzValue = analogRead(LuzPin)*(1000.0/1023.0);

  ////////////////////////////////////////////////////////////////////
  // Sensor de Velocidad del Viento - (0 - 50) m/s
  ////////////////////////////////////////////////////////////////////

  double VelocidadValue = analogRead(VelocidadPin)*(50.0/1023.0);

  ////////////////////////////////////////////////////////////////////
  // Sensor de Lluvia - (0 - 1): (Llueve - No Llueve)
  ////////////////////////////////////////////////////////////////////

  int LluviaValue = !digitalRead(LluviaPin); //0: Llueve / 1: No llueve
  
  String Lluvia_String = "Llueve: No";
  if (LluviaValue == 1) Lluvia_String = "Llueve: Si";

  ////////////////////////////////////////////////////////////////////
  // Potenciómetros + Servomotores: Orientación de Panel Solar
  ////////////////////////////////////////////////////////////////////

  int GradosVertical  = round(analogRead(PPVerticalPin)*(180.0/1023.0));
  int GradosHorizontal = round(analogRead(PPHorizontalPin)*(180.0/1023.0));

  ServoPPVertical.write(GradosVertical);
  ServoPPHorizontal.write(GradosHorizontal);

  ////////////////////////////////////////////////////////////////////
  // Nivel de Batería: Alerta al 67% de la tensión = 8 V / 12 V
  // Mínimo recomendado: 7 V / Límite: 6 V. Quedan 2 V de rango para
  // el cambio de la batería para llegar al límite de tensión.
  ////////////////////////////////////////////////////////////////////

  String Bateria_String = "Bateria: Normal";
  
  if ((analogRead(BateriaPin)*(3.0/1023.0)) <= 2) {
    
    // Entrar en Modo de Ahorro de Energía
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    
    // Encender LED de alerta
    digitalWrite(ledBateriaPin, HIGH);
    delay(500);
    digitalWrite(ledBateriaPin, LOW);
    delay(500);

    Bateria_String = "Bateria: Baja";
    
  }

  ////////////////////////////////////////////////////////////////////
  // Impresión en Pantalla LCD si Switch está en ON
  ////////////////////////////////////////////////////////////////////

  if(digitalRead(SwitchPantalla)==1){

    // *** LCD - Imprime Temperatura *** //
    lcd.setCursor(0, 0);
    lcd.print("TempR: ");
    lcd.print(TemperaturaValue, 1);
    lcd.print(" \001C "); // °C: (0 - 100)
  
    // *** LCD - Imprime Humedad Relativa *** //
    lcd.setCursor(0, 1);
    lcd.print("HumeR: ");
    lcd.print(HumedadValue, 1);
    lcd.print(" % ");     // %: (0 - 100)
    
    // *** LCD - Imprime Luminosidad *** //
    lcd.setCursor(0, 2);
    lcd.print("Luz : ");
    lcd.print(LuzValue, 1);
    lcd.print(" Lux "); // Lux: (1 - 999)
  
    // *** LCD - Imprime Velocidad del Viento *** //
    lcd.setCursor(0, 3);
    lcd.print("Veloc: ");
    lcd.print(VelocidadValue, 1);
    lcd.print(" m/s "); // m/s: (0 - 1000)

     // *** LCD - Imprime Lluvia *** //
    lcd.setCursor(0, 4);
    lcd.print(Lluvia_String);

  }
  // Apagar pantalla - Simulado - NO PIN Vin en Simulador
  else {
    // *** LCD - APAGAR *** //
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 2);
    lcd.print("                ");
    lcd.setCursor(0, 3);
    lcd.print("                ");
    lcd.setCursor(0, 4);
    lcd.print("                ");
  }
  
  ////////////////////////////////////////////////////////////////////
  // Comunicación Serial con PC si Switch está en ON
  ////////////////////////////////////////////////////////////////////
  
  if(digitalRead(SwitchUSART)==1){

    // Encender LED de comunicación USART
    digitalWrite(ledUSARTPin, HIGH);
    delay(500);
    digitalWrite(ledUSARTPin, LOW);
    delay(500);

    // *** PC - Imprime Temperatura *** //
    Serial.print("Temperatura: ");
    Serial.print( TemperaturaValue );
    Serial.print(" ");
    Serial.print(char(176));
    Serial.print("C \n");

    // *** PC - Imprime Humedad Relativa *** //
    Serial.print("Humedad    : ");
    Serial.print( HumedadValue );
    Serial.print(" % \n");

    // *** PC - Imprime Luminosidad *** //
    Serial.print("Luz        : ");
    Serial.print( LuzValue );
    Serial.print(" Lux \n");

    // *** PC - Imprime Velocidad del Viento *** //
    Serial.print("Velocidad V: ");
    Serial.print( VelocidadValue );
    Serial.print(" m/s \n");

    // *** PC - Imprime Lluvia *** //
    Serial.print(Lluvia_String);
    Serial.print(" \n");

    // *** PC - Imprime Batería *** //
    Serial.print(Bateria_String);
    Serial.print(" \n");
    
  }
  
  ////////////////////////////////////////////////////////////////////
  
}

////////////////////////////////////////////////////////////////////////////////////////////////
