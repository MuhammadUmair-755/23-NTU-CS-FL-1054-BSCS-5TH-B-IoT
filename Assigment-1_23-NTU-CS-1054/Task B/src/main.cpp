// Name: Muhammad Umair
// Roll no: 23-NTU-CS-1054

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- OLED setup ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- GPIO pin configuration ---
#define LED_PIN 19        // Blue LED pin
#define BTN_PIN 25        // Button pin
#define BUZZER_PIN 27     // Buzzer pin

// --- State variables ---
bool ledOn = false;               // Tracks LED state
unsigned long btnPressTime = 0;   // Stores time when button was pressed
bool btnActive = false;           // Tracks if button is currently pressed
bool isLongPress = false;         // Tracks if press was long

const unsigned long LONG_PRESS_DELAY = 2000; // Long press threshold (2 sec)

// --- OLED message display function ---
void showText(const char* text) {
  oled.clearDisplay();            // Clear previous text
  oled.setTextSize(1);            // Set text size
  oled.setTextColor(SSD1306_WHITE); // Set text color
  oled.setCursor(0, 10);          // Set text position
  oled.println(text);             // Print message
  oled.display();                 // Refresh display
}

void setup() {
  Serial.begin(115200);           // Start serial communication

  // Initialize GPIO pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP); // Internal pull-up for stable input

  // Initialize OLED display
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (true); // Stop program if OLED not detected
  }

  showText("Press the Button");   // Initial message on OLED
}

void loop() {
  bool btnState = digitalRead(BTN_PIN); // Read current button state

  // --- Detect button press (transition from HIGH to LOW) ---
  if (btnState == LOW && !btnActive) {
    btnActive = true;                  // Mark button as pressed
    btnPressTime = millis();           // Record press time
    isLongPress = false;               // Reset long press flag
  }

  // --- Detect long press ---
  if (btnState == LOW && btnActive) {
    // Check if button held for more than 2 seconds
    if (millis() - btnPressTime > LONG_PRESS_DELAY) {
      showText("Long Press: Buzzer ON"); // Notify on display

      tone(BUZZER_PIN, 1000);           // Activate buzzer at 1 kHz
      delay(1000);                      // Hold buzzer for 1 sec
      noTone(BUZZER_PIN);               // Turn buzzer off

      showText("Buzzer Done");          // Show completion message
      delay(300);
      isLongPress = true;               // Mark as long press handled
    }
  }

  // --- Detect short press (button release before long press) ---
  if (btnState == HIGH && btnActive) {
    if (!isLongPress) {
      ledOn = !ledOn;                   // Toggle LED state
      digitalWrite(LED_PIN, ledOn);     // Apply new LED state

      // Update OLED with LED status
      showText(ledOn ? "Short Press: LED ON" : "Short Press: LED OFF");
    }

    btnActive = false;                  // Reset button flag
    delay(100);                         // Debounce delay
  }
}
