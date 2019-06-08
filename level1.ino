
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#define TRIGGER 5
#define ECHO    4

String threshold;
const char* ssid = "Parth iphone";

const char* password = "parth1555";
int ledPin = D1; // GPIO13
WiFiServer server(80);
//ESP8266WebServer server(80);
void setup() 
{
  Serial.begin(115200);
  delay(10);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
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
}
void loop() 
{
  /*long duration, distance;
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;
  
  Serial.print("Centimeter:");
  Serial.println(distance);
  
  delay(1000);*/
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available())
  {
    delay(1);
  }
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  // Match the request
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  
  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1) 
  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
  // Set ledPin according to the request
  //digitalWrite(ledPin, value);
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  //client.print("<body style'background-color:Blue'><p align='center' style='font-size:200%'><b> Set Threshold Value:<input type='text' name='THRESHOLD' placeholder='threshold value' required></b></p><br>");
  //client.print("<p <a href=\"/SAVE\"\">align ='center' style='font-size:160%'><input type='submit' name='SUBMIT' value='Submit'></a></p>");
  client.print("<p align='center' style='font-size:160%'><b> Led pin is now:</b></p>");

  /*server.on("/save", []()
  {
    threshold = server.arg("THRESHOLD");
    Serial.print(threshold);
    delay(1000);
  });*/
  
  if(value == HIGH) 
  {
    value = HIGH;
    client.print("<p align='center' style='font-size:160%'><b>On<b></p>");
  } 
  else
  {
    client.print("<p align='center' style='font-size:160%'><b>Off<b></p>");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");  
  client.println("</body></html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
