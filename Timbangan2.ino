#include<HX711_ADC.h>
#include<EEPROM.h>
#include<LiquidCrystal_I2C.h>


#include <Wire.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int HX711_dout = 2;
const int HX711_sck = 0;

HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
long t;
int speaker = 14;
int tombol = 16;

const char* ssid[] = {"rfid"} ;
const char* pass[] = {"12345566"};
const char* host = "192.168.219.137";
const int   ssid_count = sizeof(ssid) / sizeof(ssid[0]);
WiFiClient client;
int nbVisibleNetworks, n;


void setup() {
  boolean wifiFound = false;
  Serial.begin(9600);
  lcd.init();
  lcd.init();
  lcd.backlight();
  pinMode(tombol, INPUT);
  pinMode(speaker, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Alat ON");
  delay(1000);
  lcd.clear();

  pinMode(HX711_dout, INPUT);
  delay(10);
  Serial.println();
  Serial.println("Memulai...");

  LoadCell.begin();

  float calibrationValue;
  calibrationValue = 454.0;
  EEPROM.get(calVal_eepromAdress, calibrationValue);
  long stabilizingtime = 2000;
  boolean _tare = true;
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, cek kabel MCU>HX711 pastikan sudah tepat");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue);
    Serial.println("Startup selesai");
  }




  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Setup done");

  nbVisibleNetworks = WiFi.scanNetworks();
  Serial.println(F("scan done"));
  if (nbVisibleNetworks == 0) {
    Serial.println(F("no ntwrk found"));
    while (true);
  }

  for (int i = 0; i < nbVisibleNetworks; ++i) {
    Serial.println(WiFi.SSID(i));
    for (n = 0; n < ssid_count; n++) {
      if (strcmp(ssid[n], WiFi.SSID(i).c_str())) {
        Serial.print(F("\tNot matching "));
        Serial.println(ssid[n]);
      } else { // we got a match
        wifiFound = true;
        break;
      }
    }
    if (wifiFound) break;
  }
  WiFi.begin(ssid[n], pass[n]);

  while ((!(WiFi.status() == WL_CONNECTED))) {
    Serial.print(".");
    lcd.print("Menghubungkan ...");
    delay(300);
    lcd.clear();
  }



  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Wifi Terhubung");
    digitalWrite(speaker, HIGH);
    delay(500);
    digitalWrite(speaker, LOW);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WIFI CONNECT");
    lcd.setCursor(0, 1);
    lcd.print("Enjoy");
    delay(1000);
    lcd.clear();

  }
}

void loop() {
  tombol =  digitalRead(16);
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0;
  //  lcd.setCursor(14, 0);
  //  lcd.print("gr");

  if (LoadCell.update()) newDataReady = true;
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      int i = LoadCell.getData();
      if (i < 0) {
        i = 0;
      }
      lcd.setCursor(0, 0);
      lcd.print("Berat: ");
      String reply;
      HTTPClient http;
      reply = "http://" + String(host) + "/timbangan/public/cek/data";
      http.begin(client, reply);
      http.GET();
      String responWeb = http.getString();




      if (i <= 5000 && i >= 1000) {
        float kg = 1000 ;
        kg = i / kg;
        Serial.println(kg);
        lcd.clear();
        lcd.setCursor(14, 0);
        lcd.print("Kg");
        lcd.setCursor(8, 0);
        lcd.print(kg);
        lcd.setCursor(0, 1);
        lcd.print("Jmlh : ");
        lcd.setCursor(7, 1);
        lcd.print(responWeb);
        Serial.println(responWeb);
        Serial.println(i);
        //        tampil(kg);

      }
      else if (i < 1000 && i >= 0) {
        lcd.clear();
        lcd.setCursor(14, 0);
        lcd.print("gr");
        lcd.setCursor(8, 0);
        //        lcd.print(i);
        tampil(i);

        lcd.setCursor(0, 1);
        lcd.print("Jmlh: ");
        lcd.setCursor(7, 1);
        lcd.print(responWeb);
        Serial.println(responWeb);
        Serial.println(i);

      }
      if (tombol == HIGH)
      {
        if (i > 0 && i <= 5000)
        {
          //          koneksi_database();
          String Link;
          HTTPClient http;
          Link = "http://" + String(host) + "/timbangan/public/simpan/timbang?berat=" + String(i);
          http.begin(client, Link);
          http.GET();
          //          String responWeb = http.getString();
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Simpan Data");
          Serial.println("Data dikirim");
          Serial.println(host);
          digitalWrite(speaker, HIGH);
          delay(500);
          digitalWrite(speaker, LOW);
          delay(1000);
          http.end();
        }
        else {
          Serial.println("Nilai Gagal Dikirim");
        }


      }




      newDataReady = 0;
      t = millis();
    }
  }
  if (Serial.available() > 0) {
    float i;
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tara selesai");
  }
  //  delay(1000);
  //  lcd.clear();
}

void tampil(int j) {
  lcd.setCursor(4, 0);
  lcd.print("          ");
  if (j < 10) {
    lcd.setCursor(12, 0);
  } else if (j < 100 && j >= 10) {
    lcd.setCursor(11, 0);
  } else if (j < 1000 && j >= 100) {
    lcd.setCursor(10, 0);
  } else if (j < 10000 && j >= 1000) {
    lcd.setCursor(9, 0);

  } else if (j < 100000 && j >= 10000) {
    lcd.setCursor(8, 0);
  } else if (j < 1000000 && j >= 100000) {
    lcd.setCursor(7, 0);
  } else if (j < 10000000 && j >= 1000000) {
    lcd.setCursor(6, 0);
  } else if (j < 100000000 && j >= 10000000) {
    lcd.setCursor(5, 0);
  } else {
    lcd.setCursor(4, 0);
  }
  lcd.print(j);
}

void koneksi_database()
{
  if (!client.connect(host, 80)) {
    Serial.println("Gagal Konek");
    return;
  }
  else {
    Serial.println("Berhasil Konek :");
    Serial.print(host);
    return;
  }
}
