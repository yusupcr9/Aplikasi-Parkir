#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#include "RTClib.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS3231 rtc;
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
  String tahun = now.year();
  Serial.println("============ " +tahun+ " =====================");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);//hari
  Serial.print(", ");
  Serial.print(now.year(), DEC); //tanggal
  Serial.print('-');
  Serial.print(now.month(), DEC); //bulan
  Serial.print('-');
  Serial.print(now.day(), DEC); //tahun
  Serial.print(' ');
  Serial.print(now.hour(), DEC); //jam
  Serial.print(':');
  Serial.print(now.minute(), DEC); //tanggal
  Serial.print(':');
  Serial.print(now.second(), DEC); //detik
  Serial.println();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(now.year(),DEC);
  lcd.setCursor(4,0);
  lcd.print('-');
  lcd.setCursor(5,0);
  lcd.print(now.month(),DEC);
  lcd.setCursor(7,0);
  lcd.print('-');
  lcd.setCursor(8,0);
  lcd.print(now.day(),DEC);
  lcd.setCursor(10,0);
  Serial.print(' ');
  lcd.setCursor(11,0);
  lcd.print(now.hour(),DEC);
  lcd.setCursor(14,0);
  Serial.print(':');
  lcd.setCursor(15,0);
  lcd.print(now.minute(),DEC);
  lcd.setCursor(17,0);
  Serial.print(':');
  lcd.setCursor(18,0);
  lcd.print(now.second(),DEC);
  delay(1000);
}
