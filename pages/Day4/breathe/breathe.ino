// Arduino Uno 신호등 모듈을 이용한 숨쉬기(Breathing LED) 효과 프로그램
// 핀 설정: 빨강(R) = 9번, 노랑(Y) = 10번, 초록(G) = 11번 (Arduino Uno PWM 핀)
// 동작: 세 LED가 analogWrite()를 사용하여 함께 천천히 밝아졌다가 어두워지기를 반복

const int RED_PIN = 9;     // 빨간색(R) LED 핀 (PWM ~9)
const int YELLOW_PIN = 10; // 노란색(Y) LED 핀 (PWM ~10)
const int GREEN_PIN = 11;  // 초록색(G) LED 핀 (PWM ~11)

const int FADE_DELAY = 10; // 밝기 변화 간격 (밀리초 단위, 값이 클수록 더 천천히 변함)

void setup() {
  // 시리얼 통신 시작 (9600 baud rate)
  Serial.begin(9600);

  // 모든 LED 핀을 출력(OUTPUT)으로 설정
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  // 시작 시 소등(0) 상태로 초기화
  analogWrite(RED_PIN, 0);
  analogWrite(YELLOW_PIN, 0);
  analogWrite(GREEN_PIN, 0);

  Serial.println("숨쉬기(Breathing) 효과를 시작합니다.");
}

void loop() {
  // 1. 천천히 밝아지기 (0 -> 255)
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(RED_PIN, brightness);
    analogWrite(YELLOW_PIN, brightness);
    analogWrite(GREEN_PIN, brightness);
    delay(FADE_DELAY);
  }

  // 숨을 들이쉰 상태에서 잠시 멈춤 (0.2초)
  delay(200);

  // 2. 천천히 어두워지기 (255 -> 0)
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(RED_PIN, brightness);
    analogWrite(YELLOW_PIN, brightness);
    analogWrite(GREEN_PIN, brightness);
    delay(FADE_DELAY);
  }

  // 숨을 내쉰 상태에서 잠시 멈춤 (0.3초)
  delay(300);
}
