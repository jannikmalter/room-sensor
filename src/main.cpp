#include <Arduino.h>
#include <Wire.h>

#define SDA_1 14
#define SCL_1 15

#define SDA_2 33
#define SCL_2 32

//Default I2C address for the GY-21
#define GY21_I2C_ADDR    0x40

//Read temperature register
#define GY21_READ_TEMP   0xF3

//Read humidity register
#define GY21_READ_HUM    0xF5

//Time before give up readind new reg values
#define GY21_TIMEOUT     100

TwoWire THsens = TwoWire(0);
//TwoWire I2Ctwo = TwoWire(1);

float temp;

float get_temp() {
  THsens.beginTransmission(GY21_I2C_ADDR);
  THsens.write(GY21_READ_TEMP);
  THsens.endTransmission(true);
  delay(50);
  THsens.requestFrom(GY21_I2C_ADDR, 3);
  return (((THsens.read() << 8) | (THsens.read() & 0b11111100)) * 0.002681274f - 46.85f);
}


void setup() {
  Serial.begin(115200);
  Serial.println(F("THsens test"));

  THsens.begin(SDA_1, SCL_1, 100000); 
  //I2Ctwo.begin(SDA_2, SCL_2, 100000);


  Serial.println();
}

void loop() { 
  Serial.println(get_temp());
}