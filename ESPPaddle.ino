/*
Embedded Systems MidtFinal Project - CDA4630
Based on ESP32 technology, NodeMCU-32S
Dr. Bassem Alhalabi
Florida Atlantic University, Boca Raton
Update: 10/23/2024

Edited by: Guillermo Tchelebi, Alain Padron
*/

#define CUSTOM_SETTINGS
#define INCLUDE_TERMINAL_MODULE

#include <DabbleESP32.h>

// ---------- defining LED receiver structure
struct Receiver {
  const int pin;
  int baseValue;
  int currentValue;
  int mappedValue;
  bool lastHit;
};

// ---------- defining pins and structures for LED receivers

Receiver center1 = {39, 0, 0, 0, false};
Receiver center2 = {36, 0, 0, 0, false};
Receiver bottom1 = {25, 0, 0, 0, false};
Receiver bottom2 = {33, 0, 0, 0, false};

// ---------- base values

int hitCount = 0; // Successful hit counter
bool hitFlag = false;

// ---------- Main functions
void setup() {
  
  // Set up ADC resolution to 10 bits: will get values between 0-1024
  analogReadResolution(10);

  pinMode(center1.pin, INPUT);
  pinMode(center2.pin, INPUT);
  pinMode(bottom1.pin, INPUT);
  pinMode(bottom2.pin, INPUT);

  delay(1000); // wait for 1000 ms

  Serial.begin(115200);
  Dabble.begin("ESPPaddle");

  center1.baseValue = analogRead(center1.pin);
  center2.baseValue = analogRead(center2.pin);
  bottom1.baseValue = analogRead(bottom1.pin);
  bottom2.baseValue = analogRead(bottom2.pin);
}

void loop() {

  bool centerHit = beingHit(center1) || beingHit(center2);
  bool bottomHit = beingHit(bottom1) || beingHit(bottom2);

  if (centerHit && bottomHit) {
    if (!hitFlag) {
      hitCount += 1;
      hitFlag = true; // Set the flag to true to indicate the condition has been met

      Terminal.print("Hit!\nSuccessful Hits: " + String(hitCount));
      Serial.println("----------------------------------------------");
      Serial.println("Hit!!!!!");
      Serial.println("Successful Hits: " + String(hitCount));
      Serial.println("Center Receivers:");
      Serial.print("1 - Current Value: "); Serial.print(center1.currentValue); Serial.print(" | Base Value: "); Serial.print(center1.baseValue); Serial.print(" | Mapped Value: "); Serial.println(center1.mappedValue);
      Serial.print("2 - Current Value: "); Serial.print(center2.currentValue); Serial.print(" | Base Value: "); Serial.print(center2.baseValue); Serial.print(" | Mapped Value: "); Serial.println(center2.mappedValue);
      Serial.println("Bottom Receivers:");
      Serial.print("1 - Current Value: "); Serial.print(bottom1.currentValue); Serial.print(" | Base Value: "); Serial.print(bottom1.baseValue); Serial.print(" | Mapped Value: "); Serial.println(bottom1.mappedValue);
      Serial.print("2 - Current Value: "); Serial.print(bottom2.currentValue); Serial.print(" | Base Value: "); Serial.print(bottom2.baseValue); Serial.print(" | Mapped Value: "); Serial.println(bottom2.mappedValue);
      Serial.println("----------------------------------------------");
    }
  } else {
    hitFlag = false; // Reset the flag when the condition is no longer met
  }

}

bool beingHit(Receiver &receiver) {

  receiver.currentValue = analogRead(receiver.pin);
  receiver.mappedValue = map(receiver.currentValue, receiver.baseValue, 1023, 0, 100);

  if (receiver.mappedValue >= 90) {
    return true;
  } else if (receiver.mappedValue < 10) {
    return false;
  };
}