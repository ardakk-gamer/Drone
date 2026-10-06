#include <stdio.h>
#include <stdbool.h>
#include "simmodule.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

// Donanım Pin Ayarları
#define MOTOR_PIN 18
#define PWM_FREQ 50
#define PWM_RES 16

bool MOCK_MODE = true;
bool is_armed = false;

void motor_init(void) {
    #ifdef ARDUINO
    if (!MOCK_MODE) {
        ledcAttach(MOTOR_PIN, PWM_FREQ, PWM_RES);
        ledcWrite(MOTOR_PIN, 3276); // Min gaz / arm sinyali (~%2)
        Serial.println("[DONANIM] ESC arm sinyali gönderildi.");
    }
    #endif
}

void motor_set_pulse(int pulse_us) {
    // 1000us - 2000us aralığını 16-bit duty cycle değerine çevir
    // 50Hz'de periyot 20ms = 20000us. 16-bit max = 65535
    if (pulse_us < 1000) pulse_us = 1000;
    if (pulse_us > 2000) pulse_us = 2000;

    #ifdef ARDUINO
    if (!MOCK_MODE) {
        int duty = (int)(((float)pulse_us / 20000.0f) * 65535.0f);
        ledcWrite(MOTOR_PIN, duty);
    }
    #endif

    if (MOCK_MODE) {
        printf("[MOCK] Motor PWM Sinyali: %d us\n", pulse_us);
    }
}

void setup(void) {
    #ifdef ARDUINO
    Serial.begin(115200);
    delay(1000);
    #endif

    printf("\n=== DRON SİSTEMİ BAŞLATILIYOR ===\n");
    printf("Calisma modu seçin:\n  1 -> Mock Mode (Simülasyon)\n  2 -> Final Mode (Gerçek Donanım)\nSeciminiz (1 veya 2): ");
    
    int secim = 1;
    if (scanf("%d", &secim) == 1) {
        MOCK_MODE = (secim == 1);
    }

    if (MOCK_MODE) {
        printf("\n[BİLGİ] MOCK MODE aktif. Donanım kullanılmıyor.\n");
    } else {
        printf("\n[BİLGİ] FİNAL MODE aktif. ESP32 donanımı hazırlanıyor...\n");
    }

    motor_init();
    printf("[BİLGİ] Sistem hazır. 'arm' komutu bekleniyor...\n");
}

void loop_worker(void) {
    DronePhysics fizik;
    fizik_init(&fizik, 1.0f, 9.81f);

    unsigned long onceki_zaman = 0;
    #ifdef ARDUINO
    onceki_zaman = millis();
    #endif

    while (1) {
        // Seri porttan veya konsoldan komut okuma simülasyonu
        // Gerçek testte buraya nRF24L01 veya Serial komutları bağlanacak.
        
        #ifdef ARDUINO
        unsigned long simdi = millis();
        float dt = (simdi - onceki_zaman) / 1000.0f;
        if (dt >= 0.02f) { // 50Hz döngü
            onceki_zaman = simdi;
            
            if (is_armed) {
                fizik_update(&fizik, 12.0f, dt); // Örnek itki kuvveti
                DroneState st = fizik_get_state(&fizik);
                Serial.printf("Yükseklik: %.2f m | Hız: %.2f m/s\n", st.position.z, st.velocity.z);
                motor_set_pulse(1500); // Orta gaz
            } else {
                motor_set_pulse(1000); // Min gaz
            }
        }
        delay(10);
        #else
        // Masaüstü test simülasyonu için döngü kancası
        break;
        #endif
    }
}

#ifdef ARDUINO
void setup_arduino() { setup(); }
void loop_arduino() { loop_worker(); }
#else
int main(void) {
    setup();
    loop_worker();
    return 0;
}
#endif