#include <Arduino.h>
#define LED_PIN_RED 18 
#define LED_PIN_YELLOW 19
#define PWM_CH_Yellow 0
#define PWM_CH_Red 1
#define FREQ1 5000
#define FREQ2 10000
#define BUZZER_PIN 27     // GPIO connected to buzzer
#define PWM_CH_BUZZER 2      // PWM channel (0–15)
#define FREQ 2000   // Default frequency (Hz)
#define RESOLUTION 8     // 10-bit resolution (0–1023)

void setup() {
  // Setup PWM channel
  ledcSetup(PWM_CH_BUZZER, FREQ, RESOLUTION);
  ledcAttachPin(BUZZER_PIN, PWM_CH_BUZZER);

  // Setup LEDs Channels
  ledcSetup(PWM_CH_Yellow, FREQ1, RESOLUTION);
  ledcAttachPin(LED_PIN_YELLOW, PWM_CH_Yellow);
  ledcSetup(PWM_CH_Red, FREQ2, RESOLUTION);
  ledcAttachPin(LED_PIN_RED, PWM_CH_Red);
  
}

void loop() {
  // LED Loops 
  for (int d=0; d<=255; d+=20) {
    ledcWrite(PWM_CH_Yellow, d); delay(20); 
  }
  for (int d=255; d>=0; d-=20) { 
    ledcWrite(PWM_CH_Yellow, d); delay(20); 
  }
  for (int d=0; d<=255; d+=20) {
    ledcWrite(PWM_CH_Red, d); delay(20); 
  }
  for (int d=255; d>=0; d-=20) { 
    ledcWrite(PWM_CH_Red, d); delay(20); 
  }

  // --- 1. Simple beep pattern ---
  for (int i = 0; i < 3; i++) {
    ledcWriteTone(PWM_CH_BUZZER, 2000 + i * 400); // change tone
    delay(150);
    ledcWrite(PWM_CH_BUZZER, 0);                  // stop tone
    delay(150);
  }
  // --- 2. Frequency sweep (400Hz → 3kHz) ---
  for (int f = 400; f <= 3000; f += 100) {
    ledcWriteTone(PWM_CH_BUZZER, f);
    delay(20);
  }
  ledcWrite(PWM_CH_BUZZER, 0);
  delay(500);

  // --- 3. Short melody ---
  int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
  for (int i = 0; i < 8; i++) {
    ledcWriteTone(PWM_CH_BUZZER, melody[i]);
    delay(250);
  }
  ledcWrite(PWM_CH_BUZZER, 0);
  
}