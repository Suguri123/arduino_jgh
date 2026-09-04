void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);  // 버튼 입력 (내부 풀업 저항 사용)
}

void loop()
{
  int button = digitalRead(2);  // 버튼 상태 읽기
  Serial.println(button);       // 시리얼 출력
  delay(100);  // 노이즈 방지를 위한 짧은 딜레이
}