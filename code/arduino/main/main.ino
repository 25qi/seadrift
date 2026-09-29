//0812測試成功
#include <Wire.h>
#include "i2c.h"
///////////////////////////////////////////////////////////////////////
int led = LED_BUILTIN;  // 用內建LED燈
int blueled = 6;
//////////////////////////////////////////////////////////////////////
void TCA9548A(uint8_t bus) { //調整MUX現在要處理誰的函式
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}
//////////////////////////////////////////////////////
void setup() {
  delay(5000); //預留時間，等待開機電源供應穩定
  pinMode(led, OUTPUT); //設定led的腳為輸出
  Wire.begin();
  setup_blueled();
  //setup_buzzer();
  delay(500);
  //setup_powerControl();
  //buzzergo(2);
  blueled_work(2);
  delay(1000);
  setup_bt();
  delay(1000);
  setup_gps();
  setup_gy91();
  
}

void loop() {
  digitalWrite(led, HIGH); //當開始運作，led燈亮
  //buzzergo(3);
  blueled_work(3);
  gpswork();
 // buzzergo(4);
 blueled_work(4);
  gy91work();
  //buzzergo(5);
  blueled_work(5);
  send_data_gy91();
  send_data_gps();
  //send_data_PowerControl();
  delay(2000);
  //buzzer02();
  blueled_work(6);
  reset_alarm();  
  turn_off();
  digitalWrite(led, LOW);
  delay(5000);
}
