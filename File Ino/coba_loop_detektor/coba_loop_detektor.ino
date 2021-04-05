
#include <Servo.h>

String Answer;
Servo myservo;              // The command variable that is sent to the client

int pos = 0;


//=================ULTRASONIK=======================
#define TRIGGER_PIN  0 //GPIO 0/ D8 
#define ECHO_PIN     15 //GPIO 15/ D10
long duration, distance, jarak;
//==================================================


void setup() {
  Serial.begin(115200);
  delay(100);
  myservo.attach(13);
  //=======================ULTRASONIK=============================
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  //==============================================================
  myservo.write(0);
}

void loop() {
  jarak = 400;
  while(Serial.available()==0);
  Answer = Serial.readStringUntil('\n');
  if (Answer == "buka"){
    Serial.println(Answer+" , Ganteng");
  }
  else{
    Serial.println(Answer+", wrong Answer");
  }
  Serial.println("BUKA");
  for (pos = 0; pos <= 90; pos += 1) {
    Serial.println(pos);
    myservo.write(pos);
    delay(50);
  }
  while (jarak > 100) {
    jarak = baca_jarak();
    Serial.println(jarak);
    Serial.println("DETEKSI 1");
    delay(50);
  }
  while (jarak < 100) {
    jarak = baca_jarak();
    Serial.println(jarak);
    Serial.println("MASIH ADA");
    delay(50);
  }
  Serial.println("TUTUP LAGI");
  delay(1000);
  for (pos = 90; pos >= 0; pos -= 1) {
    Serial.println(pos);
    myservo.write(pos);
    delay(100);
  }

}



long baca_jarak() {
  digitalWrite(TRIGGER_PIN, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}
