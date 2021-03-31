#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
Servo servo;

LiquidCrystal_I2C lcd(0x27, 20, 4);
/// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "wdYl0Z-VEqHEL3hurbcRkeOsxwLl4Ptz";

void setup()
{
Serial.begin(9600);
pinMode(2, OUTPUT);
servo.attach(14); //pin PWM 9 untuk servo
lcd.begin();
lcd.clear();
lcd.setCursor(0,0);
lcd.print("====================");
lcd.setCursor(6,1);
lcd.print("Silahkan");
lcd.setCursor(4,2);
lcd.print("Pindai Barcode");
lcd.setCursor(0,3);
lcd.print("====================");
Blynk.begin(auth, "Seamless@wifi.Id", "bbbbbbbb");
}

BLYNK_WRITE(V0)
{
  int i=param.asInt();
  if (i==1)
    {
      digitalWrite(2, HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("NAMA: YUSUP");
      lcd.setCursor(0,1);
      lcd.print("NOPOL: T5761MR");
      lcd.setCursor(0,2);
      lcd.print("====================");
      lcd.setCursor(0,3);
      lcd.print("Pindai Sidik Jari...");
      servo.write(90); //posisi servo 0 derajat
    }
}
BLYNK_WRITE(V3)
{
  int i=param.asInt();
  if (i==1)
    {
      digitalWrite(2, HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("NAMA: REGI");
      lcd.setCursor(0,1);
      lcd.print("NOPOL: T7863AB");
      lcd.setCursor(0,2);
      lcd.print("====================");
      lcd.setCursor(0,3);
      lcd.print("Pindai Sidik Jari...");
      servo.write(0); //posisi servo 0 derajat
    }
}
BLYNK_WRITE(V42)
{
  int i=param.asInt();
  if (i==1)
    {
      digitalWrite(2, HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("NAMA: SITI");
      lcd.setCursor(0,1);
      lcd.print("NOPOL: T1309XH");
      lcd.setCursor(0,2);
      lcd.print("====================");
      lcd.setCursor(0,3);
      lcd.print("Pindai Sidik Jari...");
    }
}
BLYNK_WRITE(V1)
{
  int i=param.asInt();
  if (i==1)
    {
      digitalWrite(2, HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("====================");
      lcd.setCursor(8,1);
      lcd.print("MAAF");
      lcd.setCursor(5,2);
      lcd.print("COBA LAGI");
      lcd.setCursor(0,3);
      lcd.print("====================");
    }
}
BLYNK_WRITE(V2)
{
  int i=param.asInt();
  if (i==1)
    {
      digitalWrite(2, HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("====================");
      lcd.setCursor(5,1);
      lcd.print("HASIL COCOK");
      lcd.setCursor(4,2);
      lcd.print("SELAMAT JALAN");
      lcd.setCursor(0,3);
      lcd.print("====================");
      delay(5000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("====================");
      lcd.setCursor(6,1);
      lcd.print("Silahkan");
      lcd.setCursor(4,2);
      lcd.print("Pindai Barcode");
      lcd.setCursor(0,3);
      lcd.print("====================");
    }
}

void loop()
{
Blynk.run();
}
