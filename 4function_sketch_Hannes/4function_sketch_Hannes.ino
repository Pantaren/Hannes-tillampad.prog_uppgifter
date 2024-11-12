//Full assignment 22/10/2024 - 12/11/2024

//Include libraries
#include <RTClib.h>
#include <Wire.h>
#include "U8glib.h"
#include <Servo.h>

//Init constants

//Init global variables
int ThermistorPin = A0;
int lightPin = 5;

//Iconstruct objects
RTC_DS3231 rtc;
Servo myservo;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);	// Display which does not send AC

void setup() {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  myservo.attach(9);
  u8g.setFont(u8g_font_unifont);
  myservo.write(90);
  for(int i = 5; i <=7; i++){
  pinMode(i, OUTPUT);
  }
}

void loop() {
  oledWrite(getTime(), 0, 22, "Temp is:" + String(getTemp()), 0, 44);
  myservo.write(slightlyHot());
  lightWarn();
}


String getTime(){
  DateTime now = rtc.now();
  return("Time is:" + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()));
}


// Oled screen fucntion
void oledWrite(String text1, int xPos, int yPos, String text2, int x, int y){
u8g.firstPage();  
  do {
    u8g.drawStr(xPos, yPos, text1.c_str());
    u8g.drawStr(x, y, text2.c_str());
  
  } while( u8g.nextPage() );

}


//Temperature reading function
float getTemp(){
  int Vo;
  float R1 = 10000; // value of R1 on board
  float logR2, R2, T;
  float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741; //steinhart-hart coeficients for thermistor

  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0); //calculate resistance on thermistor
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); // temperature in Kelvin
  T = T - 273.15; //convert Kelvin to Celcius
 // T = (T * 9.0)/ 5.0 + 32.0; //convert Celcius to Farenheit
 return T;
}

int slightlyHot(){
  return (map(getTemp(), 18, 28, 0, 179));
}

//Warning light function
void lightWarn(){
  digitalWrite(lightPin, LOW);
  lightPin = map(getTemp(), 18, 28, 5, 7);
  digitalWrite(lightPin, HIGH);
}