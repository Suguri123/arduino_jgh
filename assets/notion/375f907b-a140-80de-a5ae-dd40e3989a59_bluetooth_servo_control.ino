#include <SoftwareSerial.h>
#include <Servo.h>
Servo myservo;

SoftwareSerial BTSerial(2, 3); // RX 2, TX 3

String inputString = ""; // 전역 변수로 선언

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);

  Serial.println("Bluetooth Servo Control Ready");

  myservo.attach(9);
  myservo.write(90);
}

void loop() {
  if (BTSerial.available()) {
    inputString = BTSerial.readString();
    inputString.trim(); // 줄바꿈, 공백 제거

    Serial.print("받은 값: ");
    Serial.println(inputString);

    if (inputString == "0") {
    myservo.write(0);
    }
    else if (inputString == "90") {
    myservo.write(90);
    }
    else if (inputString == "180") {
    myservo.write(180);
    }
  }

  if (Serial.available()) {
    BTSerial.write(Serial.read());
  }
}