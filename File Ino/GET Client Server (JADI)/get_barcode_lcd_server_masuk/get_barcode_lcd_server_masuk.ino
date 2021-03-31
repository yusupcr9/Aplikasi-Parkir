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

LiquidCrystal_I2C lcd(0x27, 20, 4);
const char* ssid = "Seamless@wifi.Id";
const char* password = "bbbbbbbb";

String Commands_Reply;                 // The command variable that is sent to the client

const char * host = "192.168.43.10";          // IP Client

WiFiServer server(80);

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
}

void loop() {
  //Send an HTTP POST request every 10 minutes
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
        client.println("Barcode Tidak Ada !!");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Barcode Tidak Ada");
        return;
      }
      if (panjang1 > 1) {
        Serial.println("ANDA BELUM KELUAR");
        client.println("Anda Belum Keluar !!");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Anda Belum Keluar");
        return;
      }
      Serial.println("====================================================================");
      Serial.print("JSON object = ");
      Serial.println(myObject["result"][0]);
      Serial.println("====================================================================");
      client.println(api + '\n' + sensorReadings);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Nama: ");
      lcd.setCursor(6,0);
      lcd.print(myObject["result"][0][0]["partner_id"][1]);
      lcd.setCursor(0,1);
      lcd.print("NOPOL: ");
      lcd.setCursor(7,1);
      lcd.print(myObject["result"][0][0]["name"]);
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
