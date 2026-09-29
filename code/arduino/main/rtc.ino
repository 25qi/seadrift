int sleeptime = 15; //seconds
#include <DS3232RTC.h> //RTC's lib
//////////////////////////////////////////////////////////////////////
DS3232RTC rtc; //宣告RTC
//////////////////////////////////////////////////////////////////////
void reset_alarm() { //起床鬧鐘
  //sleeptime =get_wait_time_from_voltage();
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
