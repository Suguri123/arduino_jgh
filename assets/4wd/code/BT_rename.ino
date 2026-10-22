/* BT_rename.ino - 블루투스 모듈 이름 바꾸기 (HC-06 / HM-10 / BT05 공용)
 *
 * 한 교실에서 30대가 함께 쓰면 폰에서 같은 이름이 30개 보여 자기 차를 찾을 수 없습니다.
 * 아래 NEW_NAME 을 자기 번호로 바꾸고 업로드하면 자동으로 AT 명령을 보내 이름을 바꿉니다.
 *
 * 사용 순서
 *   1. 모듈을 쉴드에 연결 (VCC->V3, GND->G3, TXD->S3, RXD->S4) 하고 배터리는 끕니다.
 *   2. 폰과 연결되어 있지 않은 상태(모듈 LED 깜빡임)여야 AT 명령이 통합니다.
 *   3. NEW_NAME 수정 -> 업로드 -> 시리얼 모니터(9600) 를 엽니다.
 *   4. "OK" 응답이 보이면 모듈 전원을 껐다 켠 뒤 폰에서 새 이름을 검색합니다.
 *
 * 업로드 후에는 시리얼 모니터에 입력한 글자가 모듈로 그대로 전달되므로
 * AT 명령을 직접 입력해 볼 수도 있습니다. (줄바꿈 설정은 아래 표 참고)
 */

#include <SoftwareSerial.h>

// ===================== 여기만 바꾸세요 =====================
const char* NEW_NAME = "JGH01";        // 자기 번호로! 예) JGH01 ~ JGH30 (영문/숫자, 12자 이내)
// ==========================================================

#define BT_RXD 3                        // 아두이노 3번핀 = RX (모듈 TXD 연결)
#define BT_TXD 4                        // 아두이노 4번핀 = TX (모듈 RXD 연결)
SoftwareSerial bluetooth(BT_RXD, BT_TXD);

bool useCRLF = false;                   // 모듈이 줄바꿈(\r\n)을 요구하는지 자동 판별 결과

// AT 명령을 보내고 응답을 1.5초 동안 모아서 돌려줍니다
String sendAT(const char* cmd) {
  while (bluetooth.available()) bluetooth.read();   // 이전 응답 찌꺼기 비우기
  Serial.print(">> ");
  Serial.println(cmd);
  bluetooth.print(cmd);
  if (useCRLF) bluetooth.print("\r\n");

  String resp = "";
  unsigned long t = millis();
  while (millis() - t < 1500) {
    while (bluetooth.available()) {
      char c = bluetooth.read();
      resp += c;
    }
  }
  resp.trim();
  Serial.print("<< ");
  Serial.println(resp.length() ? resp : "(응답 없음)");
  return resp;
}

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  delay(1500);                          // 모듈 부팅 대기
  Serial.println("===== Bluetooth rename =====");
  Serial.print("새 이름: ");
  Serial.println(NEW_NAME);

  // 1) 줄바꿈 없이 AT -> HC-06(구형), 정품 HM-10 은 여기서 OK
  useCRLF = false;
  String r = sendAT("AT");
  if (r.indexOf("OK") < 0) {
    // 2) 줄바꿈 붙여서 AT -> BT05 등 HM-10 호환품, 신형 HC-06 은 여기서 OK
    useCRLF = true;
    r = sendAT("AT");
  }
  if (r.indexOf("OK") < 0) {
    Serial.println("!! 모듈이 응답하지 않습니다. 폰 연결을 끊고(LED 깜빡임), TXD->S3 / RXD->S4 배선을 확인한 뒤 리셋 버튼을 누르세요.");
    return;
  }

  // 3) 이름 변경: AT+NAMExxx 먼저, 안 되면 AT+NAME=xxx 형식으로 재시도
  String cmd = String("AT+NAME") + NEW_NAME;
  r = sendAT(cmd.c_str());
  if (r.indexOf("OK") < 0) {
    cmd = String("AT+NAME=") + NEW_NAME;
    r = sendAT(cmd.c_str());
  }

  if (r.indexOf("OK") >= 0) {
    sendAT("AT+RESET");                 // HM-10 계열은 재시작해야 새 이름이 보입니다 (HC-06 은 무시)
    Serial.println("===== 완료! 모듈 전원을 껐다 켠 뒤 폰에서 새 이름을 검색하세요 =====");
  } else {
    Serial.println("!! 이름 변경 실패. 시리얼 모니터에서 AT 명령을 직접 입력해 보세요.");
  }
}

// 업로드 후에는 시리얼 모니터 <-> 모듈 브리지로 동작합니다 (11_Bluetooth.ino 와 같음)
void loop() {
  if (bluetooth.available()) Serial.write(bluetooth.read());
  if (Serial.available())    bluetooth.write(Serial.read());
}
