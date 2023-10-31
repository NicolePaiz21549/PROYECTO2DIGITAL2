//***********************************************************************************
//Universidad del Valle de Guatemala
//Dulce Nicole Monney Paiz, 21549
//BE3029 - Electrónica Digital 2
//Proyecto 2 – SPI, TFT & SD
//*********************************************************************************** 

//Librerías
#include <SPI.h>
#include <SD.h>
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
#define BSENSE PF_4 //Botón para preguntar el valor del sensor al ESP32 - SW1
#define BSD PF_0 //Botón para guardar el valor de la medición  del sensor en el SD - SW2
#define RXp2 PD6
#define TXp2 PD7
#define CS_PIN PA_3 //aka pin 12 para el chip select 
//***************************************************************************************************************************************

//Variables globales 
float receivedvaluesensor; //Variable determinada para recibir el valor del sensor LM35 y que se enviará a la ILI9341 & a la SD
int clave=0; //Aviso al ESP32 de presión del botón BSENSE
unsigned long lastDebounceTime=0;
unsigned long debounceDelay=50;
File root; //Indicador del directorio de la raíz de la SD
File dataFile; 
//bool requested=false; //Bandera para identificar si se ha realizado un pedido de valor aka request
//***********************************************************************************

//Prototipos de funciones
void printDirectory(File dir, int numTabs) { //Función encargada de la enumeración del contenido del directorio del SD
  while(true){
    File entry=dir.openNextFile();
    if(!entry){
      break;
      }
      for(uint8_t i=0; i<numTabs; i++){
        Serial.print('\t');
        }
        Serial.print(entry.name());
        if(entry.isDirectory()){
          Serial.println("/");
          printDirectory(entry, numTabs+1);
          }else{
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
            }
            entry.close();
            }
}
//***************************************************************************************************************************************

//Configuración
void setup(){
  Serial.begin(115200); //Inicialización de la comunicación serial con el monitor serial
  SPI.setModule(0); //Implementación del módulo SPIO por la pantalla ili9341
  Serial2.begin(115200); //Comunicación UART con ESP32
  
  pinMode(BSENSE, INPUT_PULLUP); //Configuración de SW1
  pinMode(BSD, INPUT_PULLUP); //Configuración de SW1

  Serial.print("Inicializando la tarjeta SD...");
  if(!SD.begin(CS_PIN)){
    Serial.println("¡Falló la inicialización de la tarjeta!D:");
    while(true); 
  }
  Serial.println("Inicialización realizada:D");
  Serial.println("Archivos en la tarjeta SD:");
  root=SD.open("/");
  printDirectory(root,0);
  Serial.println("");
  }
//***************************************************************************************************************************************

//Loop principal
void loop(){
  //Verificación de BSENSE para realizar la medición del sensor
  //Serial.print("∖n");
  if(digitalRead(BSENSE) == LOW && (millis()-lastDebounceTime)>debounceDelay){
    lastDebounceTime = millis();
    clave=1; 
    Serial2.print(clave); //Pedir al ESP32 el valor del sensor
    Serial2.print("∖n");
    }
    if (Serial2.available()){ //Utilizar Serial2 para comunicarse con el ESP32
        receivedvaluesensor=Serial2.parseFloat();
        Serial.print("LM35:");
        Serial.println(receivedvaluesensor);
        //Agregar melodía indicativa de que se realizó una medición con el sensor 
        }
        
   //Verificación de BSD para guardar en la SD
   if(digitalRead(BSD)==LOW && (millis()-lastDebounceTime)>debounceDelay){
    lastDebounceTime=millis();
    //if(dataFile){
      dataFile=SD.open("sensor_data.txt", FILE_WRITE); //Apertura del archivo en modo escritura
      if(!dataFile){
        dataFile.print("LM35:");
        dataFile.println(receivedvaluesensor);
        dataFile.close(); //Se cierra el archivo
        Serial.print("Se ha guardado exitosamente la medición de la temperatura en la tarjeta SD.");
        //Serial.println("");
        //Serial.println("");
        root=SD.open("/");
        printDirectory(root, 0);
        //Serial.println("");
        //Agregar melodía indicativa que se almacenó un dato en la memoria SD
        } else {
          Serial.println("Error al abrir y crear el archivo en la tarjeta SD.");
            }
            //}
        }
}
//***************************************************************************************************************************************
