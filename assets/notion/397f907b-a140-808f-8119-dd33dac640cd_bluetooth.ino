#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2,3); //TX 2, RX 3 번 핀에 연결

void setup(){
  Serial.begin(9600);   //시리얼 통신 설정
  BTSerial.begin(9600); //블루투스 통신 설정
  Serial.println("AT command");
}
 
void loop(){
  if (BTSerial.available()) {
    Serial.write(BTSerial.read());
  }
  if (Serial.available()) {
    BTSerial.write(Serial.read());
  }
}

/* 
시리얼 모니터에서 블루투스 이름 변경
AT+NAME원하는이름
예시)AT+NAMEMYLAMP

시리얼 모니터에서 블루투스 비밀번호 변경
AT+PIN원하는비밀번호
예시)AT+PIN1234

*/