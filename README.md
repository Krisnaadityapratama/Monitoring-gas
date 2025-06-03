# Monitoring Gas Berbasis IoT dengan ESP32 dan Sensor MQ-2 & MQ-135

Penelitian ini bertujuan untuk merancang dan menguji sistem pemantauan kualitas udara berbasis Internet of Things (IoT) menggunakan NodeMCU ESP32 yang terhubung dengan sensor gas MQ-2 dan MQ-135. Sistem ini mampu memantau kadar gas berbahaya seperti karbon monoksida (CO), metana (CH₄), dan amonia (NH₃) secara real-time.

## Demo Online  
[monitoring-gas-gemareksa.vercel.app](https://monitoring-gas-gemareksa.vercel.app)

---

## Fitur

- Monitoring gas berbahaya (CO, CH₄, NH₃) secara realtime.
- Penggunaan NodeMCU ESP32 yang terhubung sensor MQ-2 dan MQ-135.
- Pengiriman data sensor ke Firebase / platform IoT lainnya (tergantung implementasi).
- Visualisasi data menggunakan grafik di web dengan Chart.js.
- Tampilan web responsif untuk desktop dan mobile.

---

## Teknologi yang Digunakan

- **Hardware:** ESP32 / NodeMCU  
- **Sensor:** MQ-2 (gas berbahaya) dan MQ-135 (kualitas udara)  
- **Platform IoT:** Firebase Realtime Database / Thingspeak (opsional)  
- **Frontend:** HTML, CSS, JavaScript, Chart.js  
- **Bahasa Pemrograman:** C++ (ESP32), HTML

---

## Cara Instalasi dan Penggunaan

1. **Persiapkan Hardware:**

   - Hubungkan sensor gas MQ-2 dan MQ-135 ke NodeMCU ESP32 sesuai pin yang ditentukan.
   - Pastikan NodeMCU terhubung ke sumber daya.

2. **Upload Firmware ESP32:**

   - Gunakan Arduino IDE.
   - Buka file `ESP32-GAS-Sensor/ESP32-GAS-Sensor.ino` dari repo.
   - Sesuaikan konfigurasi WiFi dan Firebase/Thingspeak sesuai kebutuhan.
   - Upload ke NodeMCU ESP32.

3. **Setup Backend (Firebase atau Thingspeak):**

   - Buat project Firebase dan aktifkan Realtime Database (jika menggunakan Firebase).
   - Salin konfigurasi API dan masukkan ke kode ESP32.
   - Atau buat channel Thingspeak dan sesuaikan API key.

4. **Jalankan Web Monitoring:**

   - Buka file `index.html` di browser.
   - Pastikan konfigurasi Firebase di `app.js` sudah sesuai.
   - Lihat data sensor gas secara realtime dan grafik visualisasinya.

---

## Struktur Repository

├── ESP32-GAS-Sensor/
│ └── ESP32-GAS-Sensor.ino # Firmware ESP32 utama
├── MQ135_ESP32/
├── MQ2_ESP32/
├── Mq2_RO/
├── index.html # Halaman web monitoring
├── style.css # Styling halaman web
├── app.js # Script koneksi Firebase & grafik
├── README.md
├── Blok Diagram.png
├── Flow Algoritma Sistem.jpg
├── Gambaran Umum.jpg
├── Rangkaian Keseluruhan.jpg
├── NodeMCU BMS.png
├── NodeMCU Buzzer.jpg
├── NodemCU MQ135.jpg
├── NodemCU MQ2.png
├── NodemCU Oled.png
├── ... (gambar lainnya)


---

## Screenshot / Dokumentasi

### Blok Diagram Sistem  
![Blok Diagram](Blok%20Diagram.png)

### Flow Algoritma Sistem  
![Flow Algoritma](Flow%20Algoritma%20Sistem.jpg)

### Gambaran Umum Proyek  
![Gambaran Umum](Gambaran%20Umum.jpg)

### Rangkaian Keseluruhan  
![Rangkaian Keseluruhan](Rangkaian%20Keseluruhan.jpg)

### Tampilan Web Monitoring  
![Tampilan Web](Wireframe%20Website.png)

---

## Lisensi

MIT License

---

## Kontribusi

Terima kasih jika Anda ingin berkontribusi. Silakan buka issue atau pull request.
