//不確定塑膠蓋子是否會影響訊號接收
#include <TinyGPS++.h>
//include <SoftwareSerial.h>
#include <AltSoftSerial.h>


TinyGPSPlus gps;
//SoftwareSerial ss(4, 3);//pin4是RX，接模組的TXD；pin3是TX，接模組的RXD
AltSoftSerial ss;//GPS模組，Pin8為RX，接GPS的TXD；Pin9為TX，接GPS的RXD

struct dataStruct{
  double latitude;
  double longitude;
  unsigned long date;
  unsigned long time;
}gpsData;


void setup()
{
  Serial.begin(115200);
  ss.begin(9600);
  buzzer_setup();
}

void loop()
{
  buzzer_work();
  while (ss.available() > 0){
    if (gps.encode(ss.read())){
      getInfo();
      printResults();
    }
  }
}

void getInfo(){
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

void printResults(){
  Serial.print("Location: ");
  Serial.print(gpsData.latitude, 6); Serial.print(", "); Serial.print(gpsData.longitude, 6);
  Serial.print("  Date: ");
  Serial.print(gpsData.date);
  Serial.print("  Time: ");
  Serial.print(gpsData.time);
  Serial.println();
}

void buzzer_setup(){
  pinMode(3,OUTPUT);
  }

void buzzer_work(){
    //digitalWrite(3,HIGH);
    tone(3,500);
    //Serial.print("high");
  delay(1000);
  noTone(3);

  delay(2000);
  ;}
