
//byte serverGoogle[] = { 192,168,1,193 }; // Gmail at work
//byte serverGoogle[] = { 138,4,61,132 }; // Gmail at Madrid
byte serverGoogle[] = { 130,206,138,40 }; // Gmail at server
//byte serverGoogle[] = { 192,168,1,105 }; // Gmail at home

// Set up web client, connect to server port 80
Client clientGmail(serverGoogle, 80);

int i = 0;

char valS; //lo que se lee de los Asuntos

//*******************************************************************************************************************
//Number of messages in Gmail
//*******************************************************************************************************************
void nMessages(){
 
  if (!bConnectedGm){  
    Serial.println("connecting...");
  
    if (clientGmail.connect()) {
      Serial.println("connected");
      clientGmail.println("GET http://www.morelab.deusto.es/thofu/smartvase/nmessages.php");
      //clientGmail.println("GET http://192.168.1.193/nmessages.php");
      //clientGmail.println("GET http://138.4.61.132/nmessages.php");
      
      clientGmail.println();
      bConnectedGm = true;
    } else {
      Serial.println("connection failed");
    } 
  }
  
  // Read from serial port
  if (clientGmail.available())
  {  
    valN = clientGmail.read();
    switch (valN){
      case '0':
        digitalWrite(43,LOW);
        digitalWrite(42,LOW);
        digitalWrite(40,HIGH);
        digitalWrite(41,HIGH);
        lcd.imageSD(0,0,"MENU");
        nTimes++;
        break;
      case '1':
        digitalWrite(43,HIGH);
        digitalWrite(42,HIGH);
        digitalWrite(40,LOW);
        digitalWrite(41,LOW);
        lcd.imageSD(0,0,"MENU1M");
        nTimes++;
        break;
      case '2':
        digitalWrite(43,HIGH);
        digitalWrite(42,HIGH);
        digitalWrite(40,LOW);
        digitalWrite(41,LOW);
        lcd.imageSD(0,0,"MENU2M");
        nTimes++;
        break;
      case '3':
        digitalWrite(43,HIGH);
        digitalWrite(42,HIGH);
        digitalWrite(40,LOW);
        digitalWrite(41,LOW);
        lcd.imageSD(0,0,"MENU3M");
        nTimes++;
        break;
      default:
        digitalWrite(43,HIGH);
        digitalWrite(42,HIGH);
        digitalWrite(40,LOW);
        digitalWrite(41,LOW);
        lcd.imageSD(0,0,"MENUnM");
        nTimes++;
        break;
      }
      Serial.println("Desconectando");  
      clientGmail.stop();
      bFinishedGm = true;
      bConnectedGm = false;
  }
  
//  if (!clientGmail.connected()) {   
//    clientGmail.stop();
//    for(;;)
//      ;
//  }
}

//*******************************************************************************************************************
//Subjects of messages
//*******************************************************************************************************************
char inbufferSub[50];
int contBuffSub = 0;
int contMessages = 0;
int Y = 115;
boolean isEmpty = true;
boolean isStart = false;

void Subjects(){
 
  if (!bConnectedGs){  
    Serial.println("connecting...");
    
    if (clientGmail.connect()) {
      Serial.println("connected");
      clientGmail.println("GET http://www.morelab.deusto.es/thofu/smartvase/subjects.php");
      //clientGmail.println("GET http://192.168.1.193/subjects.php");
      //clientGmail.println("GET http://138.4.61.132/subjects.php");
      Serial.println("After GET");
      clientGmail.println();
      bConnectedGs = true;
      for (i=0; i<50;i++){
         inbufferSub[i] = '\0';
       }
    } else {
      Serial.println("connection failed");
    } 
  }
  
  // Read from serial port
  if (clientGmail.available())
  {  
    valS = clientGmail.read();
    if ((valS!='|')&&(valS!='&')&&(isStart)){
        inbufferSub[contBuffSub]=valS;
        //Serial.print(inbufferSub[contBuffSub]);
        contBuffSub++;
    }
    if (valS=='|'){
//       Serial.print("inbufferSub: ");
//       Serial.println(inbufferSub);
       lcd.string(20,(Y+(contMessages*20)),300,220,BLACK,FONT2,TRANS,inbufferSub);
       for (i=0; i<contBuffSub;i++){
         inbufferSub[i] = '\0';
       }   
       contBuffSub = 0;
       contMessages++;
    }
    
    if (valS=='&'){
      isEmpty = false;
      isStart = true;
    }
      
    
    if (!isEmpty){
      lcd.imageSD(0,0,"Gmail");
      isEmpty = true;
    }
      
    if(valS=='#'){
      Serial.println("Desconectando");  
      clientGmail.stop();
      bFinishedGs = true;
      bConnectedGs = false; 
      contBuffSub = 0;
      contMessages = 0;
      isStart = false;    
    }  
  
//    if (!clientGmail.connected()) {
//      Serial.println("Desconectando");  
//      clientGmail.stop();
//      for(;;)
//        ;
//    }
  }
  
}
