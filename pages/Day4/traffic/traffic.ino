// Arduino Uno 신호등 제어 프로그램
// 핀 설정: 빨강(R) = 9번, 노랑(Y) = 10번, 초록(G) = 11번
// 동작: 초록(4초) -> 노랑(1초) -> 빨강(3초) 반복 (한 번에 한 색만 점등)

const int RED_PIN = 9;     // 빨간색(R) LED 핀
const int YELLOW_PIN = 10; // 노란색(Y) LED 핀
const int GREEN_PIN = 11;  // 초록색(G) LED 핀

void setup() {
  // 시리얼 통신 시작 (9600 baud rate)
  Serial.begin(9600);

  // 각 LED 핀을 출력(OUTPUT)으로 설정
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  // 시작 시 모든 LED 끄기
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);

  Serial.println("Arduino Uno 신호등 제어를 시작합니다.");
}

void loop() {
  // 1. 초록불 4초 (초록 ON, 나머지 OFF)
  Serial.println("초록불 ON (4초)");
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(RED_PIN, LOW);
  delay(4000);

  // 2. 노랑불 1초 (노랑 ON, 나머지 OFF)
  Serial.println("노랑불 ON (1초)");
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(YELLOW_PIN, HIGH);
  digitalWrite(RED_PIN, LOW);
  delay(1000);

  // 3. 빨강불 3초 (2초 켜짐 유지 + 마지막 1초 빠르게 깜빡임)
  Serial.println("빨강불 ON (2초)");
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(RED_PIN, HIGH);
  delay(2000);

  Serial.println("빨강불 깜빡임 (마지막 1초)");
  for (int i = 0; i < 5; i++) {
    digitalWrite(RED_PIN, LOW);
    delay(100);
    digitalWrite(RED_PIN, HIGH);
    delay(100);
  }
  digitalWrite(RED_PIN, LOW);
}
