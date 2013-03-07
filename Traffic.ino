
//byte serverTraffic[] = { 213,4,59,199 }; //Traffic
byte serverTraffic[] = { 130,206,192,18 }; //Traffic

// Set up web client, connect to server port 80
Client clientTraffic(serverTraffic, 80);

//*******************************************************************************************************************
//se conecta a Internet, hace la petición del tiempo, parsea la repuesta y la muestra por pantalla
//*******************************************************************************************************************
char vA = 'Á';
char vE = 'É';
char vI = 'Í';
char vO = 'Ó';
char vU = 'Ú';
char va = 'á';
char ve = 'é';
char vi = 'í';
char vo = 'ó';
char vu = 'ú';
char oStrange = byte(148); // '“' 
char eStrange = byte(137); // '‰'
//char aStrange = byte(); //Ã
char uStrange = byte(154); // 'š'
//char iStrange = byte(); //Ã
char space = byte(32); //' '
char base = byte(195); //'Ã'
char cubo = byte(179); //'³'
char strange = byte(141); //'�'
char comilla = byte(39); //'''
char alea;
char alea1;
char pr;
char trafficArray[99];
char trafficArrayC[35];
char trafficArrayDay[35];
char trafficArrayDate[35];
char trafficArrayNivel[35];
int nIncidents=0;
int nNiveles=0;
int jTimes = 0;
int nDayModel = 0;
int p=0;

char inBufferT[18];
char inBufferDay[20];
char inBufferDate[14];
char inBufferNivel[11];
char inBufferC[14];

char titleDay[35];
char titleDate[35];
char titleNivel[35];
char titleT[99];
char titleC[35];

String sDay="";
String sDate="";
String sNivel = "";
String sTitleT="";
String sTitleC="";

int qT = 0;
int qD = 0;
int qDate = 0;
int qN = 0;
int qC = 0;

int iT = 0;
int jT = 0;
int jD = 0;
int jDate = 0;
int jN = 0;
int jC = 0;


