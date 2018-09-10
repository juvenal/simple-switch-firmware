#include <Arduino.h>

#include "pinout.h"

int state = HIGH;
int previous = LOW;
int check = LOW;
int reading;

unsigned long time = 0;
const unsigned long debounce = 200;

void setup() {
    // put your setup code here, to run once:
    pinMode(switchPin, INPUT);      // The number of switcher pin to look for
    pinMode(LEDPin, OUTPUT);        // The number of LED pin to set output
    Serial.begin(9600);             // Set serial interface
    while (!Serial) {               // Hold while serial not active
        ;                           // Wait for the serial port to connect
    }
    Serial.println("Active code!"); // Inform completion of setup and loop start
}

void loop() {
    // put your main code here, to run repeatedly:
    reading = digitalRead(switchPin);

    if (check == LOW) {
        Serial.println("Outside test block");
        check = HIGH;
    }

    if (reading == HIGH && previous == LOW && (millis() - time) > debounce) {
        Serial.println("Inside test block");
        check = LOW;

        if (state == HIGH) {
            Serial.println("Shutting down LED");
            state = LOW;
        }
        else {
            Serial.println("Turning on LED");
            state = HIGH;
        }
        time = millis();
    }

    digitalWrite(LEDPin, state);

    previous = reading;
}
