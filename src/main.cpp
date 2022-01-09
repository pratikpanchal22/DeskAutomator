#include <Arduino.h>

const int LED_BUILTIN = 25;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  int offset=25;
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
}