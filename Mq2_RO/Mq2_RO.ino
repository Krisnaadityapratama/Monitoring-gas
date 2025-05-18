#define RL 10

float Analog_value = 0;
float VRL = 0;
float Rs = 0;
float Ro = 0;

void setup() {
  pinMode(32, INPUT);     // Mengatur pin 35 sebagai input
  Serial.begin(9600);     // Memulai komunikasi serial pada 9600 baud rate
}

void loop() {
  Analog_value = analogRead(34);  // Membaca nilai dari pin 35
  VRL = Analog_value * (3.3 / 4095.0);  // Menghitung tegangan dari pembacaan analog
  Rs = ((3.3 / VRL) - 1) * RL;     // Menghitung Rs berdasarkan tegangan
  Ro = Rs / 9.6;                   // Menghitung Ro

  Serial.print("Ro di udara bersih = ");
  Serial.println(Ro);               // Menampilkan nilai Ro di Serial Monitor
  delay(1000);                      // Menunggu selama 1 detik sebelum pembacaan berikutnya
}
