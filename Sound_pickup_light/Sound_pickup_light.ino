/*
   Sound Pick-up Light

   Parts: 1.Attiny 85 internal 8MHz
          2. Sound Sensor OCROBOT
          3. WS2812B RGB LED
          4. CR2032 LIthium Coin Battery *2
          5. Slide switch

   Case idea: 1. A serial of Capsules with different sound level ranges
              2. A round case
              3. Magnets or clips

   Parameters： 1. Change the min/max Sound value to fit in different environments.
                2. Light's color can change.
                3. Start light pattern can change.
*/

#include <WS2812.h>

const int lightPin = 1;
const int NUMPIXELS = 1;
unsigned int intensity;
unsigned int maxIntensity = 100;

const int sensorPin = A2;
unsigned long preTime = 0;
unsigned int soundVal;
const int sampleWindow = 40;    // Sample window width in mS (40 mS = 25Hz)
unsigned int sample;
unsigned int minSound = 50;
unsigned int maxSound = 500;

const int groundPin_1 = A1;     // set the rest 2 attiny pins to GND for better wiring
const int groundPin_2 = A3;

WS2812 LED(NUMPIXELS);
cRGB value;

void setup() {
  LED.setOutput(lightPin);
  lightStart();
  pinMode(groundPin_1, OUTPUT);
  digitalWrite(groundPin_1, LOW);
  pinMode(groundPin_2, OUTPUT);
  digitalWrite(groundPin_2, LOW);
}

void loop() {
  SoundLevel();
  intensity = map(soundVal, minSound, maxSound, 0, maxIntensity);
  ligtOutput();
}

void ligtOutput() {
  if (millis() - preTime > 5) {
    value.r = 0;
    value.g = intensity;
    value.b = 0;

    LED.set_crgb_at(0, value);
    LED.sync();
    preTime = millis();
  }
}

void lightStart() {
  delay(700);
  value.r = 255; value.g = 0; value.b = 0;
  LED.set_crgb_at(0, value);
  LED.sync();
  delay(700);
  value.r = 0; value.g = 0; value.b = 255;
  LED.set_crgb_at(0, value);
  LED.sync();
  delay(700);
  value.r = 0; value.g = 255; value.b = 0;
  LED.set_crgb_at(0, value);
  LED.sync();
  delay(700);
}

void SoundLevel() {
  unsigned long startMillis = millis();
  unsigned int peakToPeak = 0;

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(sensorPin);
    if (sample < 1024)
    {
      if (sample > signalMax)
      {
        signalMax = sample;
      }
      else if (sample < signalMin)
      {
        signalMin = sample;
      }
    }
  }
  peakToPeak = signalMax - signalMin;
  double volts = (peakToPeak * 6.0) / 1024;
  soundVal = constrain(volts * 100, minSound, maxSound);
}
