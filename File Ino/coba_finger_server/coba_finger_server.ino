/*******************************************************************
 *  Program Dasar Akses Sidik Jari Arduino                         *
 *                                                                 *
 *                                                                 *
 *  Dipbulikasikan Oleh : Giri Wahyu Pambudi                       *
 *                        (https://www.cronyos.com)                *
 *                                                                 *
 *  di Tulis 28/11/2018                                            *
 *******************************************************************/

#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

WidgetLCD lcd1(V2);

#define Finger_Rx 12 //D6
#define Finger_Tx 14 //D5

char auth[] = "Vd770XdnWC0Wz_6It_yv0XDevYlDcpJm";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Obiah Cantik";
char pass[] = "obiahcomel";

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial mySerial(Finger_Tx, Finger_Rx);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

const char* Commands_Reply;                 // The command variable that is sent to the client

const char * host = "192.168.100.44";          // IP Client

WiFiServer server(80);

int i =0;

void setup()  
{
  lcd.begin();
  Serial.begin(115200);
  delay(10);

  Serial.println("");
  Serial.println("Server-------------------------------");
  Serial.print("Configuring access point");
  WiFi.begin(ssid, pass);

  // Waiting to connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");

  // Start the server
    server.begin();
    Serial.println("Server started");

  // Print the IP address
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
    Serial.println("-------------------------------------");
    Serial.println("");

    

//  Blynk.begin(auth, ssid, pass);
  Blynk.begin(auth, ssid, pass, "blynk-cloud.com");
//  Serial.begin(9600);
  while (!Serial); 
  delay(100);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Counter");
  lcd.setCursor(0,1);
  lcd.print(i);
  lcd.setCursor(0,1);
  lcd.print(i++);
  
  lcd1.clear();
  lcd1.print(0, 0, "Counter");
  lcd1.print(0, 1, i);
  lcd1.print(0, 1, i++);
//  lcd1.print(0, 1, i++);
  Serial.println("Program Dasar Akses Sidik Jari Arduino");
  Serial.println("https://www.cronyos.com");

  // set the data rate for the sensor serial port
  finger.begin(57600);
//  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  Serial.println("Waiting for valid finger...");
  
}

void loop()                     // run over and over again
{
  
  Blynk.run();
  getFingerprintID();
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("Server-------------------------------");
  Serial.println("New client");
  Serial.print("From client = ");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request -------------------------------------
   String req = client.readStringUntil('\r');
   Serial.println(req);
   client.flush();

   //command counter
   if (req.indexOf("LED_Blink") != -1){
      lcd1.print(0, 1, i--); 
      lcd.setCursor(0, 1); 
      lcd.print(i--); 
   }
  delay(50);            //don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println(".");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    delay(3000);
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  lcd.setCursor(0,1);
  lcd.print(i++);
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
  delay(3000);

//  return finger.fingerID;
  
}
