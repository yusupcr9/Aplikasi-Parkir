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
String no_mesin = "G3E4E063048";
String serverName = "http://192.168.43.217:8069/api/v1/kendaraan/G3E4E063048";
String serverNamePut = "http://192.168.43.217:8069/api/v1/riwayat/G3E4E063048";
char settings_data[100];

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
  delay(100);
  
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
        delay(3000);
        return;
      }
      Serial.println("====================================================================");
      Serial.print("JSON object = ");
      Serial.println(myObject["result"][0]);
      Serial.println("====================================================================");
      delay(3000);
      httpPUTRequest(serverNamePut);
      
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

String httpGETRequest(String serverName) {
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

String httpPUTRequest(String serverNamePut) {
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(serverNamePut);
  http.addHeader("Content-Type","application/json");
  // Send HTTP POST request
  int httpResponseCodePut = http.PUT("{\"data\":{\"partner_id\":81,\"jam_keluar\":\"2021-03-26 23:08:00\",\"jenis\":\"inout\"}}");
  
  String payload = "{}"; 
  
  if (httpResponseCodePut>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCodePut);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCodePut);
  }
  // Free resources
  http.end();

  return payload;
}
