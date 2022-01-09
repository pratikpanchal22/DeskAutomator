#include <Arduino.h>
#include <Wire.h>
#include "SSD1306.h"

// onboard LED
const int LED_BUILTIN = 25;

// onboard OLED display
const int OLED_I2C_ADDRESS = 0x3C;
const int OLED_SDA = 4;
const int OLED_SCL = 15;
const int OLED_RST = 16;

SSD1306 display(OLED_I2C_ADDRESS, OLED_SDA, OLED_SCL); 
int itr=0;

void setup() {
  //initialize LED
  pinMode(LED_BUILTIN, OUTPUT);

  //initialize display
  pinMode(OLED_RST, OUTPUT);
  //reset display
  digitalWrite(OLED_RST, 0);
  //delay(100);
  //enable display
  digitalWrite(OLED_RST, 1);

  Serial.begin(115200);
	
  display.init();

  display.setFont(ArialMT_Plain_16);

  display.drawString(0, 0, "Initializing App");
  display.display();

}

void loop() {
  // put your main code here, to run repeatedly:

  int offset=10;
  for(int i=0; i<10; i++){
    digitalWrite(LED_BUILTIN, HIGH);
    delay((i+1)*offset);
    digitalWrite(LED_BUILTIN, LOW);
    delay((i+1)*offset);
  }

  for(int i=9; i>0; i--){
    digitalWrite(LED_BUILTIN, HIGH);
    delay((i+1)*offset);
    digitalWrite(LED_BUILTIN, LOW);
    delay((i+1)*offset);
  }

  itr++;
  char buffer[20];
  sprintf(buffer, "Iteration i = %d", itr);
  
  display.clear();
  //display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(0, 0, "Hello World");
  display.drawString(0, 22, buffer);
  display.display();
}