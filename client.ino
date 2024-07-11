#include <WiFi.h>

const char* ssid = "504";
const char* password = "12345678";

WiFiClient client;
const char* serverAddress = "127.0.0.1"; // Server IP address
const int serverPort = 11451;

void setup() {
    Serial.begin(115200);
    
    // Connect to WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    // Attempt to connect to the server
    if (!client.connected()) {
        Serial.println("Connecting to server...");
        if (client.connect(serverAddress, serverPort)) {
            Serial.println("Connected to server");
        } else {
            Serial.println("Connection to server failed, retrying in 5 seconds...");
            delay(5000);
            return;
      
