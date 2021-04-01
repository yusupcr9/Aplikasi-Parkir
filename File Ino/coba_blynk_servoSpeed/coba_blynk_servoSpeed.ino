#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo myservo;
/// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Vd770XdnWC0Wz_6It_yv0XDevYlDcpJm";
int pos = 0;
void setup()
{
Serial.begin(9600);
myservo.attach(13);
Blynk.begin(auth, "Seamless@wifi.Id", "bbbbbbbb");
}

BLYNK_WRITE(V4)
{
  int i=param.asInt();
  if (i==1)
    {
      for(pos = 0; pos <= 90; pos+=1){
        Serial.println(pos);
        myservo.write(pos);
        delay(50);
      }
    }
}
BLYNK_WRITE(V3)
{
  int b=param.asInt();
  if (b==1)
    {
      for(pos = 90; pos >= 0; pos-=1){
        Serial.println(pos);
        myservo.write(pos);
        delay(50);
      }
    }
}
void loop()
{
Blynk.run();
}
