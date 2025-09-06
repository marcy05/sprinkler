#include <stdio.h>
#include "wire.h"

#define PMICADD 0x6B

void pmicinnit(void);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:

}


void pmicinnit(void){

  Wire.begin();

  Wire.beginTransmission(PMICADD);
  Wire.write(0x0A);
  Wire.write(0x00);
  Wire.write(0x02);
  delay(100);

  Wire.beginTransmission(PMICADD);
  Wire.write(0x0A);
  Wire.write(0x03);
  Wire.write(0x01);
  delay(100);


  Wire.beginTransmission(PMICADD);
  Wire.write(0x02);
  Wire.write(0x01);
  Wire.write(0x05);
  delay(100);

  Wire.beginTransmission(PMICADD);
  Wire.write(0x02);
  Wire.write(0x02);
  Wire.write(0x05);
  delay(100);

}