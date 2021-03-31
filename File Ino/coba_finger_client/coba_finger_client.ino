#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "Obiah Cantik";                  // Your wifi Name       
const char* password = "obiahcomel"; // Your wifi Password

const char * host = "192.168.43.10";        // IP Server

const int httpPort = 80;

const char* Commands;                       // The command variable that is sent to the server                     

int button = 5;                             // push button is connected
bool btn_press = false;                      // The variable to detect the button has been pressed
int con = 0;                                // Variables for mode

int i =0;

#define Finger_Rx 12 //D6
#define Finger_Tx 14 //D5

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial mySerial(Finger_Tx, Finger_Rx);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  // put your setup code here, to run once:
  pinMode(button, INPUT_PULLUP);            // initialize the pushbutton pin as an input:
  Serial.begin(115200);                     // initialize serial:

  Serial.println("");
  Serial.println("Client-------------------------------");
  Serial.print("Connecting to Network");
  WiFi.mode(WIFI_STA);                      // Mode Station
  WiFi.begin(ssid, password);               // Matching the SSID and Password
  delay(1000);

  // Waiting to connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("Successfully Connecting");  
  Serial.println("Status : Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("-------------------------------------");
  Serial.println("");

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

void loop() {
  // put your main code here, to run repeatedly:
  getFingerprintID();
  if (digitalRead(button) == LOW) {
//    Serial.println("Client-------------------------------");
//    Serial.print("Send Command = ");
    if (btn_press == true){
      Commands="LED_Blink";
      Serial.println(Commands);
      send_commands();
//      if (con >= 3) {
//        con=0;
//      }
//      con++;
//
//      switch (con){
//        case 1:
//          Commands="LED_Blink";
//          Serial.println(Commands);
//          send_commands();
//          break;
//        case 2:
//          Commands="LED_On";
//          Serial.println(Commands);
//          send_commands();
//          break;
//        case 3:
//          Commands="LED_Off";
//          Serial.println(Commands);
//          send_commands();
//          break; 
//      }
      
      btn_press = false;
    }
  }
  else {
    btn_press = true;
  }
  delay(100);
}


void send_commands(){
  Serial.println("Sending command...");
  Serial.println("Don't press the button for now...");
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }

  // We now create a URI for the request  
  Serial.print("Requesting URL : ");
  Serial.println(Commands);

  // This will send the request to the server
  client.print(String("GET ") + Commands + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: Close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {      
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  Serial.print("Server Reply = "); 
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println("Now you can press the button ...");
  Serial.println("-------------------------------------");
  Serial.println("");
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
  Commands="LED_Blink";
  Serial.println(Commands);
  send_commands();
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
  delay(3000);

//  return finger.fingerID;
  
}
