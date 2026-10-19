// Arduino Uno 시리얼 통신 기반 LED 제어 프로그램
// 핀 설정: 빨강(R) = 9번, 노랑(Y) = 10번, 초록(G) = 11번
// 통신 속도: 9600 bps
// 동작:
//   '0'           : 모두 끄기
//   '1'           : 빨강 켜기
//   '2'           : 빨강 + 노랑 켜기
//   '3', '4', '5' : 빨강 + 노랑 + 초록 모두 켜기
//   그 외 문자    : 무시
//   '0'~'5' 입력 시 시리얼 모니터에 "받은 숫자: X" 출력

const int RED_PIN = 9;     // 빨간색(R) LED 핀
const int YELLOW_PIN = 10; // 노란색(Y) LED 핀
const int GREEN_PIN = 11;  // 초록색(G) LED 핀

void setup() {
  // 시리얼 통신 시작 (9600 baud rate)
  Serial.begin(9600);

  // 모든 LED 핀을 출력(OUTPUT)으로 설정
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  // 초기 상태: 모든 LED 끄기
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);

  Serial.println("LED 수신기 준비 완료. ('0'~'5' 입력 가능)");
}

void loop() {
  // 수신된 시리얼 데이터가 있는지 확인
  if (Serial.available() > 0) {
    // 한 글자씩 읽기 (parseInt 미사용)
    char c = Serial.read();

    // '0' ~ '5' 사이의 문자인 경우에만 반응
    if (c >= '0' && c <= '5') {
      // 수신된 숫자 확인 출력
      Serial.print("받은 숫자: ");
      Serial.println(c);

      switch (c) {
        case '0':
          // 0: 모두 끄기
          digitalWrite(RED_PIN, LOW);
          digitalWrite(YELLOW_PIN, LOW);
          digitalWrite(GREEN_PIN, LOW);
          break;

        case '1':
          // 1: 빨강만 켜기
          digitalWrite(RED_PIN, HIGH);
          digitalWrite(YELLOW_PIN, LOW);
          digitalWrite(GREEN_PIN, LOW);
          break;

        case '2':
          // 2: 빨강 + 노랑 켜기
          digitalWrite(RED_PIN, HIGH);
          digitalWrite(YELLOW_PIN, HIGH);
          digitalWrite(GREEN_PIN, LOW);
          break;

        case '3':
        case '4':
        case '5':
          // 3, 4, 5: 세 개 모두 켜기
          digitalWrite(RED_PIN, HIGH);
          digitalWrite(YELLOW_PIN, HIGH);
          digitalWrite(GREEN_PIN, HIGH);
          break;
      }
    }
    // '0'~'5' 이외의 문자(엔터키 \r, \n 등)는 별도 처리 없이 무시됨
  }
}
