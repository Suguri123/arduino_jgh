void setup() {
  Serial.begin(9600);    // 시리얼 통신 속도 설정 (9600bps)
  pinMode(6, OUTPUT);    // Trig 핀 (초음파 발사) → 출력 설정
  pinMode(5, INPUT);     // Echo 핀 (초음파 수신) → 입력 설정
}

void loop() {
  // 초음파 발사
  digitalWrite(6, LOW);   // Trig 핀 초기화
  delayMicroseconds(2);
  digitalWrite(6, HIGH);  // Trig 핀 HIGH → 초음파 발사
  delayMicroseconds(10);
  digitalWrite(6, LOW);   // 트리거 신호 종료

  // Echo 핀으로부터 초음파 반사 시간 측정
  float duration = pulseIn(5, HIGH);  

  // 거리 계산 (단위: cm)
  float distance = ((340.0 * duration) / 2.0) / 10000.0;

  // 시리얼 모니터에 거리 출력
  Serial.println(distance);

  delay(500); // 0.5초마다 반복
}


