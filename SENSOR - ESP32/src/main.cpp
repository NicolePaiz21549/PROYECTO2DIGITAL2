//***********************************************************************************
//Universidad del Valle de Guatemala
//Dulce Nicole Monney Paiz, 21549
//BE3029 - Electrónica Digital 2
//Proyecto 2 – SPI, TFT & SD
//***********************************************************************************

//Librerías 
#include <Arduino.h>
#include <esp_adc_cal.h>
//***********************************************************************************

//Definición de pines
#define LM35_GPIO_PIN 35 //Pin para el sensor LM35
//Definición de pines para comunicación serial UART 2 entre ESP32 y Tiva C
#define RXp2 16 
#define TXp2 17
//***********************************************************************************

//Variables globales 
int LM35_Input = 0;
float TempC=0.0;
float Voltage = 0.0;
int request=0; //Comando para activar bandera de envio del valor medido por el sensor
float Temperatura = 0;
//***********************************************************************************

//Prototipos de funciones
//Prototipo de función para readADC_Cal
uint32_t readADC_Cal(int ADC_Raw) { 
  esp_adc_cal_characteristics_t adc_chars; 
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars); 
  return esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars);
}
//***********************************************************************************

//Configuración
void setup() {
  Serial.begin(115200); //Comunicación con el monitor serial/PC
  Serial2.begin(115200); 
  pinMode(LM35_GPIO_PIN, INPUT); //Inicialización del pin LM35
  }

//Loop principal
void loop() {
//Lectura del valor LM35_ADC
Temperatura = ((analogRead(LM35_GPIO_PIN) + 70) * (5000.0/ 4096.0)); // Sumatoria de un valor OFFSET (factor de correción) para el valor análogo original multiplicado por el voltaje de 5V por pin Vin para la fórmula dividido la resolución ADC de 12bits
TempC = Temperatura / 10.0; // División dentro de 10 representando los 10mV del LM35 ya que cada cambio de 10mV representa un cambio de 1℃
double Temperature = TempC;

if(Serial2.available()){
  request = Serial2.parseInt();
  if(request==1){ //La Tiva C envió un comando
    Serial2.print(TempC); //Enviar el valor del sensor LM35 a la Tiva C
    Serial.print("LM35:");
    Serial.println(TempC);
    delay(100); //Delay para que la Tiva C pueda leer la respuesta
  }
}
}
//***********************************************************************************