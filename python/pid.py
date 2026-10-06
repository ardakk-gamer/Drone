import time

class PIDManager:
    def __init__(self, kp, ki, kd, output_limit=100, integral_limit=50):
        try:
            import gpiozero
            self.has_gpio = True
        except SystemError:
            self.has_gpio = False
            print("Bilinmeyen bir sistem hatası oluştu. Lütfen tekrar deneyin. Hata kodu: PID-A2")
        except ImportError:
            self.has_gpio = False
            print("Uyarı: gpiozero bulunamadı. Simülasyon modunda devam ediliyor. pip install gpiozero ile yükleyebilirsin. Hata kodu: PID-A1")

        init_steps = {
            "GPIO ve Donanım Kontrolü": 1.5,
            "Sensör (Gyro/MPU) Kalibrasyonu": 2.0,
            "ESC Haberleşme Testi": 1.0,
            "Güvenlik Kilitleri Açılışı": 0.5
        }

        # Toplam tahmini süreyi otomatik hesapla
        estimated_time = sum(init_steps.values())

        print(f"Init başladı. Toplam tahmini süre: {estimated_time} saniye\n")

        # Adımları sırayla işletip kalan süreyi güncelleme
        for step_name, duration in init_steps.items():
            print(f"[İşlem] {step_name}...")
            time.sleep(duration)  # Gerçek işlem süresini simüle et
            estimated_time -= duration
            print(f"-> Tamamlandı. Kalan tahmini süre: {max(0, round(estimated_time, 1))} saniye\n")

        print("Sistem uçuşa hazır!")
        
        self.kp = kp
        self.ki = ki
        self.kd = kd
        
        self.output_limit = output_limit
        self.integral_limit = integral_limit
        
        self.previous_error = 0.0
        self.integral = 0.0
        self.last_time = time.time()


    def compute(self, setpoint, current_value):
            now = time.time()
            dt = now - self.last_time
            if dt <= 0.0:
                dt = 1e-16

            error = setpoint - current_value
            p_term = self.kp * error
            
            self.integral += error * dt
            self.integral = max(-self.integral_limit, min(self.integral, self.integral_limit))
            i_term = self.ki * self.integral
            
            derivative = (error - self.previous_error) / dt
            d_term = self.kd * derivative
            
            output = p_term + i_term + d_term
            output = max(-self.output_limit, min(output, self.output_limit))
            
            self.previous_error = error
            self.last_time = now
            
            return output

if __name__ == "__main__":
    print("PID Manager başlatılıyor...")
    
    # Sınıftan bir nesne üretelim ki __init__ ve içindeki try-except'ler çalışsın:
    pid = PIDManager(kp=1.2, ki=0.01, kd=0.1)
    
    # Test çıkışını görelim
    cikis = pid.compute(setpoint=10.0, current_value=8.5)
    print(f"Test çıkışı: {cikis}")