#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver();

#define SERVOMIN  150
#define SERVOMAX  600

#define BASE_SERVO     0
#define SHOULDER_SERVO 1
#define ELBOW_SERVO    2
#define HOOK_SERVO     3

int lastShoulder = 90;
int lastElbow = 90;
int lastBase = 90;

// Arm pose mapping
struct ArmPose {
  int shoulder;
  int elbow;
};

ArmPose poseMap[] = {
  {20, 160}, {30, 150}, {40, 145}, {50, 140}, {60, 135}, {70, 130},
  {80, 125}, {90, 120}, {100, 110}, {110, 100}, {120, 90}, {130, 80},
  {140, 70}, {150, 60}
};

void setup() {
  Serial.begin(9600);
  pca9685.begin();
  pca9685.setPWMFreq(50);
  delay(1000);

  int defaultShoulderAngle = 20;
  int defaultElbowAngle = getMappedElbow(defaultShoulderAngle);
  moveArm(defaultShoulderAngle, defaultElbowAngle);

  
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    

    // Handle material label input
    if (command == "paper" || command == "cardboard") {
      
      moveBaseSmooth(90); //go to default 90
      delay(1000);
      moveArm(30, 150); //arm goes down
      delay(1000);
      delay(1000);
      setServoAngle(HOOK_SERVO, 0); //hook close
      delay(1000);
      moveArm(100, 80); //arm goes up
      delay(1000);
      moveBaseSmooth(0); //go to position 
      delay(1000);
      setServoAngle(HOOK_SERVO, 90); //hook open
      
    

    } else if (command == "glass" || command == "plastic") {
      moveBaseSmooth(90); //go to default 90
      delay(1000);
      moveArm(30, 150);  //arm goes down
      delay(1000);
      delay(1000);
      setServoAngle(HOOK_SERVO, 0); //hook close
      delay(1000);
      moveArm(100, 80); //arm goes up
      delay(1000);
      moveBaseSmooth(45); //go to position 
      delay(1000);
      setServoAngle(HOOK_SERVO, 90); //hook open
  

    } else if (command == "metal") {
      moveBaseSmooth(90); //go to default 90
      delay(1000);
      moveArm(30, 150);  //arm goes down
      delay(1000);
      delay(1000);
      setServoAngle(HOOK_SERVO, 0); //hook close
      delay(1000);
      moveArm(100, 80); //arm goes up
      delay(1000);
      moveBaseSmooth(135); //go to position 
      delay(1000);
      setServoAngle(HOOK_SERVO, 90); //hook open


    } else if (command == "trash") {
      moveBaseSmooth(90); //go to default 90
      delay(1000);
      moveArm(30, 150); //arm goes down
      delay(1000);
      delay(1000);
      setServoAngle(HOOK_SERVO, 0); //hook close
      delay(1000);
      moveArm(100, 80); //arm goes up
      delay(1000);
      moveBaseSmooth(180); //go to position 
      delay(1000);
      setServoAngle(HOOK_SERVO, 90); //hook open

      }
      
     else {
      Serial.print(" Unknown command: ");
      Serial.println(command);
    }

    while (Serial.available()) Serial.read(); // clear input buffer
  }
}

// Arm helper functions
int getMappedElbow(int shoulderAngle) {
  int elbowAngle = map(shoulderAngle, 20, 150, 160, 30);
  return constrain(elbowAngle, 30, 160);
}

void setServoAngle(uint8_t servo, int angle) {
  angle = constrain(angle, 0, 180);
  int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pca9685.setPWM(servo, 0, pulse);
}

void moveArm(int shoulderAngle, int elbowAngle) {
  int steps = 30;
  for (int i = 1; i <= steps; i++) {
    float t = i / float(steps);
    int sAngle = lastShoulder + (shoulderAngle - lastShoulder) * t;
    int eAngle = lastElbow + (elbowAngle - lastElbow) * t;
    setServoAngle(SHOULDER_SERVO, sAngle);
    setServoAngle(ELBOW_SERVO, eAngle);
    delay(15);
  }
  lastShoulder = shoulderAngle;
  lastElbow = elbowAngle;
}

void moveBaseSmooth(int targetAngle) {
  int steps = 20;
  for (int i = 1; i <= steps; i++) {
    float t = i / float(steps);
    int currentAngle = lastBase + (targetAngle - lastBase) * t;
    setServoAngle(BASE_SERVO, currentAngle);
    delay(10);
  }
  lastBase = targetAngle;
}

