#include <FastLED.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

#define NUM_LEDS 9
#define DATA_PIN 2

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial

  FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  leds[0] = CRGB(255,0,0);
  leds[1] = CRGB(0,255,0);
  leds[2] = CRGB(0,0,255);
  leds[3] = CRGB(255,255,0);
  leds[4] = CRGB(255,255,0);
  leds[5] = CRGB(0,255,255);
  leds[6] = CRGB(0,255,255);
  leds[7] = CRGB(0,255,255);
  leds[8] = CRGB(0,255,255);
  FastLED.show();//Посылаем данные на гирлянду
  
  tmElements_t tm = getTime();
  time2console(tm);
  delay(1000);
}

//read time
tmElements_t getTime() {
  tmElements_t tm;

  if (RTC.read(tm)) {
    Serial.println("read time complete");
  } else {
    if (RTC.chipPresent()) {
      Serial.println("time not initialize");
      Serial.println();
    } else {
      Serial.println("time read error!");
      Serial.println();
    }
  }

  return tm;
}

void time2console(tmElements_t tm) {
  print2digits(tm.Hour);
  Serial.write(':');
  print2digits(tm.Minute);
  Serial.write(':');
  print2digits(tm.Second);
  Serial.println();
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
