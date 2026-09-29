//https://forum.arduino.cc/t/unlocking-the-gy-91-mpu-9250-bmp-280/701895
//原先的mx、my、mz(磁力計是錯的，需要回傳數值在本地計算heading)
//a4、a5
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
  float mx, my, mz;
//  float temp;
  float temperature1;
  float pascal;
  float meters1;
  float max_height,min_height;
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
  mpu.readMagnetXYZ(&gy91data.mx, &gy91data.my, &gy91data.mz);
//  mpu.readTemperature(&gy91data.temp);
  bmp280.awaitMeasurement();

  bmp280.getTemperature(gy91data.temperature1);

  bmp280.getAltitude(gy91data.meters1);
  bmp280.getPressure(gy91data.pascal);
  bmp280.triggerMeasurement();

//這裡不確定！
  unsigned long gy_start_time = millis();
    gy91data.max_height =gy91data.meters1;
  gy91data.min_height =gy91data.meters1;
  float temp_pressure,temp_altitude,min_height,max_height;
    while(millis() - gy_start_time < 3000){
      
    //temp_pressure = bmp280.getPressure();
    //temp_pressure = bmp280.getPressure(true);
    //bmp280.getPressure(&temp_pressure);
    bmp280.getAltitude(temp_altitude); //pressure應改成當前得到的高度
    if (temp_altitude < min_height) min_height = temp_altitude;
    if (temp_altitude > max_height) max_height = temp_altitude;
  }
  

  delay(1000);
}
//////////////////////////////////////////////////////////////////////
void send_data_gy91(){
    Serial.print("AccX: ");
  Serial.print(gy91data.ax);
  Serial.print(" AccY: ");
  Serial.print(gy91data.ay);
  Serial.print(" AccZ: ");
  Serial.println(gy91data.az);

  Serial.print("GyX: ");
  Serial.print(gy91data.gx);
  Serial.print(" GyY: ");
  Serial.print(gy91data.gy);
  Serial.print(" GyZ: ");
  Serial.println(gy91data.gz);

  Serial.print("MagX: ");
  Serial.print(gy91data.mx);
  Serial.print(" MagY: ");
  Serial.print(gy91data.my);
  Serial.print(" MagZ: ");
  Serial.println(gy91data.mz);

//  Serial.print("Temp: ");
//  Serial.println(gy91data.temp);

  Serial.print(" m; Height: ");
  Serial.print(gy91data.meters1);
  Serial.print(" Pressure: ");
  Serial.print(gy91data.pascal,1);//保留小數點後1位
  Serial.print(" Pa; T: ");
  Serial.print(gy91data.temperature1);
  Serial.println(" C");

  delay(1000);
  }
