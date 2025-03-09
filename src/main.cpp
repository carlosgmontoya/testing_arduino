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
  

  int ppg = particleSensor.getIR();

  int filterHP = filter.EMAFilter(ppg);

  debug.print(">filterLP:");
  debug.println(filterHP);


}