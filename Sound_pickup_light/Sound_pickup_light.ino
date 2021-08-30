#include <WS2812.h>

const int lightPin = 1;
const int NUMPIXELS = 1;
unsigned int intensity;
unsigned int maxIntensity = 100;

const int sensorPin = A2;
unsigned long preTime = 0;
unsigned int soundVal;
const int sampleWindow = 40; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
unsigned int minSound = 50;
unsigned int maxSound = 500;

WS2812 LED(NUMPIXELS);
cRGB value;

void setup() {
  LED.setOutput(lightPin);
  lightStart();
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
  double volts = (peakToPeak * 5.0) / 1024;
  soundVal = constrain(volts * 100, minSound, maxSound);
}