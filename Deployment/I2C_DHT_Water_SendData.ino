#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Galaxy A225EAD"; 
const char* password = "12qwaszx";
const char* serverName = "http://192.168.195.20:5000/api/data"; // Ganti dengan alamat IP server 

// Definisikan pin sensor dan jenisnya
#define DHTPIN 15 // Definisikan pin untuk DHT11
#define DHTTYPE DHT11
#define WATER_LEVEL_PIN 12 // Definisikan pin untuk sensor ketinggian air

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Atur alamat I2C untuk LCD

// Variabel untuk menyimpan pembacaan sensor
float humidity;
float temperature;
float waterLevel; // Deklarasikan waterLevel sebagai variabel global
int waterLevelRaw; // Deklarasikan waterLevelRaw sebagai variabel global
int sensorMin = 0; // Nilai sensor saat tidak ada air
int sensorMax = 1920; // Nilai sensor saat sensor terendam penuh

unsigned long previousMillis = 0;
unsigned long previousMillisSend = 0;
unsigned long previousMillisDisplay = 0;

const long interval = 2000; // Interval untuk mengganti data tampilan (2000 ms = 2 detik)
const unsigned long sendInterval = 2000; // Interval untuk mengirim data (2000 ms = 2 detik)

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000); 
    Serial.println("Connecting to WiFi..."); // Mencetak status koneksi WiFi
  } 
  Serial.println("Connected to WiFi"); // Mencetak jika koneksi berhasil
  
  dht.begin();
  pinMode(WATER_LEVEL_PIN, INPUT);
  lcd.init(); // Inisialisasi LCD
  lcd.backlight(); // Nyalakan backlight
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Membaca sensor setiap 2 detik untuk keperluan tampilan
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Baca sensor DHT11
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    read_water(); // Baca sensor ketinggian air
  }

  // Perbarui tampilan setiap 2 detik
  if (currentMillis - previousMillisDisplay >= interval) {
    previousMillisDisplay = currentMillis;
    
    // Tampilkan data DHT11 pada LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: " + String(temperature) + "C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: " + String(humidity) + "%");
  }

  // Kirim data setiap 2 detik
  if (currentMillis - previousMillisSend >= sendInterval) {
    previousMillisSend = currentMillis;
    
    // Kirim data ke server
    send_data();
  }
}

void read_water(){
  int waterLevelRaw = analogRead(WATER_LEVEL_PIN);
  
  // Konversi pembacaan mentah ke sentimeter
  int waterLevel = map(waterLevelRaw, sensorMin, sensorMax, 0, 4);

  // Debugging: Cetak nilai mentah dan yang telah dikonversi
  Serial.print("Raw Value: ");
  Serial.print(waterLevelRaw);
  Serial.print(" - Mapped Value: ");
  Serial.println(waterLevel);
}

void send_data() {
  // Buat data yang akan dikirim dalam format JSON
  String postData = "{\"temperature\":" + String(temperature) + 
  ",\"humidity\":" + String(humidity) + 
  ",\"waterlevel\":" + String(waterLevel) + "}";
  
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(postData);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("HTTP Response code: " + String(httpResponseCode));
    Serial.println("Response: " + response);
  } else {
    Serial.print("Error sending POST request! HTTP Response code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}
