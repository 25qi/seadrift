#include <DS3232RTC.h>
#include <Wire.h> 
int led = LED_BUILTIN; 
DS3232RTC rtc;
void setup() {
  Serial.begin(115200);
  Wire.begin();
  pinMode(led, OUTPUT);
}

void loop() {
  
  digitalWrite(led, HIGH);
  Serial.println("led on 3 sec");
  delay(3000);
  reset_alarm(); //放最後一行，會睡著
}

void reset_alarm(){
  
  Wire.beginTransmission(0x68);
  Serial.println("reset alarm 5 sec");
  Wire.write(1 << 6);
  Wire.endTransmission();
  rtc.alarmInterrupt(DS3232RTC::ALARM_1, true); // alarm is an output trigger
  rtc.squareWave(DS3232RTC::SQWAVE_NONE);
  //      0h 0m 0s 
  setTime(0, 0, 0, 1, 1, 1970); 
  rtc.set(now());
  // set new alarm
  rtc.setAlarm(DS3232RTC::ALM1_MATCH_MINUTES, 0, 1, 0, 1);//改成半小時的秒數
  // clear old alarm flag - turning off system
  rtc.alarm(DS3232RTC::ALARM_1);
}
