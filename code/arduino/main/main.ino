//8/5BT&GPS成功！還未加上電源監控模組
//正在測試2022/08/02
//測試不算成功，GPS因雨收不到，還未與BT一起測試成功，有加入蜂鳴片聽聲音
//location收到都是0;
#include <Wire.h>
#include "i2c.h"
///////////////////////////////////////////////////////////////////////
int led = LED_BUILTIN;  // 用內建LED燈
int blueled=6;
//////////////////////////////////////////////////////////////////////
void TCA9548A(uint8_t bus) { //調整MUX現在要處理誰的函式
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}
/////////////////////////////////////////////////////////////////////
#define Do  523
#define Re  587
#define Mi  659
#define Fa  698
#define So  784
#define La  880
#define Si  988
int melody[7] = {Do, Re, Mi, Fa, So, La, Si};
int buzzer = 7;
//////////////////////////////////////////////////////
void setup() {
  delay(5000); //預留時間，等待開機電源供應穩定
  pinMode(led, OUTPUT); //設定led的腳為輸出
  setup_blueled();
  setup_buzzer();
 
  Wire.begin();
  delay(1000);
  setup_bt();
  delay(1000);
  buzzer03();//gps開始
  setup_gps();
  setup_gy91();
}

void loop() {
  digitalWrite(led, HIGH); //當開始運作，led燈亮
  gpswork();
  buzzer02();
  gy91work();
  buzzer03();//結束，準備send  
  send_data_gy91();
  send_data_gps();
  delay(3000);
  reset_alarm();
  turn_off();
}
