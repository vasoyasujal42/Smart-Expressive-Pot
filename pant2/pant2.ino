/*
 * This program is property of SME Dehradun. for any query related to this program, contact us at www.smedehradn.com
 * if your want any soluiton related for any IoT Customized Boards and Sensor to www.nuttyengineer.com 
 */

#include <LiquidCrystal_I2C.h>
#define BLYNK_TEMPLATE_ID "TMPLfLR1qlFu"
#define BLYNK_DEVICE_NAME "Plant Monitoring"
#define BLYNK_FIRMWARE_VERSION "0.1.0"
#define BLYNK_PRINT Serial

#include "BlynkEdgent.h"
#include "DHT.h"

char auth[] = "GVzp6oFDY6CtHXA43k9BPqD7XAedxqe2";   //Enter your Auth token
char ssid[] = "Sujal's Galaxy S20 FE 5G";           //Enter your WIFI name
char pass[] = "mwdy2003";                           //Enter your WIFI password

#define DHTPIN 26 
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
float t, h;

int lcdColumns = 16;
int lcdRows = 2;
int a; // IR sensor
int ldr; // for LDR
int touch; //for touch
int delta; //for sound
int pinValue1; //MOTOR
int mm; //motor manual
int soundd;
int airr;
int sol;
int motorr; //motor stauts

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void setup()
{ 
  
  pinMode(2,OUTPUT); //in built led
  pinMode(27,INPUT); //IR sensor
  pinMode(13,INPUT_PULLUP); //motor pushbutton
  pinMode(25,OUTPUT); // motor out
  digitalWrite(25,LOW);
  
  lcd.init();
  lcd.backlight();
  dht.begin();
  Serial.begin(115200);
  
  lcd.setCursor(0, 0);
  lcd.print("Plant Monitiring");
  delay(500);
  
  Blynk.begin(auth, ssid, pass);
  //BlynkEdgent.begin();
    
  lcd.clear(); 
  
  timer.setInterval(100L,soilsensor);
  timer.setInterval(100L,dht11);
  timer.setInterval(100L,IR);
  timer.setInterval(100L,ldrsensor);
  timer.setInterval(100L,touchsensor);
  timer.setInterval(100L,sound);
  timer.setInterval(100L,air);
  timer.setInterval(100L,motor);
   
}

void loop() 
{ 
  mm= digitalRead(13);// motor pin
  if(pinValue1==1 || mm==0 )
  {
    digitalWrite(25,HIGH);
    motorr=1;    
  }
  else{
    digitalWrite(25,LOW);
    motorr=0;    
  }
  Blynk.run();
  timer.run(); // Initiates SimpleTimer
  
}

void soilsensor()
{ 
  sol = analogRead(33);
  sol = map(sol,1500,4095,100,0);
  Blynk.virtualWrite(V7,sol); //moisture
  if (sol < 20)
  {
   Blynk.logEvent("low_soil_moisture");
  }
  if (sol > 90)
  {
   Blynk.logEvent("high_soil_moisture");
  }
  lcd.setCursor(8, 1);
  lcd.print("S:");
  lcd.print(sol);
  lcd.print(" ");
}



void dht11()
{
  h = dht.readHumidity();
  t = dht.readTemperature();  
  Blynk.virtualWrite(V6, h);
  Blynk.virtualWrite(V5, t);
  if (t > 45)
  {
   Blynk.logEvent("high_temperature");
  }
  if (t < 10 )
  {
   Blynk.logEvent("low_temperature");
  }  
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t);

  lcd.setCursor(8, 0);
  lcd.print("H:");
  lcd.print(h);
}

void IR()
{
  a=digitalRead(27);
  a=!a;
  Blynk.virtualWrite(V1, a);
}

void ldrsensor()
{
  ldr = analogRead(34);
  ldr = map(ldr,0,4095,100,0);
  Blynk.virtualWrite(V2,ldr);
  
  lcd.setCursor(0, 1);
  lcd.print("L:");
  lcd.print(ldr);
  lcd.print(" ");
}

void touchsensor()
{
  touch=touchRead(12);
  Blynk.virtualWrite(V3,touch);
}



void sound()
{
 soundd = analogRead(35);
 Blynk.virtualWrite(V4,soundd);
}

void air()
{
  airr = analogRead(32);
  Blynk.virtualWrite(V9,airr); //Air quality
}

void motor()
{
   Blynk.virtualWrite(V10,motorr); //motor stauts 
}

BLYNK_WRITE(V0){
  int pinValue=param.asInt();
  digitalWrite(2,pinValue);
  Serial.println(pinValue);
}
BLYNK_WRITE(V8){  // motor
  pinValue1=param.asInt();
  
}
