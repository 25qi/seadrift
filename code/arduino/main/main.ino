//0731修改中
#include <DS3232RTC.h> //RTC's lib
#include <Wire.h>
#include <FaBo9Axis_MPU9250.h>
#include "i2c.h"
#include "i2c_BMP280.h"
#include "I2Cdev.h"
#include <TinyGPS++.h>//GPS模組S
#include <AltSoftSerial.h>
//#include <TinyGPSPlus.h>
//#include <SoftwareSerial.h>

TinyGPSPlus gps;
AltSoftSerial GPSss;//GPS模組，Pin8為RX，接GPS的TXD；Pin9為TX，接GPS的RXD
//SoftwareSerial GPSss(3,4);//GPS模組，Pin3為RX，接GPS的TXD；Pin4為TX，接GPS的RXD

int led = LED_BUILTIN;  // 用內建LED燈
DS3232RTC rtc; //宣告RTC
BMP280 bmp280;
FaBo9Axis fabo_9axis;

//以下是GPS的變數
struct dataStruct {
  double latitude;
  double longitude;
  unsigned long date;
  unsigned long time;
} gpsData;



//以上是GPS的變數

void TCA9548A(uint8_t bus) { //調整MUX現在要處理誰的函式
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}

void setup() {
  delay(5000); //預留時間，等待開機電源供應穩定
  Serial.begin(115200);
  Wire.begin();
  GPSss.begin(9600);//GPS模組啟動


  TCA9548A(1);//切換至GY91
  Serial.println("RESET");
  Serial.println();

  Serial.print("Probe BMP280: ");
  if (bmp280.initialize()) Serial.println("Sensor found");
  else
  {
    Serial.println("Sensor missing");
    while (1) {}
  }

  // onetime-measure:
  bmp280.setEnabled(0);
  bmp280.triggerMeasurement();
  Serial.println("configuring device.");

  if (fabo_9axis.begin()) {
    Serial.println("configured FaBo 9Axis I2C Brick");
  }
  else {
    Serial.println("device error");
    while (1);
  }
  pinMode(led, OUTPUT); //設定led的腳為輸出
}

void loop() {
  digitalWrite(led, HIGH); //當開始運作，led燈亮
  TCA9548A(1);//切換至GY86
  gy91work();
  delay(200000);//延遲一下!讓gps抓訊號
  //Serial.println("finish delay");

  while (GPSss.available() > 0) {
    //Serial.println("in the while");
    if (gps.encode(GPSss.read())) {
      //Serial.println("in the if");
      GPSgetInfo();
      if (x != 0) {
        GPSprintResults();
        break;
      }
      //Serial.println("finish the if");
    }

  }
  Serial.println("time to sleep");
  TCA9548A(0);//切換至RTC
  delay(1000);
  reset_alarm();//設定起床鬧鐘
}

void reset_alarm() { //起床鬧鐘
  TCA9548A(0);//RTC
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
void gy91work() {
  TCA9548A(1);//切換至GY86
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
  float temp;

  fabo_9axis.readAccelXYZ(&ax, &ay, &az);
  fabo_9axis.readGyroXYZ(&gx, &gy, &gz);
  fabo_9axis.readMagnetXYZ(&mx, &my, &mz);
  fabo_9axis.readTemperature(&temp);
  bmp280.awaitMeasurement();

  float temperature;
  bmp280.getTemperature(temperature);

  float pascal;
  bmp280.getPressure(pascal);

  static float meters, metersold;
  bmp280.getAltitude(meters);
  metersold = (meters);

  bmp280.triggerMeasurement();

  Serial.print("AccX: ");
  Serial.print(ax);
  Serial.print(" AccY: ");
  Serial.print(ay);
  Serial.print(" AccZ: ");
  Serial.println(az);

  Serial.print("GyX: ");
  Serial.print(gx);
  Serial.print(" GyY: ");
  Serial.print(gy);
  Serial.print(" GyZ: ");
  Serial.println(gz);

  Serial.print("MagX: ");
  Serial.print(mx);
  Serial.print(" MagY: ");
  Serial.print(my);
  Serial.print(" MagZ: ");
  Serial.println(mz);

  Serial.print("Temp: ");
  Serial.println(temp);

  Serial.print(" HeightPT1: ");
  Serial.print(metersold);
  Serial.print(" m; Height: ");
  Serial.print(meters);
  Serial.print(" Pressure: ");
  Serial.print(pascal);
  Serial.print(" Pa; T: ");
  Serial.print(temperature);
  Serial.println(" C");

  delay(1000);
}

void GPSgetInfo() {
  int x;
  if (gps.location.isValid()) {
    gpsData.latitude = gps.location.lat();
    gpsData.longitude = gps.location.lng();
  }
  else {
    Serial.println("Invalid location");
  }
  if (gps.date.isValid()) {
    gpsData.date = gps.date.value();
  }
  else {
    Serial.println("Invalid date");
  }
  if (gps.time.isValid()) {
    gpsData.time = gps.time.value();
    x = gpsData.time;
  }
  else {
    Serial.println("Invalid time");
  }
}

void GPSprintResults() {
  Serial.print("Location: ");
  Serial.print(gpsData.latitude, 6); Serial.print(", "); Serial.print(gpsData.longitude, 6);
  Serial.print("  Date: ");
  Serial.print(gpsData.date);
  Serial.print("  Time: ");
  Serial.print(gpsData.time);
  Serial.println();
}
