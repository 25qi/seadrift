int gpsworktime = 10000; //milseconds 000
#include <TinyGPS++.h>//GPS模組S
#include <AltSoftSerial.h>
TinyGPSPlus gps;
AltSoftSerial GPSss;//GPS模組，Pin8為RX，接GPS的TXD；Pin9為TX，接GPS的RXD
//////////////////////////////////////////////////////////////////////
struct structgps {
  double latitude;
  double longitude;
  unsigned long gpsdate;
  unsigned long gpstime;
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
  while ( millis() - start_time < gpsworktime ) {
    while (GPSss.available() > 0) {
      if (gps.encode(GPSss.read())) {
        if (gps.location.isValid()) {
          gpsdata.latitude = gps.location.lat();
          gpsdata.longitude = gps.location.lng();
        }
        if (gps.date.isValid() && gps.time.isValid()) {
          gpsdata.gpstime = gps.time.value();
          gpsdata.gpsdate = gps.date.value();
        }
      }
    }
    delay(10);

    if ( gps.location.isValid() && gps.date.isValid() && gps.time.isValid()) {
      work_blueled();
      if (gpsdata.gpsdate != 0) {
        //這裡再看要怎麼寫
        buzzergo(1);
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
//  Serial.print("Date: ");
//  Serial.print(gpsdata.gpsdate);
//  Serial.print("  Time: ");
//  Serial.print(gpsdata.gpstime);

  addToQueue(gpsdata.latitude*100, 29);
  addToQueue(gpsdata.longitude*100, 29);
  delay(1000);
}
