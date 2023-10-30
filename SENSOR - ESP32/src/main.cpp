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
#define RXp2 16
#define TXp2 17
//***********************************************************************************

//Variables globales 
int LM35_Input = 0;
float TempC = 0.0;
float Voltage = 0.0;
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
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2); //Comunicación UART 2 con la Tiva C 
  pinMode(LM35_GPIO_PIN, INPUT); //Inicialización del pin LM35
//pinMode(BUTTON_PIN, INPUT_PULLUP); //Botón con resistencia de pull-up interna
  }

//Loop principal
void loop() {
// Display de temperatura en monitor serial 
//int tempInt = int(TempC);
//int tempDecimal = int((TempC - tempInt) * 10); //Cálculo del decimal del valor de temperatura, lo multiplica por 10 para convertirlo en una fracción de 10 grados y almacena el resultado como un número entero en la variable tempDecimal.
//int tempDecimalTens = tempInt / 10;
//int tempDecimalUnits = tempInt % 10;

//Lectura del valor LM35_ADC
TempC = ((analogRead(LM35_GPIO_PIN) + 70) * (5000.0/ 4096.0)); // Sumatoria de un valor OFFSET (factor de correción) para el valor análogo original multiplicado por el voltaje de 5V por pin Vin para la fórmula dividido la resolución ADC de 12bits
TempC = TempC / 10.0; // División dentro de 10 representando los 10mV del LM35 ya que cada cambio de 10mV representa un cambio de 1℃
double Temperature = TempC;
Serial.print("LM35: ");
Serial.println(TempC); //Impresión de backup
}
//***********************************************************************************