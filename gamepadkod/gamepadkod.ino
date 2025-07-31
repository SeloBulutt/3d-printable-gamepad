#include <BleGamepad.h>

// Joystick 1 pinleri
const int joy1XPin = A0;  // X ekseni
const int joy1YPin = A1;  // Y ekseni
const int joy1SWPin = 2;  // Switch (buton)

// Joystick 2 pinleri
const int joy2XPin = A2;  // X ekseni
const int joy2YPin = A3;  // Y ekseni
const int joy2SWPin = 3;  // Switch (buton)

// Push button pinleri
const int pushBtn1Pin = 4;  // Push button 1
const int pushBtn2Pin = 5;  // Push button 2
const int pushBtn3Pin = 6;  // Push button 3
const int pushBtn4Pin = 7;  // Push button 4

// Debounce delay
const int debounceDelay = 50;

// Stabilizasyon için değişkenler
const int numReadings = 3;        // Kaç okuma ortalaması alınacak
const int deadZone = 250;         // Dead zone sadece merkez için (0-4095 aralığında)
const int filterStrength = 5;     // Noise filter gücü (yüksek = daha stabil, düşük = daha hassas)

// Önceki değerleri saklamak için
int prevJoy1X = 2048, prevJoy1Y = 2048;
int prevJoy2X = 2048, prevJoy2Y = 2048;

// BLE gamepad nesnesi
BleGamepad bleGamepad;

// Çoklu okuma ortalaması fonksiyonu
int getAverageReading(int pin, int readings) {
  long total = 0;
  for(int i = 0; i < readings; i++) {
    total += analogRead(pin);
    delayMicroseconds(100);  // Küçük gecikme
  }
  return total / readings;
}

// Dead zone uygulama fonksiyonu (sadece merkez için)
int applyDeadZone(int value, int center, int deadZoneSize) {
  if (abs(value - center) < deadZoneSize) {
    return center;  // Merkez değeri döndür
  }
  return value;
}

// Noise filtreleme fonksiyonu (tüm pozisyonlar için)
int applyNoiseFilter(int newValue, int oldValue, int filterStrength) {
  return (oldValue * filterStrength + newValue) / (filterStrength + 1);
}

void setup() {
  Serial.begin(115200);
  
  // Joystick switch pinlerini input pullup olarak ayarla
  pinMode(joy1SWPin, INPUT_PULLUP);
  pinMode(joy2SWPin, INPUT_PULLUP);
  
  // Push button pinlerini input pullup olarak ayarla
  pinMode(pushBtn1Pin, INPUT_PULLUP);
  pinMode(pushBtn2Pin, INPUT_PULLUP);
  pinMode(pushBtn3Pin, INPUT_PULLUP);
  pinMode(pushBtn4Pin, INPUT_PULLUP);
  
  // ADC attenuation ayarla (daha stabil okumalar için)
  analogSetAttenuation(ADC_11db);

  // BLE Gamepad konfigürasyonu
  BleGamepadConfiguration bleGamepadConfig;
  bleGamepadConfig.setAutoReport(false);
  bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD);
  bleGamepadConfig.setVid(0xe502);
  bleGamepadConfig.setPid(0xbbab);
  bleGamepadConfig.setAxesMin(0);        // Minimum değer 0
  bleGamepadConfig.setAxesMax(32767);    // Maksimum değer 32767
  
  // Sadece gerekli kontrolleri etkinleştir
  bleGamepadConfig.setButtonCount(16);
  bleGamepadConfig.setHatSwitchCount(0);
  bleGamepadConfig.setWhichAxes(true, true, false, true, true, false, false, false);  // X, Y, Z, rZ aktif
  
  bleGamepad.begin(&bleGamepadConfig);
  
  Serial.println("BLE Gamepad başlatıldı - Push Button'lı versiyon");
}

