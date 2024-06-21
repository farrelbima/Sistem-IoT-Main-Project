#include <ESP32Servo.h>

#define ULTRASONIC_TRIG_PIN 2 // Definisikan pin untuk trigger sensor ultrasonik
#define ULTRASONIC_ECHO_PIN 5 // Definisikan pin untuk echo sensor ultrasonik
#define SERVO_PIN 18 // Definisikan pin untuk mikroservo
#define RELAY_PIN 4 // Definisikan pin untuk relay

Servo microservo;

long duration; // Variabel untuk menyimpan durasi pulsa dari sensor ultrasonik
int distance; // Variabel untuk menyimpan jarak yang diukur

unsigned long previousMillis = 0; // Variabel untuk menyimpan waktu sebelumnya untuk interval sensor
const long sensorInterval = 100; // Interval pengecekan sensor setiap 100ms

unsigned long relayPreviousMillis = 0; // Variabel untuk menyimpan waktu sebelumnya untuk relay
const long relayOnTime = 5000; // Waktu nyala relay dalam milidetik
const long relayOffTime = 5000; // Waktu mati relay dalam milidetik

bool relayState = LOW; // Status awal relay (mati)

void setup() {
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT); // Setel pin trigger sebagai output
  pinMode(ULTRASONIC_ECHO_PIN, INPUT); // Setel pin echo sebagai input
  pinMode(RELAY_PIN, OUTPUT); // Setel pin relay sebagai output
  microservo.attach(SERVO_PIN); // Hubungkan servo ke pin yang ditentukan
  microservo.write(0); // Posisi awal servo
  digitalWrite(RELAY_PIN, LOW); // Pastikan relay awalnya mati
}

void loop() {
  checkUltrasonicSensor(); // Fungsi untuk memeriksa sensor ultrasonik
  controlRelay(); // Fungsi untuk mengontrol relay
}

void checkUltrasonicSensor() {
  unsigned long currentMillis = millis(); // Mendapatkan waktu saat ini
  
  // Periksa apakah sudah saatnya membaca sensor lagi
  if (currentMillis - previousMillis >= sensorInterval) {
    previousMillis = currentMillis; // Perbarui waktu sebelumnya
    
    // Memicu sensor ultrasonik
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
    delayMicroseconds(2); // Tunggu 2 mikrodetik
    digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
    delayMicroseconds(10); // Tetapkan trigger HIGH selama 10 mikrodetik
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
    
    // Baca pin echo
    duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
    
    // Hitung jarak
    distance = duration * 0.034 / 2;
    
    // Jika jarak kurang dari 10 cm
    if (distance < 10) { 
      microservo.write(90); // Putar servo ke 90 derajat
      delay(500); // Tahan servo di sudut tersebut selama 0,5 detik
      microservo.write(0); // Kembalikan ke posisi awal (0 derajat)
      delay(500); // Tunggu selama 0,5 detik sebelum pembacaan berikutnya
    } else {
      microservo.write(0); // Pastikan servo di posisi awal
    }
  }
}

void controlRelay() {
  unsigned long currentMillis = millis(); // Mendapatkan waktu saat ini
  
  // Periksa apakah relay harus dihidupkan atau dimatikan berdasarkan waktu yang telah ditentukan
  if (relayState == LOW && (currentMillis - relayPreviousMillis >= relayOffTime)) {
    relayState = HIGH; // Ubah status relay menjadi hidup
    relayPreviousMillis = currentMillis; // Perbarui waktu sebelumnya
    digitalWrite(RELAY_PIN, HIGH); // Nyalakan relay
  } else if (relayState == HIGH && (currentMillis - relayPreviousMillis >= relayOnTime)) {
    relayState = LOW; // Ubah status relay menjadi mati
    relayPreviousMillis = currentMillis; // Perbarui waktu sebelumnya
    digitalWrite(RELAY_PIN, LOW); // Matikan relay
  }
}
