#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#include "RTClib.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS3231 rtc;
char dateBuffer[12];
char timeBuffer[12];
char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};
void setup () {
  Serial.begin(115200);
  lcd.begin();
  delay(3000);
  if (! rtc.begin()) {
    Serial.println("RTC tidak terbaca");
    while (1);
  }
  if (rtc.lostPower()) {
    //atur waktu sesuai waktu pada komputer
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //atur waktu secara manual
    // January 21, 2019 jam 10:30:00
    //rtc.adjust(DateTime(2021, 3, 28, 15, 42, 0));
  }
}
void loop () {
  DateTime now = rtc.now();


  sprintf(dateBuffer, "%04u-%02u-%02u ", now.year(), now.month(), now.day());
  Serial.print("DATE : ");
  Serial.println(dateBuffer);

  sprintf(timeBuffer, "%02u:%02u:%02u ", now.hour(), now.minute(), now.second());
  Serial.print("TIME : ");
  Serial.println(timeBuffer);
  String datetime = String(dateBuffer) + String(timeBuffer);
  Serial.println("========================================");
  Serial.print("DATETIME : ");
  Serial.println(datetime);
  Serial.println("========================================");
  delay(1000);
}
