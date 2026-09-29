void setup_blueled() {pinMode(blueled, OUTPUT);}
void work_blueled(){  digitalWrite(blueled,HIGH);}
void setup_buzzer() {pinMode(7, OUTPUT);}
//////////////////////////////////////////////////////////
void buzzergo(int times) {
  //開始和準備send資料
  for (int i = 0; i < times; i++) {
    tone(7, 587);
    delay(300);
    noTone(7);
    delay(300);
  }
  noTone(7);
}
void buzzer02() {
  //準備睡覺
  tone(7, 587);
  delay(1300);
  noTone(7);
}
