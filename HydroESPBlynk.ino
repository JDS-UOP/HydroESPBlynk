// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
#define BLYNK_TEMPLATE_ID           "xxxxx"
#define BLYNK_DEVICE_NAME           "HydroESP"
#define BLYNK_AUTH_TOKEN            "xxxxx"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer;
BlynkTimer timer2;

// Credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "xxxxx";
char pass[] = "xxxxx";

///// Pin/Variable Definitions /////
byte redLEDpin = 2;
byte blueLEDpin = 27;
byte TDSSensor = 33;
byte waterLevelSensor = 32;
byte waterThermometer = 35;
byte airThermometer = 34;

int TDS;
int waterLevel;
float waterTemp;
float waterThermReading;
float airTemp;
float airThermReading;

///// Functions /////
void redLEDPWM(int redLEDValue);
void blueLEDPWM(int blueLEDValue);

///// The following functions are called every time their respective Virtual Pin states change.
BLYNK_WRITE(V4) //Red LED Control
{
  // Set incoming value from pin V4 to redLEDPWM variable.
  redLEDPWM(param.asInt());
}

BLYNK_WRITE(V5) //Blue LED Control
{
  // Set incoming value from pin V5 to blueLEDPWM variable.
  blueLEDPWM(param.asInt());
}

void redLEDPWM(int redLEDValue){
  analogWrite(redLEDpin, redLEDValue);
}

void blueLEDPWM(int blueLEDValue){
  analogWrite(blueLEDpin, blueLEDValue);
}

// This timer sends the following values every second to their respective pins.
void sensorTimer()
{
  TDS = analogRead(TDSSensor);
  Blynk.virtualWrite(V0, TDS);

  waterLevel = analogRead(waterLevelSensor);
  Blynk.virtualWrite(V1, waterLevel);
  
  waterThermReading = analogRead(waterThermometer);
  float Voltage1 = waterThermReading * (5000/4096);
  waterTemp = Voltage1/10;
  Blynk.virtualWrite(V2, waterTemp);

  airThermReading = analogRead(airThermometer);
  float Voltage2 = airThermReading * (5000/4096);
  airTemp = Voltage2/10;
  Blynk.virtualWrite(V3, airTemp);
}


// This timer contains if statements that check if thresholds have been crossed 
// and sends notifications to the user's app every 5 minutes if so.
void notificationTimer()
{
  if (waterLevel < 50)
  {
     Blynk.logEvent("waterLevelLow");
  }
  
  if (TDS > 250)
  {
     Blynk.logEvent("TDSValueHigh");
  }
  
  if (waterTemp > 26)
  {
     Blynk.logEvent("waterTempHigh");
  }
  
  if (waterTemp < 18)
  {
     Blynk.logEvent("waterTempLow");
  }
  
  if (airTemp < 22)
  {
     Blynk.logEvent("airTempLow");
  }
  
  if (airTemp < 17)
  {
     Blynk.logEvent("airTempLow");
  }
}

void setup()
{
  //Start the serial console for debugging purposes.
  Serial.begin(115200);

  pinMode(redLEDpin, OUTPUT);
  pinMode(blueLEDpin, OUTPUT);
  
  Blynk.begin(auth, ssid, pass);
  
  timer.setInterval(1000L, sensorTimer);
  timer2.setInterval(300000L, notificationTimer);
}

void loop()
{
  Blynk.run();
  timer.run();
  timer2.run();
}
