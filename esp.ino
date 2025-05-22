#include <Arduino.h>

#define TRIG_PIN 18
#define ECHO_PIN 19
#define MAX_DISTANCE_CM 400                // max reliable range
#define TIMEOUT_US (MAX_DISTANCE_CM * 58)  // timeout in micro sec

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);  // ensure trigger starts LOW
  delay(100);                   // allow sensor to stabilize
  Serial.println("Ultrasonic Sensor Test");
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure echo pulse width (with timeout)
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, TIMEOUT_US);

  // check for errors
  if (duration == 0) {
    Serial.println("Error: No echo (check wiring/sensor)");
    if (digitalRead(ECHO_PIN)) {
      Serial.println("ECHO_PIN is stuck HIGH (sensor issue)");
    } else {
      Serial.println("ECHO_PIN is stuck LOW (no signal)");
    }
  } else {
    // calculate distance in cm
    float distance = duration * 0.0343 / 2;  

    if (distance >= 5.0 && distance <= 10.0) {
      
      Serial.println("within range");
        delay(4000); 

    } else {
      
      Serial.println("out of range");
      delay(1000);
    }
  }

}
