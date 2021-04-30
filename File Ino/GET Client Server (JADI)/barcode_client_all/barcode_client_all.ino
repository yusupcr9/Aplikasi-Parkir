#include <ESP8266WiFi.h>

const char* ssid = "Seamless@wifi.Id";                  // Your wifi Name
const char* password = "bbbbbbbb"; // Your wifi Password

const char * host = "192.168.43.34";        // IP Server

const int httpPort = 80;

String Commands;                       // The command variable that is sent to the server
String Answer;

int button = 5;                             // push button is connected
bool btn_press = false;                      // The variable to detect the button has been pressed
int con = 0;                                // Variables for mode

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
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() == 0);
  Answer = Serial.readStringUntil('\n');
  Commands = Answer;
  Serial.println(Commands);
  send_commands();
  btn_press = false;
  delay(100);
}


void send_commands() {
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
  client.println(Commands);
  unsigned long timeout = millis();
//  while (client.available() == 0) {
//    if (millis() - timeout > 5000) {
//      Serial.println(">>> Client Timeout !");
//      client.stop();
//      return;
//    }
//  }

  Serial.print("Server Reply = ");
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
  Serial.println("Now you can press the button ...");
  Serial.println("-------------------------------------");
  Serial.println("");
}
