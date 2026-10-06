import time
import os
import random
from pid import PIDManager

def clear_screen():
    os.system('cls' if os.name == 'nt' else 'clear')

if __name__ == "__main__":
    print("Drone Kontrol Yazılımı Başlatılıyor...")
    
    # 1. Aşama: pid.py içindeki init sekansı ve kontroller çalışır
    pid = PIDManager(kp=1.2, ki=0.01, kd=0.1)
    
    input("Başlatma tamamlandı. SOC Terminal arayüzüne geçmek için ENTER'a bas...")
    
    target_angle = 0.0
    current_angle = 30.0  # Başlangıçtaki yatış açısı simülasyonu
    flight_time = 0.0
    
    try:
        while True:
            # PID ile düzeltme hesapla
            correction = pid.compute(target_angle, current_angle)
            
            # Simüle edilmiş fizik tepkisi
            current_angle = current_angle - (correction * 0.15) + random.uniform(-0.2, 0.2)
            flight_time += 0.2
            
            # 2. Aşama: Canlı Terminal SOC Paneli
            clear_screen()
            print("==================================================")
            print("       DRONE TERMINAL SOC // LIVE TELEMETRY     ")
            print("==================================================")
            print(f" [STATUS] SYSTEM: ONLINE  | MODE: MOCK SIMULATION")
            print(f" [TIME]   FLIGHT TIME: {flight_time:.1f}s")
            print("--------------------------------------------------")
            print(f" TARGET ANGLE   : {target_angle:6.2f}°")
            print(f" CURRENT ANGLE  : {current_angle:6.2f}°")
            print(f" ERROR VALUE    : {target_angle - current_angle:6.2f}°")
            print("--------------------------------------------------")
            print(f" PID OUTPUT     : {correction:6.2f}")
            print(f" P-Term         : {pid.kp * (target_angle - current_angle):6.2f}")
            print(f" I-Term         : {pid.ki * pid.integral:6.2f}")
            print(f" D-Term         : {(target_angle - current_angle - pid.previous_error):6.2f}")
            print("==================================================")
            print(" [LOGS] GPIO: SIMULATED | MPU6050: MOCK_ACTIVE")
            print(" Press Ctrl+C to abort telemetry session.")
            print("==================================================")
            
            time.sleep(0.2)
            
    except KeyboardInterrupt:
        print("\n[!] SOC Terminal oturumu güvenli bir şekilde kapatıldı.")