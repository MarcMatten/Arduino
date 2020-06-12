// SDCard
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;
File myFile;

String myFileName = "logfile.csv";
String headerStr = "";

// RTC Module
#include <Wire.h>     //needed because DS3231 uses I2C Bus
#include <RTClib.h>   //needed becuase we have ready-made functions of this librray

RTC_DS3231 rtc;     //the object rtc is created from the class RTC_DS3231
#define deviceAddress 0x68

// DHT11 and Thermistor
#include <SimpleDHT.h>

int pinDHT11 = 2; // digital output of DHT11 sensor
int pinThermistor = 0; // thermistor

SimpleDHT11 dht11;

void initSDCard(String fileName, String dataStr){
  if (!SD.begin(chipSelect)) {
    while (1);
  }
  if (SD.exists(fileName)) {
    ;
  }
  else {
    writeToSDCard(fileName, dataStr);
  }
}

void writeToSDCard(String fileName, String dataStr){
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(fileName, FILE_WRITE);
  // if the file is available, write to it:
  if (myFile){
    myFile.println(dataStr);
    myFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    ;
  }
}

void initRTC()
{
  rtc.begin();
  
  // use the following line only to set the time of the RTC module, not in every programming application!
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  
}

String getTime()
{  
  Wire.beginTransmission(deviceAddress); //START, Roll Cal
  Wire.write(0x00); //set SEC Register address
  Wire.endTransmission(); //Execute the above queued data, ACK, STOP

  Wire.requestFrom(deviceAddress, 3);   //SEC, MIN, and HRS to read from RTC as BCD
  byte bcdSeconds = Wire.read();
  byte bcdMinutes = Wire.read();
  byte bcdHours = Wire.read();

  String timeStr = "";
  
  timeStr = bcdHours >> 4;
  timeStr += bcdHours & 0x0F;
  timeStr += ":";

  timeStr += bcdMinutes >> 4;
  timeStr += bcdMinutes & 0x0F;
  timeStr += ":";
  
  timeStr += bcdSeconds >> 4;
  timeStr += bcdSeconds & 0x0F;
  return timeStr;
}

String getDate()
{
  //--read Date and Tme from DS3231 using the method now()---------------------;
  //--store the Date and Time into another user define object bamed nowTime
  DateTime nowTime = rtc.now();

  //---show Day and Date on Top Line of LCD--------------
  int x = nowTime.dayOfTheWeek(); //dyOfTheWeek() is a pre-defined method

  String dateStr = "";
  
  dateStr += nowTime.year();
  dateStr += "/";
  dateStr += nowTime.month();
  dateStr += "/";
  dateStr += nowTime.day();

  return dateStr;
}

String getDateTime(){
  String dateTimeStr = "";
  dateTimeStr += getDate();
  dateTimeStr += " ";
  dateTimeStr += getTime();
  
  return dateTimeStr;
}

void setup() {
  initSDCard(myFileName, headerStr);
  initRTC();
}

void loop() {
  String myDataString = "";

  myDataString += getDateTime();
  myDataString += ", ";

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

  myDataString += T_DHT11;
  myDataString += ", ";
  myDataString += TC_Thermistor;
  myDataString += ", ";
  myDataString += rHum_DHT11;    

  writeToSDCard(myFileName, myDataString);

  delay(1000);
}
