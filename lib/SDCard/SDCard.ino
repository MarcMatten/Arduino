#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;
String defaultFileName = "default.csv";
String defaultDataStr = "default header";
File myFile;

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

void setup() {
  initSDCard(defaultFileName, defaultDataStr);
}

void loop() {
  // make a string for assembling the data to log:
  String myDataString = "";

  // read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    myDataString += String(sensor);
    if (analogPin < 2) {
      myDataString += ",";
    }
  }
  writeToSDCard(defaultFileName, myDataString);
}
