#include <Arduino.h>
#include <math.h> // Untuk pow() dan log10()

// Konstanta dan parameter kalibrasi MQ2
#define RL 10
#define Ro_CO 2.4
#define Ro_CH4 2.6
#define aco -0.301
#define bco 1.079
#define ach4 -0.398
#define bch4 1.194

// Pin analog MQ2
const int MQ2_sensor = 34;
const int numReadings = 10;

// Variabel global
float VRL, Rs, ratio, co, ch4;

void setup() {
  Serial.begin(115200); // Inisialisasi serial
  pinMode(MQ2_sensor, INPUT); // Atur pin sensor
}

void loop() {
  readAndCalculateMQ2(); // Fungsi baca dan hitung gas
  delay(2000); // Tunggu 2 detik sebelum baca ulang
}

void readAndCalculateMQ2() {
  int totalValue = 0;
  for (int i = 0; i < numReadings; i++) {
    totalValue += analogRead(MQ2_sensor);
    delay(10);
  }

  int averageValue = totalValue / numReadings;
  Serial.print("MQ2 Raw Value: ");
  Serial.println(averageValue);

  VRL = averageValue * (3.3 / 4095.0); // Tegangan analog
  Rs = ((3.3 * RL) / VRL) - RL;        // Hitung Rs
  ratio = Rs / Ro_CO;                 // Rasio untuk CO

  if (ratio > 0) {
    co = pow(10, (log10(ratio) - bco) / aco);   // ppm CO
    ch4 = pow(10, (log10(ratio) - bch4) / ach4); // ppm CH4
  } else {
    co = 0;
    ch4 = 0;
  }

  Serial.print("CO (ppm): ");
  Serial.println(co, 2);
  Serial.print("CH4 (ppm): ");
  Serial.println(ch4, 2);
}
