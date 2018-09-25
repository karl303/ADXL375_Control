/*
  This is a controller for the ADXL375, based on the Arduino example for a
  digital potentiometer.


 created 23 Sep 2018
 by Karl Erickson

*/


// inslude the SPI library:
#include <SPI.h>

// set pin 6 as the slave select for the accelerometer:
const int slaveSelectPin = 6;

// Declare variables for holding upper and lower bytes of accelerometer readings
int x_lower = 0;
int x_upper = 0;
int y_lower = 0;
int y_upper = 0;
int z_lower = 0;
int z_upper = 0;
int x_temp = 0;
int y_temp = 0;
int z_temp = 0;

float x_g = 0;
float y_g = 0;
float z_g = 0;


void setup() {
  // Set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);

  // Start the serial interface
  Serial.begin(9600);

  
  // initialize SPI:
  SPI.begin();
  // Set SPI bit rate to 1-Mbps and SPI mode to 3
  // Page 15 of 32 on ADXL375 datasheet calls for clock polarity
  // and clock phase both being equal to 1
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));

  Serial.println("Setting DATA_FORMAT register");
  
  digitalWrite(slaveSelectPin, LOW);  // take the SS pin low to select the chip
  //  send in the address and value via SPI:
  SPI.transfer(0x31);   // 0x31 is the DATA_FORMAT register
  SPI.transfer(0x0F);   // 0x0F sets:
                        //    D7 - 0 - SELF_TEST - off
                        //    D6 - 0 - SPI bit - 4-wire mode
                        //    D5 - 0 - INT_INVERT - active high interrupts
                        //    D4 - 0
                        //    D3 - 1
                        //    D2 - 1 - Justify - left justified MSB mode
                        //    D1, D0 - 1
  digitalWrite(slaveSelectPin, HIGH); // take the SS pin high to de-select the chip

  Serial.println("Setting the data rate field");
  digitalWrite(slaveSelectPin, LOW);
  //  send in the address and value via SPI:
  SPI.transfer(0x2C);   // 0x2C BW_RATE
  SPI.transfer(0x08);   // Output data rate of 25-Samples/sec, BW of 12.5-Hz
  digitalWrite(slaveSelectPin, HIGH);

  digitalWrite(slaveSelectPin, LOW);
  //  send in the address and value via SPI:
  SPI.transfer(0x2D);   // 0x2D POWER_CTL
  SPI.transfer(0x08);   // 0x08 Places part into measurement mode
  digitalWrite(slaveSelectPin, HIGH);
}

void loop() {
  
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0xF2); // Registers 0x32 to 0x37 are data registers
                      // The upper bits are read (active high) and 
                      // multi-byte (active high), which transforms
                      // the 0x32 into 0xF2
  x_lower = SPI.transfer(0x00);
  x_upper = SPI.transfer(0x00);
  y_lower = SPI.transfer(0x00);
  y_upper = SPI.transfer(0x00);
  z_lower = SPI.transfer(0x00);
  z_upper = SPI.transfer(0x00);
  digitalWrite(slaveSelectPin, HIGH);


  // Take the upper byte, left shift by 8 bits and bitwise
  // OR with the lower byte of the reading.  Then, left shift
  // by 16 bits so we're using the full 32 bits of the
  // integer variable
  x_temp = (x_lower | (x_upper << 8)) << 16;
  y_temp = (y_lower | (y_upper << 8)) << 16;
  z_temp = (z_lower | (z_upper << 8)) << 16;

  // Divide by a scale factor
  // 10747904 = 2^16 * 20 * 8
  //    2^16 accounts for the 16-bit left-shift performed above
  //    20 is the typical LSB/g acceleration response
  //    8 accounts for the actual measurement resolution being 13-bit
  //    rather than the 16-bit presented by the SPI interface
  x_g = (float)x_temp / 10747904;
  y_g = (float)y_temp / 10747904;
  z_g = (float)z_temp / 10747904;



  Serial.print(x_g);
  Serial.print(",");
  Serial.print(y_g);
  Serial.print(",");
  Serial.println(z_g);
  
  delay(500);
}


