#include "Keyboard.h"
#include <ArduinoJson.h>
#include <EEPROM.h>

uint16_t firstModifier1 = UINT16_MAX;
char key1 = '\0';
uint8_t key1modifier1 = '\0';
int key1address = 0;
int key1modifier1address = 1;

volatile int timerTarget = INT_MAX;
int buttonDelay = 30; // ms
int modifierDelay = 10; //ms


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
  key1modifier1 = EEPROM.read(key1modifier1address);
}

void loop() {
  if (Serial.available()) {
    // Read input as a string
    String inputString = Serial.readStringUntil('\n');
  
    // Prepare the JSON document
    DynamicJsonDocument doc(1024);
  
    // Parse the JSON string
    DeserializationError err = deserializeJson(doc, inputString);
    bool changed = false;
    // Get values
    if (doc.containsKey("key1") && doc["key1"].containsKey("value")) {
      const char* tempStr = doc["key1"]["value"];
      if (tempStr != nullptr && strlen(tempStr) > 0) {
        key1 = tempStr[0];
        EEPROM.write(key1address, (int) key1);
        EEPROM.write(key1modifier1address, '\0');

        changed = true;
      }

    if (doc.containsKey("key1") && doc["key1"].containsKey("modifier1")) {
      const char* modifier = doc["key1"]["modifier1"];

      key1modifier1 = getKeyModifier(modifier);
      EEPROM.write(key1modifier1address, key1modifier1);
      changed = true;
      
    }

    if (changed && EEPROM.commit()) {
        Serial.println("EEPROM successfully committed");
      } else {
        Serial.println("ERROR! EEPROM commit failed");
      }
     
    }
  }
  
  if (millis() > timerTarget && digitalRead(2) == LOW) {
    Keyboard.press(key1modifier1);
    delay(modifierDelay);
    Keyboard.press(key1);

    timerTarget = INT_MAX;
  }

  if (millis() > timerTarget && digitalRead(2) == HIGH) {
    Keyboard.release(key1modifier1);
    delay(modifierDelay);
    Keyboard.release(key1);

    timerTarget = INT_MAX;
  }
}

uint8_t getKeyModifier(const char* modifier) {
  if (strcmp(modifier, "Command") == 0) {
    return KEY_LEFT_GUI;
  }
  if (strcmp(modifier, "Control") == 0) {
    return KEY_LEFT_CTRL;
  }
   if (strcmp(modifier, "Shift") == 0) {
    return KEY_LEFT_SHIFT;
  }
  if (strcmp(modifier, "Option") == 0) {
    return KEY_LEFT_ALT;
  }
  if (strcmp(modifier, "Tab") == 0) {
    return KEY_TAB;
  }

  return '\0';
}

void buttonAction() {
  timerTarget = millis() + buttonDelay;
}






