/*
* Palang Otomatis menggunakan Ultrasonic Transceiver dan Servo
* Kelompok 2 TK-1A
*/

#include <Servo.h>
#define MAX_DISTANCE 500
Servo servo;

int pinTrigger = 7; //Set trigger Ultrasonic Transceiver di pin digital 12
int pinEcho = 8; //Set echo Ultrasonic Transceiver di pin digital 12
int pinHijau = 10; //Set kaki positif LED Hijau di pin digital 10
int pinMerah = 11; //Set kaki positif LED Merah di pin digital 10
const int button3 = 4;

int buttonState3 = 0;
float durasi, jarak;

void setup()
{
  pinMode(button3, INPUT);
pinMode(pinTrigger, OUTPUT); //pin trigger sebagai output
pinMode(pinEcho, INPUT); //pin echo sebagai input
pinMode(pinHijau, OUTPUT); //pin LED hijau sebagai output
pinMode(pinMerah, OUTPUT); //pin LED merah sebagai output
Serial.begin(9600); //kecepatan komunikasi Serial dengan komputer
servo.attach(9); //pin PWM 9 untuk servo
}

void loop()
{
//program trigger memancarkan ultrasonik
buttonState3 = digitalRead(button3);
digitalWrite(pinTrigger, LOW);
delayMicroseconds(2);
digitalWrite(pinTrigger, HIGH);
delayMicroseconds(10);
digitalWrite(pinTrigger, LOW);

//Durasi adalah waktu tunggu pin Echo menjadi bernilai HIGH
//Setelah mendapat pantulan gelombang ultrasonic dari pin Trigger
durasi = pulseIn(pinEcho, HIGH);
jarak = (durasi * 0.034) / 2; //Konversi nilai durasi ke jarak
//Mencetak jarak pada serial monitor
Serial.print(jarak);
Serial.println(" cm");
if(buttonState3 == HIGH)
{
digitalWrite(pinHijau, LOW); //led hijau mati
digitalWrite(pinMerah, HIGH); //led merah nyala
servo.write(90); //posisi servo 0 derajat
}
if (jarak <= 10) //jarak (cm) dapat diatur dan disesuaikan
{
digitalWrite(pinMerah, LOW); //led merah mati
digitalWrite(pinHijau, HIGH); //led hijau nyala
delay(2000);
servo.write(0); //posisi servo 90 derajat
delay(100);
}

delay(450);
}
