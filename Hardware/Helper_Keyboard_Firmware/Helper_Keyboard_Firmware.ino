#include "Keyboard.h"
char ctrlKey = KEY_LEFT_GUI;

volatile int key1 = 0;
volatile int key2 = 0;
volatile int key3 = 0;
volatile int key1PressTimer = 0;
volatile int key1ReleaseTimer = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, INPUT_PULLUP);  // Set pin 2 as INPUT
     
  attachInterrupt(digitalPinToInterrupt(2), buttonAction, CHANGE);  // Attach interrupt
  // put your setup code here, to run once:
  Serial.begin(9600);
  Keyboard.begin();
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)

}

void loop() {
  if (key1PressTimer>0){
    key1PressTimer++;
  }

  if (key1ReleaseTimer>0){
    key1ReleaseTimer++;
  }

  if (key1PressTimer == 300000) {
    Serial.println("Button 1 Pressed!");
    key1PressTimer = 0;          
  }



   if (key1ReleaseTimer == 300000) {
    Serial.println("Button 1 Released!");
    key1ReleaseTimer = 0;
  }


}

void buttonAction() {
  if (digitalRead(2) == LOW) {
      if (key1PressTimer == 0) {
        key1PressTimer = 1;
    }
  

    if (key1ReleaseTimer > 0) {
      key1ReleaseTimer = 0;
    }
  } else {
    if (key1ReleaseTimer == 0) {
    key1ReleaseTimer = 1;

  }
  
    if (key1PressTimer > 0) {
      key1PressTimer = 0;
    }
  }
}






