#include <Wire.h>     //needed because DS3231 uses I2C Bus
#include <RTClib.h>   //needed becuase we have ready-made functions of this librray

RTC_DS3231 rtc;     //the object rtc is created from the class RTC_DS3231
#define deviceAddress 0x68

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

void setup()
{
  Serial.begin(9600);
  initRTC();
}

void  loop()
{
  String myDateTimeStr = getDateTime();

  Serial.println(myDateTimeStr);
}
