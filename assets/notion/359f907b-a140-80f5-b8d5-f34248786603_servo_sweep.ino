#include <Servo.h>

Servo myservo;

void setup() {
  myservo.attach(3);     // 서보모터를 3번 핀에 연결
}

void loop() {
  myservo.write(0);      // 0도
  delay(1000);           // 1초 대기

  myservo.write(90);     // 90도
  delay(1000);

  myservo.write(180);    // 180도
  delay(1000);

  myservo.write(90);     // 90도
  delay(1000);

}

