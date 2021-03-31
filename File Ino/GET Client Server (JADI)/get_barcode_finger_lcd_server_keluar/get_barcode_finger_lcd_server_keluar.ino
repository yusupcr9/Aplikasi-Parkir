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
#include <Adafruit_Fingerprint.h>


#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(14, 12);
#else
#define mySerial Serial1
#endif
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

LiquidCrystal_I2C lcd(0x27, 20, 4);
const char* ssid = "Seamless@wifi.Id";
const char* password = "bbbbbbbb";

String Commands_Reply;                 // The command variable that is sent to the client

const char * host = "192.168.43.10";          // IP Client

WiFiServer server(80);

//Your Domain name with URL path or IP address with path
String serverName = "http://192.168.43.217:8069/api/v1/kendaraan/";
String serverFinger = "http://192.168.43.217:8069/api/v1/sidik-jari/";
String serverNamePut = "http://192.168.43.217:8069/api/v1/riwayat/G3E4E063048";
String api;
String apiFinger;
char settings_data[100];

int Finger_ID = 255;

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
  lcd.begin();

  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }

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
      Finger_ID = 255;
      String req = client.readStringUntil('\n');
      String reqFinger = client.readStringUntil('\r');
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
        Serial.println("ANDA HARUS MASUK DULU");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("====================");
        lcd.setCursor(5, 1);
        lcd.print("Anda Harus");
        lcd.setCursor(4, 2);
        lcd.print("Masuk Dulu !!");
        lcd.setCursor(0, 3);
        lcd.print("====================");
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
      lcd.print("====== Pindai ======");
      lcd.setCursor(0, 3);
      lcd.print("==== Sidik Jari ====");
      delay(1000);

      //      =======================================================FINGERPRINT===========================================================
scanFinger:
      while (Finger_ID == 255) {
        Finger_ID = getFingerprintID();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Scan FInger ....");
        delay(50);
      }
      if (Finger_ID == 254) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("====================");
        lcd.setCursor(5, 1);
        lcd.print("Sidik Jari");
        lcd.setCursor(3, 2);
        lcd.print("Tidak Terdaftar");
        lcd.setCursor(0, 3);
        lcd.print("====================");
        Finger_ID = 255;
        delay(1000);
        goto scanFinger;
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ID FINGER = ");
      lcd.setCursor(12, 0);
      lcd.print(Finger_ID);
      delay(1000);
      req.trim();
      String apiFInger = serverFinger + req + '/' + String(Finger_ID);
      FingerReadings = httpGETRequestFinger(apiFInger);
      Serial.println(FingerReadings);
      JSONVar myObjectFinger = JSON.parse(FingerReadings);

      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObjectFinger) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      int panjang2 = (myObjectFinger["result"][0]).length();
      Serial.println(panjang2);
      if (panjang2 == 0) {
        Serial.println("Finger TIDAK Cocok");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("====================");
        lcd.setCursor(5, 1);
        lcd.print("Sidik Jari");
        lcd.setCursor(4, 2);
        lcd.print("Tidak Cocok");
        lcd.setCursor(0, 3);
        lcd.print("====================");
        delay(1000);
        Finger_ID = 255;
        goto scanFinger;
      }
      Serial.println("====================================================================");
      Serial.print("JSON object = ");
      Serial.println(myObjectFinger["result"][0]);
      Serial.println("====================================================================");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("====================");
      lcd.setCursor(5, 1);
      lcd.print("Hasil Cocok");
      lcd.setCursor(2, 2);
      lcd.print("Selamat Jalan !!");
      lcd.setCursor(0, 3);
      lcd.print("====================");
      delay(1000);
      Finger_ID = 255;
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

String httpGETRequestFinger(String apiFinger) {
  HTTPClient http;

  // Your IP address with path or Domain name with URL path
  http.begin(apiFinger);
  http.addHeader("Content-Type", "application/json");
  // Send HTTP POST request
  int httpResponseCodeFinger = http.GET();

  String payloadFinger = "{}";

  if (httpResponseCodeFinger > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCodeFinger);
    payloadFinger = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCodeFinger);
  }
  // Free resources
  http.end();

  return payloadFinger;
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return -1;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return -1;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return -1;
    default:
      Serial.println("Unknown error");
      return -1;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return -1;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return -1;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return -1;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return -1;
    default:
      Serial.println("Unknown error");
      return -1;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return -1;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return -2;
  } else {
    Serial.println("Unknown error");
    return -1;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
