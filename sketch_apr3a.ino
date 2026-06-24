#include <WiFi.h>
#include <WebServer.h>

// LED pin
int ledPin = 13;

// Fade postavke
int totalPercent = 100;
int stepPercent = 5;
int currentPercent = 0;

// WiFi AP postavke
const char* ap_ssid = "Marko_AP";
const char* ap_pass = "12345678";

WebServer server(80);

// Fade funkcija preko LEDC (ispravno za ESP32)
void fadeLED(int delayTime) {
  for (int i = 0; i <= 255; i++) {
    ledcWrite(0, i);
    delay(delayTime);
  }
  for (int i = 255; i >= 0; i--) {
    ledcWrite(0, i);
    delay(delayTime);
  }
}

// /start endpoint
void handleStart() {

  currentPercent = 0;

  for (int percent = 0; percent <= totalPercent; percent += stepPercent) {
    currentPercent = percent;
    fadeLED(4);
  }

  currentPercent = 100;

  server.send(200, "text/plain", "DONE");
}

void setup() {
  // LEDC PWM setup
  ledcAttachPin(ledPin, 0);      // kanal 0
  ledcSetup(0, 5000, 8);         // 5 kHz, 8-bit

  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);

  // Endpointi
  server.on("/start", handleStart);

  server.on("/progress", []() {
    server.send(200, "text/plain", String(currentPercent));
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