boolean Traffic(){
  if (!bConnectedT){  
    Serial.print("Connecting to ");
    Serial.println("dgt.es");
    if (clientTraffic.connect()) {
      Serial.println(" Current traffic state:");
  
      clientTraffic.println("GET http://infocar.dgt.es/etraffic/rss_ca_17.xml");
      clientTraffic.println();
      bConnectedT = true;
      lcd.imageSD(0,0,"Traffic");
    } else {
      Serial.print("Conn. failed...");
    }
  }
  
  
  
  
  if (clientTraffic.available()) {
    char c = clientTraffic.read();     // Get the next available character
    //Serial.print(c);
   
    for (iT=2;iT<=16;iT++) {        // scroll the buffer to make room for char
      inBufferT[iT-1]=inBufferT[iT];
      //Serial.println(inBufferT[iT]);
    }
    for (iT=2;iT<=18;iT++) {        // scroll the buffer to make room for char
      inBufferDay[iT-1]=inBufferDay[iT];
      //Serial.println(inBufferT[iT]);
    }
    for (iT=2;iT<=12;iT++) {        // scroll the buffer to make room for char
      inBufferDate[iT-1]=inBufferDate[iT];
      //Serial.println(inBufferT[iT]);
    }
    for (iT=2;iT<=9;iT++) {        // scroll the buffer to make room for char
      inBufferNivel[iT-1]=inBufferNivel[iT];
      //Serial.println(inBufferT[iT]);
    }
    for (iT=2;iT<=12;iT++) {        // scroll the buffer to make room for char
      inBufferC[iT-1]=inBufferC[iT];
      //Serial.println(inBufferC[iT]);
    }
    
    inBufferT[15]=c;    // write recieved char to buffer
    inBufferDay[17]=c;
    inBufferDate[11]=c;
    inBufferNivel[8]=c;
    inBufferC[11]=c;
   
   //BUSCANDO <lastBuildDate> para detectar DÍA DE LA CONSULTA
    if ((qD==0)     // If we are looking for interesting stuff
    && (inBufferDay[3]=='<')
    && (inBufferDay[4]=='l')
    && (inBufferDay[5]=='a')
    && (inBufferDay[6]=='s')
    && (inBufferDay[7]=='t')
    && (inBufferDay[8]=='B')
    && (inBufferDay[9]=='u')
    && (inBufferDay[10]=='i')
    && (inBufferDay[11]=='l')
    && (inBufferDay[12]=='d')
    && (inBufferDay[13]=='D')
    && (inBufferDay[14]=='a')
    && (inBufferDay[15]=='t')
    && (inBufferDay[16]=='e')
    && (inBufferDay[17]=='>')) {
      qD=1;     // Found start of title
      jD=0;                     // Reset pointer to titlebuffer
      //Serial.println("Patron encontrado");
    }
    
   //BUSCANDO <pubDate> para detectar DÍA DE LAS INCIDENCIAS
    if ((qDate==0)     // If we are looking for interesting stuff
    && (inBufferDate[3]=='<')
    && (inBufferDate[4]=='p')
    && (inBufferDate[5]=='u')
    && (inBufferDate[6]=='b')
    && (inBufferDate[7]=='D')
    && (inBufferDate[8]=='a')
    && (inBufferDate[9]=='t')
    && (inBufferDate[10]=='e')
    && (inBufferDate[11]=='>')) {
      qDate=1;     // Found start of title
      jDate=0;                     // Reset pointer to titlebuffer
      nDayModel++;
      //Serial.println("Patron encontrado");
    }
    
    //BUSCANDO <item><title> para detectar Carretera
    if ((qT==0)                   // If we are looking for interesting stuff
    && (inBufferT[3]=='<')        // check if "<item><title>" is in the buffer
    && (inBufferT[4]=='i')
    && (inBufferT[5]=='t')
    && (inBufferT[6]=='e')
    && (inBufferT[7]=='m')
    && (inBufferT[8]=='>')
    && (inBufferT[9]=='<')
    && (inBufferT[10]=='t')
    && (inBufferT[11]=='i')
    && (inBufferT[12]=='t')
    && (inBufferT[13]=='l')
    && (inBufferT[14]=='e')
    && (inBufferT[15]=='>')) {
      qT=1;                     // Found start of title
      jT=0;                     // Reset pointer to titlebuffer
      //Serial.println("Patron encontrado");
    }
    
    //BUSCANDO Nivel
    if ((qN==0)                   // If we are looking for interesting stuff
    && (inBufferNivel[3]=='N')        // check if "<item><title>" is in the buffer
    && (inBufferNivel[4]=='i')
    && (inBufferNivel[5]=='v')
    && (inBufferNivel[6]=='e')
    && (inBufferNivel[7]=='l')
    && (inBufferNivel[8]==' ')) {
      qN=1;                     // Found start of title
      jN=0;                     // Reset pointer to titlebuffer
      //Serial.println("Patron encontrado");
    }
    
    //BUSCANDO Causa
//    if ((qC==0)                   // If we are looking for interesting stuff
//    && (inBufferC[3]=='C')        // check if "<item><title>" is in the buffer
//    && (inBufferC[4]=='a')
//    && (inBufferC[5]=='u')
//    && (inBufferC[6]=='s')
//    && (inBufferC[7]=='a')
//    && (inBufferC[8]==' ')) {
//      qC=1;                     // Found start of title
//      jC=0;                     // Reset pointer to titlebuffer
//      //Serial.println("Patron encontrado");
//    }
    if ((qC==0)                   // If we are looking for interesting stuff
    && (inBufferC[3]=='t')        // check if "<item><title>" is in the buffer
    && (inBufferC[4]=='i')
    && (inBufferC[5]=='p')
    && (inBufferC[6]=='o')
    && (inBufferC[7]=='I')
    && (inBufferC[8]=='n')
    && (inBufferC[9]=='c')
    && (inBufferC[10]=='i')
    && (inBufferC[11]=='=')) {
      qC=1;                     // Found start of title
      jC=0;                     // Reset pointer to titlebuffer
      //Serial.println("Patron encontrado");
    }

    
    //CARRETERA
    if ((qT==1) && (c=='<') ) {
         for (int n=0; n<jT; n++){
            titleT[n] = trafficArray[n];
         }
         qT=0;
        //Searching 'Ã?'
        for (int k=0; k<=jT; k++){
          alea = titleT[k];
          alea1 = titleT[k+1];
          if ((alea == base)&&(alea1==strange)){
                titleT[k]='I';
                for(int m=1; m<(jT-1); m++){
                  titleT[k+m] = titleT[k+m+1];
                }
                titleT[jT] = '\0';   
          }
        }
        for (int k=0; k<=jT; k++){
          alea = titleT[k];
          alea1 = titleT[k+1];
          if ((alea == base)&&(alea1==oStrange)){
                titleT[k]=vO;
                for(int m=1; m<(jT-1); m++){
                  titleT[k+m] = titleT[k+m+1];
                }
                titleT[jT] = '\0';   
          }
        }
        for (int k=0; k<=jT; k++){
          alea = titleT[k];
          alea1 = titleT[k+1];
          if ((alea == base)&&(alea1==uStrange)){
                titleT[k]=vU;
                for(int m=1; m<(jT-1); m++){
                  titleT[k+m] = titleT[k+m+1];
                }
                titleT[jT] = '\0';   
          }
        }
        for (int k=0; k<=jT; k++){
          alea = titleT[k];
          alea1 = titleT[k+1];
          if ((alea == base)&&(alea1==eStrange)){
                titleT[k]=vE;
                for(int m=1; m<(jT-1); m++){
                  titleT[k+m] = titleT[k+m+1];
                }
                titleT[jT] = '\0';   
          }
        }
        for (int k=0; k<=jT; k++){
          alea = titleT[k];
          if (alea == base){
                titleT[k]=vA;       
          }
        } 
        titleT[jT] = '\0';
      
        sTitleT = "";
        for(iT=0;iT<jT;iT++){
          sTitleT = sTitleT + titleT[iT];
        }
//        char cTitleT[jT];
//        sTitleT.toCharArray(cTitleT, sTitleT.length());
        
        //Serial.println(sTitleT);
        //lcd.string(5,10+nIncidents*20,300,220,BLACK,FONT1,TRANS,cTitleT);
        //nIncidents++;
        qT=0;   
             
    }
    
     //CAUSA
//   if ((qC==1) && (c=='<') ) {
//         for (int n=0; n<jC; n++){
//            titleC[n] = trafficArrayC[n];
//         }
//         qC=0;
//        //Searching 'Ã?'
//        for (int k=0; k<=jC; k++){
//          alea = titleC[k];
//          alea1 = titleC[k+1];
//          if ((alea == base)&&(alea1==strange)){
//                titleC[k]='I';
//                for(int m=1; m<(jC-1); m++){
//                  titleC[k+m] = titleC[k+m+1];
//                }
//                titleC[jC] = '\0';   
//          }
//        } 
//      
//        sTitleC = "";
//        for(iT=1;iT<=jC;iT++){
//          sTitleC = sTitleC + titleC[iT];
//        }
////        char cTitleC[jC];
////        sTitleC.toCharArray(cTitleC, sTitleC.length());
//        
//        Serial.println(sTitleC);
//        //lcd.string(5,10+nIncidents*20,300,220,BLACK,FONT1,TRANS,cTitleT);
//        //nIncidents++;
//        qC=0;   
//             
//    }
    
   if ((qC==1) && (c=='<') ) {
         for (int n=0; n<jC; n++){
            titleC[n] = trafficArrayC[n];
         }
         qC=0;
        //Searching 'Ã?'
        for (int k=0; k<=jC; k++){
          alea = titleC[k];
          alea1 = titleC[k+1];
          if ((alea == base)&&(alea1==strange)){
                titleC[k]='I';
                for(int m=1; m<(jC-1); m++){
                  titleC[k+m] = titleC[k+m+1];
                }
                titleC[jC] = '\0';   
          }
        } 
      
        sTitleC = "";
        for(iT=1;iT<=jC;iT++){
          sTitleC = sTitleC + titleC[iT];
        }
//        char cTitleC[jC];
//        sTitleC.toCharArray(cTitleC, sTitleC.length());
        
        //Serial.print("sTitleC: ");
        //Serial.println(sTitleC);
        //lcd.string(5,10+nIncidents*20,300,220,BLACK,FONT1,TRANS,cTitleT);
        //nIncidents++;
        qC=0;   
             
    }
    
    //DÍA EN QUE SE HACE LA CONSULTA
    if ((qD==1) && (c=='<') ) {
         for (int n=5; n<16; n++){
            titleDay[n-5] = trafficArrayDay[n];
         }   
        sDay = "";
        for(iT=0;iT<12;iT++){
          sDay = sDay + titleDay[iT];
        }
        char cDay[jD-5];
        sDay.toCharArray(cDay, sDay.length());
        
        //Serial.println(sDay);
        lcd.string(210,45,300,220,BLACK,FONT3,TRANS,cDay);
        qD=0;              
    }
    //DÍA DE LAS INCIDENCIAS
    if ((qDate==1) && (c=='<')) {
       if (nDayModel>1){
         for (int n=5; n<16; n++){
            titleDate[n-5] = trafficArrayDate[n];
         }
      
        sDate = "";
        for(iT=0;iT<12;iT++){
          sDate = sDate + titleDate[iT];
        }
        char cDate[jDate-5];
        sDate.toCharArray(cDate, sDate.length());
        char cTitleT[jT+1];
        sTitleT.toCharArray(cTitleT, sTitleT.length()+1);
        char cTitleC[jC];
        sTitleC.toCharArray(cTitleC, sTitleC.length());
        //Serial.println("sDay: ");
        //Serial.println(sDay);
        //Serial.println("sDate: ");
        //Serial.println(sDate); 
        if(sDay.compareTo(sDate)==0){
          //Serial.print("Carretera: ");
          //Serial.print(sTitleT);
          //Serial.println("|");
          Serial.print("Causa: ");
          Serial.print(cTitleC);
          //Serial.println("|");
          if (nIncidents < 4){  
            Serial.println (cTitleT);  
            lcd.string(5,105+nIncidents*32,300,220,BLACK,FONT2,TRANS,cTitleC);
            lcd.string(5,105+nIncidents*32+12,300,220,BLACK,FONT2,TRANS,cTitleT);
          }
          nIncidents++;
        }
       }
        qDate=0;      
    }
    //NIVEL DE CIRCULACIÓN
    if ((qN==1) && (c==comilla)) {
      nNiveles++;
      if (nIncidents == nNiveles){
        if ((nIncidents-1) < 4){ 
          switch (trafficArrayNivel[1]){
            case 'N':{ lcd.string(190,105+(nIncidents-1)*32,300,220,BLACK,FONT2,TRANS,"Interrumpida"); break;}
            case 'R':{ lcd.string(190,105+(nIncidents-1)*32,300,220,BLACK,FONT2,TRANS,"Dificil"); break;}
            case 'A':{ lcd.string(190,105+(nIncidents-1)*32,300,220,BLACK,FONT2,TRANS,"Irregular"); break;}
            case 'V':{ lcd.string(190,105+(nIncidents-1)*32,300,220,BLACK,FONT2,TRANS,"Condicionada"); break;}
            case 'G':{ lcd.string(190,105+(nIncidents-1)*32,300,220,BLACK,FONT2,TRANS,"No aplica"); break;}
          }
        }        
      }
      else{
        nNiveles--; 
      }
      qN = 0;
    } 
   
    if ((nIncidents==4)&&(nNiveles==4)){
       bFinishedT = true;
       bConnectedT = false;
       nIncidents = 0;
       nNiveles = 0;
       nDayModel = 0;
    }
    
    //BUSCANDO </channel> para detectar el final
    if ((qT==0)
        && (inBufferT[6]=='<')
        && (inBufferT[7]=='/')
        && (inBufferT[8]=='c')
        && (inBufferT[9]=='h')
        && (inBufferT[10]=='a')
        && (inBufferT[11]=='n')
        && (inBufferT[12]=='n')
        && (inBufferT[13]=='e')
        && (inBufferT[14]=='l')
        && (inBufferT[15]=='>')) {
          bFinishedT = true;
          bConnectedT = false;
          nIncidents = 0;
          nNiveles = 0;
          nDayModel = 0;
          Serial.println("desconectando");
          clientTraffic.stop();
    }

    if ((qT) && (c!='>')){      // While scanning the title
          trafficArray[jT]=c;        // Store chars in character buffer
          jT++;        
    }
    if ((qD) && (c!='>')){      // While scanning the title
          trafficArrayDay[jD]=c;        // Store chars in character buffer
          jD++;        
    }
    
    if ((qDate) && (c!='>')){      // While scanning the title
          trafficArrayDate[jDate]=c;        // Store chars in character buffer
          jDate++;             
    }
    
    if ((qN) && (c!='>')){      // While scanning the title
          trafficArrayNivel[jN]=c;        // Store chars in character buffer
          jN++;  
    }
    
    if ((qC) && (c!='>')){      // While scanning the title
          trafficArrayC[jC]=c;        // Store chars in character buffer
          jC++;  
    }

  }
  
  
  if (!clientTraffic.connected()) {
    bFinishedT = true;
    bConnectedT = false;
    nIncidents = 0;
    nNiveles = 0;
    nDayModel = 0; 
    Serial.println("desconectando");
    clientTraffic.stop();
    //for(;;)
    //  ;
  }
  
}

