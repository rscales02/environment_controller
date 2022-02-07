#include <ESP8266WiFi.h>

// Set up Access Point SSID, Password
String const AP_SSID = "You're a wizard, Harry";
String const AP_PASS = "Alohomorrah";

//Set up local wifi login
char* localSSID = "MiFibra-1620";
char* localPass = "Nacho1983";

//Set up Web Server
WiFiServer espServer(80); /* Instance of WiFiServer with port number 80 */
/* 80 is the Port Number for HTTP Web Server */

// Set up LED pins
int blueLEDPin = 13;
int yellowLEDPin = 4;
int redLEDPin = 5;
// Set variables for HIGH/LOW for HTML
int blueValue;
int redValue;
int yellowValue;

//Set temp / humidity sensor pin
int TEMP_HUM_PIN = 12;

void setup() {
  Serial.begin(115200);
  // Confingure LED pins for output
  pinMode(blueLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(yellowLEDPin, OUTPUT);
  //Set LED Pins off/on
  digitalWrite(blueLEDPin, LOW);
  digitalWrite(redLEDPin, LOW);
  digitalWrite(yellowLEDPin, LOW);

//  Set temp/hum pin for input
  pinMode(TEMP_HUM_PIN, INPUT);

  //Set up Access Point
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(AP_SSID, AP_PASS);

  // Connect to local wifi if available
  WiFi.begin(localSSID, localPass);
  Serial.print("\n");
  Serial.print("Connecting to: ");
  Serial.println(localSSID);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("*");
    delay(500);
  }

  //Start webserver
  Serial.print("\n");
  Serial.println("Starting Temp Controller Web Server...");
  espServer.begin();
  Serial.println("Temp Controller Web Server has started");


  // Connected to WiFi
  Serial.println();
  Serial.println("Connected!");
  Serial.print("IP address for network ");
  Serial.print(localSSID);
  Serial.print(" : ");
  Serial.println(WiFi.localIP());
  Serial.print("IP address for network ");
  Serial.print(AP_SSID);
  Serial.print(" : ");
  Serial.print(WiFi.softAPIP());
  Serial.print("\n");
}

void loop() {
  //  Check for webserver and return for another loop if unavailable
  WiFiClient client = espServer.available();
  if (!client) {
    delay(200);
    return;
  }

  Serial.print("New Client");
  //  Read first line of request from client
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  if (request.indexOf("/GPIO5ON") != -1)
  {
    Serial.println("GPIO5/Red LED is ON");
    digitalWrite(redLEDPin, HIGH);
    redValue = HIGH;
  }
  if (request.indexOf("/GPIO5OFF") != -1)
  {
    Serial.println("GPIO5/Red LED is OFF");
    digitalWrite(redLEDPin, LOW);
    redValue = LOW;
  }
  if (request.indexOf("/GPIO4ON") != -1)
  {
    Serial.println("GPIO4/Yellow LED is ON");
    digitalWrite(yellowLEDPin, HIGH);
    yellowValue = HIGH;
  }
  if (request.indexOf("/GPIO4OFF") != -1)
  {
    Serial.println("GPIO4/Yellow LED is OFF");
    digitalWrite(yellowLEDPin, LOW);
    yellowValue = LOW;
  }
  if (request.indexOf("/GPIO13ON") != -1)
  {
    Serial.println("GPIO13/Blue LED is ON");
    digitalWrite(blueLEDPin, HIGH);
    blueValue = HIGH;
  }
  if (request.indexOf("/GPIO13OFF") != -1)
  {
    Serial.println("GPIO13/Blue LED is OFF");
    digitalWrite(blueLEDPin, LOW);
    blueValue = LOW;
  }

  /* HTTP Response in the form of HTML Web Page */
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(); //  IMPORTANT
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  /* CSS Styling for Buttons and Web Page */
  client.println("<style>");
  client.println("html { font-family: Courier New; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button {border: none; color: white; padding: 10px 20px; text-align: center;");
  client.println("text-decoration: none; font-size: 25px; margin: 2px; cursor: pointer; background-color: gray}");
  client.println(".button1 {background-color: #F9E076;}");
  client.println(".button2 {background-color: red;}");
  client.println(".button3 {background-color: blue;}");
  client.println("</style>");
  client.println("</head>");

  /* The main body of the Web Page */
  client.println("<body>");
  client.println("<h2>ESP8266 Web Server</h2>");

  if (yellowValue == LOW)
  {
    client.println("<p>GPIO4/Yellow LED Status: OFF</p>");
    client.print("<p><a href=\"/GPIO4ON\"><button class=\"button button1\">Click to turn ON</button></a></p>");
  }
  else
  {
    client.println("<p>GPIO4/Yellow LED Status: ON</p>");
    client.print("<p><a href=\"/GPIO4OFF\"><button class=\"button\">Click to turn OFF</button></a></p>");
  }

  if (redValue == LOW)
  {
    client.println("<p>GPIO5/Red LED Status: OFF</p>");
    client.print("<p><a href=\"/GPIO5ON\"><button class=\"button button2\">Click to turn ON</button></a></p>");
  }
  else
  {
    client.println("<p>GPIO5/Red LED Status: ON</p>");
    client.print("<p><a href=\"/GPIO5OFF\"><button class=\"button\">Click to turn OFF</button></a></p>");
  }
  if (blueValue == LOW)
  {
    client.println("<p>GPIO13/Blue LED Status: OFF</p>");
    client.print("<p><a href=\"/GPIO13ON\"><button class=\"button button3\">Click to turn ON</button></a></p>");
  }
  else
  {
    client.println("<p>GPIO13/Blue LED Status: ON</p>");
    client.print("<p><a href=\"/GPIO13OFF\"><button class=\"button\">Click to turn OFF</button></a></p>");
  }
  client.print("<p>");
  client.print(digitalRead(TEMP_HUM_PIN));
  client.print("</p>");
  client.println("</body>");
  client.println("</html>");
  client.print("\n");

  delay(1);
  /* Close the connection */
//  client.stop();
  Serial.println("Client disconnected");
  Serial.print("\n");

}

// 18 C Vo = 477
