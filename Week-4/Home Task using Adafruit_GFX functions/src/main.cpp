// Muhammad Umair     23-NTU-CS-1054
// Home Task week-4 OLED display

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup() {
  Wire.begin(21, 22); // ESP32 default I2C pins

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    for (;;); // Stop if display not found
  }

  display.clearDisplay();

  // Set text settings
  display.setTextSize(1);             // Text size 1 (normal)
  display.setTextColor(SSD1306_WHITE); // White color text
  display.setCursor(20, 20);          // Starting position of text

  // Write your name and ID
  display.println("Muhammad Umair");
  display.setCursor(10, 35);
  display.println("ID: 23-NTU-CS-1054");

  // Draw rectangle around the text
  display.drawRect(5, 5, 120, 55, SSD1306_WHITE);

  display.display(); // Show everything on OLED
}

void loop() {
  // Nothing in loop
}
