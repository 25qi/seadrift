#include <DS3232RTC.h> //RTC's lib
#include <Wire.h>
#include "I2Cdev.h" //   
#include "MPU6050.h" //  
#include "HMC5883L.h" // 
#include <LiquidCrystal_I2C.h> //lcd's lib 
#include <SoftwareSerial.h> 

SoftwareSerial BT(10, 11); 
DS3232RTC rtc; //宣告RTC
LiquidCrystal_I2C lcd(0x27, 20, 4); //宣告LCD
MPU6050 mpu; //宣告
HMC5883L mag; //宣告
int led = LED_BUILTIN;  // 用內建LED燈
int16_t mx, my, mz; //初始化Compass
float declination = (-4.0 - (54.0 / 60.0)) * (PI / 180); //TAIPEI

void TCA9548A(uint8_t bus) { //調整MUX現在要處理誰的函式
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
  //Serial.print(bus);
}

void setup() {
  delay(2000); //預留時間，等待開機電源供應穩定
  Serial.begin(115200);
  BT.begin(115200);BT.listen();//hc05藍牙
  Wire.begin();
  TCA9548A(1);//切換至LCD
  lcd.init(); //初始化
  lcd.backlight(); //背板發亮
  lcd.setCursor(1, 0); //從第一行第0個格子開始
  lcd.print("Hello World!");
  Serial.println("Hello World");
  BT.println("Hello World");
  delay(1000);
  lcd.clear();
  lcd.print("Initializing...");
  Serial.println("Initializing...");
  BT.println("Initializing");
  TCA9548A(0);//切換至GY86
  mpu.setI2CMasterModeEnabled(false);
  mpu.setI2CBypassEnabled(true) ;
  mpu.setSleepEnabled(false);
  mpu.initialize();
  mag.initialize();
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");
  BT.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");
  pinMode(led, OUTPUT); //設定led的腳為輸出
}

void loop() {
  digitalWrite(led, HIGH); //當開始運作，led燈亮
  TCA9548A(1); //切換至lcd
  lcd.init();
  lcd.clear();
  lcd.print("Working");
  Serial.println("Working");
  BT.println("Working");

  TCA9548A(0);//切換至GY86
  mag.getHeading(&mx, &my, &mz);
  printResults();
  delay(200);

  TCA9548A(1); //切換回lcd
  lcd.init();
  lcd.clear();
  lcd.print("It's time"); //做完事情，準備睡覺
  lcd.setCursor(6, 1);
  lcd.print("to sleep");
  Serial.println("It's time to sleep");
  BT.println("It's time to sleep");

  TCA9548A(2);//切換至RTC
  delay(1000);
  reset_alarm();//設定起床鬧鐘
}

void reset_alarm() { //起床鬧鐘
  TCA9548A(1); //切換至lcd
  lcd.clear();
  lcd.print("reset alarm 5 sec");
  Serial.println("reset alarm 5 sec");
  BT.println("reset alarm 5 sec");

  TCA9548A(2);//RTC
  Wire.beginTransmission(0x68);
  Wire.write(1 << 6);
  Wire.endTransmission();
  rtc.alarmInterrupt(DS3232RTC::ALARM_1, true);
  rtc.squareWave(DS3232RTC::SQWAVE_NONE);
  //      0h 0m 0s
  setTime(0, 0, 0, 1, 1, 1970);
  rtc.set(now());
  // set new alarm
  rtc.setAlarm(DS3232RTC::ALM1_MATCH_SECONDS, 5, 0, 0, 1); //可以調整睡多久
  // clear old alarm flag - turning off system
  rtc.alarm(DS3232RTC::ALARM_1);

}
void printResults() {
  float heading = atan2(my, mx);
  heading += declination;
  if (heading < 0) heading += 2 * PI;
  if (heading > 2 * PI) heading -= 2 * PI;
  heading *= 180 / M_PI;
  TCA9548A(1);
  lcd.clear();
  lcd.print(heading);
  Serial.print("Heading °:");
  Serial.println(heading);
  BT.print("Heading:");BT.println(heading);
}
