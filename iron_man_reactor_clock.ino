#include <WiFi.h>
#include <NTPClient.h>
#include <TFT_eSPI.h>
#include <Adafruit_NeoPixel.h>

// Wi-Fi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// NTP Client
WiFiUDP udp;
NTPClient timeClient(udp);

// TFT Display
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

// LED Ring
#define LED_PIN 6
#define NUM_LEDS 16
Adafruit_NeoPixel ledRing(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Initialize TFT display
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // Initialize LED Ring
  ledRing.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize NTP Client
  timeClient.begin();
  timeClient.setTimeOffset(0);
}

void loop() {
  // Update time from NTP
  timeClient.update();

  // Display current time
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.printf("Current Time: %02d:%02d:%02d", timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds());

  // LED Animation
  animateLedRing();

  // Rebuild Hourly
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate >= 3600000) {
    rebuildClock();
    lastUpdate = millis();
  }

  delay(1000);
}

void animateLedRing() {
  for (int i = 0; i < NUM_LEDS; i++) {
    ledRing.setPixelColor(i, ledRing.Color(255, 0, 0)); // Red
    ledRing.show();
    delay(50);
    ledRing.setPixelColor(i, ledRing.Color(0, 0, 0)); // Off
  }
}

void rebuildClock() {
  // Custom logic to rebuild the clock
  Serial.println("Rebuilding clock...");
  // Add your rebuild routines here
}