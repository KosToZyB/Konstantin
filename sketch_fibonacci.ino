#include <FastLED.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

#define NUM_LEDS 9
#define DATA_PIN 2
#define BTN_MODE 3
#define BTN_SET 4
#define LED1 5
#define LED2 6

CRGB colorLeds[NUM_LEDS];

enum SEQUENCE_ELEMENT {
  SE_FIRST = 0,
  SE_SECOND = 1,
  SE_THREE = 2,
  SE_FOUR = 3,
  SE_FIVE = 4
};

enum BUTTON_MODE {
  BM_LIGHT = 0,
  BM_HOUR = 1,
  BM_MINUTE = 2,
  BM_SCHEME = 3
};

SEQUENCE_ELEMENT typeLeds[NUM_LEDS] {SE_FIRST,
                   SE_SECOND,
                   SE_THREE,
                   SE_FOUR, SE_FOUR,
                   SE_FIVE, SE_FIVE, SE_FIVE, SE_FIVE
};

enum CONTAINCE_VALUE {
  CV_NONE = 0,
  CV_MINUTE = 1,
  CV_HOUR,
  CV_HOUR_WITH_MINUTE
};

BUTTON_MODE gMode;

bool gLightLed;
int gBright;

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial

  gMode = BM_LIGHT;
  gLightLed = true;

  tmElements_t tm;
  if (!RTC.read(tm)) {
    if (RTC.chipPresent()) {
      Serial.println("need the SetTime");

      getTime(__TIME__, tm);
      if (RTC.write(tm)) {
        Serial.println("set compile time");
      } else {
        Serial.println("set time error");
      }
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
    }
  }

  FastLED.addLeds<WS2811, DATA_PIN>(colorLeds, NUM_LEDS);

  pinMode(BTN_MODE, INPUT);
}

void loop() {
  FastLED.show();
  tmElements_t tm = getTime();
  time2console(tm);
  timeToLeds(tm.Hour, tm.Minute);
  autoLight(tm.Hour, tm.Minute);

  int btnMode = digitalRead(BTN_MODE);
  if (btnMode == HIGH) {
    modeUp();
    delay(500);
  }

  ledMode();

  int btnSet = digitalRead(BTN_SET);
  if (btnSet == HIGH) {
    setUp(gMode);
    delay(500);
  }

  delay(100);
}

//read time
tmElements_t getTime() {
  tmElements_t tm;

  if (RTC.read(tm)) {
    //Serial.println("read time complete");
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

void modeUp()
{
  switch (gMode) {
    case BM_LIGHT:
      gMode = BM_HOUR;
      break;
    case BM_HOUR:
      gMode = BM_MINUTE;
      break;
    case BM_MINUTE:
      gMode = BM_SCHEME;
      break;
    case BM_SCHEME:
      gMode = BM_LIGHT;
      break;
    default:
      gMode = BM_LIGHT;
  }
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
  if (gLightLed) {
    hourToLeds(hour, leds);
    minuteToLeds(minute, leds);
  }
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
        colorLeds[j] = CRGB(0, 0, 0);
        break;
      case CV_MINUTE:
        colorLeds[j] = CRGB(0, 0, gBright);
        break;
      case CV_HOUR:
        colorLeds[j] = CRGB(gBright, 0, 0);
        break;
      case CV_HOUR_WITH_MINUTE:
        colorLeds[j] = CRGB(0, gBright, 0);
        break;
      default:
        colorLeds[j] = CRGB(0, 0, 0);
    }
  }
}

bool getTime(const char *str, tmElements_t &tm) {
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

void ledMode() {
  switch (gMode) {
    case BM_LIGHT:
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      break;
    case BM_HOUR:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      break;
    case BM_MINUTE:
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      break;
    case BM_SCHEME:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      break;
    default:
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
  }
}

void setUp(const BUTTON_MODE &mode) {
  switch (mode) {
    case BM_LIGHT:
      lightUp();
      break;
    case BM_HOUR:
      hourUp();
      break;
    case BM_MINUTE:
      minuteUp();
      break;
    case BM_SCHEME:
      schemeUp();
      break;
    default:
      schemeUp();
  }
}

void lightUp() {
  gLightLed = !gLightLed;
}

void hourUp() {
  tmElements_t tm;
  if (RTC.read(tm)) {
    tm.Hour++;
    if (tm.Hour == 24) {
      tm.Hour = 1;
    }
    RTC.write(tm);
  } else {
    Serial.print("Can't read time");
  }
}

void minuteUp() {
  tmElements_t tm;
  if (RTC.read(tm)) {
    tm.Minute += 5;
    tm.Minute = tm.Minute - (tm.Minute % 5);
    
    if (tm.Minute >= 60) {
      tm.Minute = 0;
    }
    RTC.write(tm);
  } else {
    Serial.print("Can't read time");
  }
}

void schemeUp() {

}

void autoLight(const uint8_t hour, const uint8_t minute) {
  gBright = 255;
  int lowBright = 64;
  if (hour < 6 || hour >= 23)
    gBright = lowBright;
}
