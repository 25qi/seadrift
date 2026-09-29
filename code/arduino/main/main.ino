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
  Serial.begin(9600);
  Serial.print("SerialBegin");
  delay(5000); //預留時間，等待開機電源供應穩定
  pinMode(led, OUTPUT); //設定led的腳為輸出
  setup_blueled();Serial.println("blueledOK");
  setup_buzzer();Serial.println("buzzerOK");
  //setup_powerControl();
  Wire.begin();Serial.println("wireOK");
  delay(1000);
  buzzergo(3);//gps開始
  setup_gps();Serial.println("gpssetupOK");
  setup_gy91();Serial.println("gy91setupOK");
  setup_iridium();Serial.println("iridiumsetupOK");
  delay(1000);

  digitalWrite(led, HIGH); //當開始運作，led燈亮
  Serial.println("gpsStartWork");gpswork();
  buzzergo(2);
  Serial.println("gy91startwork");gy91work();
  buzzergo(3);//結束，準備send
  Serial.println("send data gy91");send_data_gy91();
  Serial.println("send data gps");send_data_gps();
  //send_data_PowerControl();
  queueToText();
  delay(2000);
  Serial.println("reset alarm");reset_alarm();
  Serial.println("ready to turn off");turn_off(); //已經移至IridiumSend

}

void loop() {
}
