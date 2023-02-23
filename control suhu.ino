#include <Arduino.h>
#include <FirebaseESP32.h>
#include <Wifi.h>
#include <WiFiClient.h>
#include <DHT.h>  //menyertakan library DHT kedalam program

#define FIREBASE_HOST "pengamatan-benih-kedelai-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "eF2IU1mEE8DdqkqpVoMmiXKfLoEKJOgIrnB0uw9q"
#define WIFI_SSID "RG LAB KERING" // Change the name of your WIFI
#define WIFI_PASSWORD "" // Change the password of your WIFI
#define DHTPIN 5  //kita pasang sensor pada pin 5 digital
#define DHTTYPE DHT11 //kita menggunakan jenis sensor DHT11
#define relaykipas 33
#define relaykipas2 32
#define relayheater 14

DHT dht(DHTPIN, DHTTYPE); //deklarasi pin sensor dengan jenis sensor yang dipilih
FirebaseData fbdo;

bool Fanstatus = 0;
bool Fanstatus2 = 0;
bool Heaterstatus = 0;

void setup() {
  dht.begin(); //program komunikasi atau setup untuk sensor DHT 
  Serial.begin(115200); //program komunikasi atau setup untuk serial monitor dan kecepatan komunikasi (baudrate)
  
  delay(300);
  pinMode(relaykipas, OUTPUT); 
  pinMode(relaykipas2, OUTPUT);
  pinMode(relayheater, OUTPUT);

  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connect to ");
  Serial.println(WIFI_SSID);
  Serial.println("IP Address is: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  
}

void loop() {
  float t = dht.readTemperature(); //menyimpan nilai suhu pada variabel suhu
  delay(10000); //mengatur jeda waktu pembacaan sensor selama 5 detik
 //pembacaan gagal membaca sensor 
 if (isnan(t)) { 
 Serial.println(F("Failed to read from DHT sensor!"));
 return; 
 } 
 
 if (t < 29) { 
 digitalWrite(relayheater , LOW); //heater nyala 
 Heaterstatus = 1;    
 digitalWrite(relaykipas , LOW); //kipas nyala
 Fanstatus = 1;
 digitalWrite(relaykipas2 , LOW); //kipas2 nyala
 Fanstatus2 = 1; 
 } 
 else if (t > 29) { 
 digitalWrite(relaykipas , HIGH); //kipas mati
 Fanstatus = 0; 
 digitalWrite(relaykipas2 , HIGH); //kipas2 mati
 Fanstatus2 = 0; 
 digitalWrite(relayheater , HIGH); //heater mati 
 Heaterstatus = 0; 

 } 
 
 if (Heaterstatus == 1) 
 { 
 Serial.print("Heater nyala"); 
 } 
 else 
 { 
 Serial.print("Heater Mati"); 
 } 
 Serial.print(","); 
 if (Fanstatus == 1) 
 { 
 Serial.print("Kipas Nyala"); 
 }
 else 
 { 
 Serial.print("Kipas Mati"); 
 } 
 Serial.print(",");
 if (Fanstatus2 == 1) 
 { 
 Serial.print("Kipas2 Nyala"); 
 }
 else 
 { 
 Serial.print("Kipas2 Mati"); 
 } 
 Serial.print(",");   
 Serial.println(); 
 delay(500); 
  
  delay(10000);
  Serial.print("% Temperature: ");
  Serial.print(t); 
  Serial.println("°C"); 
  String  fireTemp = String(t) + String ("°C");

  Firebase.pushString(fbdo, "/DHT11", fireTemp);
  
  }
