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
#include "SignalToolbox.h"

MAX30105 particleSensor;

#define debug Serial //Uncomment this line if you're using an Uno or ESP
//#define debug SerialUSB //Uncomment this line if you're using a SAMD21



/*
//EMA filter
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

// HR and SPO2
PPGfilter filterIR;
PPGfilter filterRed;
SignalToolbox featureIR;
SignalToolbox featureRed;

// RR
// PPGfilter filterAmpRR;


int SpO2 = 0;

float alphaPPG = 0.6;
int debouncePPG = 0;

float alphaRR = 0.05;
int debounceRR = 0;


void setup()
{
  debug.begin(9600);

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

  int IR = particleSensor.getIR();
  debug.print(">IR:");
  debug.println(IR);
  int signalIR = filterIR.EMAFilter(IR, alphaPPG, debouncePPG);
  debug.print(">SignalIR:");
  debug.println(signalIR);
  featureIR.SetSignal(signalIR);

  int Red = particleSensor.getRed();
  debug.print(">Red:");
  debug.println(Red);
  int signalRed = filterRed.EMAFilter(Red, alphaPPG, debouncePPG);
  debug.print(">SignalRed:");
  debug.println(signalRed); 
  featureRed.SetSignal(signalRed);

  //////////////// HR from IR
  int periodhr = featureIR.GetPeriod();
  Serial.print(">periodhr:");
  Serial.println(periodhr);

  int freqhr = featureIR.GetFreq();
  Serial.print(">freqhr:");
  Serial.println(freqhr);

  int promhr = featureIR.GetAve();
  Serial.print(">promhr:");
  Serial.println(promhr);

  int valleyhr = featureIR.GetValley();
  Serial.print(">valleyhr:");
  Serial.println(valleyhr);

  int peakhr = featureIR.GetPeak();
  Serial.print(">peakhr:");
  Serial.println(peakhr);

  ////////////// SPO2
  int ampIR = featureIR.GetAmp();
  Serial.print(">ampIR:");
  Serial.println(ampIR);

  int intIR = featureIR.GetInt();
  Serial.print(">intIR:");
  Serial.println(intIR);

  int ampRed = featureRed.GetAmp();
  Serial.print(">ampRed:");
  Serial.println(ampRed);

  int intRed = featureRed.GetInt();
  Serial.print(">intRed:");
  Serial.println(intRed);  

  if(ampRed > 0 && intIR >0 && ampIR > 0 && intIR >0)
  {
    float R = (float)(ampRed * intIR)/(intRed*ampIR);
    Serial.print(">R:");
    Serial.println(R);  
 
    SpO2 = 110 - 25*R;

  }

  if(SpO2 > 100)
  {
    SpO2 = 100;
  }
  Serial.print(">SpO2:");
  Serial.println(SpO2);  

/*
  ////////////// RR from IR
  int signalAmpRR = filterAmpRR.EMAFilter(ampIR, alphaRR, debounceRR);
  debug.print(">SignalAmpRR:");
  debug.println(signalAmpRR);
  featureIR.SetSignal(signalAmpRR);
*/



}
