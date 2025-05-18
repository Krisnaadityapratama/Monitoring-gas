#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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
#define buzzer 19

const int MQ2_sensor = 34;
const int MQ135_sensor = 32;
const int numReadings = 10;

const char* ssid = "sigid";
const char* password = "12345678";
const char* apiKey = "KGADI7AJD27XWE35";
const char* server = "api.thingspeak.com";

unsigned long previousMillis = 0;
const long sendInterval = 120000;
unsigned long lastReadTime = 0;
const long readInterval = 1000;

String lastSendTimeFormatted = "00:00:00";

void setup() {
  Serial.begin(115200);
  pinMode(MQ2_sensor, INPUT);
  pinMode(MQ135_sensor, INPUT);
  pinMode(buzzer, OUTPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("Booting...");
  display.display();
  delay(2000);
  display.clearDisplay();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi!");
  display.setCursor(0, 20);
  display.setTextSize(1);
  display.println("Connected to WiFi");
  display.display();
  delay(2000);
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
  
  // Kondisi untuk menyalakan buzzer
  if (ch4 > 10 || co > 10 || nh3 > 3) {
    digitalWrite(buzzer, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
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
      Serial.println(httpResponseCode);
      Serial.println(payload);

      unsigned long t = millis() / 1000;
      int hours = t / 3600;
      int minutes = (t % 3600) / 60;
      int seconds = t % 60;
      char timeBuffer[9];
      sprintf(timeBuffer, "%02d:%02d:%02d", hours, minutes, seconds);
      lastSendTimeFormatted = String(timeBuffer);

      display.setCursor(100, 0);
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.print("OK");
      display.display();
    } else {
      Serial.print("Error on sending: ");
      Serial.println(httpResponseCode);
      display.setCursor(100, 0);
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.print("FAIL");
      display.display();
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
    display.setCursor(100, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.print("NO-WiFi");
    display.display();
  }
}

void displayValues() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("CO : "); display.println(co, 2);
  display.print("CH4: "); display.println(ch4, 2);
  display.print("NH3: "); display.println(nh3, 2);
  display.print("MQ2: "); display.println(raw_mq2);
  display.print("MQ135: "); display.println(raw_mq135);
  display.print("Update: ");
  display.println(lastSendTimeFormatted);
  display.display();
}

void readAndCalculateMQ2() {
  int totalValue = 0;
  for (int i = 0; i < numReadings; i++) {
    totalValue += analogRead(MQ2_sensor);
    delay(10);
  }
  raw_mq2 = totalValue / numReadings;
  Serial.print("MQ2 Raw Value: ");
  Serial.println(raw_mq2);

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

    if (co > 10 | ch4 > 10 ){
    digitalWrite(buzzer, HIGH);
  }else{
    digitalWrite(buzzer, LOW);
  }
}

void readAndCalculateMQ135() {
  int totalValue = 0;
  for (int i = 0; i < numReadings; i++) {
    totalValue += analogRead(MQ135_sensor);
    delay(10);
  }
  raw_mq135 = totalValue / numReadings;
  Serial.print("MQ135 Raw Value: ");
  Serial.println(raw_mq135);

  VRL = raw_mq135 * (3.3 / 4095.0);
  if (VRL > 0) {
    Rs = ((3.3 * RL) / VRL) - RL;
    ratio = Rs / Ro_NH3;
    nh3 = (ratio > 0) ? pow(10, (log10(ratio) - b) / m) : 0;
  } else {
    nh3 = 0;
  }

  if (nh3 > 2){
    digitalWrite(buzzer, HIGH);
  }else{
    digitalWrite(buzzer, LOW);
  }
}
