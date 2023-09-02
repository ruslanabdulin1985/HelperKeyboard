#include "Keyboard.h"
#include <ArduinoJson.h>
#include <EEPROM.h>
// char ctrlKey = KEY_LEFT_GUI;

uint16_t firstModifier1 = UINT16_MAX;
char key1 = '\0';
int key1address = 0;

volatile int key1PressTimer = 0;
volatile int key1ReleaseTimer = 0;
volatile int timerTarget = INT_MAX;
int buttonDelay = 30; // ms
String receivedData = "";


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, INPUT_PULLUP);  // Set pin 2 as INPUT
     
  attachInterrupt(digitalPinToInterrupt(2), buttonAction, CHANGE);  // Attach interrupt
  // put your setup code here, to run once:
  Serial.begin(9600);
  EEPROM.begin(512);

  Keyboard.begin();
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);

  key1 = EEPROM.read(key1address);
    Serial.println("Key 1 is ");

  Serial.println(char (EEPROM.read(key1address)));
}



void loop() {
  if (Serial.available()) {
    // Read input as a string
    String inputString = Serial.readStringUntil('\n');
  
    // Prepare the JSON document
    DynamicJsonDocument doc(1024);
  
    // Parse the JSON string
    DeserializationError err = deserializeJson(doc, inputString);
  
    // Get values
    if (doc.containsKey("key1") && doc["key1"].containsKey("value")) {
      const char* tempStr = doc["key1"]["value"];
      if (tempStr != nullptr && strlen(tempStr) > 0) {
        key1 = tempStr[0];
        EEPROM.write(key1address, (int) key1);
        if (EEPROM.commit()) {
          Serial.println("EEPROM successfully committed");
        } else {
          Serial.println("ERROR! EEPROM commit failed");
        }
      }
    }

    // Do something with values
  }
  
  if (millis() > timerTarget && digitalRead(2) == LOW) {
    Serial.println("Button 1 Pressed!");
    Keyboard.press(key1);
    timerTarget = INT_MAX;
  }

  if (millis() > timerTarget && digitalRead(2) == HIGH) {
    Serial.println("Button 1 Released!");
    Keyboard.release(key1);
    timerTarget = INT_MAX;
  }
}

void buttonAction() {
  timerTarget = millis() + buttonDelay;
}






