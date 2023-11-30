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

TwoWire THsens = TwoWire(0);
//TwoWire I2Ctwo = TwoWire(1);

float temp;
float humid;

void meas_TH() {
  float calc;

  THsens.beginTransmission(GY21_I2C_ADDR);
  THsens.write(GY21_READ_TEMP);
  THsens.endTransmission(true);
  delay(50);
  THsens.requestFrom(GY21_I2C_ADDR, 3);
  temp = ((THsens.read() << 8) | (THsens.read() & 0b11111100)) * 0.002681274f - 46.85f;


  THsens.beginTransmission(GY21_I2C_ADDR);
  THsens.write(GY21_READ_HUM);
  THsens.endTransmission(true);
  delay(16);
  THsens.requestFrom(GY21_I2C_ADDR, 3);  
  uint16_t Srh = (THsens.read() << 8) | (THsens.read() & 0xFC);
  calc = -6.0 + 125.0 * Srh / 65536.0;
  humid = calc + (25.0 - temp) * -0.15;
}


void setup() {
  Serial.begin(115200);
  Serial.println("Hello!");
  THsens.begin(SDA_1, SCL_1, 100000); 
  //I2Ctwo.begin(SDA_2, SCL_2, 100000);
}

void loop() { 
  meas_TH();
  Serial.print(temp,6);
  Serial.print("\t");
  Serial.println(humid,6);
}