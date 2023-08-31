#include "Keyboard.h"
// char ctrlKey = KEY_LEFT_GUI;

volatile int key1 = 0;
volatile int key2 = 0;
volatile int key3 = 0;
volatile int key1PressTimer = 0;
volatile int key1ReleaseTimer = 0;
volatile int timerTarget = INT_MAX;
int buttonDelay = 30; // ms


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
  if (millis() > timerTarget && digitalRead(2) == LOW) {
    Serial.println("Button 1 Pressed!");
    Keyboard.press('n');
    timerTarget = INT_MAX;
  }

  if (millis() > timerTarget && digitalRead(2) == HIGH) {
    Serial.println("Button 1 Released!");
    Keyboard.release('n');
    timerTarget = INT_MAX;
  }
}

void buttonAction() {
  timerTarget = millis() + buttonDelay;
}






