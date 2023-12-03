#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_SSD1306.h>
#include "Adafruit_CCS811.h"
#include <HTU21D.h>


#define SDA_1 14
#define SCL_1 15

#define SDA_2 2
#define SCL_2 4

#define I2C_FREQ 400000
#define SCREEN_ADDRESS 0x3C


#define N 3

TwoWire I2C_1 = TwoWire(0);

Adafruit_CCS811 ccs;
Adafruit_SSD1306 display(128,64,&I2C_1);
HTU21D THsensor(0x40, I2C_1);


float co2;
float tvoc;
float temp;
float humid;

unsigned long t;
unsigned long last_stat;
unsigned long t_max[N];
unsigned long t_min[N];
float maxima[N] = {0,0,0};
float minima[N] = {1000,1000,100000};

int disp_mode = 0;


void setup() {
  Serial.begin(115200);


  I2C_1.begin(SDA_1, SCL_1, I2C_FREQ);

  ccs.begin(0x5A,&I2C_1);
  while(!ccs.available());

  THsensor.begin();
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
 

  display.clearDisplay();
      
  display.setTextColor(SSD1306_WHITE);    
  display.cp437(true);       

}

void loop() { 

  t = millis();

  if(ccs.available()){
    ccs.readData();
    co2 = ccs.geteCO2();
    tvoc = ccs.getTVOC();

    if (co2 > maxima[2]){
      maxima[2] = co2;
      t_max[2] = t;
    } 
  }
  
  if(THsensor.measure()) {
    temp = THsensor.getTemperature();
    humid = THsensor.getHumidity();

    if (temp > maxima[0]){
      maxima[0] = temp;
      t_max[0] = t;
    } 
    if (temp < minima[0]) {
      minima[0] = temp;
      t_min[0] = t;
    }
    if (humid > maxima[1]){
      maxima[1] = humid;
      t_max[1] = t;
    } 
    if (humid < minima[1]) {
      minima[1] = humid;
      t_min[1] = t;
    }
  }

  

  if (disp_mode == 0){
    display.clearDisplay();
    display.setTextSize(2); 
    display.setCursor(0, 0);    
    display.print(temp);
    display.print(" C");
    display.setCursor(0, 20); 
    display.print(humid);
    display.print(" %");
    display.setCursor(0, 40); 
    display.print(co2,0);
    display.print(" ppm");

    display.display();

    if (t > last_stat + 10000){
      last_stat = t;
      disp_mode = 1;
    }
  }
  else if (disp_mode == 1)
  {
    display.clearDisplay();
    display.setTextSize(1); 
    display.setCursor(0, 0);   


    display.println("MAX");

    display.print(maxima[0]);
    display.print("C ");
    display.print(float(t-t_max[0])/1000/60,0);
    display.println("min ago");

    display.print(maxima[1]);
    display.print("% ");
    display.print(float(t-t_max[1])/1000/60,0);
    display.println("min ago");

    display.print(maxima[2],0);
    display.print("ppm ");
    display.print(float(t-t_max[2])/1000/60,0);
    display.println("min ago");
    display.println(" ");



    display.println("MIN");

    display.print(minima[0]);
    display.print("C ");
    display.print(float(t-t_min[0])/1000/60,0);
    display.println("min ago");

    display.print(minima[1]);
    display.print("% ");
    display.print(float(t-t_min[1])/1000/60,0);
    display.println("min ago");   

    display.display();

    if (t > last_stat + 10000){
      last_stat = t;
      disp_mode = 0;
    }
  }
  

  

}