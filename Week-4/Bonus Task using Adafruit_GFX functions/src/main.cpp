#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Wire.begin(21, 22); // ESP32 default I2C pins (SDA=21, SCL=22)

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    for (;;); // Stop if display not found
  }

  display.clearDisplay();

  // --- Draw a simple smiley face emoji ---
  // Face circle
  display.drawCircle(64, 32, 20, SSD1306_WHITE);

  // Eyes
  display.fillCircle(56, 26, 2, SSD1306_WHITE); // Left eye
  display.fillCircle(72, 26, 2, SSD1306_WHITE); // Right eye

  // Smile (arc using line segments)
  display.drawLine(55, 38, 60, 42, SSD1306_WHITE);
  display.drawLine(60, 42, 68, 42, SSD1306_WHITE);
  display.drawLine(68, 42, 73, 38, SSD1306_WHITE);

  // Optional: box around emoji
  display.drawRect(34, 12, 60, 40, SSD1306_WHITE);

  display.display();
}

void loop() {
  // Nothing in loop
}
