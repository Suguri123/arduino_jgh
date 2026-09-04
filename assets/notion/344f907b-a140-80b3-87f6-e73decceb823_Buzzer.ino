int read;

void setup()
{
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  tone(8, 523, 1000); //8번핀에서 도 음 출력(C5 = 523Hz)
  delay(1000); 

}

void loop()
{
  delay(10); 
}



