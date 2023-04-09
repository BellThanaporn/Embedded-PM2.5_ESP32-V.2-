
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLog0ab_Ll"
#define BLYNK_DEVICE_NAME "Sharp Dust Sensor "

#define BLYNK_FIRMWARE_VERSION        "1.0.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_WROVER_BOARD

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "BlynkEdgent.h"
#include <Wire.h>
#include <TridentTD_LineNotify.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);   //Module IIC/I2C Interface 

#define LED 16 //LED Sensor
#define input_pin 33 //input analog sensor
#define LINE_TOKEN "LdNT4cHqLe25t4cSNjUqTP42lvibgtzS17ykC6wpMtP"

char auth[] = "ebiPSRmZPByk8dZjhXD5YFnkHENjik3q";
char ssid[] = "Galaxy A5111DA";
char pass[] = "big123456";
//BlynkTimer timer1;

int samplingTime = 280; 
int deltaTime = 40;
int sleepTime = 9680;
float avgDust = 0;
float tempDust = 0;
int time1 = 0;
int count ;
int num = 0;



float voMeasured = 0;
float V_Voltage = 0;
float dustDensity = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  lcd.init();
  lcd.backlight();  // เปิด backlight
  BlynkEdgent.begin();
  LINE.setToken(LINE_TOKEN);
}

void loop() {
  BlynkEdgent.run();
  //timer1.run(); 
  digitalWrite(LED, LOW); // power on the LED
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(input_pin); // read the dust value

  delayMicroseconds(deltaTime);
  digitalWrite(LED, HIGH); // turn the LED off
  delayMicroseconds(sleepTime);

  // Calculate dust value
  V_Voltage = voMeasured * (3.3 / 1024);
  dustDensity = 0.17 * V_Voltage - 0.1;
  dustDensity *= 1000;
 // float dustDensity = (0.172*(voMeasured*(5/1024.0))-0.0999)*1000;

  if (dustDensity <= 0.00) {
    dustDensity = 0.00;
  }

  
  // Calculate Average dust
  count = 8;
  if (time1 == count) {
      avgDust = tempDust/time1;
      tempDust = 0;
      time1 = 0;
      Serial.print("Average 1 min : ");
      Serial.print(avgDust);
      LINE.notify(String(avgDust)+" µg/m³"); 
      Serial.println(" µg/m³");
  } 
  
  else {
      tempDust += dustDensity;
      time1++;
      Serial.println(time1);
      Serial.print("Raw Signal Value (0-1023): "); 
      Serial.println(voMeasured);
    
      Serial.print("Voltage: ");
      Serial.println(V_Voltage);
      
      Serial.print("Dust density  : ");
      Serial.print(dustDensity);
      Blynk.virtualWrite(V22, dustDensity);
      Serial.println(" µg/m³");

  }
  
  Serial.println("_______________________________________________");
 
 
  lcd.clear();

  // LCD Print Now
 
  // LCD Print Avg
  lcd.setCursor(0, 1);
  lcd.print("Avg:");
  lcd.print(avgDust);
  Blynk.virtualWrite(V23, avgDust);
  
  lcd.print(" ug/m3");

  // LCD Print Level
  lcd.setCursor(0,0);
  if (dustDensity <= 25.00) {
        lcd.setCursor(3, 0);
        lcd.print("VERY GOOD");
        Blynk.virtualWrite(V0,"VERY GOOD");
  } else if (dustDensity >= 26.00 && dustDensity <= 37.00) {
        lcd.setCursor(5, 0);
        lcd.print("GOOD");
        Blynk.virtualWrite(V0,"GOOD");
  } else if (dustDensity >= 38.00 && dustDensity <= 50.00) {
        lcd.setCursor(4, 0);
        lcd.print("NORMAL"); 
        Blynk.virtualWrite(V0,"NORMAL");
  } else if (dustDensity >= 51.00 && dustDensity <= 80.00) {       
        lcd.setCursor(5, 0);
        lcd.print("BAD");
        Blynk.virtualWrite(V0,"BAD");
  } else if (dustDensity >= 80.00) {
        lcd.setCursor(3, 0);
        lcd.print("VERY BAD");
        Blynk.virtualWrite(V0,"VERY BAD");
  }
  

  delay(1000);
}
