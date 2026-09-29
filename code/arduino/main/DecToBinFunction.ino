String queue;
uint8_t buf[50];

void addToQueue(long decnum, int digit){
  String bin;
  if(decnum<0){bin = String(decnum*-1,BIN);}else{bin = String(decnum,BIN);} 
  while(bin.length()<digit-1){bin=String("0"+bin);} //補0直到指定位數
  if(decnum<0){bin=String("1"+bin);}else{bin=String("0"+bin);} //負則開頭為1，正則開頭為0
  queue = queue + bin; 
  Serial.print("傳送序列已增加「");Serial.print(bin);Serial.println("」"); 
  Serial.print("傳送序列現在為「");Serial.print(queue);Serial.println("」"); 
}

void queueToText(){
  String text=""; String bin=queue;String zeroorone;
  
  while(bin.length()%8!=0){bin+="0"; }
  //Serial.print("bin已被擴寫為「");Serial.print(bin);Serial.println("」"); //這個註解掉才行

  for(int counter=0;counter<(bin.length()/8);counter=counter+1){
    buf[counter]=0b11111111;
    
    for(int digit=8;digit>0;digit-=1){
    zeroorone = bin.charAt((counter+1)*8+digit-9);
    bitWrite(buf[counter], 8-digit, zeroorone.toInt());}
    
    //Serial.print("buf:");Serial.println(char(buf[counter])); //這個註解掉才行
  }
//  Serial.print("傳送的內容為：");Serial.println(text);
//  Serial.print("傳送的內容為：");Serial.println(buf);
    //iridiumsend();
  }
