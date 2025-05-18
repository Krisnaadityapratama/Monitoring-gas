// Versi serial monitor tanpa OLED
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define RL 10
#define Ro_CO 2.4
#define Ro_CH4 2.6
#define Ro_NH3 1.88
#define aco -0.301
#define bco 1.079
#define ach4 -0.398
#define bch4 1.194
#define m -0.417
#define b 0.843

float VRL, Rs, ratio;
float co, ch4, nh3;
int raw_mq2, raw_mq135;

const int MQ2_sensor = 34;
const int MQ135_sensor = 32;
const int numReadings = 10;

const char* ssid = "Support";
const char* password = "1223334444";
const char* apiKey = "KGADI7AJD27XWE35";
const char* server = "api.thingspeak.com";

unsigned long previousMillis = 0;
const long sendInterval = 120000;
unsigned long lastReadTime = 0;
const long readInterval = 1000;

void setup() {
  Serial.begin(115200);
  pinMode(MQ2_sensor, INPUT);
  pinMode(MQ135_sensor, INPUT);

  Serial.println("Booting...");
  delay(2000);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastReadTime >= readInterval) {
    lastReadTime = currentMillis;
    readAndCalculateMQ2();
    readAndCalculateMQ135();
    displayValues();
  }

  if (currentMillis - previousMillis >= sendInterval) {
    previousMillis = currentMillis;
    sendDataToThingSpeak(co, ch4, nh3, raw_mq2, raw_mq135);
  }
}

void sendDataToThingSpeak(float co, float ch4, float nh3, int raw_mq2, int raw_mq135) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String("http://") + server + "/update?api_key=" + apiKey +
                 "&field1=" + String(co) +
                 "&field2=" + String(ch4) +
                 "&field3=" + String(nh3) +
                 "&field4=" + String(raw_mq2) +
                 "&field5=" + String(raw_mq135);

    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("[ThingSpeak] Response Code: " + String(httpResponseCode));
      Serial.println("[ThingSpeak] Response: " + payload);
    } else {
      Serial.println("[ThingSpeak] Error sending data: " + String(httpResponseCode));
    }
    http.end();
  } else {
    Serial.println("[WiFi] Disconnected. Data not sent.");
  }
}

void displayValues() {
  Serial.println("=========================");
  Serial.println("Sensor Readings:");
  Serial.println("CO  (ppm): " + String(co, 2));
  Serial.println("CH4 (ppm): " + String(ch4, 2));
  Serial.println("NH3 (ppm): " + String(nh3, 2));
  Serial.println("Raw MQ2:    " + String(raw_mq2));
  Serial.println("Raw MQ135:  " + String(raw_mq135));
  Serial.println("=========================");
}

void readAndCalculateMQ2() {
  int totalValue = 0;
  for (int i = 0; i < numReadings; i++) {
    totalValue += analogRead(MQ2_sensor);
    delay(10);
  }
  raw_mq2 = totalValue / numReadings;

  VRL = raw_mq2 * (3.3 / 4095.0);
  if (VRL > 0) {
    Rs = ((3.3 * RL) / VRL) - RL;
    float ratio_co = Rs / Ro_CO;
    float ratio_ch4 = Rs / Ro_CH4;

    co = (ratio_co > 0) ? pow(10, (log10(ratio_co) - bco) / aco) : 0;
    ch4 = (ratio_ch4 > 0) ? pow(10, (log10(ratio_ch4) - bch4) / ach4) : 0;
  } else {
    co = 0;
    ch4 = 0;
  }
}

void readAndCalculateMQ135() {
  int totalValue = 0;
  for (int i = 0; i < numReadings; i++) {
    totalValue += analogRead(MQ135_sensor);
    delay(10);
  }
  raw_mq135 = totalValue / numReadings;

  VRL = raw_mq135 * (3.3 / 4095.0);
  if (VRL > 0) {
    Rs = ((3.3 * RL) / VRL) - RL;
    ratio = Rs / Ro_NH3;
    nh3 = (ratio > 0) ? pow(10, (log10(ratio) - b) / m) : 0;
  } else {
    nh3 = 0;
  }
}
