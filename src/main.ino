const int joy1XPin = A0; 
const int joy1YPin = A1; 
const int joy2XPin = A2; 
const int joy2YPin = A3; 

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print(analogRead(joy1XPin));
  Serial.print(",");
  Serial.print(analogRead(joy1YPin));
  Serial.print(",");
  Serial.print(analogRead(joy2XPin));
  Serial.print(",");
  Serial.println(analogRead(joy2YPin));
  delay(50);
}
