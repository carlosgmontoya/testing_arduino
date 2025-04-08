#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "MAX30105.h"
#include "PPGfilter.h"
#include "SignalToolbox.h"

const char* ssid = "LAPTOP 9542";
const char* password = "43z21Z8!";
const char* mqtt_server = "192.168.137.1";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(20)
char msg_temp[MSG_BUFFER_SIZE];
char msg_bpm[MSG_BUFFER_SIZE];
char msg_spo2[MSG_BUFFER_SIZE];
char msg_rr[MSG_BUFFER_SIZE];

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/*
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}
*/
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("test/monitor/data", "Connected");
      // ... and resubscribe
      //client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

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

//int N=128;
//int data[128];
//int A=1;
//int i=0;
//float r=2*PI/N;
//int real=0;
//int imag=0;
//int sumReal=0;
//int sumImag=0;
//int bin=0;
//int binmax=0;
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

int temp=28;

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

  setup_wifi();
  client.setServer(mqtt_server, 1883);
//  client.setCallback(callback);
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
  debug.print(">Red:");
  debug.println(Red);
  int signalRed = filterRed.EMAFilter(Red, alphaPPG, debouncePPG);
  debug.print(">SignalRed:");
  debug.println(signalRed); 
  featureRed.SetSignal(signalRed);

  

  //////////////// HR from IR
  //int periodhr = featureIR.GetPeriod();
  //Serial.print(">periodhr:");
  //Serial.println(periodhr);

  //int freqhr = featureIR.GetFreq();
  //Serial.print(">freqhr:");
  //Serial.println(freqhr);

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
    float R = (float)((ampRed+400) * intIR)/(intRed*(ampIR+700));
//    Serial.print(">R:");
//    Serial.println(R);  
 
    SpO2 = 110 - 25*R;

  }

  if(SpO2 > 100)
  {
    SpO2 = 100;
  }
  Serial.print(">SpO2:");
  Serial.println(SpO2);  


  ////////////// TEMP RESERVED SPACE

  ////////////// RR from IR

  //  PPG Valley
  int valleyhr = featureIR.GetValley();
  Serial.print(">valleyhr:");
  Serial.println(valleyhr);

  va = 0.05 * valleyhr + (1 - 0.05) * va;

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

        // MQTT Communication
        if (!client.connected()) {
          reconnect();
        }
        client.loop();

        unsigned long now = millis();
        if (now - lastMsg > 5000) {
          lastMsg = now;

          temp = 32.10;

          snprintf (msg_temp, MSG_BUFFER_SIZE, "%i", temp);
          Serial.println(msg_temp);
          client.publish("temp", msg_temp);
      
          snprintf (msg_bpm, MSG_BUFFER_SIZE, "%i", promhr);
          Serial.println(msg_bpm);
          client.publish("bpm", msg_bpm);
      
          snprintf (msg_spo2, MSG_BUFFER_SIZE, "%i", SpO2);
          Serial.println(msg_spo2);
          client.publish("spo2", msg_spo2);
      
          snprintf (msg_rr, MSG_BUFFER_SIZE, "%i", promrr);
          Serial.println(msg_rr);
          client.publish("rr", msg_rr);
        }

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
