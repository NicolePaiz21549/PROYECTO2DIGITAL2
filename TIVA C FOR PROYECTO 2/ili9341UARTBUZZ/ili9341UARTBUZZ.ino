//***********************************************************************************
//Universidad del Valle de Guatemala
//Dulce Nicole Monney Paiz, 21549
//BE3029 - Electrónica Digital 2
//Proyecto 2 – SPI, TFT & SD
//*********************************************************************************** 

//Librerías
/*#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"*/
//*********************************************************************************** 

//Definición de pines
/*#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};*/  
#define BSENSE PF_4 //Botón para preguntar el valor del sensor al ESP32
#define RXp2 PD6
#define TXp2 PD7
//***************************************************************************************************************************************

//Variables globales 
int receivedvaluesensor; //Variable determinada para recibir el valor del sensor LM35
int clave=0; //Aviso al ESP32 de presión del botón BSENSE
unsigned long lastDebounceTime=0;
unsigned long debounceDelay=50;
//bool requested=false; //Bandera para identificar si se ha realizado un pedido de valor aka request
//***********************************************************************************

//Prototipos de funciones
//***************************************************************************************************************************************

//Configuración
void setup(){
  Serial.begin(115200); //Inicialización de la comunicación serial con el monitor serial
  Serial2.begin(115200); //Comunicación UART con ESP32
  pinMode(BSENSE, INPUT_PULLUP); //Configuración de SW1
  }
//***************************************************************************************************************************************

//Loop principal
void loop(){
  //Verificación de BSENSE 
  //Serial.print("∖n");
  if(digitalRead(BSENSE) == LOW && (millis()-lastDebounceTime)>debounceDelay){
    lastDebounceTime = millis();
    clave=1; 
    Serial2.print(clave); //Pedir al ESP32 el valor del sensor
    Serial2.print("∖n");
    }
    if (Serial2.available()>0){ //Utilizar Serial2 para comunicarse con el ESP32
        receivedvaluesensor=Serial2.parseInt();
        Serial.print("LM35:");
        Serial.println(receivedvaluesensor);
        }
        }
//***************************************************************************************************************************************
