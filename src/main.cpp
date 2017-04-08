#include "Arduino.h"

#include "FastLED.h"
#include <EEPROM.h>

#define DATA_PIN 1

// LEDS variables
#define NUM_LEDS 20
CRGB leds[NUM_LEDS];

// global brightness
const int brightness = 32;

// store current pattern for restoration
const byte address = 0;

// pattern variables
byte pattern = 0;
const byte patternMax = 3;

// alarm variables
bool blinkOn = false;
const unsigned long timeAlarmInterval = 800l;
unsigned long timeAlarmIntervalStart = 0l;

// key press bounce variables
const volatile unsigned long timeBounce = 250l;
volatile unsigned long timeBounceStart = 0l;

// traffic Light variables
byte trafficSlot = 0;
const byte trafficSlotMax = 13;
const unsigned long timeTraffic = 2000l;
unsigned long timeTrafficStart = 0l;
int white[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
int red[] = { };
int yellow[] = { };
int green[] = { };


void interruptServiceRoutine();
void showReady();
void showOn();
void showOff();
void showBlink();
void showWhite();
void showTrafficLight();


void setup() {
        FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
        FastLED.setBrightness(brightness);
        pattern = EEPROM.read(address);
        attachInterrupt(0, interruptServiceRoutine, HIGH);
        showReady();
}

void loop() {
        switch (pattern) {
        case 0:
                showBlink();
                break;
        case 1:
                showTrafficLight();
                showWhite();
                break;
        case 2:
                showOff();
                break;
        case 3: // disabled
                showOn();
                break;
        }
}

void interruptServiceRoutine() {
        if (millis() > timeBounceStart + timeBounce) {
                timeBounceStart = millis();
                if (pattern++ > patternMax - 2) {
                        pattern = 0;
                }
                EEPROM.write(address, pattern);
        }
}

void showReady() {
        for (int i = 0; i < 3; i++) {
                leds[0] = CRGB::Green;
                FastLED.show();
                delay(200);
                leds[0] = CRGB::Black;
                FastLED.show();
                delay(200);
        }
}

void setAllLedColor(CRGB color) {
        for (int led = 0; led < NUM_LEDS; led++) {
                leds[led] = color;
        }
        FastLED.show();
}

void showOn() {
        setAllLedColor(CRGB::White);
}

void showOff() {
        setAllLedColor(CRGB::Black);
}

void showBlink() {
        if (millis() > timeAlarmIntervalStart + timeAlarmInterval) {
                timeAlarmIntervalStart = millis();
                if (blinkOn) {
                        blinkOn = false;
                        setAllLedColor(CRGB::Black);
                } else {
                        blinkOn = true;
                        setAllLedColor(CRGB::Red);
                }
        }
}

void showWhite() {
        for (int i : white) {
                leds[i] = CRGB::White;
        }
        FastLED.show();
}

void showTrafficLight() {
        if (millis() > timeTrafficStart + timeTraffic) {
                timeTrafficStart = millis();
                if (trafficSlot++ > trafficSlotMax - 2) {
                        trafficSlot = 0;
                }
                switch (trafficSlot) {
                case 0:
                        for (int i : red) {
                                leds[i] = CRGB::Red;
                        }
                        for (int i : yellow) {
                                leds[i] = CRGB::Black;
                        }
                        for (int i : green) {
                                leds[i] = CRGB::Black;
                        }
                        break;
                case 1:
                        for (int i : red) {
                                leds[i] = CRGB::Red;
                        }
                        for (int i : yellow) {
                                leds[i] = CRGB::Yellow;
                        }
                        for (int i : green) {
                                leds[i] = CRGB::Black;
                        }
                        break;
                case 2:
                case 3:
                case 4:
                        for (int i : red) {
                                leds[i] = CRGB::Black;
                        }
                        for (int i : yellow) {
                                leds[i] = CRGB::Black;
                        }
                        for (int i : green) {
                                leds[i] = CRGB::Green;
                        }
                        break;
                case 5:
                        for (int i : red) {
                                leds[i] = CRGB::Black;
                        }
                        for (int i : yellow) {
                                leds[i] = CRGB::Yellow;
                        }
                        for (int i : green) {
                                leds[i] = CRGB::Black;
                        }
                        break;
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                        for (int i : red) {
                                leds[i] = CRGB::Red;
                        }
                        for (int i : yellow) {
                                leds[i] = CRGB::Black;
                        }
                        for (int i : green) {
                                leds[i] = CRGB::Black;
                        }
                        break;
                }
                FastLED.show();
        }
}
