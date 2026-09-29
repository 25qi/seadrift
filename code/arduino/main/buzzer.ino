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
void work_blueled(){  
  digitalWrite(blueled,HIGH);
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
