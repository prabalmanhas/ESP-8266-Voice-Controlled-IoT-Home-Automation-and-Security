// HOME SECURITY MOTION DETECTION - IFTTT EMAIL ALERTS
// PRABAL MANHAS ANTI THEFT HOME 
#include <ESP8266WiFi.h>
// wifi config
const char* ssid      = "";
const char* password  = "";

// ifttt.com access details
const char* host      = "maker.ifttt.com";
const char* urlPt1    = "https://maker.ifttt.com/trigger/";
const char* urlPt2    = "/with/key/";
const char* eventName = "";                       // the event to trigger at ifttt.com
const char* apikey    = "";                          // your ifttt.com api key
const byte  port      = 80;                            // http standard port


#define sensorPin  D1    // front door switch input: low = closed  high = open
#define led     D4    // status LED on esp8266 module

WiFiClient client;    // wifi client object

void setup() {
  Serial.begin(9600);

  pinMode(sensorPin, INPUT_PULLUP);    //  switch input
  pinMode(led, OUTPUT);             // use on board LED for sensor state indication and wifi joining progress
  digitalWrite(led, HIGH);          // turn LED off

  // join wifi network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi= ");
  Serial.print(ssid);
  Serial.println(" ...");

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(led, !(digitalRead(led)));  // toggle LED
    delay(500);
    Serial.print('.');
  }

  digitalWrite(led, HIGH);          // turn LED off

  Serial.println();
  Serial.println("IOT HOME ANTI-THEFT ABET PROJECT ....");
  Serial.print("Connecting to WiFi= ");
  Serial.println(ssid);
  Serial.println("Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.println("WAITING FOR EVENT TRIGER ....\n");
}

void loop() {
  if (digitalRead(sensorPin) == HIGH) {               // check for motion

    Serial.println("Event Occured ");
    Serial.print("SENDING EMAIL ALERTS TO HOME MEMBERS ....");
    digitalWrite(led, LOW);                        // turn LED on

    if (!client.connect(host, port)) {             // connect to ifttt.com
      Serial.println("Connection failed.");
      return;
    }

    // request ifttt.com to trigger the front door access event email
    String url = String(urlPt1) + String(eventName) + String(urlPt2) + String(apikey);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    delay(100);  // without the delay, the request may not be completed

    
        while (client.available())
        {
          Serial.write(client.read()); // optionally print out the response from ifttt.com
          Serial.println(client.read());
        }
    

    Serial.println("\nDISCONNECTING FROM IFTTT.COM ....");
    client.stop();

    // wait 5 seconds as a crude switch debounce when door is first opened,
    // then assuming front door is likely still going to be open (as people enter/exit),
    // wait until front door is closed before looking for next door open event
    delay(5000);
    while (digitalRead(sensorPin) == HIGH) {
      delay(100);
    }

    digitalWrite(led, HIGH);                         // turn LED off
    Serial.println("\nWAITING FOR INTRUSION TO DETECT ....\n");
  }
}
