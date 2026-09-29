//https://forum.arduino.cc/t/unlocking-the-gy-91-mpu-9250-bmp-280/701895
//gy91接i2c：a4、a5
#include <FaBo9Axis_MPU9250.h>
#include "i2c_BMP280.h"
#include "I2Cdev.h"
//////////////////////////////////////////////////////////////////////
BMP280 bmp280;
FaBo9Axis mpu;
//////////////////////////////////////////////////////////////////////
struct structgy91 {
  float ax, ay, az;
  float gx, gy, gz;
  //float mx, my, mz;這裡的mx、my、mz(磁力計是錯的，需要回傳數值在本地計算heading)
  float temp;
  float temperature1;
  float pascal;
  float wave_height;
  float meters1, meters2;
} gy91data;
//////////////////////////////////////////////////////////////////////
void setup_gy91() {
  TCA9548A(1);//切換至GY91
  bmp280.initialize();
  bmp280.setEnabled(0);
  bmp280.triggerMeasurement();
  mpu.begin();
  delay(1000);
}
//////////////////////////////////////////////////////////////////////
void gy91work() {
  TCA9548A(1);//切換至GY91
  mpu.readAccelXYZ(&gy91data.ax, &gy91data.ay, &gy91data.az);
  mpu.readGyroXYZ(&gy91data.gx, &gy91data.gy, &gy91data.gz);
  //mpu.readMagnetXYZ(&gy91data.mx, &gy91data.my, &gy91data.mz);地磁不準，要回本地計算
  mpu.readTemperature(&gy91data.temp);
  bmp280.awaitMeasurement();
  bmp280.getPressure(gy91data.pascal);
  bmp280.getTemperature(gy91data.temperature1);
  bmp280.getAltitude(gy91data.meters1);
  gy91data.meters2 = gy91data.meters1 - 20; //有誤差
  bmp280.getPressure(gy91data.pascal);//第一次會不準，要測兩次
  bmp280.triggerMeasurement();

  //以下計算浪高
  float temp_pressure, temp_altitude, min_height, max_height, mid_point;
  unsigned long gy_start_time = millis();
  max_height = gy91data.meters2;
  min_height = gy91data.meters2;
  while (millis() - gy_start_time < 3000) {
    bmp280.getAltitude(temp_altitude);
    if (temp_altitude < min_height) {
      min_height = temp_altitude;
    }
    if (temp_altitude > max_height) {
      max_height = temp_altitude;
    }
  }
  mid_point = (max_height + min_height) / 2.0;
  gy91data.wave_height = (max_height - mid_point) / 2.0;


  delay(1000);
}
//////////////////////////////////////////////////////////////////////
void send_data_gy91() {
  //Serial.print(F("AccX: "));
  Serial.println(gy91data.ax*100);
  //Serial.print(F(" AccY: "));
  Serial.println(gy91data.ay*100);
  //Serial.print(F(" AccZ: "));
  Serial.println(gy91data.az*100);

  //Serial.print(F("GyX: "));
  Serial.println(gy91data.gx*100);
  //Serial.print(F(" GyY: "));
  Serial.println(gy91data.gy*100);
  //Serial.print(F(" GyZ: "));
  Serial.println(gy91data.gz*100);

  //Serial.print(F("Pressure: "));
  Serial.println(gy91data.pascal);
  //Serial.print(F(" Pa; T: "));
  Serial.println(gy91data.temperature1);
  //Serial.println(F(" C"));
    //Serial.print(F("Height: "));
  Serial.println(gy91data.meters2);
  //Serial.print(F(" m; WaveHeight: "));
  Serial.println(gy91data.wave_height);
  //Serial.println(" m");
  delay(1000);

  addToQueue(gy91data.ax*100, 10);//Serial.println(gy91data.ax*100);
  addToQueue(gy91data.ay*100, 10);//Serial.println(gy91data.ay*100);
  addToQueue(gy91data.az*100, 10);//Serial.println(gy91data.az*100);

  addToQueue(gy91data.gx*100, 17);
  addToQueue(gy91data.gy*100, 17);
  addToQueue(gy91data.gz*100, 17);

  addToQueue(gy91data.pascal*100, 25); //改成20位
  addToQueue(gy91data.temperature1*100, 14); 
  addToQueue(gy91data.meters2*100, 18);
  addToQueue(gy91data.wave_height*100, 18); 
}
