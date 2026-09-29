//0820修改的版本
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
//  Serial.print(F("SerialBegin"));
  delay(5000); //預留時間，等待開機電源供應穩定
  pinMode(led, OUTPUT); //設定led的腳為輸出
  pinMode(6, OUTPUT);
  Wire.begin();//Serial.println(F("wireOK"));
  delay(1000);
}

void loop() {
  
  digitalWrite(led, HIGH);//當開始運作，led燈亮
  blueled_work(1);//gps開始
  setup_gps();//Serial.println(F("gpssetupOK"));
  blueled_work(2);
  setup_gy91();//Serial.println(F("gy91setupOK"));
  blueled_work(3);
  setup_iridium();//Serial.println(F("iridiumsetupOK"));
  blueled_work(4);
  delay(1000);
  
  Serial.println(F("gpsStartWork"));gpswork();
  blueled_work(2);
  Serial.println(F("gy91startwork"));gy91work();
  blueled_work(3);//結束，準備send
  Serial.println(F("send data gy91"));send_data_gy91();
  Serial.println(F("send data gps"));send_data_gps();
  //send_data_PowerControl();
  Serial.println(F("queueToText"));queueToText();
  delay(2000);

  Serial.println(F("ready to rest"));
  digitalWrite(led, LOW);
  delay(10000);
}
