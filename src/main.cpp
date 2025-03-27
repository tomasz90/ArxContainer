#include "Arduino.h"

void setup() {
    Serial.begin(BAUD_RATE);

    while (!Serial);
    delay(1000);
}

void loop() {
    Serial.println("Hello, world!");
    delay(1000);
}