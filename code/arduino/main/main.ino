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
//  Serial.begin(9600);
//  Serial.print("SerialBegin");
  delay(5000); //預留時間，等待開機電源供應穩定
  pinMode(led, OUTPUT); //設定led的腳為輸出
  setup_blueled();//Serial.println(F("blueledOK"));
  setup_buzzer();//Serial.println(F("buzzerOK"));
  //setup_powerControl();
  Wire.begin();//Serial.println(F("wireOK"));
  delay(1000);
  buzzergo(1);//gps開始
  setup_gps();//Serial.println(F("gpssetupOK"));
  buzzergo(2);
  setup_gy91();//Serial.println(F("gy91setupOK"));
  buzzergo(3);
  setup_iridium();//Serial.println(F("iridiumsetupOK"));
  buzzergo(4);
  delay(1000);

  digitalWrite(led, HIGH); //當開始運作，led燈亮
  gpswork();
  buzzergo(5);
  gy91work();
  buzzergo(6);//結束，準備send
  send_data_gy91();
  send_data_gps();
  //send_data_PowerControl();
  queueToText();
  delay(2000);
  reset_alarm();
  turn_off(); //移至IridiumSend
}

void loop() {
}
