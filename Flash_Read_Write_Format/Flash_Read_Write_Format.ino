/******************************************************************************
  Flash_Read_Write_Format.ino
  Example of using the Prop Shield's Flash Memory with a Teensy 3.2

  Find the full documentation of the SerialFlash Library here:
  https://github.com/PaulStoffregen/SerialFlash

  Adapted from Marshall Taylors code:
  https://github.com/sparkfun/SparkFun_TeensyView_Arduino_Library

  Modified by Paul Strohmeier to learn and share how to use the SerialFlash Library

  Distributed as-is; no warranty is given.
******************************************************************************/


#include <SerialFlash.h>
#include <SPI.h>
const int FlashChipSelect = 6; // digital pin for flash chip CS pin
const int fileSize = 6;
SerialFlashFile file; //Working file



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
  Serial.println("Now the testing begins, uncomment any of the functions at the end of the setup function, to create files, read from them and reformat the Flash memory");
   // clearFlash();  //Use this to reformat the Flash chip... this holds the program in a while loop when done.
   createFile();  //This creates a test file IF it doesn't exist
   readFile();    //this reads the test file


}

void loop() {
  //nothing happens here
}

/////////////////////////////////////////////
//this file will be written to flash memory//
/////////////////////////////////////////////

uint16_t dataToWrite[3] = {
  1164, 222, 1010
};



/////////////////////////////////////////////////////
//This function delets *everything* in flash memory//
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



////////////////////////////////////////////////////////////////////////////
//This function creates a file and writes the array specified above to it //
////////////////////////////////////////////////////////////////////////////

void createFile( void )
{
  Serial.println("");
  //Check if file exists, if not, create it.
  if (SerialFlash.exists("testfile.dat") == 0)
  {
    //File doesn't exist
    if (SerialFlash.create("testfile.dat", fileSize) == true)
    {
      Serial.println("Created testfile.dat");
    }
    else
    {
      Serial.println("File creation failed!!!");
    }
  }
  if (SerialFlash.exists("testfile.dat"))
  {
    Serial.println("File Exists, trying to open...");
    file = SerialFlash.open("testfile.dat");
    if (file)
    {
      // true if the file exists
      Serial.println("testfile.dat opened");
      uint8_t buffer[fileSize];

      buffer[0] = 11;
      Serial.print("add ");
      Serial.println(buffer[0]);
      buffer[1] = 12;
      Serial.print("add ");
      Serial.println(buffer[1]);
      buffer[2] = 13;
        Serial.print("add ");
      Serial.println(buffer[2]);
      buffer[3] = 14;
        Serial.print("add ");
      Serial.println(buffer[3]);
      buffer[4] = highByte(dataToWrite[2]);
        Serial.print("add ");
      Serial.println(buffer[4]);
      buffer[5] = lowByte(dataToWrite[2]);
        Serial.print("add ");
      Serial.println(buffer[5]);
      Serial.print("Writing Data to File ...");
      //IF YOU WANT TO WRITE TO ANOTHER POSITION IN THE FILE, USE "file.seek();"
      //IF YOU WANT TO CHECK YOUR CURRENT POSITION, USE "file.position()"
      
      file.write(buffer, 0);
      Serial.println("... completed");
    }
    else
    {
      Serial.println("testfile.dat not opened!!!");
    }
  }
}



///////////////////////////////////////////
//This function reads from the test file //
///////////////////////////////////////////

void readFile( void )
{ 
  Serial.println("");
  file = SerialFlash.open("testfile.dat");
  Serial.println("file opened");
  if (file)
  { // true if the file exists
    Serial.println("testfile.dat opened");
    int buffer[fileSize];
    //IF YOU WANT TO READ FROM ANOTHER POSITION IN THE FILE, USE "file.seek();"
    //IF YOU WANT TO CHECK YOUR CURRENT POSITION, USE "file.position()"
    Serial.print("Position: ");
    Serial.println(file.position());
    file.read(buffer, fileSize);
    Serial.println(file.position());
    Serial.println(buffer[0]);
    Serial.println(buffer[1]);
    Serial.println(buffer[2]);
    Serial.println(buffer[3]);
    Serial.println(buffer[4]);
    Serial.println(buffer[5]);

    file.close();
  }
  else
  {
    Serial.println("testfile.dat not opened!!!");
  }
}



