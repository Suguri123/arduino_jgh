// Arduino Uno 신호등 모듈을 이용한 물결(Wave) 효과 프로그램
// 핀 설정: 빨강(R) = 9번, 노랑(Y) = 10번, 초록(G) = 11번
// 동작: 빨강 -> 노랑 -> 초록 순서로 0.2초(200ms)씩 차례대로 켜졌다 꺼지는 물결 효과 반복

const int RED_PIN = 9;     // 빨간색(R) LED 핀
const int YELLOW_PIN = 10; // 노란색(Y) LED 핀
const int GREEN_PIN = 11;  // 초록색(G) LED 핀

const int DELAY_TIME = 200; // 간격: 0.2초 (200ms)

void setup() {
  // 시리얼 통신 시작 (9600 baud rate)
  Serial.begin(9600);

  // 모든 LED 핀을 출력(OUTPUT)으로 설정
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  // 시작 시 모든 LED 끄기
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);

  Serial.println("물결(Wave) 효과 시작 (빨강 -> 노랑 -> 초록)");
}

void loop() {
  // 1. 빨간색 LED 켜졌다 꺼짐
  digitalWrite(RED_PIN, HIGH);
  delay(DELAY_TIME);
  digitalWrite(RED_PIN, LOW);

  // 2. 노란색 LED 켜졌다 꺼짐
  digitalWrite(YELLOW_PIN, HIGH);
  delay(DELAY_TIME);
  digitalWrite(YELLOW_PIN, LOW);

  // 3. 초록색 LED 켜졌다 꺼짐
  digitalWrite(GREEN_PIN, HIGH);
  delay(DELAY_TIME);
  digitalWrite(GREEN_PIN, LOW);
}
