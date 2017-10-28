#include <NXPMotionSense.h>
#include <Wire.h>
#include <EEPROM.h>
#include <util/crc16.h>

NXPMotionSense imu;

void setup() {
  Serial.begin(115200);
  while (!Serial) ; // wait for serial port open
  imu.begin();
}

void loop() {
  int ax, ay, az;
  int gx, gy, gz;
  int mx, my, mz;

  // get and print uncalibrated data
  if (imu.available()) {
    imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);
    Serial.print("Raw Acceleration:");
    Serial.print('\t');
    Serial.print(ax);
    Serial.print(',');
    Serial.print('\t');
    Serial.print(ay);
    Serial.print(',');
    Serial.print('\t');
    Serial.print(az);
    Serial.print('\t');
    Serial.print('\t');
    Serial.print("Raw Gyroscope:");
    Serial.print('\t');
    Serial.print(gx);
    Serial.print(',');
    Serial.print('\t');
    Serial.print(gy);
    Serial.print(',');
    Serial.print('\t');
    Serial.print(gz);
    Serial.print('\t');
    Serial.print('\t');
    Serial.print("Raw Magnetometer:");
    Serial.print('\t');
    Serial.print(mx);
    Serial.print(',');
    Serial.print('\t');
    Serial.print(my);
    Serial.print(',');
    Serial.print('\t');
    Serial.print(mz);
    Serial.println();
  }
}


