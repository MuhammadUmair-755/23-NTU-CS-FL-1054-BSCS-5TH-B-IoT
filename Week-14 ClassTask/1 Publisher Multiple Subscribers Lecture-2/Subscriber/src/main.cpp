/****************************************************
 * ESP32 + MQTT Multi-Subscriber + OLED
 * Subscribes to multiple topics from 2+ publishers
 * Displays data from multiple sensors on OLED
 * Task: MQTT multiple publisher/multiple subscriber
 ****************************************************/

#include <WiFi.h>
#include <PubSubClient.h> 

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------- WiFi ----------
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

// ---------- MQTT ----------
const char* mqtt_server = "10.153.74.16"; // Change to your MQTT broker IP
const int mqtt_port = 1883;

// Multiple topics from different publishers
const char* TOPIC_PUBLISHER1_TEMP = "home/lab1/temp"; // Publisher 1 - Temperature
const char* TOPIC_PUBLISHER1_HUMID = "home/lab1/hum";   // Publisher 1 - Humidity

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------- MQTT ----------
WiFiClient espClient;
PubSubClient mqtt(espClient);

// latest data from multiple publishers
String pub1Temp = "--"; 
String pub1Humid = "--";

// Function to update OLED display with multi-publisher data
void showMultiPublisherData() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  
  // Title
  display.setCursor(0, 0);
  display.println("Zohaib");
  display.println("====================");
  
  // Publisher 1 Data
  display.setCursor(0, 20);
  display.print("Pub1-Temp: ");
  display.println(pub1Temp + "C");
  
  display.setCursor(0, 30);
  display.print("Pub1-Humid: ");
  display.println(pub1Humid + "%");
  
  display.display();
}

// MQTT callback: handles messages from all publishers
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  msg.trim();

  String topicStr = String(topic);
  
  // Route message to appropriate variable based on topic
  if (topicStr == TOPIC_PUBLISHER1_TEMP) {
    pub1Temp = msg;
    Serial.print("Publisher 1 - Temp received: ");
    Serial.println(pub1Temp);
  }
  else if (topicStr == TOPIC_PUBLISHER1_HUMID) {
    pub1Humid = msg;
    Serial.print("Publisher 1 - Humidity received: ");
    Serial.println(pub1Humid);
  }
  
  // Update display whenever any message arrives
  showMultiPublisherData();
}

void connectWiFi() {
  Serial.print("Connecting WiFi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

// Connect to MQTT broker as subscriber with unique Client ID
void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Connecting MQTT...");
    
    // Unique Client ID for this subscriber
    String clientId = "Subscriber_";
    clientId += WiFi.macAddress();
    
    if (mqtt.connect(clientId.c_str())) {
      Serial.println("connected");
      
      // Subscribe to all publisher topics
      mqtt.subscribe(TOPIC_PUBLISHER1_TEMP);
      mqtt.subscribe(TOPIC_PUBLISHER1_HUMID);
      
      Serial.println("\nSubscribed to topics:");
      Serial.println("  - " + String(TOPIC_PUBLISHER1_TEMP));
      Serial.println("  - " + String(TOPIC_PUBLISHER1_HUMID));
      
      showMultiPublisherData();
    } else {
      Serial.print("failed rc=");
      Serial.println(mqtt.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // OLED init (ESP32 default: SDA=21, SCL=22)
  Wire.begin(21, 22);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (true) delay(3);
  }
  display.clearDisplay();
  display.display();

  showMultiPublisherData();       // initial screen
  connectWiFi();

  mqtt.setServer(mqtt_server, mqtt_port); // MQTT broker
  mqtt.setCallback(callback);
  connectMQTT();
}

void loop() {
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop(); 
}
