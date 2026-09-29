unsigned long gpstime= 20000; //milseconds 1000=1秒 //gps>420000
//注意資料型別！
#include <TinyGPS++.h>//GPS模組S
#include <AltSoftSerial.h>
TinyGPSPlus gps;
AltSoftSerial GPSss;//GPS模組，Pin8為RX，接GPS的TXD；Pin9為TX，接GPS的RXD
//////////////////////////////////////////////////////////////////////
struct structgps {
  double latitude;
  double longitude;
  //unsigned long gpsdate;
  //unsigned long gpstime;
} gpsdata;
//////////////////////////////////////////////////////////////////////
void setup_gps() {
  GPSss.begin(9600);
  delay(1000);
}
//////////////////////////////////////////////////////////////////////
void gpswork() {
  unsigned long start_time = millis();
  // try for 250 seconds and break if all is valid early
  while ( (millis() - start_time) < gpstime ) {
    //Serial.println( (millis() - start_time) < 60000 );
    //Serial.println((millis() - start_time));
    //types(millis());
    //types(millis() - start_time);
    while (GPSss.available() > 0){
      if (gps.encode(GPSss.read())) {
        if (gps.location.isValid()) {
          gpsdata.latitude = gps.location.lat();
          gpsdata.longitude = gps.location.lng();
        }
        if (gps.date.isValid() && gps.time.isValid()) {
          gps.time.value();
          gps.date.value();
        }
      }
    }
    delay(10);

    if ( gps.location.isValid()) {
      blueled_work(1);
      if (gpsdata.latitude != 0) {
        //這裡再看要怎麼寫
        buzzer01();
        break;
      }
    }
  }
  delay(1000);
}
//////////////////////////////////////////////////////////////////////
void send_data_gps() {
  Serial.print("Location: ");
  Serial.print(gpsdata.latitude, 6); 
  Serial.print(", "); 
  Serial.println(gpsdata.longitude, 6);

  delay(1000);
}
