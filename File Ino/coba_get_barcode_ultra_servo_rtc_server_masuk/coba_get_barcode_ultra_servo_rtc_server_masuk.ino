/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-http-get-post-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Wire.h>
#include "RTClib.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo myservo;

RTC_DS3231 rtc;
char dateBuffer[12];
char timeBuffer[12];

const char* ssid = "Seamless@wifi.Id";
const char* password = "bbbbbbbb";

String Commands_Reply;                 // The command variable that is sent to the client

int pos = 0;

const char * host = "192.168.43.10";          // IP Client

WiFiServer server(80);

//=================ULTRASONIK=======================
#define TRIGGER_PIN  0 //GPIO 0/ D8 
#define ECHO_PIN     15 //GPIO 15/ D10
long duration, distance, jarak;
//==================================================

//Your Domain name with URL path or IP address with path
String serverName = "http://192.168.43.217:8069/api/v1/kendaraan-masuk/";
String serverNamePost = "http://192.168.43.217:8069/api/v1/riwayat/";
String api;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

String sensorReadings;
float sensorReadingsArr[3];


void setup() {
  Serial.begin(115200);
  delay(100);
  lcd.begin();
  //=======================ULTRASONIK=============================
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  //==============================================================
  //============================== RTC ==========================
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
  //==================================================================
  myservo.attach(13); //D7

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
  server.begin();
  Serial.println("Server started");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====================");
  lcd.setCursor(3, 1);
  lcd.print("Selamat Datang");
  lcd.setCursor(2, 2);
  lcd.print("Scan Barcode ...");
  lcd.setCursor(0, 3);
  lcd.print("====================");
  myservo.write(90);
}

void loop() {
  //============================== RTC ==========================

  //==================================================================
  //Send an HTTP POST request every 10 minutes
  jarak = 400;
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client = server.available();
      if (!client) {
        return;
      }
      // Read the first line of the request -------------------------------------
      String req = client.readStringUntil('\n');
      Serial.println(req);
      client.flush();
      Commands_Reply = req;
      Commands_Reply.trim();
      api = serverName + Commands_Reply;
      sensorReadings = httpGETRequest(api);
      Serial.println(sensorReadings);
      JSONVar myObject = JSON.parse(sensorReadings);

      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      int panjang1 = (myObject["result"][0]).length();
      Serial.println(panjang1);
      if (panjang1 == 0) {
        Serial.println("BARCODE TIDAK ADA");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Barcode Tidak Ada");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("====================");
        lcd.setCursor(3, 1);
        lcd.print("Selamat Datang");
        lcd.setCursor(2, 2);
        lcd.print("Scan Barcode ...");
        lcd.setCursor(0, 3);
        lcd.print("====================");
        return;
      }
      if (panjang1 > 1) {
        Serial.println("ANDA BELUM KELUAR");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Anda Belum Keluar");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("====================");
        lcd.setCursor(3, 1);
        lcd.print("Selamat Datang");
        lcd.setCursor(2, 2);
        lcd.print("Scan Barcode ...");
        lcd.setCursor(0, 3);
        lcd.print("====================");
        return;
      }
      Serial.println("====================================================================");
      Serial.print("JSON object = ");
      Serial.println(myObject["result"][0]);
      Serial.println("====================================================================");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nama: ");
      lcd.setCursor(6, 0);
      lcd.print(myObject["result"][0][0]["partner_id"][1]);
      lcd.setCursor(0, 1);
      lcd.print("NOPOL: ");
      lcd.setCursor(7, 1);
      lcd.print(myObject["result"][0][0]["name"]);
      lcd.setCursor(0, 2);
      lcd.print("====================");
      lcd.setCursor(4, 3);
      lcd.print("Selamat Jalan");
      delay(1000);
      for (pos = 90; pos >= 0; pos -= 1) {
        Serial.println(pos);
        myservo.write(pos);
        delay(50);
      }
      while (jarak > 100) {
        jarak = baca_jarak();
        delay(50);
      }
      delay(3000);
      lcd.clear();
      lcd.setCursor(0, 0);
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
      lcd.print(datetime);
      
      for (pos = 0; pos <= 90; pos += 1) {
        Serial.println(pos);
        myservo.write(pos);
        delay(50);
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("====================");
      lcd.setCursor(3, 1);
      lcd.print("Selamat Datang");
      lcd.setCursor(2, 2);
      lcd.print("Scan Barcode ...");
      lcd.setCursor(0, 3);
      lcd.print("====================");

    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

String httpGETRequest(String api) {
  HTTPClient http;

  // Your IP address with path or Domain name with URL path
  http.begin(api);
  http.addHeader("Content-Type", "application/json");
  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
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
