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

const char* ssid = "Seamless@wifi.Id";
const char* password = "bbbbbbbb";

//Your Domain name with URL path or IP address with path
const char* serverName = "http://192.168.43.217:8069/api/v1/kendaraan/G3E4E063048";
const char* serverNameFinger = "http://192.168.43.217:8069/api/v1/sidik-jari/G3E4E063048/3";
const char* serverNamePOST = "http://192.168.43.217:8069/api/v1/riwayat";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

String sensorReadings;
String FingerReadings;
float sensorReadingsArr[3];

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
              
      sensorReadings = httpGETRequest(serverName);
      Serial.println(sensorReadings);
      JSONVar myObject = JSON.parse(sensorReadings);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      int panjang1 = (myObject["result"][0]).length();
      Serial.println(panjang1);
      if (panjang1 == 0){
        Serial.println("BARCODE TIDAK ADA");
        return;
      }
      Serial.println("====================================================================");
      Serial.print("JSON object = ");
      Serial.println(myObject["result"][0]);
      Serial.println("====================================================================");
      FingerReadings = httpGETRequestFinger(serverNameFinger);
      Serial.println(FingerReadings);
      JSONVar myObjectFinger = JSON.parse(FingerReadings);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObjectFinger) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      int panjang2 = (myObjectFinger["result"][0]).length();
      Serial.println(panjang2);
      if (panjang2 == 0){
        Serial.println("Finger Tidak Cocok");
        return;
      }
      Serial.println("====================================================================");
      Serial.print("Finger JSON object = ");
      Serial.println(myObjectFinger["result"][0]);
      Serial.println("====================================================================");
      httpPOSTRequest(serverNamePOST);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

String httpGETRequest(const char* serverName) {
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(serverName);
  http.addHeader("Content-Type","application/json");
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
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

String httpGETRequestFinger(const char* serverNameFinger) {
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(serverNameFinger);
  http.addHeader("Content-Type","application/json");
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
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

String httpPOSTRequest(const char* serverNamePOST) {
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(serverNamePOST);
  http.addHeader("Content-Type","application/json");
  // Send HTTP POST request
  int httpResponseCodePost = http.POST("{\"data\":[{\"pemilik_id\":1,\"jam_keluar\":\"2021-03-24 23:08:00\",\"jenis\":\"out\"}]}");
  
  String payload = "{}"; 
  
  if (httpResponseCodePost>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCodePost);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCodePost);
  }
  // Free resources
  http.end();

  return payload;
}
