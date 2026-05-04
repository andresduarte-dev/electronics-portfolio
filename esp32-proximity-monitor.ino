#include <WiFi.h>      // Library to enable WiFi capabilities
#include <WebServer.h> // Library to handle HTTP requests and serve web pages

// 1. Network credentials and Static IP Configuration
const char* ssid = "Soza .";           // Your WiFi Name (SSID)
const char* password = "1227541069";   // Your WiFi Password



// 2. Hardware pin definitions for HC-SR04 Sensor
const int trigPin = 5;  // Pin to trigger the ultrasonic pulse
const int echoPin = 18; // Pin to receive the reflected echo
const int buzzerPin = 13; // Pin for the Active Buzzer

// 3. Global instances and variables
WebServer server(80);   // Create a web server object listening on port 80
float distanceCm = 0;   // Global variable to store the measured distance

// 4. Function to serve the Web Page (HTML)
void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta charset='UTF-8'><meta http-equiv='refresh' content='1'>"; // Refresh every 1s
  html += "<title>Proximity Monitor</title>";
  html += "<style>body { font-family: Arial; text-align: center; background: #1a1a2e; color: white; padding: 50px; }";
  String distColor = (distanceCm > 0 && distanceCm < 20) ? "#ff4444" : "#00ff88";
  html += "h1 { color: #00d4ff; } .distance { font-size: 80px; font-weight: bold; color: " + distColor + "; }";
  html += ".unit { font-size: 30px; color: #aaaaaa; } </style></head>";
  html += "<body><h1>Proximity Monitor</h1>";
  html += "<div class='distance'>" + String(distanceCm, 1) + "</div>"; // Converts float to String
  html += "<div class='unit'>centimeters</div>";
  html += "</body></html>";
  
  server.send(200, "text/html", html); // Sends the HTML with an 'OK' (200) status
}

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(115200);

  // Configure hardware pins
  pinMode(trigPin, OUTPUT); // TRIG sends signal
  pinMode(echoPin, INPUT);  // ECHO receives signal

  pinMode(buzzerPin, OUTPUT); // Set buzzer as output

  // Start WiFi connection process
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Link the root URL ("/") to our handleRoot function
  server.on("/", handleRoot);
  
  // Start the server
  server.begin();
Serial.println("\nWiFi Connected! IP: ");
Serial.println(WiFi.localIP());

}

void loop() {
  // Keep the web server active to listen for clients
  server.handleClient();

  // Ultrasonic Sensor logic:
  // Step 1: Send a 10-microsecond pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Step 2: Measure the time the pulse takes to return
  long duration = pulseIn(echoPin, HIGH);

  // Step 3: Calculate distance (speed of sound is 0.034 cm/us)
  distanceCm = duration * 0.034 / 2;

  // Step 4: Smart Beep Logic
if (distanceCm > 0 && distanceCm < 30) { // If object is closer than 30cm
  digitalWrite(buzzerPin, HIGH);        // Turn on sound
  delay(distanceCm * 5);                // Delay proportional to distance
  digitalWrite(buzzerPin, LOW);         // Turn off sound
  delay(distanceCm * 5);                // Wait before next beep
} else {
  digitalWrite(buzzerPin, LOW);         // Silence if far away
}

  delay(100); // Short pause to stabilize readings
}
