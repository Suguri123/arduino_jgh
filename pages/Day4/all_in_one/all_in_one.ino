// =================================================================
// Arduino Uno 5-in-1 통합 펌웨어 (all_in_one.ino)
// 핀 설정: 빨강(R)=9, 노랑(Y)=10, 초록(G)=11 (Uno 하드웨어 PWM 핀)
// 시리얼 속도: 9600 bps
// 논블로킹(millis()) 설계로 언제든 웹에서 모드 전환 즉시 반영!
//
// [모드 명령표]
//   'T' : 1. 신호등 모드 (초록 4s -> 노랑 1s -> 빨강 2s + 1s 깜빡임)
//   'W' : 2. 물결 효과 모드 (0.2s 간격 순차 점등)
//   'B' : 3. 숨쉬기 모드 (PWM analogWrite 호흡 효과)
//   'D' : 4. 불빛 주사위 모드 (1초마다 무작위 1개 점등)
//   '0' : 전체 소등 및 대기(IDLE)
//   '1' : 수신기 - 빨강만 켜기
//   '2' : 수신기 - 빨강+노랑 켜기
//   '3','4','5' : 수신기 - 모두 켜기
// =================================================================

const int RED_PIN = 9;
const int YELLOW_PIN = 10;
const int GREEN_PIN = 11;

// 현재 동작 모드
enum Mode {
  MODE_IDLE,
  MODE_TRAFFIC,
  MODE_WAVE,
  MODE_BREATHE,
  MODE_DICE,
  MODE_RECEIVER
};

Mode currentMode = MODE_IDLE;

// 타이밍 및 상태 관리 변수 (논블로킹 millis)
unsigned long prevMillis = 0;
int subStep = 0;

// 숨쉬기 효과용 변수
int breatheVal = 0;
int breatheDir = 1;

void setup() {
  Serial.begin(9600);

  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  allOff();
  randomSeed(analogRead(A0));

  Serial.println("[ALL_IN_ONE] 5-in-1 통합 펌웨어가 준비되었습니다.");
  Serial.println("[ALL_IN_ONE] 명령 대기 중: T(신호등), W(물결), B(숨쉬기), D(주사위), 0~5(수신기)");
}

void loop() {
  // 1. 시리얼 명령 수신 및 모드 즉시 전환
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    // 줄바꿈 문자 무시
    if (cmd != '\r' && cmd != '\n') {
      handleCommand(cmd);
    }
  }

  // 2. 현재 모드에 따른 동작 실행
  unsigned long currentMillis = millis();

  switch (currentMode) {
    case MODE_TRAFFIC:
      runTrafficMode(currentMillis);
      break;

    case MODE_WAVE:
      runWaveMode(currentMillis);
      break;

    case MODE_BREATHE:
      runBreatheMode(currentMillis);
      break;

    case MODE_DICE:
      runDiceMode(currentMillis);
      break;

    case MODE_RECEIVER:
    case MODE_IDLE:
    default:
      // 수신 모드 및 대기 상태는 이벤트 기반이므로 대기
      break;
  }
}

// -------------------------------------------------------------
// 명령 처리기
// -------------------------------------------------------------
void handleCommand(char cmd) {
  // 명령 에코 출력
  Serial.print("받은 명령: ");
  Serial.println(cmd);

  prevMillis = millis();
  subStep = 0;

  switch (cmd) {
    case 'T':
    case 't':
      currentMode = MODE_TRAFFIC;
      Serial.println("[모드] 🚦 신호등 시작");
      setTrafficStep(0);
      break;

    case 'W':
    case 'w':
      currentMode = MODE_WAVE;
      Serial.println("[모드] 🌊 물결 효과 시작");
      setLeds(HIGH, LOW, LOW);
      break;

    case 'B':
    case 'b':
      currentMode = MODE_BREATHE;
      breatheVal = 0;
      breatheDir = 1;
      Serial.println("[모드] 🫁 숨쉬기 효과 시작");
      break;

    case 'D':
    case 'd':
      currentMode = MODE_DICE;
      Serial.println("[모드] 🎲 불빛 주사위 시작");
      rollDice();
      break;

    case '0':
      currentMode = MODE_IDLE;
      allOff();
      Serial.println("[모드] ⏹ 정지 / 모두 끄기");
      break;

    case '1':
      currentMode = MODE_RECEIVER;
      setLeds(HIGH, LOW, LOW);
      Serial.println("[수신기] 빨강 ON");
      break;

    case '2':
      currentMode = MODE_RECEIVER;
      setLeds(HIGH, HIGH, LOW);
      Serial.println("[수신기] 빨강+노랑 ON");
      break;

    case '3':
    case '4':
    case '5':
      currentMode = MODE_RECEIVER;
      setLeds(HIGH, HIGH, HIGH);
      Serial.println("[수신기] 세 개 모두 ON");
      break;

    default:
      // 알 수 없는 명령 무시
      break;
  }
}

