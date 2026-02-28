#include <WiFi.h>
#include <NTPClient.h>
#include <TFT_eSPI.h>
#include <Adafruit_NeoPixel.h>
#include <SD.h>

// Wi-Fi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// NTP Client
WiFiUDP udp;
NTPClient timeClient(udp);

// TFT Display (1.28" 240x240 Round Display)
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

// LED Ring
#define LED_PIN 6
#define NUM_LEDS 16
Adafruit_NeoPixel ledRing(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// SD Card
#define SD_CS 5

// Image file path
const char* imagePath = "/00_ironman.bmp";

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\nIron Man Reactor Clock Starting...");
  
  // Initialize TFT display
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // Initialize LED Ring
  ledRing.begin();
  ledRing.show();
  
  // Initialize SD Card
  if (!SD.begin(SD_CS)) {
    Serial.println("SD Card initialization failed!");
    displayErrorMessage("SD Card Error");
  } else {
    Serial.println("SD Card initialized successfully");
  }
  
  // Connect to Wi-Fi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  int wifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
    delay(500);
    Serial.print(".");
    wifiAttempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi - running in offline mode");
  }

  // Initialize NTP Client
  timeClient.begin();
  timeClient.setTimeOffset(0); // Set your timezone offset here (e.g., -18000 for EST)
  
  delay(1000);
}

void loop() {
  // Update time from NTP
  timeClient.update();

  // Display Iron Man image with time overlay
  displayClockScreen();

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

void displayClockScreen() {
  // Draw the background (Iron Man image or solid background)
  if (SD.exists(imagePath)) {
    // Display BMP image from SD card
    drawBMP(imagePath, 0, 0);
  } else {
    // Fallback: Draw a circular gradient background
    tft.fillScreen(TFT_BLACK);
    drawRadialGradient();
  }

  // Draw time overlay in the center of the circular display
  tft.setTextColor(TFT_WHITE, TFT_TRANSPARENT);
  tft.setTextSize(3);
  tft.setTextDatum(MC_DATUM); // Middle Center alignment
  
  int centerX = 120;  // 240/2
  int centerY = 120;  // 240/2
  
  // Format time string
  char timeStr[9];
  sprintf(timeStr, "%02d:%02d:%02d", timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds());
  
  // Draw time with shadow effect
  tft.setTextColor(TFT_BLACK);
  tft.drawString(timeStr, centerX + 2, centerY + 2, 4);
  
  tft.setTextColor(TFT_YELLOW);
  tft.drawString(timeStr, centerX, centerY, 4);
}

void drawBMP(const char* filename, int x, int y) {
  File bmpFile = SD.open(filename, FILE_READ);
  
  if (!bmpFile) {
    Serial.print("Cannot open file: ");
    Serial.println(filename);
    return;
  }

  // Read BMP file header
  uint8_t header[54];
  bmpFile.read(header, 54);

  // Extract image info
  uint32_t imageWidth = *(uint32_t*)&header[18];
  uint32_t imageHeight = *(uint32_t*)&header[22];
  uint16_t bitsPerPixel = *(uint16_t*)&header[28];

  Serial.print("BMP Image - Width: ");
  Serial.print(imageWidth);
  Serial.print(" Height: ");
  Serial.print(imageHeight);
  Serial.print(" BPP: ");
  Serial.println(bitsPerPixel);

  // Read and display image data
  uint32_t pixelDataOffset = *(uint32_t*)&header[10];
  bmpFile.seek(pixelDataOffset);

  // Note: BMP format stores pixels bottom-to-top, left-to-right
  uint8_t rowBuffer[imageWidth * 3]; // Assuming 24-bit color

  for (int row = imageHeight - 1; row >= 0; row--) {
    bmpFile.read(rowBuffer, imageWidth * 3);
    for (uint32_t col = 0; col < imageWidth; col++) {
      // BMP format is BGR, convert to RGB
      uint8_t b = rowBuffer[col * 3];
      uint8_t g = rowBuffer[col * 3 + 1];
      uint8_t r = rowBuffer[col * 3 + 2];
      
      uint16_t color = tft.color565(r, g, b);
      tft.drawPixel(x + col, y + (imageHeight - 1 - row), color);
    }
  }

  bmpFile.close();
}

void drawRadialGradient() {
  // Draw a simple radial gradient as fallback
  for (int i = 119; i >= 0; i--) {
    int intensity = map(i, 0, 119, 50, 200);
    uint16_t color = tft.color565(intensity / 4, 0, intensity);
    tft.drawCircle(120, 120, i, color);
  }
}

void animateLedRing() {
  // Get current second to determine reactor animation phase
  int seconds = timeClient.getSeconds();
  int phase = seconds % 16;

  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t brightness = 0;
    uint8_t red = 255;
    uint8_t green = 100;
    uint8_t blue = 0;

    // Create pulsing effect
    if (i == phase) {
      brightness = 255;
    } else if (i == (phase - 1 + NUM_LEDS) % NUM_LEDS) {
      brightness = 150;
    } else if (i == (phase - 2 + NUM_LEDS) % NUM_LEDS) {
      brightness = 75;
    }

    // Set color with brightness
    uint32_t color = ledRing.Color(
      (red * brightness) / 255,
      (green * brightness) / 255,
      (blue * brightness) / 255
    );
    
    ledRing.setPixelColor(i, color);
  }
  
  ledRing.show();
}

void rebuildClock() {
  // Custom logic to rebuild the clock
  Serial.println("Rebuilding clock...");
  // Refresh NTP time
  timeClient.forceUpdate();
  Serial.println("Clock rebuilt and time updated");
}

void displayErrorMessage(const char* message) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(message, 120, 120, 2);
  Serial.println(message);
}