void loop() {
  if (bleGamepad.isConnected()) {
    // Joystick buton kontrolü
    static bool joy1BtnPrev = HIGH, joy2BtnPrev = HIGH;
    bool joy1Btn = digitalRead(joy1SWPin);
    bool joy2Btn = digitalRead(joy2SWPin);
    
    if (joy1Btn == LOW && joy1BtnPrev == HIGH) {
      Serial.println(F("Left Joystick Button"));
      bleGamepad.press(BUTTON_10);
      delay(debounceDelay);
    } else if (joy1Btn == HIGH && joy1BtnPrev == LOW) {
      bleGamepad.release(BUTTON_10);
    }
    
    if (joy2Btn == LOW && joy2BtnPrev == HIGH) {
      Serial.println(F("Right Joystick Button"));
      bleGamepad.press(BUTTON_9);
      delay(debounceDelay);
    } else if (joy2Btn == HIGH && joy2BtnPrev == LOW) {
      bleGamepad.release(BUTTON_9);
    }
    
    joy1BtnPrev = joy1Btn;
    joy2BtnPrev = joy2Btn;
    
    // Push button kontrolü
    static bool pushBtn1Prev = HIGH, pushBtn2Prev = HIGH;
    static bool pushBtn3Prev = HIGH, pushBtn4Prev = HIGH;
    
    bool pushBtn1 = digitalRead(pushBtn1Pin);
    bool pushBtn2 = digitalRead(pushBtn2Pin);
    bool pushBtn3 = digitalRead(pushBtn3Pin);
    bool pushBtn4 = digitalRead(pushBtn4Pin);
    
    // Push Button 1 (BUTTON_5)
    if (pushBtn1 == LOW && pushBtn1Prev == HIGH) {
      Serial.println(F("Push Button 1 Pressed"));
      bleGamepad.press(BUTTON_5);
      delay(debounceDelay);
    } else if (pushBtn1 == HIGH && pushBtn1Prev == LOW) {
      bleGamepad.release(BUTTON_5);
    }
    
    // Push Button 2 (BUTTON_6)
    if (pushBtn2 == LOW && pushBtn2Prev == HIGH) {
      Serial.println(F("Push Button 2 Pressed"));
      bleGamepad.press(BUTTON_7);
      delay(debounceDelay);
    } else if (pushBtn2 == HIGH && pushBtn2Prev == LOW) {
      bleGamepad.release(BUTTON_7);
    }
    
    // Push Button 3 (BUTTON_7)
    if (pushBtn3 == LOW && pushBtn3Prev == HIGH) {
      Serial.println(F("Push Button 3 Pressed"));
      bleGamepad.press(BUTTON_6);
      delay(debounceDelay);
    } else if (pushBtn3 == HIGH && pushBtn3Prev == LOW) {
      bleGamepad.release(BUTTON_6);
    }
    
    // Push Button 4 (BUTTON_8)
    if (pushBtn4 == LOW && pushBtn4Prev == HIGH) {
      Serial.println(F("Push Button 4 Pressed"));
      bleGamepad.press(BUTTON_8);
      delay(debounceDelay);
    } else if (pushBtn4 == HIGH && pushBtn4Prev == LOW) {
      bleGamepad.release(BUTTON_8);
    }
    
    // Önceki button durumlarını güncelle
    pushBtn1Prev = pushBtn1;
    pushBtn2Prev = pushBtn2;
    pushBtn3Prev = pushBtn3;
    pushBtn4Prev = pushBtn4;
    
    // Joystick 1 - Stabilize edilmiş okuma
    int joy1X_raw = getAverageReading(joy1XPin, numReadings);
    int joy1Y_raw = getAverageReading(joy1YPin, numReadings);
    
    // Sadece merkez için dead zone uygula
    joy1X_raw = applyDeadZone(joy1X_raw, 2048, deadZone);
    joy1Y_raw = applyDeadZone(joy1Y_raw, 2048, deadZone);
    
    // Noise filtreleme
    int joy1X = applyNoiseFilter(joy1X_raw, prevJoy1X, filterStrength);
    int joy1Y = applyNoiseFilter(joy1Y_raw, prevJoy1Y, filterStrength);
    
    // Joystick 2 - Stabilize edilmiş okuma
    int joy2X_raw = getAverageReading(joy2XPin, numReadings);
    int joy2Y_raw = getAverageReading(joy2YPin, numReadings);
    
    // Sadece merkez için dead zone uygula
    joy2X_raw = applyDeadZone(joy2X_raw, 2048, deadZone);
    joy2Y_raw = applyDeadZone(joy2Y_raw, 2048, deadZone);
    
    // Noise filtreleme
    int joy2X = applyNoiseFilter(joy2X_raw, prevJoy2X, filterStrength);
    int joy2Y = applyNoiseFilter(joy2Y_raw, prevJoy2Y, filterStrength);
    
    // Değerleri map et (16-bit signed aralığı)
    int mappedJoy1X = map(joy1X, 0, 4095, 32767, 0);
    int mappedJoy1Y = map(joy1Y, 0, 4095, 0, 32767);  
    int mappedJoy2X = map(joy2X, 0, 4095, 32767, 0);
    int mappedJoy2Y = map(joy2Y, 0, 4095, 0, 32767);  
    
    // Joystick değerlerini gamepad'e ata (doğru eksenlere)
    bleGamepad.setX(mappedJoy1X);       // Sol joystick X -> Eksen 0
    bleGamepad.setY(mappedJoy1Y);       // Sol joystick Y -> Eksen 1
    bleGamepad.setZ(0);                 // Z ekseni sıfır
    bleGamepad.setRX(mappedJoy2X);      // Sağ joystick X -> Eksen 3 (RX)
    bleGamepad.setRY(mappedJoy2Y);      // Sağ joystick Y -> Eksen 4 (RY)
    bleGamepad.setRZ(0);                // RZ ekseni sıfır
    
    // Önceki değerleri güncelle
    prevJoy1X = joy1X;
    prevJoy1Y = joy1Y;
    prevJoy2X = joy2X;
    prevJoy2Y = joy2Y;
    
    // Debug çıktısı
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 500) {
      Serial.print("Joy1 X: "); Serial.print(joy1X); Serial.print(" -> "); Serial.print(mappedJoy1X);
      Serial.print(" | Joy1 Y: "); Serial.print(joy1Y); Serial.print(" -> "); Serial.print(mappedJoy1Y);
      Serial.print(" | Joy2 X: "); Serial.print(joy2X); Serial.print(" -> "); Serial.print(mappedJoy2X);
      Serial.print(" | Joy2 Y: "); Serial.print(joy2Y); Serial.print(" -> "); Serial.print(mappedJoy2Y);
      Serial.println();
      lastPrint = millis();
    }
    
    // Gamepad raporunu gönder
    bleGamepad.sendReport();
    delay(10);
  }
  else {
    Serial.println("Gamepad Not Connected");
    delay(1000);
  }
}