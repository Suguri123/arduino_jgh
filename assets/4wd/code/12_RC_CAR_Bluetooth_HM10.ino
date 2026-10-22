/* 12_RC_CAR_Bluetooth.ino (에듀이노 SmartCar 기본예제 8) 를 바탕으로
 * HM-10(BLE) + 스마트폰 웹 조종기 실습용으로 수정한 버전입니다.
 *
 * 웹 조종기(https://suguri123.github.io/RCcar_blu_jhg/)는 숫자 문자를 보냅니다.
 *   '1' 전진   '2' 후진   '3' 우회전   '4' 좌회전   '5' 정지 (버튼을 놓으면 자동 전송)
 *   '6' '7' 은 라인트레이싱용 명령이므로 이 코드에서는 무시합니다.
 *
 * 원본의 문자 명령도 그대로 동작합니다.
 *   'g' 전진   'b' 후진   'l' 좌회전   'r' 우회전   's' 정지   'q' 제자리 좌회전   'w' 제자리 우회전
 *
 * 배선 (센서 쉴드 V5.0)
 *   HM-10 VCC -> V3 (5V)      HM-10 GND -> G3
 *   HM-10 TXD -> S3 (D3, 아두이노 RX)
 *   HM-10 RXD -> S4 (D4, 아두이노 TX)
 */

#include <SoftwareSerial.h>                            // 가상 시리얼 통신을 위한 라이브러리 선언

#define BT_RXD 3                                       // 아두이노 3번핀 = RX (모듈 TXD 연결)
#define BT_TXD 4                                       // 아두이노 4번핀 = TX (모듈 RXD 연결)

SoftwareSerial bluetooth(BT_RXD, BT_TXD);              // 블루투스 통신을 위한 설정

int RightMotor_E_pin = 5;                              // 오른쪽 모터의 Enable & PWM
int LeftMotor_E_pin = 6;                               // 왼쪽 모터의 Enable & PWM
int RightMotor_1_pin = 8;                              // 오른쪽 모터 제어선 IN1
int RightMotor_2_pin = 9;                              // 오른쪽 모터 제어선 IN2
int LeftMotor_3_pin = 10;                              // 왼쪽 모터 제어선 IN3
int LeftMotor_4_pin = 11;                              // 왼쪽 모터 제어선 IN4

int L_MotorSpeed = 185;                                // 왼쪽 모터 속도
int R_MotorSpeed = 160;                                // 오른쪽 모터 속도

int R_Motor = HIGH;                                    // 시작 방향은 전진으로 둡니다
int L_Motor = HIGH;
int mode = 3;                                          // 시작 모드 3 = 정지

void setup() {
  pinMode(RightMotor_E_pin, OUTPUT);                   // 출력모드로 설정
  pinMode(RightMotor_1_pin, OUTPUT);
  pinMode(RightMotor_2_pin, OUTPUT);
  pinMode(LeftMotor_3_pin, OUTPUT);
  pinMode(LeftMotor_4_pin, OUTPUT);
  pinMode(LeftMotor_E_pin, OUTPUT);

  Serial.begin(9600);
  bluetooth.begin(9600);

  Serial.println("HM-10 RC Car Ready (1:Go 2:Back 3:Right 4:Left 5:Stop)");
}

void loop() {
  if (bluetooth.available()) {
    char Blue_Val = bluetooth.read();
    Serial.print(Blue_Val);                            // 수신 문자를 시리얼 모니터에 표시
    control_SmartCar(Blue_Val);

    if (mode == 0) {
      motor_role(R_Motor, L_Motor);
    }
    else if (mode == 1) {
      Right_role(R_Motor, L_Motor);
    }
    else if (mode == 2) {
      Left_role(R_Motor, L_Motor);
    }
    else if (mode == 4) {
      left_rotation(R_Motor, L_Motor);
    }
    else if (mode == 5) {
      right_rotation(R_Motor, L_Motor);
    }
    else {
      analogWrite(RightMotor_E_pin, 0);
      analogWrite(LeftMotor_E_pin, 0);
    }
  }
}

