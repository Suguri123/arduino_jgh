#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2, 3); // RX 2, TX 3

String inputString = ""; // 전역 변수로 선언

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);

  Serial.println("Bluetooth LED Control Ready");

  pinMode(5, OUTPUT); // 빨강 LED
  pinMode(6, OUTPUT); // 노랑 LED
  pinMode(7, OUTPUT); // 초록 LED

  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
}

void loop() {
  if (BTSerial.available()) {
    inputString = BTSerial.readString();
    inputString.trim(); // 줄바꿈, 공백 제거

    Serial.print("받은 값: ");
    Serial.println(inputString);

    if (inputString == "R") {
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
    }
    else if (inputString == "Y") {
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
    }
    else if (inputString == "G") {
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
    }
    else if (inputString == "off") {
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
    }
    else if (inputString == "on") {
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
    }
  }

  if (Serial.available()) {
    BTSerial.write(Serial.read());
  }
}