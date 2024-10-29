#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "secrets.h"  // Include the secrets.h file

#define DHTPIN D1       // Define the DHT11 data pin
#define DHTTYPE DHT11   // Define sensor type as DHT11
DHT dht(DHTPIN, DHTTYPE);

// WiFi credentials and InfluxDB token from secrets.h
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* influxToken = INFLUXDB_TOKEN;
const char* influxIP = INFLUXDB_IP;  // Reference IP from secrets.h

// Construct the full InfluxDB server URL using the IP from secrets.h
String serverName = "http://" + String(influxIP) + ":8086/api/v2/write?org=myorg&bucket=91dd6a0599bce6a4&precision=s";

// Function prototype for sendToInfluxDB
void sendToInfluxDB(String postData);

void setup() {
    Serial.begin(9600);
    Serial.println("Hello, Serial Monitor!");
    dht.begin();

    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
}

void loop() {
    // Read temperature and humidity from DHT11
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Check if any readings failed
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Prepare data in InfluxDB line protocol format
    String postData = "environment,location=room temperature=" + String(temperature) + ",humidity=" + String(humidity);
    sendToInfluxDB(postData);

    delay(10000);  // Send data every 10 seconds
}

void sendToInfluxDB(String postData) {
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;  // Create a WiFiClient instance
        HTTPClient http;
        
        // Begin HTTP connection with WiFiClient and URL
        http.begin(client, serverName);
        
        // Add headers for InfluxDB authentication and content type
        http.addHeader("Content-Type", "text/plain");
        http.addHeader("Authorization", "Token " + String(influxToken));

        int httpResponseCode = http.POST(postData);
        if (httpResponseCode > 0) {
            Serial.println("Data sent to InfluxDB: " + postData);
        } else {
            Serial.println("Error sending data: " + String(httpResponseCode));
        }
        http.end();
    } else {
        Serial.println("WiFi disconnected");
    }
}
