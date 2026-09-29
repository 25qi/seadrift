int sleeptime = 5; //seconds
#include <DS3232RTC.h> //RTC's lib
//////////////////////////////////////////////////////////////////////
DS3232RTC rtc; //宣告RTC
//////////////////////////////////////////////////////////////////////
void reset_alarm() { //起床鬧鐘
  //sleeptime =get_wait_time_from_voltage();
  Serial.println(F("0"));
  TCA9548A(0);//RTC
  Wire.beginTransmission(0x68);
  Wire.write(1 << 6);
  Wire.endTransmission();
  rtc.alarmInterrupt(DS3232RTC::ALARM_1, true);
  rtc.squareWave(DS3232RTC::SQWAVE_NONE);
  //      0h 0m 0s
  setTime(0, 0, 0, 1, 1, 1970);
  rtc.set(now());
  // set new alar
  rtc.setAlarm(DS3232RTC::ALM1_MATCH_SECONDS, sleeptime, 0, 0, 1); //可以調整睡多久
  // clear old alarm flag - turning off system
}
//////////////////////////////////////////////////////////////////////
void turn_off() {  //程式有對就會醒來！
  // clear old alarm flag - turning off system
  TCA9548A(0);//RTC
  rtc.alarm(DS3232RTC::ALARM_1);
}

//////////////////////////////////////////////////////////////////////
//以下是電源偵測模組，不好用so先不理
#include <Adafruit_INA219.h>
//Adafruit_INA219 ina219;
//
//struct structina219 {
//  float shuntvoltage=10; // 並聯電壓
//  float busvoltage=10; //負載電壓、工作電壓
//  float loadvoltage=10; //母線電壓
//  float power_mW=10; //功率？
//  float current_mA=10; //現在電流
//} ina219data;
//void setup_powerControl() {
//  TCA9548A(2);
//  ina219.begin();
//  ina219.setCalibration_32V_1A();
//}
//int get_wait_time_from_voltage() {
//  TCA9548A(2);
//  ina219data.shuntvoltage = ina219.getShuntVoltage_mV(); // 並聯電壓
//  ina219data.busvoltage = ina219.getBusVoltage_V();//負載電壓、工作電壓
//  ina219data.loadvoltage = ina219data.busvoltage + (ina219data.shuntvoltage / 1000); //母線電壓
//  ina219data.power_mW = ina219.getPower_mW(); //功率？
//  ina219data.current_mA = ina219.getCurrent_mA(); //現在電流
//  // Samsung 18650 % capacity at a given voltage
//  // batt_voltages            0.0, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.1, 4.2, 4.5
//  // batt_percentages           0,   0,   9,  22,  52,  64,  75,  84,  93, 100, 100
//  if (ina219data.loadvoltage < 3.6) sleeptime = 35;
//  else if (ina219data.loadvoltage < 3.8) sleeptime = 30;
//  else if (ina219data.loadvoltage < 4.0) sleeptime = 20;
//  else if (ina219data.loadvoltage < 4.1) sleeptime = 10;
//  else sleeptime = 2;
//  
//  return sleeptime;
//}
//
//void send_data_PowerControl() {
//    Serial.print("shuntvoltage:  "); Serial.print(ina219data.shuntvoltage); Serial.println(" V");
//      Serial.print("busvoltage:  "); Serial.print(ina219data.busvoltage); Serial.println(" V");
//  Serial.print("Load Voltage:  "); Serial.print(ina219data.loadvoltage); Serial.println(" V");
//  Serial.print("Current:       "); Serial.print(ina219data.current_mA);  Serial.println(" mA");
//  Serial.print("Power:         "); Serial.print(ina219data.power_mW);    Serial.println(" mW");
//  Serial.println();
//  delay(1000);
//}
