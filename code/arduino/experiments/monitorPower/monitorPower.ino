#include <Wire.h>
#include <Adafruit_INA219.h>// INA219 - Power Monitor
Adafruit_INA219 ina219;
float shuntvoltage = 0; // 並聯電壓
float busvoltage = 0; //母線電壓
float current_mA = 0; //現在電流
float loadvoltage = 0; //負載電壓、工作電壓
float power_mW = 0; //功率？
void setup() {
  TCA9548A(2);
  Serial.begin(115200);
  while (!Serial) {
      delay(1);
  }  // initialise INA219 - Power Monitor
  ina219.begin();
  ina219.setCalibration_32V_1A();
  
}
void TCA9548A(uint8_t bus) { //調整MUX現在要處理誰的函式
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}
void loop() {
  TCA9548A(2);
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  Serial.print("Shunt Voltage:  "); Serial.print(shuntvoltage); Serial.println(" V");
  Serial.print("Bus Voltage:  "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA);  Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW);    Serial.println(" mW");
  Serial.println();
  delay(1000);
}
