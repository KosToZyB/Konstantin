#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
}

void loop() {
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
