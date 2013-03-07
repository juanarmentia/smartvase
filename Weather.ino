
byte serverWeather[] = { 207,242,93,22 }; // Accuweather

// Set up web client, connect to server port 80
Client clientWeather(serverWeather, 80);

char inbuffer[12];

int q = 0;

int j = 0;

char title[80];
String sTitle="";

//*******************************************************************************************************************
//se conecta a Internet, hace la petición del tiempo, parsea la repuesta y la muestra por pantalla
//*******************************************************************************************************************
boolean Weather(){
  
  if (!bConnectedW){  
    Serial.print("Connecting to ");
    Serial.println("AccuWeather.com");
    if (clientWeather.connect()) {
      Serial.print(" Current weather:");
  
      //clientWeather.println("GET http://rss.accuweather.com/rss/liveweather_rss.asp?metric=1&locCode=EUR|ES|SP013|MADRID");
      clientWeather.println("GET http://rss.accuweather.com/rss/liveweather_rss.asp?metric=1&locCode=EUR|ES|SP017|BILBAO");
      //clientWeather.println("GET http://rss.accuweather.com/rss/liveweather_rss.asp?metric=1&locCode=EUR|ES|SP017|VITORIA-GASTEIZ");
      
      clientWeather.println();
      bConnectedW = true;
    } else {
      Serial.print("Conn. failed...");
    }
  }
  
  if (clientWeather.available()) {
    char c = clientWeather.read();      // Get the next available character
    p++;
   
    for (i=2;i<=10;i++) {        // scroll the buffer to make room for char
      inbuffer[i-1]=inbuffer[i];
    }
    inbuffer[9]=c;               // write recieved char to buffer
   
    if ((q==0)                   // If we are looking for interesting stuff
    && (inbuffer[3]=='<')        // check if "<title>" is in the buffer
    && (inbuffer[4]=='t')
    && (inbuffer[5]=='i')
    && (inbuffer[6]=='t')
    && (inbuffer[7]=='l')
    && (inbuffer[8]=='e')
    && (inbuffer[9]=='>')) {
      q=1;                     // Found start of title
      j=0;                       // Reset pointer to titlebuffer
    }

    if ((q==1) && (c=='<') ) {
      q=0;                    // Found end of title
      if((j>9)
      && (title[0]=='C')        // Check if title contains current weather
      && (title[1]=='u')
      && (title[2]=='r')
      && (title[3]=='r')
      && (title[4]=='e')
      && (title[5]=='n')
      && (title[6]=='t')
      && (title[7]=='l')
      && (title[8]=='y')
      && (title[9]==':')) {
        sTitle = "";
        for(i=11;i<j;i++){
          sTitle = sTitle + title[i];
        }
        Serial.println(" " +sTitle);
        ShowForecast(sTitle);
        bFinishedW = true;
        bConnectedW = false;
        Serial.println("desconectando");
        clientWeather.stop();
      }      
    }

    if ((q) && (c!='>')){        // While scanning the title
      title[j++]=c;              // Store chars in character buffer
    }

  }
   
//  if (!clientWeather.connected()) {   
//    Serial.println("desconectando");
//    clientWeather.stop();
//    Serial.println("desconectado");
//    for(;;)
//      ;
//  }
  
}