void control_SmartCar(char Blue_val) {
  if (Blue_val == 'g' || Blue_val == '1') {            // 전진
    R_Motor = HIGH; L_Motor = HIGH; mode = 0;
  }
  else if (Blue_val == 'b' || Blue_val == '2') {       // 후진
    R_Motor = LOW; L_Motor = LOW; mode = 0;
  }
  else if (Blue_val == '3') {                          // 웹 조종기 우회전 (전진 방향으로 회전)
    R_Motor = HIGH; L_Motor = HIGH; mode = 1;
  }
  else if (Blue_val == '4') {                          // 웹 조종기 좌회전 (전진 방향으로 회전)
    R_Motor = HIGH; L_Motor = HIGH; mode = 2;
  }
  else if (Blue_val == 'r') {                          // 원본 우회전 (직전 전진/후진 방향 유지)
    mode = 1;
  }
  else if (Blue_val == 'l') {                          // 원본 좌회전 (직전 전진/후진 방향 유지)
    mode = 2;
  }
  else if (Blue_val == 's' || Blue_val == '5') {       // 정지
    R_Motor = HIGH; L_Motor = HIGH; mode = 3;
  }
  else if (Blue_val == 'q') {                          // 제자리 좌회전
    mode = 4;
  }
  else if (Blue_val == 'w') {                          // 제자리 우회전
    mode = 5;
  }
  else {                                               // '6', '7', 줄바꿈 등은 무시
  }
}

void motor_role(int R_motor, int L_motor) {
  digitalWrite(RightMotor_1_pin, R_motor);
  digitalWrite(RightMotor_2_pin, !R_motor);
  digitalWrite(LeftMotor_3_pin, L_motor);
  digitalWrite(LeftMotor_4_pin, !L_motor);

  analogWrite(RightMotor_E_pin, R_MotorSpeed);         // 우측 모터 속도값
  analogWrite(LeftMotor_E_pin, L_MotorSpeed);          // 좌측 모터 속도값
}

void Right_role(int R_motor, int L_motor) {
  digitalWrite(RightMotor_1_pin, R_motor);
  digitalWrite(RightMotor_2_pin, !R_motor);
  digitalWrite(LeftMotor_3_pin, L_motor);
  digitalWrite(LeftMotor_4_pin, !L_motor);

  analogWrite(RightMotor_E_pin, max(R_MotorSpeed * 0.4, 90));   // 우측 모터 속도값
  analogWrite(LeftMotor_E_pin, 255);                             // 좌측 모터 속도값
}

void Left_role(int R_motor, int L_motor) {
  digitalWrite(RightMotor_1_pin, R_motor);
  digitalWrite(RightMotor_2_pin, !R_motor);
  digitalWrite(LeftMotor_3_pin, L_motor);
  digitalWrite(LeftMotor_4_pin, !L_motor);

  analogWrite(RightMotor_E_pin, 255);                            // 우측 모터 속도값
  analogWrite(LeftMotor_E_pin, max(L_MotorSpeed * 0.4, 90));    // 좌측 모터 속도값
}

void left_rotation(int R_motor, int L_motor) {
  digitalWrite(RightMotor_1_pin, HIGH);
  digitalWrite(RightMotor_2_pin, LOW);
  digitalWrite(LeftMotor_3_pin, LOW);
  digitalWrite(LeftMotor_4_pin, HIGH);

  analogWrite(RightMotor_E_pin, R_MotorSpeed);         // 우측 모터 속도값
  analogWrite(LeftMotor_E_pin, L_MotorSpeed);          // 좌측 모터 속도값
}

void right_rotation(int R_motor, int L_motor) {
  digitalWrite(RightMotor_1_pin, LOW);
  digitalWrite(RightMotor_2_pin, HIGH);
  digitalWrite(LeftMotor_3_pin, HIGH);
  digitalWrite(LeftMotor_4_pin, LOW);

  analogWrite(RightMotor_E_pin, R_MotorSpeed);         // 우측 모터 속도값
  analogWrite(LeftMotor_E_pin, L_MotorSpeed);          // 좌측 모터 속도값
}
