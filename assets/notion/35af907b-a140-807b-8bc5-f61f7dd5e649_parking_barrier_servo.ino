#include <Servo.h>

Servo myservo;

void setup() {
  Serial.begin(9600);       // 시리얼 통신 시작
  pinMode(6, OUTPUT);       // Trig 핀
  pinMode(5, INPUT);        // Echo 핀

  myservo.attach(3);        // 서보모터 3번 핀에 연결
  myservo.write(90);         // 차단기 초기 위치 (내려감)
}

void loop() {
  // 초음파 신호 보내기
  digitalWrite(6, LOW);
  delayMicroseconds(2);
  digitalWrite(6, HIGH);
  delayMicroseconds(10);
  digitalWrite(6, LOW);

  // 반사 시간 측정
  float duration = pulseIn(5, HIGH);
  float distance = (340.0 * duration) / 2.0 / 10000.0;  // 단위: cm

  Serial.print("Distance: ");
  Serial.println(distance);

  // 거리가 10cm 이내이면 차단기 열기
  if (distance > 0 && distance < 10) {    //&& : and와 동일한 논리연산자
    myservo.write(180);    // 차단기 올림
  } else {
    myservo.write(90);     // 차단기 내림
  }

  delay(300);  // 측정 간격 0.3초
}