// -------------------------------------------------------------
// 1. 신호등 모드 (초록 4s -> 노랑 1s -> 빨강 2s 점등 + 1s 깜빡임)
// -------------------------------------------------------------
void setTrafficStep(int step) {
  subStep = step;
  prevMillis = millis();

  if (subStep == 0) {
    // 초록불 ON (4초)
    Serial.println("초록불 ON (4초)");
    setLeds(LOW, LOW, HIGH);
  } else if (subStep == 1) {
    // 노랑불 ON (1초)
    Serial.println("노랑불 ON (1초)");
    setLeds(LOW, HIGH, LOW);
  } else if (subStep == 2) {
    // 빨강불 2초 지속 점등
    Serial.println("빨강불 ON (2초)");
    setLeds(HIGH, LOW, LOW);
  } else if (subStep == 3) {
    // 빨강불 1초 고속 깜빡임 시작
    Serial.println("빨강불 깜빡임 (마지막 1초)");
  }
}

void runTrafficMode(unsigned long currentMillis) {
  unsigned long elapsed = currentMillis - prevMillis;

  if (subStep == 0 && elapsed >= 4000) {
    setTrafficStep(1); // 노랑불로 전환
  } else if (subStep == 1 && elapsed >= 1000) {
    setTrafficStep(2); // 빨강불 점등으로 전환
  } else if (subStep == 2 && elapsed >= 2000) {
    setTrafficStep(3); // 빨강불 깜빡임으로 전환
  } else if (subStep == 3) {
    // 1초 동안 100ms마다 깜빡임 (총 1000ms)
    if (elapsed >= 1000) {
      setTrafficStep(0); // 다시 초록불로 순환
    } else {
      bool blinkState = ((elapsed / 100) % 2) == 1;
      digitalWrite(RED_PIN, blinkState ? HIGH : LOW);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
    }
  }
}

// -------------------------------------------------------------
// 2. 물결 효과 모드 (0.2초마다 빨강 -> 노랑 -> 초록 순차 점등)
// -------------------------------------------------------------
void runWaveMode(unsigned long currentMillis) {
  if (currentMillis - prevMillis >= 200) {
    prevMillis = currentMillis;
    subStep = (subStep + 1) % 3;

    if (subStep == 0) setLeds(HIGH, LOW, LOW);       // 빨강
    else if (subStep == 1) setLeds(LOW, HIGH, LOW);  // 노랑
    else if (subStep == 2) setLeds(LOW, LOW, HIGH);  // 초록
  }
}

// -------------------------------------------------------------
// 3. 숨쉬기 모드 (analogWrite PWM 0~255 호흡)
// -------------------------------------------------------------
void runBreatheMode(unsigned long currentMillis) {
  if (currentMillis - prevMillis >= 10) {
    prevMillis = currentMillis;

    analogWrite(RED_PIN, breatheVal);
    analogWrite(YELLOW_PIN, breatheVal);
    analogWrite(GREEN_PIN, breatheVal);

    breatheVal += breatheDir;
    if (breatheVal >= 255) {
      breatheVal = 255;
      breatheDir = -1;
    } else if (breatheVal <= 0) {
      breatheVal = 0;
      breatheDir = 1;
    }
  }
}

// -------------------------------------------------------------
// 4. 불빛 주사위 모드 (1초마다 무작위 1개 점등)
// -------------------------------------------------------------
void rollDice() {
  int r = random(0, 3);
  setLeds(r == 0 ? HIGH : LOW, r == 1 ? HIGH : LOW, r == 2 ? HIGH : LOW);

  if (r == 0) Serial.println("주사위 결과: 빨강 (RED)");
  else if (r == 1) Serial.println("주사위 결과: 노랑 (YELLOW)");
  else Serial.println("주사위 결과: 초록 (GREEN)");
}

void runDiceMode(unsigned long currentMillis) {
  if (currentMillis - prevMillis >= 1000) {
    prevMillis = currentMillis;
    rollDice();
  }
}

// -------------------------------------------------------------
// 보조 헬퍼 함수
// -------------------------------------------------------------
void setLeds(int r, int y, int g) {
  digitalWrite(RED_PIN, r);
  digitalWrite(YELLOW_PIN, y);
  digitalWrite(GREEN_PIN, g);
}

void allOff() {
  analogWrite(RED_PIN, 0);
  analogWrite(YELLOW_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
}
