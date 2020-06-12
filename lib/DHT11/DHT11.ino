#include <SimpleDHT.h>

int pinDHT11 = 2; // digital output of DHT11 sensor
int pinThermistor = 0; // thermistor

SimpleDHT11 dht11;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("T_DHT11,T_Thermistor,rHum_DHT11");
}

void loop() {
  // read with raw sample data.
  float T_DHT11 = 0.0;
  float rHum_DHT11 = 0.0;
  byte data_DHT11[40] = {0};

  dht11.read2(pinDHT11, &T_DHT11, &rHum_DHT11, data_DHT11);

  // thermistor
  int TRaw_Thermistor = analogRead(pinThermistor);
  double TK_Thermistor = log(10000.0 * ((1024.0 / TRaw_Thermistor - 1)));
  TK_Thermistor = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * TK_Thermistor * TK_Thermistor )) * TK_Thermistor ) - 1.73;      //  Temp Kelvin
  float TC_Thermistor = TK_Thermistor - 273.15;            // Convert Kelvin to Celcius
  float TF_Thermistor = (TC_Thermistor * 9.0) / 5.0 + 32.0; // Convert Celcius to Fahrenheit

  Serial.print(T_DHT11);
  Serial.print(",");
  Serial.print(TC_Thermistor);
  Serial.print(",");
  Serial.println(rHum_DHT11);

  // DHT11 sampling rate is 1HZ.
  delay(1000);

}
