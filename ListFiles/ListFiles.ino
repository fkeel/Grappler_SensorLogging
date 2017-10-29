#include <SerialFlash.h>
#include <SPI.h>
SerialFlashFile file; //Working file
const int FlashChipSelect = 6; // digital pin for flash chip CS pin
//const int FlashChipSelect = 21; // Arduino 101 built-in SPI Flash

void setup() {
  //uncomment these if using Teensy audio shield
  //SPI.setSCK(14);  // Audio shield has SCK on pin 14
  //SPI.setMOSI(7);  // Audio shield has MOSI on pin 7

  //uncomment these if you have other SPI chips connected
  //to keep them disabled while using only SerialFlash
  //pinMode(4, INPUT_PULLUP);
  //pinMode(10, INPUT_PULLUP);

  Serial.begin(9600);

  // wait for Arduino Serial Monitor
  while (!Serial) ;
  delay(100);
  Serial.println("All Files on SPI Flash chip:");

  if (!SerialFlash.begin(FlashChipSelect)) {
    error("Unable to access SPI Flash chip");
  }

  SerialFlash.opendir();
  while (true) {
    char filename[64];
    uint32_t filesize;

    if (SerialFlash.readdir(filename, sizeof(filename), filesize)) {
     // Serial.print("  ");
     // Serial.print(filename);
    //  spaces(20 - strlen(filename));
    //  Serial.print("  ");
    //  Serial.print(filesize);
    //  Serial.print(" bytes");
   //   Serial.println();
      file = SerialFlash.open(filename);
  //    Serial.println("file opened");
      if (file)
      { // true if the file exists
        char buffer2[filesize];
        //IF YOU WANT TO READ FROM ANOTHER POSITION IN THE FILE, USE "file.seek();"
        //IF YOU WANT TO CHECK YOUR CURRENT POSITION, USE "file.position()"
  
        file.read(buffer2, filesize);
        
 
        // Serial.println(buffer2);
        for (int i = 0; i < filesize+1; i++) {
          Serial.print(buffer2[i]);
        }


        file.close();
        Serial.println("");
      }
    } else {
      Serial.println("no more files");
      break; // no more files
    }
  }
}

void spaces(int num) {
  for (int i = 0; i < num; i++) {
    Serial.print(" ");
  }
}

void loop() {
}

void error(const char *message) {
  while (1) {
    Serial.println(message);
    delay(2500);
  }
}
