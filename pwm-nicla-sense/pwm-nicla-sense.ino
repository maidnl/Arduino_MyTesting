//#include "Nicla_System.h"

#define PWM_OUT_PIN 10
#define OUT_PIN 8

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.println("*** Setup ***");

  
  analogWrite(D10, 30);
  analogWrite(D11, 150);


  analogWrite(D9,155);
  
  
  pinMode(OUT_PIN, OUTPUT);
}

void loop() {
  static int counter = 0;
  
  digitalWrite(OUT_PIN, HIGH);
  delay(500);
  
  digitalWrite(OUT_PIN, LOW);
  delay(500);
  Serial.println("*** LOOP ***");

  counter++;

  if(counter >= 10) {
    analogWrite(D9,180);
    counter = 0;
  }


}
