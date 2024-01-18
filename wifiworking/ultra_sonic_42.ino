#include <WiFi.h>

const char* ssid = "Modin-Students";
const char* password = "";
WiFiServer server(80);
int blueledPin = 2; // Pin connected to the blue channel of the RGB LED
int redledPin = 26; // Pin connected to the red channel of the RGB LED
int greenledPin = 27; // Pin connected to the green channel of the RGB LED
void setup()
{
Serial.begin(115200);
Serial.println("Adafruit AHT10/AHT20 demo!");
pinMode(blueledPin, OUTPUT);
pinMode(redledPin, OUTPUT);
pinMode(greenledPin, OUTPUT);
// Connect to Wi-Fi
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED)
{
delay(1000);
Serial.println("Connecting to WiFi...");
}
// Print Wi-Fi connection details
Serial.println("");
Serial.println("WiFi connected.");
Serial.print("IP address: ");
Serial.println(WiFi.localIP());
// Start the server
server.begin();
}

void loop() {

WiFiClient client = server.available();

if (client) {

Serial.println("New client connected.");

// Read the request

String request = client.readStringUntil('\r');

// Handle the request

if (request.indexOf("/LED=RED") != -1) {

digitalWrite(redledPin, HIGH);

digitalWrite(greenledPin, LOW);

digitalWrite(blueledPin, LOW);

sendResponse(client);

delay(1000);//Delay to allow time for the browser to display the response

}

else if (request.indexOf("/LED=GREEN") != -1) {

digitalWrite(redledPin, LOW);

digitalWrite(greenledPin, HIGH);

digitalWrite(blueledPin, LOW);

sendResponse(client);

delay(1000);// Delay to allow time for the browser to display the response

}

else if (request.indexOf("/LED=BLUE") != -1) {

digitalWrite(redledPin, LOW);

digitalWrite(greenledPin, LOW);

digitalWrite(blueledPin, HIGH);

sendResponse(client);

delay(1000);

}

Serial.println("Client disconnected.");

delay(10);

client.stop(); // Close the client connection after a delay

}

}

void sendResponse(WiFiClient client) {

// Send the HTTP response header

// Send the HTML page with buttons

client.println("<!DOCTYPE html>");

client.println("<html>");

client.println("<body>");

client.println("<h1 align=center style=color:#3d5a80; >Nevo and Asaf Projects And Fun</h1>");

client.println("<h2 align=center style=color:#3d5a80; >ESP32 Web Server</h2>");

client.println("<h2 align=center style=color:#3d5a80; >RGB LED - RED/GREEN/BLUE</h2>");

client.println("<p align=center> <button style=background-color:#ee6c4d;width:180px;height:40px onclick=\"location.href='/LED=RED'\">Red</button></p>");

client.println("<p align=center> <button style=background-color:#57cc99;width:180px;height:40px onclick=\"location.href='/LED=GREEN'\">Green</button></p>");

client.println("<p align=center> <button style=background-color:#98c1d9;width:180px;height:40px onclick=\"location.href='/LED=BLUE'\">Blue</button></p>");

client.println("<body style=background-color:#e0fbfc>");

client.println("</body>");

client.println("</html>");

}