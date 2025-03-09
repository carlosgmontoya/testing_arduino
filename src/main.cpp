#include <Arduino.h>

/*
  MAX30105 Breakout: Output all the raw Red/IR/Green readings
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 2nd, 2016
  https://github.com/sparkfun/MAX30105_Breakout

  Outputs all Red/IR/Green values.

  Hardware Connections (Breakoutboard to Arduino):
  -5V = 5V (3.3V is allowed)
  -GND = GND
  -SDA = A4 (or SDA)
  -SCL = A5 (or SCL)
  -INT = Not connected

  The MAX30105 Breakout can handle 5V or 3.3V I2C logic. We recommend powering the board with 5V
  but it will also run at 3.3V.

  This code is released under the [MIT License](http://opensource.org/licenses/MIT).
*/

#include <Wire.h>
#include "MAX30105.h"
#include "PPGfilter.h"

MAX30105 particleSensor;

#define debug Serial //Uncomment this line if you're using an Uno or ESP
//#define debug SerialUSB //Uncomment this line if you're using a SAMD21

// Peak detector
int xn = 0;
int xn_1 = 0;
int xn_2 = 0;

// Peaks of peaks (intensity)
int pn = 0;
int pn_1 = 0;
int pn_2 = 0;

// Peaks of amplitude
int pa = 0;
int pa_1 = 0;
int pa_2 = 0;

// Peaks of frequency
int pb = 0;
int pb_1 = 0;
int pb_2 = 0;

// Peaks of sum
int pc = 0;
int pc_1 = 0;
int pc_2 = 0;

// Frequency of intensity
int startr = 0;
int endr = 0;
int halfr = 0;
int periodr = 0;
int freqr = 0;

// Frequency of amplitude
int starta = 0;
int enda = 0;
int halfa = 0;
int perioda = 0;
int freqa = 0;
int contra = 0;

// Frequency of frequency
int startb = 0;
int endb = 0;
int halfb = 0;
int periodb = 0;
int freqb = 0;
int contrb = 0;

// Frequency of intensity
int startc = 0;
int endc = 0;
int halfc = 0;
int periodc = 0;
int freqc = 0;
int contrc = 0;

// Frequency of heart
int starthr = 0;
int endhr = 0;
int periodhr = 0;
int freqhr = 0;
int conthr = 0;
int pahr = 0;
int pbhr = 0;
int amphr = 0;
int conta = 0;
int periodhr_1=0;
int datahr[10]={0,0,0,0,0,0,0,0,0,0};
int sumahr=0;
int promhr=0;
int ihr=0;


/*
//EMA filter
float EMA_ALPHA_ = 0.05;
float EMA_ALPHA = 0.2;
int EMA_LP = 0;
int EMA_HP = 0;
int EMA_LP_ = 0;

int EMALowPassFilter(int value)
{
  EMA_LP_ = EMA_ALPHA_ * value + (1 - EMA_ALPHA_) * EMA_LP_;
  return EMA_LP_;
}

int EMAHighPassFilter(int value)
{
  EMA_LP = EMA_ALPHA * value + (1 - EMA_ALPHA) * EMA_LP;
  EMA_HP = value - EMA_LP;

  return EMA_HP;
}
*/

PPGfilter filter;

void setup()
{
  debug.begin(9600);
  filter.init();

  // Initialize sensor
  if (particleSensor.begin() == false)
  {
    debug.println("Sensor was not found. Please check wiring/power. ");
    while (1);
  }

  particleSensor.setup(); //Configure sensor. Use 6.4mA for LED drive
}

void loop()
{

//  debug.print(">Red:");
//  debug.println(particleSensor.getRed());
  debug.print(">IR:");
  debug.println(particleSensor.getIR());
  

 // int ppg = particleSensor.getIR();
 // int filterSignal = filter.EMAFilter(ppg);
 

 ////////////////TESTING HR BEGIN
  int ppg = particleSensor.getIR();
  int filterSignal = filter.EMAFilter(ppg);
  debug.print(">Filtered signal:");
  debug.println(filterSignal);

  //Peak and valley detector
  xn = filterSignal;

  // Valley (cambia a pendiente positiva)
  if(xn - xn_1 > 0 && xn_1 - xn_2 <= 0)
  {
    if(conthr >= 1)
    {
      endhr = millis();
      periodhr = endhr - starthr;
      freqhr = 60000 / periodhr;


      datahr[0]=freqhr;
      for(int ihr=0; ihr<9; ihr++)
      {
        datahr[9-ihr]=datahr[8-ihr];
        sumahr = sumahr + datahr[9-ihr];
      }
      promhr=sumahr/10;
      sumahr=0;

      conthr = 0;
      starthr = millis();
    }
  }    
    
  // Pico (cambia a pendiente negativa)
  if(xn - xn_1 <0 && xn_1 - xn_2 >= 0)
  {
    conthr++;
  }

  Serial.print(">freqhr:");
  Serial.println(freqhr);
  Serial.print(">promhr:");
  Serial.println(promhr);
}
