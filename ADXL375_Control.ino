/*
  This is a controller for the ADXL375, based on the Arduino example for a
  digital potentiometer.


 created 23 Sep 2018
 by Karl Erickson

*/


// inslude the SPI library:
#include <SPI.h>


// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 6;

int x_lower = 0;
int x_upper = 0;
int y_lower = 0;
int y_upper = 0;
int z_lower = 0;
int z_upper = 0;
int x_temp = 0;
int y_temp = 0;
int z_temp = 0;

int x_neg = 0;
int y_neg = 0;
int z_neg = 0;

void setup() {
  // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);

  Serial.begin(9600);

  
  // initialize SPI:
  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));

  Serial.println("Setting DATA_FORMAT register");
  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin, LOW);
  //  send in the address and value via SPI:
  SPI.transfer(0x31);
  SPI.transfer(0x0F);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin, HIGH);

  Serial.println("Setting the data rate field");
    // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin, LOW);
  //  send in the address and value via SPI:
  SPI.transfer(0x2C);
  SPI.transfer(0x03);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin, HIGH);

  digitalWrite(slaveSelectPin, LOW);
  //  send in the address and value via SPI:
  SPI.transfer(0x2D);
  SPI.transfer(0x08);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin, HIGH);
/*
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0x80);
  Serial.print(SPI.transfer(0x01), HEX);
  digitalWrite(slaveSelectPin, HIGH);
*/
}

void loop() {
  x_neg = 0;
  y_neg = 0;
  z_neg = 0;
  
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0xB2);
  x_lower = SPI.transfer(0x00);
  x_upper = SPI.transfer(0x00);
  y_lower = SPI.transfer(0x00);
  y_upper = SPI.transfer(0x00);
  z_lower = SPI.transfer(0x00);
  z_upper = SPI.transfer(0x00);
  digitalWrite(slaveSelectPin, HIGH);

/*
  x_neg = x_upper >> 7;
  y_neg = y_upper >> 7;
  z_neg = z_upper >> 7;
*/

  x_temp = (x_lower | (x_upper << 8)) << 16;
  y_temp = (y_lower | (y_upper << 8)) << 16;
  z_temp = (z_lower | (z_upper << 8)) << 16;

  x_temp = x_temp / 1310720;
  y_temp = y_temp / 1310720;
  z_temp = z_temp / 1310720;

  /*
  if(x_neg)
  {
    x_temp = x_temp & 0x00007FFF;
    x_temp = x_temp | 0x80000000;
  }

  if(y_neg)
  {
    y_temp = y_temp & 0x00007FFF;
    y_temp = y_temp | 0x80000000;
  }

  if(z_neg)
  {
    z_temp = z_temp & 0x00007FFF;
    z_temp = z_temp | 0x80000000;
  }
  */
  
/*
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0xB6);
  temp_z = SPI.transfer(0x00);
  digitalWrite(slaveSelectPin, HIGH);

  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0xB7);
  temp_z = temp_z | (SPI.transfer(0x00) << 8);
  digitalWrite(slaveSelectPin, HIGH);
*/

/*
  Serial.print(x_temp);
  Serial.print(",");
  Serial.print(y_temp);
  Serial.print(",");
*/
  Serial.println(z_temp);

  /*
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0xB3);
  Serial.print(SPI.transfer(0x03), HEX);
  digitalWrite(slaveSelectPin, HIGH);

  Serial.print(',');

  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0xB4);
  Serial.print(SPI.transfer(0x04), HEX);
  digitalWrite(slaveSelectPin, HIGH);

  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0xB5);
  Serial.print(SPI.transfer(0x05), HEX);
  digitalWrite(slaveSelectPin, HIGH);
  
  Serial.print(',');

  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0xB6);
  Serial.print(SPI.transfer(0x06), HEX);
  digitalWrite(slaveSelectPin, HIGH);

  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0xB7);
  Serial.println(SPI.transfer(0x07), HEX);
  digitalWrite(slaveSelectPin, HIGH);
  */
  
  delay(3000);
}


