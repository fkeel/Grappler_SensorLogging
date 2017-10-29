//////////////////////////////
//This sketch sequentially opens every file in flash memory and prints its contents to the serial port.
//////////////////////////////

#include <SerialFlash.h>
#include <SPI.h>
SerialFlashFile file; //Working file
const int FlashChipSelect = 6; // digital pin for flash chip CS pin
//const int FlashChipSelect = 21; // Arduino 101 built-in SPI Flash

void setup() {

  Serial.begin(9600);

  // wait for Arduino Serial Monitor (Just so we don't miss the display)
  while (!Serial) ;
  delay(100);
  Serial.println("All Data from all Files on SPI Flash chip:");
  Serial.println("");

  if (!SerialFlash.begin(FlashChipSelect)) {
    Serial.print("Unable to access SPI Flash chip");
    delay(2500);
  }

  SerialFlash.opendir();
  while (true) { //this lists by order of creation all files
    char filename[64]; //filename
    uint32_t filesize; //filesize (we will use this to read from them.

    if (SerialFlash.readdir(filename, sizeof(filename), filesize)) { //if it found a file
      file = SerialFlash.open(filename);  //open it
      char buffer2[filesize];             //create a buffer the size of the file
      file.read(buffer2, filesize);       //copy the file to the buffer
      Serial.print(filename);
      Serial.print(": ");

      for (int i = 0; i < filesize; i++) {
        Serial.print(buffer2[i]); //print the content of the buffer to the serial port
      }
      Serial.println(""); //new line at end of file
      file.close(); //close file

    } else { //if no more files are found
      Serial.println("");
      Serial.println("no more Data");
      break; // no more files
    }
  }
}


void loop() {
}

