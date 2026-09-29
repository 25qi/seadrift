#include <IridiumSBD.h> // Click here to get the library: http://librarymanager/All#IridiumSBDI2C
#include <SoftwareSerial.h>
#define DIAGNOSTICS false // Change this to see diagnostics
SoftwareSerial IridiumSerial (4,5);
IridiumSBD modem(IridiumSerial);

void setup_iridium()
{
  int signalQuality = -1;
  int err;
  Serial.begin(115200);
  while (!Serial);
  IridiumSerial.begin(19200);
//  Serial.println(F("Starting modem..."));
  err = modem.begin();
  if (err != ISBD_SUCCESS)
  {
    buzzer05();
    if (err == ISBD_NO_MODEM_DETECTED)
    buzzer05();
//    return;
  }
  err = modem.clearBuffers(ISBD_CLEAR_MO); // Clear MO buffer
  buzzer04();
}

void iridiumsend()
{
//  Send the message
int err;
  buzzer04();
//  Serial.println(F("Trying to send the message.  This might take several minutes."));
  err = modem.sendSBDBinary(buf, 49);
  if (err != ISBD_SUCCESS)
  {   buzzer05();
    if (err == ISBD_SENDRECEIVE_TIMEOUT)
      buzzer05();
  }

  else
  {
    buzzer04();buzzer04();
    queue="";
    turn_off();
  }
}

#if DIAGNOSTICS
void ISBDConsoleCallback(IridiumSBD *device, char c)
{
  Serial.write(c);
}

void ISBDDiagsCallback(IridiumSBD *device, char c)
{
  Serial.write(c);
}
#endif
