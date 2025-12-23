/****************************************************
 * ESP32 - MULTI-PUBLISHER NODE
 ****************************************************/

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// ---------- WiFi ----------
char ssid[] = "Wokwi-GUEST"; 
char pass[] = "";

// ---------- MQTT ----------
const char* mqtt_server = "10.153.74.16"; 
const int   mqtt_port   = 1883;

// --- TASK CONFIGURATION ---
// 1. UNIQUE CLIENT ID: Change this for each publisher (e.g., "ESP32_Source_A", "ESP32_Source_B")
const char* CLIENT_ID = "ESP32_Source_A"; 

// 2. TOPIC STRATEGY: 
// Use a shared root topic so subscribers can use wildcards (e.g., "home/lab1/#")
const char* TOPIC_TEMP = "home/lab1/temp";
const char* TOPIC_HUM  = "home/lab1/hum";
// --------------------------

#define DHTPIN  23
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient mqtt(espClient);

void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.printf("Attempting MQTT connection as [%s]...", CLIENT_ID);
    
    // Using the unique CLIENT_ID variable defined above
    if (mqtt.connect(CLIENT_ID)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" - retrying in 2 seconds");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  connectWiFi();
  mqtt.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!mqtt.connected()) {
    connectMQTT();
  }
  mqtt.loop();

  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT read failed");
  } else {
    char tBuf[8], hBuf[8];
    dtostrf(temperature, 4, 2, tBuf);
    dtostrf(humidity,    4, 2, hBuf);

    // Publishing to your unique source topics
    mqtt.publish(TOPIC_TEMP, tBuf);
    mqtt.publish(TOPIC_HUM,  hBuf);

    Serial.printf("[%s] -> Temp: %s C | Hum: %s %%\n", CLIENT_ID, tBuf, hBuf);
  }

  delay(5000); // Send data every 5 seconds
}