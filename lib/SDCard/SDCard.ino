#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;

File myFile;

void setup() {
  if (!SD.begin(chipSelect)) {
    while (1);
  }

  if (SD.exists("logfile.txt")) {
    ;
  } else {
    myFile = SD.open("logfile.txt", FILE_WRITE);
    myFile.println("der test");
    myFile.close();
  }
}

void loop() {
  // make a string for assembling the data to log:
  String dataString = "";

  // read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("logfile.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    ;
  }
}
