#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include "DHT.h"


//api başı
#define FIREBASE_HOST "kulucka-ba611-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "eqrC57Zs1Tg0GOPcOtTVfLsVE1k1xhV3GU8F1P3X"



//api sonu

#define ssid "SaMaNEv"                                     // Modem SSID adı
#define password  "bismillah.25"                           // Modem Şifre                           

#define ssid1 "Gultekin"                                    // Modem SSID adı
#define password1  "68426633.mg"                            // Modem Şifre    

#define ssid2 "MyGul"                                        // Modem SSID adı
#define password2  "68426633.mg"                             // Modem Şifre     

//Pin başı
DHT dht1(4, DHT11); // kulucka sensör
DHT dht2(5, DHT11); // yavruluk sensör

int led = 2;
int lamp = 9;
int viyol = 12;
int fan = 13;
int lamp1 = 14;
int pomp = 16;

//Pin sonu

// Otomasyon Başı

int lampd = LOW;
int viyold = LOW;
int fand = LOW;
int lamp1d = LOW;
int pompd = LOW;
int oto = 1;

unsigned long sn;
unsigned long viyolsn;
unsigned long pompsn;
unsigned long fansn;
unsigned long otosn;

// Otomasyon Sonu

FirebaseData fbdata;

void setup() {
//  Serial.begin(115200);

// Pin Tanımlama Başı
  pinMode(led, OUTPUT);
  pinMode(lamp, OUTPUT);
  pinMode(viyol, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(lamp1, OUTPUT);
  pinMode(pomp, OUTPUT);
// Pin Tanımlama Sonu

// Çıkış test başı
  digitalWrite(lamp, HIGH);
  digitalWrite(viyol, HIGH);
  digitalWrite(fan, HIGH);
  digitalWrite(lamp1, HIGH);
  digitalWrite(pomp, HIGH);
delay(1000);
  digitalWrite(lamp, LOW);
  digitalWrite(viyol, LOW);
  digitalWrite(fan, LOW);
  digitalWrite(lamp1, LOW);
  digitalWrite(pomp, LOW);
// Çıkış test sonu

  //wifi başı
WiFi.mode(WIFI_STA);


  WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        digitalWrite(led, LOW); delay(50); digitalWrite(led, HIGH);    
      }


  if (WiFi.status() == WL_CONNECTED) {
  digitalWrite(led, LOW); delay(1000); digitalWrite(led, HIGH);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
} //wifi sonu

  dht1.begin();
  dht2.begin();
  delay(1000);

}


void loop() {
  
digitalWrite(led, LOW); delay(50); digitalWrite(led, HIGH); delay(50);
sn = millis();

  String statu = "";
  
  float n1 = dht1.readHumidity();
  float s1 = dht1.readTemperature();

  float n2 = dht2.readHumidity();
  float s2 = dht2.readTemperature();



  if (isnan(n1) || isnan(s1)) {
    statu = statu + " Kuluçka Sensör Okunamadı.";
    s1 = 0;
    n1 = 0;
  }
  if (isnan(n2) || isnan(s2)) {
    statu = statu + " Yavruluk Sensör Okunamadı.";
    s2 = 0;
    n2 = 0;
  }

if (WiFi.status() == WL_CONNECTED) {
  if (Firebase.getString(fbdata, "/auto")){
    if (fbdata.stringData() == "1"){
      oto = 1;
      otosn = sn; 
      }
    else if (fbdata.stringData() == "0"){
      oto = 0;
      }
  }
} else {
  oto = 1;
}

if (oto == 0){
  //otomatik kapalı ise
      digitalWrite(led, LOW);


  
if (WiFi.status() == WL_CONNECTED) {
  
  if (Firebase.getString(fbdata, "/slamp")){
    if (fbdata.stringData() == "1") lampd = 1; 
    else if (fbdata.stringData() == "0") lampd = 0;
   }
  if (Firebase.getString(fbdata, "/viyol")){
    if (fbdata.stringData() == "1") viyold = 1; 
    else if (fbdata.stringData() == "0") viyold = 0;
   }
  if (Firebase.getString(fbdata, "/sfan")){
    if (fbdata.stringData() == "1") fand = 1; 
    else if (fbdata.stringData() == "0") fand = 0;
   }
  if (Firebase.getString(fbdata, "/npomp")){
    if (fbdata.stringData() == "1") pompd = 1; 
    else if (fbdata.stringData() == "0") pompd = 0;
   }
  if (Firebase.getString(fbdata, "/slamp1")){
    if (fbdata.stringData() == "1") lamp1d = 1; 
    else if (fbdata.stringData() == "0") lamp1d = 0;
   }

  digitalWrite(lamp, lampd);
  digitalWrite(viyol, viyold);
  digitalWrite(fan, fand);
  digitalWrite(lamp1, lamp1d);
  digitalWrite(pomp, pompd);

  
} else {
  oto = 1;
}

if (sn - otosn >= 10000){
   otosn = sn;
   oto = 1;
}

  
} else if (oto == 1){
  //otomatik açık ise
   otosn = sn;

if (s1 <= 37){
  lampd = HIGH;
  fand = HIGH;
  fansn = sn;
} else {
  lampd = LOW;
}

if (s2 <= 32){
  lamp1d = HIGH;
} else {
  lamp1d = LOW;
}



if (n1 <= 55) {
  if (sn - pompsn <= 5000){
    pompd = HIGH;
  } else if (sn - pompsn <= 30000){
    pompd = LOW;
  } else {
    pompsn = sn;
  }
}

if (sn - viyolsn <= 300000){
  viyold = HIGH;
} else if (sn - viyolsn <= 3600000){
  viyold = LOW;
} else {
  viyolsn = sn;
}
  

if (sn - fansn <= 300000){
  fand = HIGH;
} else if (sn - fansn <= 1800000){
  fand = LOW;
} else {
  fansn = sn;
}

  digitalWrite(lamp, lampd);
  digitalWrite(viyol, viyold);
  digitalWrite(fan, fand);
  digitalWrite(lamp1, lamp1d);
  digitalWrite(pomp, pompd);

if (WiFi.status() == WL_CONNECTED) {
Firebase.setInt(fbdata, "/slamp", lampd);  
Firebase.setInt(fbdata, "/viyol", viyold);  
Firebase.setInt(fbdata, "/slamp1", lamp1d);  
Firebase.setInt(fbdata, "/sfan", fand);  
Firebase.setInt(fbdata, "/npomp", pompd);  
}  


  //otomatik açık ise son  
} else {
  oto = 1;
}


if (WiFi.status() == WL_CONNECTED) {
Firebase.setInt(fbdata, "/auto", oto);  
Firebase.setFloat(fbdata, "/s1", s1);  
Firebase.setFloat(fbdata, "/n1", n1);  
Firebase.setFloat(fbdata, "/s2", s2);  
Firebase.setFloat(fbdata, "/n2", n2);  
Firebase.setString(fbdata, "/statu", statu);  
}

}