//*******************************************************************************************************************
//Muestra por pantalla la predicción
//*******************************************************************************************************************
void ShowForecast (String sTitle){
  //SHOWERS
  if ((sTitle[0]=='S')&&(sTitle[7]==':')){
    lcd.imageSD(0,0,"FS");
    ParseForecast(9, sTitle);
  }
  
  //RAIN
  if ((sTitle[0]=='R')&&(sTitle[4]==':')){
    lcd.imageSD(0,0,"FR");
    ParseForecast(6, sTitle);
  }

  //PARTLY SUNNY W/ SHOWERS
  if ((sTitle[0]=='P')&&(sTitle[23]==':')){
    lcd.imageSD(0,0,"FPSWS");
    ParseForecast(25, sTitle);
  }  
  
  //PARTLY SUNNY
  if ((sTitle[0]=='P')&&(sTitle[12]==':')){
    lcd.imageSD(0,0,"FPS");
    ParseForecast(14, sTitle);
  }
  
  //MOSTLY SUNNY
  if ((sTitle[0]=='M')&&(sTitle[12]==':')){
    lcd.imageSD(0,0,"FMS");
    ParseForecast(14, sTitle);
  }
  
  //MOSTLY CLOUDY
  if ((sTitle[0]=='M')&&(sTitle[13]==':')){
    lcd.imageSD(0,0,"FMC");
    ParseForecast(15, sTitle);
  }
  
  //MOSTLY CLOUDY W/ SHOWERS
  if ((sTitle[0]=='M')&&(sTitle[24]==':')){
    lcd.imageSD(0,0,"FMCWS");
    ParseForecast(26, sTitle);
  }
  
  //INTERMITTENT CLOUDS
  if ((sTitle[0]=='I')&&(sTitle[19]==':')){
    lcd.imageSD(0,0,"FIC");
    ParseForecast(9, sTitle);
  }
  
  //FOG
  if ((sTitle[0]=='F')&&(sTitle[3]==':')){
    lcd.imageSD(0,0,"FF");
    ParseForecast(5, sTitle);
  }
  
  //CLOUDY
  if ((sTitle[0]=='C')&&(sTitle[6]==':')){
    lcd.imageSD(0,0,"FC");
    ParseForecast(8, sTitle);
  }
  
  //SUNNY
  if ((sTitle[0]=='S')&&(sTitle[5]==':')){
    lcd.imageSD(0,0,"FSU");
    ParseForecast(7, sTitle);
  }
  
  //CLEAR
  if ((sTitle[0]=='C')&&(sTitle[2]=='e')){
    lcd.imageSD(0,0,"FCL");
    ParseForecast(7, sTitle);
  }
}


//*******************************************************************************************************************
//Parseo de la tempertatura y muestra por pantalla
//*******************************************************************************************************************
void ParseForecast (int nChar, String sTitle){
  
  switch (sTitle[nChar]) {
            case '0':
              lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"0C");
              break;
            case '1':
              switch (sTitle[nChar+1]){
                case '0':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"10C");
                  break;
                case '1':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"11C");
                  break;
                case '2':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"12C");
                  break;
                case '3':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"13C");
                  break;
                case '4':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"14C");
                  break;
                case '5':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"15C");
                  break;
                case '6':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"16C");
                  break;
                case '7':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"17C");
                  break;
                case '8':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"18C");
                  break;
                case '9':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"19C");
                  break;
                default:
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"1C");
                  break;
              }              
              break;
            case '2':
              switch (sTitle[nChar+1]){
                case '0':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"20C");
                  break;
                case '1':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"21C");
                  break;
                case '2':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"22C");
                  break;
                case '3':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"23C");
                  break;
                case '4':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"24C");
                  break;
                case '5':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"25C");
                  break;
                case '6':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"26C");
                  break;
                case '7':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"27C");
                  break;
                case '8':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"28C");
                  break;
                case '9':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"29C");
                  break;
                default:
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"2C");
                  break;
              }
              break;
            case '3':
              switch (sTitle[nChar+1]){
                case '0':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"30C");
                  break;
                case '1':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"31C");
                  break;
                case '2':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"32C");
                  break;
                case '3':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"33C");
                  break;
                case '4':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"34C");
                  break;
                case '5':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"35C");
                  break;
                case '6':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"36C");
                  break;
                case '7':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"37C");
                  break;
                case '8':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"38C");
                  break;
                case '9':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"39C");
                  break;
                default:
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"3C");
                  break;
              }
              break;
            case '4':
              switch (sTitle[nChar+1]){
                case '0':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"40C");
                  break;
                case '1':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"41C");
                  break;
                case '2':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"42C");
                  break;
                case '3':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"43C");
                  break;
                case '4':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"44C");
                  break;
                case '5':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"45C");
                  break;
                case '6':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"46C");
                  break;
                case '7':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"47C");
                  break;
                case '8':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"48C");
                  break;
                case '9':
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"49C");
                  break;
                default:
                  lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"4C");
                  break;
              }
              break;
            case '5':
              lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"5C");
              break;
            case '6':
              lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"6C");
              break;
            case '7':
              lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"7C");
              break;
            case '8':
              lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"8C");
              break;
            case '9':
              lcd.string(100,170,300,220,BLACK,FONT7,TRANS,"9C");
              break;
          }
  
}
