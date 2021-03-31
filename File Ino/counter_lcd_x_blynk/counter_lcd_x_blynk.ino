//http://alialkarkuki.blogspot.com/
//Ali Al Karkuki 

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
#define e 7
int a =0;


void setup() {
  // set up the LCD's number of columns and rows:
  Serial.begin(9600);
  lcd.begin();
  // Print a message to the LCD.
  lcd.print("Ali Al Karkuki");
  pinMode(e,INPUT);
}

void loop() {
  int button = digitalRead(e);
  
  
   lcd.setCursor(3, 1);
    lcd.print(a);
    Serial.print(a);
  
  
  if (button == HIGH) {
    a ++;
    lcd.setCursor(3, 1);
    lcd.print(a);
    Serial.print(a);
     delay(200);
    }
 
   //http://alialkarkuki.blogspot.com/
//Ali Al Karkuki 

    
  
}
