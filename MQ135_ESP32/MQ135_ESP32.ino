#include <Arduino.h>
#include <math.h> // Untuk log10 dan pow

// Konstanta sensor MQ135
#define RL 10
#define Ro_NH3 1.88
#define m -0.417
#define b 0.843

// Pin sensor MQ135
const int MQ135_sensor = 34;
const int numReadings = 10; // Jumlah pembacaan untuk dirata-ratakan

// Variabel global
float VRL, Rs, ratio, nh3;

void setup() {
  Serial.begin(115200); // Inisialisasi komunikasi serial
  pinMode(MQ135_sensor, INPUT); // Atur pin sensor sebagai input
}

void loop() {
  readAndCalculateMQ135(); // Panggil fungsi pembacaan dan kalkulasi
  delay(2000); // Delay antar pembacaan (2 detik)
}

void readAndCalculateMQ135() {
  int totalValue = 0;
  for (int i = 0; i < numReadings; i++) {
    totalValue += analogRead(MQ135_sensor);
    delay(10); // Delay kecil antar pembacaan
  }

  int averageValue = totalValue / numReadings;
  Serial.print("MQ135 Raw Value: ");
  Serial.println(averageValue);

  VRL = averageValue * (3.3 / 4095.0); // Tegangan output sensor
  Rs = ((3.3 * RL) / VRL) - RL; // Resistansi sensor
  ratio = Rs / Ro_NH3; // Rasio Rs/Ro

  if (ratio > 0) {
    nh3 = pow(10, (log10(ratio) - b) / m); // Hitung konsentrasi NH3
  } else {
    nh3 = 0;
  }

  Serial.print("NH3 (ppm): ");
  Serial.println(nh3, 2); // Tampilkan nilai dengan 2 angka di belakang koma
}
