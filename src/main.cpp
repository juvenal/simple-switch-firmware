#include <Arduino.h>

#include "pinout.h"

int reading;
#ifdef DEBUG
int check = LOW;
#endif
int state = HIGH;
int previous = LOW;
byte timeInterval = 0;
int dimStepTime = 7;                // Step for dim intensity change (in milliseconds)
int dimLEDStep = 0;                 // Initial value for LED dim step to use
byte dimLEDValue = 0;               // Initial value for LED dim attenuator
unsigned long time = 0;             // Time counter used to check for debounce
unsigned long previousTime = 0;     // Time counter used to set LED dim intensity

const unsigned long debounce = 200; // Time in milliseconds to ignore false switches

void setup() {
    // Put your setup code here, to run once:
    pinMode(switchPin, INPUT);      // The number of switcher pin to look for
    pinMode(LEDPin, OUTPUT);        // The number of LED pin to set output
    #ifdef DEBUG
    Serial.begin(9600);             // Set serial interface
    while (!Serial) {               // Hold while serial not active
        ;                           // Wait for the serial port to connect
    }
    Serial.println("Active code!"); // Inform completion of setup and loop start
    #endif
}

void loop() {
    // Put your main code here, to run repeatedly:
    reading = digitalRead(switchPin);

    #ifdef DEBUG
    if (check == LOW) { Serial.println("Outside test block"); check = HIGH; }
    #endif

    if (reading == HIGH && previous == LOW && (millis() - time) > debounce) {
        #ifdef DEBUG
        Serial.println("Inside test block");
        check = LOW;
        #endif

        if (state == HIGH) {
            #ifdef DEBUG
            Serial.println("Shutting down LED");
            #endif
            state = LOW;
            dimLEDStep = -1;
        }
        else {
            #ifdef DEBUG
            Serial.println("Turning on LED");
            #endif
            state = HIGH;
            dimLEDStep = 1;
        }
        time = millis();
    }

    if ((timeInterval = millis() - previousTime) > 0) {
        dimLEDValue += dimLEDStep * (timeInterval / dimStepTime);
        if (dimLEDValue <= 0) { dimLEDValue = 0; }
        if (dimLEDValue >= 255) { dimLEDValue = 255; }
        previousTime = millis();
    }

    analogWrite(LEDPin, dimLEDValue);

    previous = reading;
}
