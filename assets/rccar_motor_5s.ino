#include <SoftwareSerial.h>

// ==========================================
// HM-10 Bluetooth
// Arduino D2(RX) <- HM-10 TX
// Arduino D3(TX) -> HM-10 RX
// ==========================================
SoftwareSerial bluetooth(3, 2);

// ==========================================
// L9110S Motor
// ==========================================

// A Motor
const int motorA_IA = 9;
const int motorA_IB = 8;

// B Motor
const int motorB_IA = 5;
const int motorB_IB = 4;

// Motor speed
const int driveSpeed = 200;


void setup() {

  // Motor pin setting
  pinMode(motorA_IA, OUTPUT);
  pinMode(motorA_IB, OUTPUT);

  pinMode(motorB_IA, OUTPUT);
  pinMode(motorB_IB, OUTPUT);

  // Motor stop
  stopMotors();

  // USB Serial Monitor
  Serial.begin(9600);

  // HM-10
  bluetooth.begin(9600);

  Serial.println("============================");
  Serial.println("HM-10 Bluetooth Test");
  Serial.println("============================");
  Serial.println("Bluetooth : 1 = Forward");
  Serial.println("Bluetooth : 0 = Stop");
  Serial.println("");
  Serial.println("You can also type 1 or 0");
  Serial.println("in Serial Monitor.");
  Serial.println("============================");
}


void loop() {

  // ======================================
  // 1. HM-10 Bluetooth receive
  // ======================================
  if (bluetooth.available()) {

    char command = bluetooth.read();

    Serial.print("[Bluetooth RX] ");
    Serial.print("Character = ");
    Serial.print(command);

    Serial.print(" / ASCII = ");
    Serial.println((int)command);

    controlMotor(command);
  }


  // ======================================
  // 2. Serial Monitor receive
  // ======================================
  if (Serial.available()) {

    char command = Serial.read();

    // Ignore Enter and Line Feed
    if (command == '\r' || command == '\n') {
      return;
    }

    Serial.print("[Serial Monitor RX] ");
    Serial.println(command);

    controlMotor(command);
  }
}


// ==========================================
// Command
// ==========================================
void controlMotor(char command) {

  if (command == '1') {

    Serial.println(">>> FORWARD");

    forward();
  }

  else if (command == '0') {

    Serial.println(">>> STOP");

    stopMotors();
  }

  else {

    Serial.print(">>> Unknown command : ");
    Serial.println(command);
  }
}


// ==========================================
// Forward
// ==========================================
void forward() {

  // Motor A
  analogWrite(motorA_IA, driveSpeed);
  digitalWrite(motorA_IB, LOW);

  // Motor B
  analogWrite(motorB_IA, driveSpeed);
  digitalWrite(motorB_IB, LOW);
}


// ==========================================
// Stop
// ==========================================
void stopMotors() {

  digitalWrite(motorA_IA, LOW);
  digitalWrite(motorA_IB, LOW);

  digitalWrite(motorB_IA, LOW);
  digitalWrite(motorB_IB, LOW);
}