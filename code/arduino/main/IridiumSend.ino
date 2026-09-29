//////
#define Do  523
#define Re  587
#define Mi  659
#define Fa  698
#define So  784
#define La  880
#define Si  988
int melody[7] = {Do, Re, Mi, Fa, So, La, Si};
int buzzer = 7;
/////////////////////////////////////////////////////////////////////
void setup_blueled() {
  pinMode(blueled, OUTPUT);
}
void work_blueled() {
  digitalWrite(blueled, HIGH);
}
//////////////////////////////////////////////////////////
void setup_buzzer() {
  pinMode(buzzer, OUTPUT);
}
void buzzer01() {
  //gps成功
  for (int i = 0; i < 5; i++) {
    tone(buzzer, melody[i]);
    delay(500);
  }
  noTone(buzzer);
}
void buzzer02() {
  //完成gps
  tone(buzzer, melody[2]);
  delay(1300);
  noTone(buzzer);
}
void buzzer03() {
  //開始和準備send資料
  for (int i = 0; i < 3; i++) {
    tone(buzzer, melody[1]);
    delay(300);
    noTone(buzzer);
    delay(300);
  }
  noTone(buzzer);
}
void buzzer04() {
  //衛星成功
  tone(buzzer, melody[1]);
  delay(1300);
  tone(buzzer, melody[3]);
  delay(1300);
  tone(buzzer, melody[5]);
  noTone(buzzer);
}
void buzzer05() {
  //衛星失敗
  tone(buzzer, melody[6]);
  delay(1300);
  tone(buzzer, melody[3]);
  delay(1300);
  tone(buzzer, melody[1]);
  noTone(buzzer);
  noTone(buzzer);
}



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
