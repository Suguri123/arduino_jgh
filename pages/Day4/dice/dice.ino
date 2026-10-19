// Arduino Uno 신호등 모듈을 이용한 '불빛 주사위' 프로그램
// 핀 설정: 빨강(R) = 9번, 노랑(Y) = 10번, 초록(G) = 11번
// 동작: 1초마다 세 LED 중 하나만 무작위(Random)로 켜짐

const int RED_PIN = 9;     // 빨간색(R) LED 핀
const int YELLOW_PIN = 10; // 노란색(Y) LED 핀
const int GREEN_PIN = 11;  // 초록색(G) LED 핀

// LED 핀 및 이름 배열
const int ledPins[3] = {RED_PIN, YELLOW_PIN, GREEN_PIN};
const char* ledNames[3] = {"빨강 (RED)", "노랑 (YELLOW)", "초록 (GREEN)"};

void setup() {
  // 시리얼 통신 시작 (9600 baud rate)
  Serial.begin(9600);

  // 모든 LED 핀을 출력(OUTPUT) 모드로 설정하고 초기 상태로 끔
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // 연결되지 않은 아날로그 핀(A0)의 전기적 노이즈를 읽어 난수 시드 초기화
  randomSeed(analogRead(A0));

  Serial.println("불빛 주사위를 시작합니다!");
}

void loop() {
  // 0, 1, 2 중 하나를 무작위로 선택
  int chosen = random(0, 3);

  // 선택된 LED만 켜고 나머지는 끔 (한 번에 하나만 점등)
  for (int i = 0; i < 3; i++) {
    if (i == chosen) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }

  // 시리얼 모니터에 결과 출력
  Serial.print("주사위 결과: ");
  Serial.println(ledNames[chosen]);

  // 1초 동안 유지
  delay(1000);
}
