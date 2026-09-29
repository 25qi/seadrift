void setup_blueled() {pinMode(blueled, OUTPUT);}
void work_blueled(){  digitalWrite(blueled,HIGH);}
void setup_buzzer() {pinMode(7, OUTPUT);}
//////////////////////////////////////////////////////////
void blueled_work(int times){  
  for (int i = 0; i < times; i++) {
  digitalWrite(6,HIGH);
  delay(300);
  digitalWrite(6,LOW);
  delay(180);
  }
}
  
