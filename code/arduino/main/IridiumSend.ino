#include <IridiumSBD.h> // Click here to get the library: http://librarymanager/All#IridiumSBDI2C
#include <SoftwareSerial.h>
//#define DIAGNOSTICS false // Change this to see diagnostics
SoftwareSerial IridiumSerial (4,5);
IridiumSBD modem(IridiumSerial);

void setup_iridium()
{
  int signalQuality = -1;
  int err;
//  Serial.begin(115200);
  while (!Serial);
  IridiumSerial.begin(19200);
  Serial.println(F("Starting modem..."));
  err = modem.begin();
  if (err != ISBD_SUCCESS)
  {
    Serial.print(F("Begin failed "));
    Serial.println(err);
    if (err == ISBD_NO_MODEM_DETECTED)
    Serial.println(F("No modem detected"));
//    return;
  }
  err = modem.clearBuffers(ISBD_CLEAR_MO); // Clear MO buffer
}

void iridiumsend()
{
int err;
  Serial.println(F("Trying to send the message..."));
  err = modem.sendSBDBinary(buf, 50);
  if (err != ISBD_SUCCESS)
  {   Serial.println("ISBDNOTSUCCESS");
    if (err == ISBD_SENDRECEIVE_TIMEOUT){Serial.println("IridiumTimeout");}
  }

  else
  {
    queue="";
    turn_off();
    Serial.println("SendSUCCESS");
  }
}

//#if DIAGNOSTICS
//void ISBDConsoleCallback(IridiumSBD *device, char c)
//{
//  Serial.write(c);
//}
//
//void ISBDDiagsCallback(IridiumSBD *device, char c)
//{
//  Serial.write(c);
//}
//#endif
