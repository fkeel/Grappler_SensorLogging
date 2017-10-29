///////////////////////////
// Logs data to flash. Goes with readFiles.ino
/////////////////


#include <SerialFlash.h>
#include <SPI.h>
const int FlashChipSelect = 6; // digital pin for flash chip CS pin

//for IMU ////////
#include <NXPMotionSense.h>
#include <Wire.h>
#include <EEPROM.h>
#include <util/crc16.h>

NXPMotionSense imu;
//////////////////

int counter = 0;


SerialFlashFile file; //Working file


String dataAsString;


void setup()
{
  Serial.begin(9600); //open serial port
  while (!Serial); //wait until the serial port is open
  delay(100);
  Serial.println("Checking if it finds the Flash Chip...");

  if (!SerialFlash.begin(FlashChipSelect)) {
    Serial.println("");
    Serial.println("Unable to access SPI Flash chip");
  }
  Serial.println("");
  Serial.println("SPI Flash found");
  uint8_t id[5];
  SerialFlash.readID(id);
  Serial.println("ID:");
  Serial.print("0x");
  Serial.print(id[0], HEX);
  Serial.print(", 0x");
  Serial.print(id[1], HEX);
  Serial.print(", 0x");
  Serial.print(id[2], HEX);
  Serial.print(", 0x");
  Serial.println(id[3], HEX);
  Serial.println("... so far, so good");
  Serial.println("");

  imu.begin();

 // clearFlash();  //REMOVE THIS FOR LATER VERSION - it DELETES all data we collect



}

void loop() {
  //IMU data
  int ax, ay, az;
  int gx, gy, gz;
  int mx, my, mz;

  // get and print uncalibrated data
  if (imu.available()) {
    imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);
 dataAsString = "" + String(ax) + "!!";
   // dataAsString = "Acc: " + String(ax) + ", " + String(ay) + ", " + String(az) + "!!";
    Serial.print("Testing the String: ");
    Serial.println(dataAsString);
    String currentTime = String(counter);
    createFile(dataAsString, currentTime);  //This creates a test file IF it doesn't exist
    counter++;
  }

}








////////////////////////////////////////////////////////////////////////////
//This function creates a file and writes the array specified above to it //
////////////////////////////////////////////////////////////////////////////

void createFile( String dataString, String millisForFilename ) {

  //create the name (this should be cleaned up)
  String fileNameAsString = millisForFilename + ".dat" ;
  int nameLength = fileNameAsString.length() + 1;
  char filename[nameLength];
  fileNameAsString.toCharArray(filename, nameLength);
  
  String dataAsString = dataString;
  int  fileSize = dataAsString.length();
  char dataToWrite [fileSize + 1];
  dataAsString.toCharArray(dataToWrite, fileSize + 1);
  Serial.print("String length: ");
  Serial.println(fileSize);
  Serial.print("Char array length: ");
  Serial.println(sizeof(dataToWrite));



  Serial.println("");
  //Check if file exists, if not, create it.
  if (SerialFlash.exists(filename) == 0)
  {
    //File doesn't exist
    if (SerialFlash.create(filename, 20000) == true) //this should be filesize. just testing  
    {
      Serial.println(filename);
    }
    else
    {
      Serial.println("File creation failed!!!");
    }
  }
  if (SerialFlash.exists(filename  ))
  {
    Serial.println("File Exists, trying to open...");
    file = SerialFlash.open(filename  );
    if (file)
    {
      // true if the file exists
      //      Serial.println(filename + ".dat opened");
      uint8_t buffer[fileSize];
      for (int i = 0; i < fileSize; i++) {
        buffer[i] = dataToWrite[i];
      }


      Serial.print("Writing Data to File ...");
      //IF YOU WANT TO WRITE TO ANOTHER POSITION IN THE FILE, USE "file.seek();"
      //IF YOU WANT TO CHECK YOUR CURRENT POSITION, USE "file.position()"

      file.write(buffer, fileSize);
      Serial.println("... completed");
      Serial.print("Position after writing: ");
      Serial.println(file.position());
    }
    else
    {
      //      Serial.println(filename + ".dat not opened!!!");
    }
  }
}





/////////////////////////////////////////////////////
//This function delets *everything* in flash memory//
// DELETE THIS FORE FINAL VERSION!///////////////////
/////////////////////////////////////////////////////

void clearFlash( void )
{
  Serial.println("");
  Serial.print("Erasing flash");
  SerialFlash.eraseAll();
  while (SerialFlash.ready() == false)
  {
    // wait, 30 seconds to 2 minutes for most chips
    Serial.print(".");
    delay(100);
  }
  Serial.println("Done!");
  Serial.println("Flash is now formatted.  Comment out clearFlash(); to continue experimenting");
  while (1);
}

