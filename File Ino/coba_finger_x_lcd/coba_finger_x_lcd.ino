/*******************************************************************
 *  Program Dasar Akses Sidik Jari Arduino                         *
 *                                                                 *
 *                                                                 *
 *  Dipbulikasikan Oleh : Giri Wahyu Pambudi                       *
 *                        (https://www.cronyos.com)                *
 *                                                                 *
 *  di Tulis 28/11/2018                                            *
 *******************************************************************/

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
SoftwareSerial mySerial(14, 12);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
String id;
void setup()  
{
  lcd.begin();
  Serial.begin(9600);
  while (!Serial); 
  delay(100);
  Serial.println("Program Dasar Akses Sidik Jari Arduino");
  Serial.println("https://www.cronyos.com");
  
  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  Serial.println("Waiting for valid finger...");
  lcd.setCursor(0,0);
  lcd.print("Scan Sidik jari");
}

void loop()                     // run over and over again
{
  Serial.println('Scan Barcode');
  String inChar;

  inChar = Serial.readString();
  
  if (inChar = 'SMMO0009OL.XL') {
    Serial.println(inChar);
    Serial.println('Scan Sidik Jari');
    getFingerprintID();
  }
  else {
    Serial.println('Salah');
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
    lcd.clear(); 
    lcd.setCursor(0,0);
    lcd.print("Sidik jari");
    lcd.setCursor(0,1);
    lcd.print("tidak cocok");
    delay(3000);
    lcd.clear(); 
    lcd.setCursor(0,0);
    lcd.print("Scan Sidik Jari");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sidik jari cocok");
  lcd.setCursor(0,1);
  lcd.print("ID #");
  lcd.print(finger.fingerID);  
  delay(3000);
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Scan Sidik Jari");

  return finger.fingerID;
}
