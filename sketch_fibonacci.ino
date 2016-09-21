#include <FastLED.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

#define NUM_LEDS 9
#define DATA_PIN 2

CRGB colorLeds[NUM_LEDS];

enum SEQUENCE_ELEMENT {
  SE_FIRST = 0,
  SE_SECOND = 1,
  SE_THREE = 2,
  SE_FOUR = 3,
  SE_FIVE = 4  
};

SEQUENCE_ELEMENT typeLeds[NUM_LEDS] {SE_FIRST, 
                                     SE_SECOND, 
                                     SE_THREE, 
                                     SE_FOUR, SE_FOUR, 
                                     SE_FIVE, SE_FIVE, SE_FIVE, SE_FIVE};

enum CONTAINCE_VALUE {
  CV_NONE = 0,
  CV_MINUTE = 1,
  CV_HOUR,
  CV_HOUR_WITH_MINUTE
};

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial

  FastLED.addLeds<WS2811, DATA_PIN>(colorLeds, NUM_LEDS);
}

void loop() {
  FastLED.show();//Посылаем данные на гирлянду
  tmElements_t tm = getTime();
  time2console(tm);
  timeToLeds(tm.Hour, tm.Minute);
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

void time2console(const tmElements_t &tm) {
  print2digits(tm.Hour);
  Serial.write(':');
  print2digits(tm.Minute);
  Serial.write(':');
  print2digits(tm.Second);
  Serial.println();
}

void print2digits(const int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

void timeToLeds(const uint8_t hour, const uint8_t minute) {
  CONTAINCE_VALUE leds[5] = {CV_NONE, CV_NONE, CV_NONE, CV_NONE, CV_NONE}; 
  hourToLeds(hour, leds);
  minuteToLeds(minute, leds);
  setColorLed(leds);
}

void hourToLeds(uint8_t hour, CONTAINCE_VALUE leds[]) {
  switch (hour % 12) {
  case 1: // 1, 13
    setHourToElement(leds[0]);
    break;
  case 2: // 2, 14
    setHourToElement(leds[2]);
    break;
  case 3: // 3, 15
    setHourToElement(leds[3]);
    break;
  case 4: // 4, 16
    setHourToElement(leds[0]);
    setHourToElement(leds[3]);
    break;
  case 5: // 5, 17
    setHourToElement(leds[4]);
    break;
  case 6: // 6, 18
    setHourToElement(leds[0]);
    setHourToElement(leds[4]);
    break;
  case 7: // 7, 19
    setHourToElement(leds[2]);
    setHourToElement(leds[4]);
    break;
  case 8: // 8, 20
    setHourToElement(leds[3]);
    setHourToElement(leds[4]);
    break;
  case 9: // 9, 21
    setHourToElement(leds[0]);
    setHourToElement(leds[3]);
    setHourToElement(leds[4]);
    break;
  case 10: // 10, 22
    setHourToElement(leds[2]);
    setHourToElement(leds[3]);
    setHourToElement(leds[4]);
    break;
  case 11: // 11, 23
    setHourToElement(leds[0]);
    setHourToElement(leds[2]);
    setHourToElement(leds[3]);
    setHourToElement(leds[4]);
    break;
  case 0: // 12, 0
    setHourToElement(leds[0]);
    setHourToElement(leds[1]);
    setHourToElement(leds[2]);
    setHourToElement(leds[3]);
    setHourToElement(leds[4]);
    break;
  default:
    break;
  }
}

void minuteToLeds(uint8_t minute, CONTAINCE_VALUE leds[]) {
  switch (minute / 5) {
  case 1: // 1
    setMinuteToElement(leds[0]);
    break;
  case 2: // 2
    setMinuteToElement(leds[2]);
    break;
  case 3: // 3
    setMinuteToElement(leds[3]);
    break;
  case 4: // 4
    setMinuteToElement(leds[0]);
    setMinuteToElement(leds[3]);
    break;
  case 5: // 5
    setMinuteToElement(leds[4]);
    break;
  case 6: // 6
    setMinuteToElement(leds[0]);
    setMinuteToElement(leds[4]);
    break;
  case 7: // 7
    setMinuteToElement(leds[2]);
    setMinuteToElement(leds[4]);
    break;
  case 8: // 8
    setMinuteToElement(leds[3]);
    setMinuteToElement(leds[4]);
    break;
  case 9: // 9
    setMinuteToElement(leds[0]);
    setMinuteToElement(leds[3]);
    setMinuteToElement(leds[4]);
    break;
  case 10: // 10
    setMinuteToElement(leds[2]);
    setMinuteToElement(leds[3]);
    setMinuteToElement(leds[4]);
    break;
  case 11: // 11
    setMinuteToElement(leds[0]);
    setMinuteToElement(leds[2]);
    setMinuteToElement(leds[3]);
    setMinuteToElement(leds[4]);
    break;
  case 0: // 12
    setMinuteToElement(leds[0]);
    setMinuteToElement(leds[1]);
    setMinuteToElement(leds[2]);
    setMinuteToElement(leds[3]);
    setMinuteToElement(leds[4]);
    break;
  default:
    break;
  }
}

void setHourToElement(CONTAINCE_VALUE &led) {
  if (led == CV_NONE) {
    led = CV_HOUR;
  } else {
    led = CV_HOUR_WITH_MINUTE;
  }
}

void setMinuteToElement(CONTAINCE_VALUE &led) {
  if (led == CV_NONE) {
    led = CV_MINUTE;
  } else {
    led = CV_HOUR_WITH_MINUTE;
  }
}

void setColorLed(CONTAINCE_VALUE leds[]) {
   for (char j = 0; j < NUM_LEDS; j++) {
    SEQUENCE_ELEMENT type = typeLeds[j];
    switch (leds[type]) {
    case CV_NONE:
      colorLeds[j] = CRGB(0,0,0);
      Serial.println("none");
      break;
    case CV_MINUTE:
      colorLeds[j] = CRGB(255,0,0);
      Serial.println("m");
      break;
   case CV_HOUR:
      colorLeds[j] = CRGB(0,255,0);
      Serial.println("h");
      break;
    case CV_HOUR_WITH_MINUTE:
      colorLeds[j] = CRGB(0,0,255);
      Serial.println("mh");
      break;
    default:
      colorLeds[j] = CRGB(0,0,0);
      Serial.println("d");
    }
   }
}

