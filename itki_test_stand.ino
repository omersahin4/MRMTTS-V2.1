#include <HX711_ADC.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <SPI.h>

// --- Pinler ---
const int HX711_dout = 3;
const int HX711_sck = 2;
const int chipSelect = 4;
const int butonPin = 6;
const int ateslemePin = 5;

// --- Nesneler ---
HX711_ADC LoadCell(HX711_dout, HX711_sck);
LiquidCrystal_I2C lcd(0x27, 16, 2);
File myFile;

// --- Değişkenler ---
float maxThrust = 0;
bool kayitAktif = false;
String fileName = "";
int dosyaNo = 1;

void setup() {
  Serial.begin(115200);
  pinMode(butonPin, INPUT_PULLUP);
  pinMode(ateslemePin, OUTPUT);
  digitalWrite(ateslemePin, LOW);

  lcd.init();
  lcd.backlight();
  lcd.print("Sistem Aciliyor");

  if (!SD.begin(chipSelect)) {
    lcd.setCursor(0, 1);
    lcd.print("SD Hatasi!");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("SD Hazir.");
  }

  LoadCell.begin();
  LoadCell.setSamplesInUse(32);
  LoadCell.start(3000, true);
  LoadCell.setCalFactor(102.5); // Kalibrasyon faktörün

  lcd.clear();
  lcd.print("Hazir - Baslat");
}

void loop() {
  static boolean newDataReady = 0;
  if (LoadCell.update()) newDataReady = true;

  float gram = LoadCell.getData();
  if (abs(gram) < 5.0) gram = 0.0;
  if (gram > maxThrust) maxThrust = gram;

  // --- BUTON KONTROLÜ ---
  if (digitalRead(butonPin) == LOW) {
    delay(300); // Buton arkı (Debounce) önleme

    if (!kayitAktif) {
      // --- YENİ DOSYA ADI BULMA ---
      dosyaNo = 1;
      while (SD.exists("test" + String(dosyaNo) + ".csv")) {
        dosyaNo++;
      }
      fileName = "test" + String(dosyaNo) + ".csv";

      // --- GERİ SAYIM ---
      for (int i = 5; i > 0; i--) {
        lcd.clear();
        lcd.print("GERI SAYIM: "); lcd.print(i);
        delay(1000);
      }
      
      lcd.clear();
      lcd.print("KAYIT: "); lcd.print(fileName);
      
      kayitAktif = true;
      maxThrust = 0; // Yeni test için max değerini sıfırla

      // ATEŞLEME (Opsiyonel: Eğer otomatik olsun istersen)
      digitalWrite(ateslemePin, HIGH);
      delay(2000); 
      digitalWrite(ateslemePin, LOW);
      
    } else {
      // --- KAYDI DURDUR VE GÜVENLİ KAPAT ---
      kayitAktif = false;
      if (myFile) {
        myFile.close();
      }
      lcd.clear();
      lcd.print("KAYIT DURDU!");
      lcd.setCursor(0, 1);
      lcd.print("GUCU KESEBILIRSN");
      while(1); // Güvenlik için sistemi kilitle
    }
  }

  // --- VERİ KAYDI (Sadece aktifken) ---
  if (newDataReady && kayitAktif) {
    String gramStr = String(gram, 2);
    gramStr.replace(".", ","); // Excel için virgül yapıyoruz

    String dataLine = String(millis()) + ";" + gramStr;

    myFile = SD.open(fileName, FILE_WRITE);
    if (myFile) {
      myFile.println(dataLine);
      myFile.close(); // Veriyi her satırda mühürle (Biraz yavaşlatır ama veri güvenlidir)
    }
    
    // LCD Güncelleme
    static unsigned long lastLCD = 0;
    if (millis() - lastLCD > 300) {
      lcd.setCursor(0, 0);
      lcd.print("Itki: "); lcd.print(gram, 1); lcd.print("g    ");
      lcd.setCursor(0, 1);
      lcd.print("Max : "); lcd.print(maxThrust, 1); lcd.print("g    ");
      lastLCD = millis();
    }
    newDataReady = false;
  }
}