void setup()
{
  pinMode(7, OUTPUT);  // 빨간 LED
  pinMode(8, OUTPUT);  // 노란 LED
  pinMode(9, OUTPUT);  // 초록 LED
}

void loop()
{
  // 7번 LED ON, 나머지 OFF
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  delay(1000);

  // 8번 LED ON, 나머지 OFF
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  delay(1000);

  // 9번 LED ON, 나머지 OFF
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  delay(1000);
}