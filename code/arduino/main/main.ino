#include <DS3232RTC.h> //RTC's lib
#include <Wire.h>
#include "I2Cdev.h" //   
#include "MPU6050.h" //  
#include "HMC5883L.h" // 
#include <LiquidCrystal_I2C.h> //lcd's lib 
#include <SoftwareSerial.h> //hc05藍牙和GPS模組
#include <TinyGPS++.h>//GPS模組
TinyGPSPlus gps;
SoftwareSerial BTss(10, 11); //hc05藍牙，Pin10為RX，接HC05的TXD；Pin11為TX，接HC05的RXD
SoftwareSerial GPSss(3, 4);//GPS模組，Pin3為RX，接GPS的TXD；Pin4為TX，接GPS的RXD

int led = LED_BUILTIN;  // 用內建LED燈
DS3232RTC rtc; //宣告RTC
LiquidCrystal_I2C lcd(0x27, 20, 4); //宣告LCD
MPU6050 mpu; //宣告
HMC5883L mag; //宣告

int16_t mx, my, mz; //初始化Compass
float declination = (-4.0 - (54.0 / 60.0)) * (PI / 180); //TAIPEI

//以下是GPS的變數
struct dataStruct{
  double latitude;
  double longitude;
  unsigned long date;
  unsigned long time;
}gpsData;
//以上是GPS的變數

void TCA9548A(uint8_t bus) { //調整MUX現在要處理誰的函式
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
  //Serial.print(bus);
  }

void setup() {
  delay(2000); //預留時間，等待開機電源供應穩定
  Serial.begin(115200);
  BTss.begin(115200);BTss.listen();//hc05藍牙啟動
  GPSss.begin(9600);GPSss.listen();//GPS模組啟動
  Wire.begin();
  
  TCA9548A(1);//切換至LCD
  lcd.init(); //初始化
  lcd.backlight(); //背板發亮
  lcd.setCursor(1, 0); //從第一行第0個格子開始
  lcd.print("Hello World!");
  Serial.println("Hello World");
  BTss.println("Hello World");
  delay(1000);
  lcd.clear();
  lcd.print("Initializing...");
  Serial.println("Initializing...");
  BTss.println("Initializing");
  
  TCA9548A(0);//切換至GY86
  mpu.setI2CMasterModeEnabled(false);
  mpu.setI2CBypassEnabled(true) ;
  mpu.setSleepEnabled(false);
  mpu.initialize();
  mag.initialize();
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");
  BTss.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");
  pinMode(led, OUTPUT); //設定led的腳為輸出
}

void loop() {
  digitalWrite(led, HIGH); //當開始運作，led燈亮
  delay(90000);//延遲一下!
  TCA9548A(1); //切換至lcd
  lcd.init();
  lcd.clear();
  lcd.print("Working");
  Serial.println("Working");
  BTss.println("Working");

  TCA9548A(0);//切換至GY86
  mag.getHeading(&mx, &my, &mz);
  gyprintResults();
  
  GPSss.listen();
  delay(90000);//延遲一下!讓gps抓訊號
  TCA9548A(1);lcd.init();lcd.clear(); //切換回lcd且GPS開始抓資料

    while (GPSss.available() > 0){
    if (gps.encode(GPSss.read())){
      GPSgetInfo();
      GPSprintResults();
    }
  }
  
  
 
  TCA9548A(1); //切換回lcd
  lcd.init();
  lcd.clear();
  lcd.print("It's time"); //做完事情，準備睡覺
  lcd.setCursor(6, 1);
  lcd.print("to sleep");
  Serial.println("It's time to sleep");
  BTss.println("It's time to sleep");

  TCA9548A(2);//切換至RTC
  delay(1000);
  reset_alarm();//設定起床鬧鐘，開始睡覺
  }

void reset_alarm() { //起床鬧鐘
  TCA9548A(1); //切換至lcd
  lcd.clear();
  lcd.print("reset alarm 7 sec");
  Serial.println("reset alarm 7 sec");
  BTss.println("reset alarm 7 sec");

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
  rtc.setAlarm(DS3232RTC::ALM1_MATCH_SECONDS, 7, 0, 0, 1); //可以調整睡多久
  // clear old alarm flag - turning off system
  rtc.alarm(DS3232RTC::ALARM_1);

}

void gyprintResults() {
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
  BTss.print("Heading:");BTss.println(heading);
}


void GPSgetInfo(){
  GPSss.listen();
  if (gps.location.isValid()){
    gpsData.latitude = gps.location.lat();
    gpsData.longitude = gps.location.lng();
  }
  else{
    Serial.println("Invalid location");
  }
  if (gps.date.isValid()){
    gpsData.date = gps.date.value();
  }
  else{
    Serial.println("Invalid date");
  }
  if (gps.time.isValid()){
  gpsData.time = gps.time.value();
  }
  else{
    Serial.println("Invalid time");
  }
}

void GPSprintResults(){
  Serial.print("Location: ");
  Serial.print(gpsData.latitude, 6); Serial.print(", "); Serial.print(gpsData.longitude, 6);
  Serial.print("  Date: ");
  Serial.print(gpsData.date);
  Serial.print("  Time: ");
  Serial.print(gpsData.time);
  Serial.println();

  lcd.print("Location: ");
  lcd.print(gpsData.latitude, 6); Serial.print(", "); Serial.print(gpsData.longitude, 6);
  lcd.print("  Date: ");
  lcd.print(gpsData.date);
  lcd.print("  Time: ");
  lcd.print(gpsData.time);
  lcd.println();

  BTss.listen();
  BTss.print("Location: ");
  BTss.print(gpsData.latitude, 6); Serial.print(", "); Serial.print(gpsData.longitude, 6);
  BTss.print("  Date: ");
  BTss.print(gpsData.date);
  BTss.print("  Time: ");
  BTss.print(gpsData.time);
  BTss.println();
}
