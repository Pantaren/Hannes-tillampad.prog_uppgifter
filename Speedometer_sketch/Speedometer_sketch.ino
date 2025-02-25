/*
  Speedometer
  Author: Hannes Viktor
  Date: 18/02/2025
  
  Description:
  The program measures the speed of an object using two light sensors and an OLED screen.
  Two laser beams are used to detect when the object passes.
  The time between the two sensors is measured and used to calculate speed.
*/

// Include library for OLED display
#include "U8glib.h"

// Create OLED display object
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

// Define pin configuration for light sensors and lasers
const int lightSensor1Pin = A0; // Light sensor 1 connected to analog pin A0
const int lightSensor2Pin = A1; // Light sensor 2 connected to analog pin A1
const int laserPin1 = 13; // Laser 1 connected to digital pin 13
const int laserPin2 = 12; // Laser 2 connected to digital pin 12

// Variables for timing
int i = 0;
int time = 0;

    /*
      Initializes the system
      - Starts serial communication
      - Sets up laser pins as outputs
      - Configures font for the OLED display
    */
void setup() {
    Serial.begin(9600);
    pinMode(laserPin1, OUTPUT);
    pinMode(laserPin2, OUTPUT);
    u8g.setFont(u8g_font_unifont);
}

void loop() {
    /*
      Main loop that keeps the lasers active and calls the speed measurement function
    */
    digitalWrite(laserPin1, HIGH);
    digitalWrite(laserPin2, HIGH);
    speedVal();
}

    /*
      Measures the speed of an object passing through the light sensors.
      Parameters:
      -Void

      Returns:
      - The time in milliseconds for the object to pass between the sensors.
    */
void speedVal() {
    if (analogRead(lightSensor1Pin) > 900) {
        Serial.println("Timer Start:");
        oledWrite("Timer Start", 24, 32, "", 24, 64);
        
        while (analogRead(lightSensor2Pin) < 900) {
            i += 1;
            delay(1);
        }
        
        time = i;
        oledWrite("Score:" + String(1000 - time), 24, 32, "Speed:" + String(float(6.5)/(time)) + "m/s", 24, 64); //Calls on the oledWrite function to write down the score and speed on the oled screen
        Serial.println(i);
        i = 0;
    }
}

    /*
      Writes text to the OLED display.
      
      Parameters:
      - text1: First text to be displayed
      - xPos, yPos: Position for the first text
      - text2: Second text to be displayed
      - x, y: Position for the second text
    */
void oledWrite(String text1, int xPos, int yPos, String text2, int x, int y) {
    u8g.firstPage();  
    do {
        u8g.drawStr(xPos, yPos, text1.c_str());
        u8g.drawStr(x, y, text2.c_str());
    } while (u8g.nextPage());
}