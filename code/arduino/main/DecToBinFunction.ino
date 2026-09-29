String queue;
uint8_t buf[30];
int emptybuf=0;
void addToQueue(long decnum, int digit){
  String bin;
  if(decnum<0){bin = String(decnum*-1,BIN);}else{bin = String(decnum,BIN);} 
  while(bin.length()<digit-1){bin=String("0"+bin);
//  Serial.println(bin.length());
  } //補0直到指定位數
  if(decnum<0){bin=String("1"+bin);}else{bin=String("0"+bin);} //負則開頭為1，正則開頭為0
  queue = queue + bin; 
//  Serial.print(F("傳送序列已增加「"));Serial.print(bin);Serial.println("」"); 
  //Serial.print(F("傳送序列現在為「"));Serial.print(queue);Serial.println("」");
   
  while(queue.length()>7){
//    for(int counter=0;queue.length()>7;counter+=1){
    buf[emptybuf]=0b00000000;
      for(int digit=8;digit>0;digit-=1){
        String zeroorone;
        zeroorone = queue.charAt(0);//永遠寫入第0位
        bitWrite(buf[emptybuf], digit-1, zeroorone.toInt());//從左到右寫入，7到0（digit是8到1）
        queue.remove(0,1);
        }
//    Serial.print(F("buf"));Serial.print(emptybuf);Serial.print(F("已新增為"));Serial.println(buf[emptybuf]);
//    Serial.print("傳送序列現在為「");Serial.print(queue);Serial.println("」");
    emptybuf+=1;
//    counter+=1; }
  }
  
}

void queueToText(){
  String bin=queue;String zeroorone;
  
  while(bin.length()%8!=0){bin+="0"; }
//  Serial.print(F("bin已被擴寫為「"));Serial.print(bin);Serial.println("」");
  for(int counter=0;counter<(bin.length()/8);counter=counter+1){
    buf[emptybuf]=0b00000000;
    
    for(int digit=8;digit>0;digit-=1){
      zeroorone = bin.charAt(counter*8+digit-1);
      bitWrite(buf[emptybuf], 8-digit, zeroorone.toInt());
    }
//    Serial.print(F("buf"));Serial.print(emptybuf);Serial.print(F("已新增為"));Serial.println(buf[emptybuf]);
//    Serial.print(F("傳送序列現在為「"));Serial.print(queue);Serial.println("」");
    emptybuf+=1;
//    Serial.print("buf:");Serial.println(char(buf[counter]));
  }
//  Serial.print(F("傳送的內容為："));Serial.println(text);
//  Serial.print(F("傳送的內容為："));Serial.println(buf);
  iridiumsend();
  }
