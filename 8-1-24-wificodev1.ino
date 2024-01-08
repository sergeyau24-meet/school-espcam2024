#include <WiFi.h>
const char* ssid = "******";
const char* password = "******";
WiFiServer server(80);


void setup() {
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
}
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}


void loop() {
  // put your main code here, to run repeatedly:
  

}
