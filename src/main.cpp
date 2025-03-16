#include <Arduino.h>
#include <Wire.h>
#include "MAX30105.h"
#include "PPGfilter.h"
#include "SignalToolbox.h"

MAX30105 particleSensor;

#define debug Serial //Uncomment this line if you're using an Uno or ESP
//#define debug SerialUSB //Uncomment this line if you're using a SAMD21

// HR and SPO2
PPGfilter filterIR;
PPGfilter filterRed;
SignalToolbox featureIR;
SignalToolbox featureRed;

int SpO2 = 0;

float alphaPPG = 0.6;
int debouncePPG = 10;

int N=128;
int data[128];
int A=1;
int i=0;
float r=2*PI/N;
int real=0;
int imag=0;
int sumReal=0;
int sumImag=0;
int bin=0;
int binmax=0;
int va=0;
int va_1=0;
int va_2=0;
int startrr=0;
int endrr=0;
int periodrr=0;
int freqrr=0;
int sumarr=0;
int promrr=0;
int contrr=0;
int datarr[3]={0,0,0};

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
  int startTime = micros();

  int IR = particleSensor.getIR();
  debug.print(">IR:");
  debug.println(IR);
  int signalIR = filterIR.EMAFilter(IR, alphaPPG, debouncePPG);
  debug.print(">SignalIR:");
  debug.println(signalIR);
  featureIR.SetSignal(signalIR);

  int Red = particleSensor.getRed();
  //debug.print(">Red:");
  //debug.println(Red);
  int signalRed = filterRed.EMAFilter(Red, alphaPPG, debouncePPG);
  //debug.print(">SignalRed:");
  //debug.println(signalRed); 
  featureRed.SetSignal(signalRed);

  

  //////////////// HR from IR
  //int periodhr = featureIR.GetPeriod();
  //Serial.print(">periodhr:");
  //Serial.println(periodhr);

  int freqhr = featureIR.GetFreq();
  Serial.print(">freqhr:");
  Serial.println(freqhr);

  int promhr = featureIR.GetAve();
  Serial.print(">promhr:");
  Serial.println(promhr);

  //int peakhr = featureIR.GetPeak();
  //Serial.print(">peakhr:");
  //Serial.println(peakhr);

  ////////////// SPO2
  int ampIR = featureIR.GetAmp();
  //Serial.print(">ampIR:");
  //Serial.println(ampIR);

  int intIR = featureIR.GetInt();
  //Serial.print(">intIR:");
  //Serial.println(intIR);

  int ampRed = featureRed.GetAmp();
  //Serial.print(">ampRed:");
  //Serial.println(ampRed);

  int intRed = featureRed.GetInt();
  //Serial.print(">intRed:");
  //Serial.println(intRed);  

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


  ////////////// RR from IR

  //  PPG Valley
  int valleyhr = featureIR.GetValley();
  Serial.print(">valleyhr:");
  Serial.println(valleyhr);

  va=valleyhr;

  if(va!=va_1){

  // PEAK DETECTION - VALLE (cambia a pendiente positiva)
    if(va - va_1 > 0 && va_1 - va_2 <= 0)
    {
      if(contrr >= 1)
      {
        int endrr = millis();
        int periodrr = endrr - startrr;
        int freqrr = 60000 / periodrr;  

        Serial.print(">freqrr:");
        Serial.println(freqrr);

        // rr prom
        datarr[0]=freqrr;

        for(int i=0; i<2; i++)
        {
          datarr[2-i]=datarr[1-i];
        }

        int sumarr = datarr[2]+datarr[1]+datarr[0];
        int promrr=sumarr/3;
        Serial.print(">promrr:");
        Serial.println(promrr);

        sumarr=0;
    
        contrr = 0;
        startrr = millis();
      }

    }  
 
    // PICO (cambia a pendiente negativa)
    if(va - va_1 <0 && va_1 - va_2 >= 0)
    {
      contrr++;
    }

    Serial.print(">va:");
    Serial.println(va);
    
/*  METODO DTF

    if(i==N){

      for(int f=0; f<=4; f++ ){
        for(int n=0; n<=N; n++){
          real = real+data[n]*cos(r*f*n);
          imag = imag+data[n]*sin(r*f*n);
        }
        bin = sqrt(real*real + imag*imag);      
        Serial.print(">bin:");
        Serial.println(bin);
        real=0;
        imag=0;
        if(bin >= binmax){
          binmax=bin;
          int fmax=f;
          Serial.print(">fmax:");
          Serial.println(fmax);
        }
      }
    
      i=0;
    }

    data[i]=(A*va);
    Serial.print(">data[i]:");
    Serial.println(data[i]);
    i++;
 */  

  }

  va_2=va_1;
  va_1=va;

  while(62500 > micros()-startTime){
  }

}
