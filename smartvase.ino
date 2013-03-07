
#include "WiFly.h"
#include "Credentials.h"
#include <SMARTGPU.h> //include the SMARTGPU library!
#include <string.h>

SMARTGPU lcd;             //create our object called LCD

//Variables para controlar cuándo han terminado de ejecutarse los procesos de Gmail, Traffic y Weather
boolean bConnectedW = false;
boolean bConnectedGm = false;
boolean bConnectedGs = false;
boolean bConnectedT = false;
boolean bFinishedW = false;
boolean bFinishedGm = false;
boolean bFinishedGs = false;
boolean bFinishedT = false;

//Variables para controlar el lugar de la pantalla donde se ha pulsado
int iOp = 0; //zona donde se ha marcado en pantalla {1,2,3}
int touch[2];

char valN = '0'; //número de mails leído

long initTime;  //Momento de tiempo en el que el programa comienza
int nTimes=1; //Variable para controlar cuándo se tiene que ejecutar la comprobación de correo
float tiempo; //Variable que almacena el tiempo que ha pasado desde que comenzó el programa

void setup()
{
  Serial.begin(9600);

  initWifly(); 
  
  initLCD(lcd);
  while (!bFinishedGm){
    nMessages();
  } 
  bFinishedGm = false;
  
  initTime=millis();
  
  //pinMode(A6, INPUT);
  pinMode(40, OUTPUT);
  pinMode(41, OUTPUT);
  pinMode(42, OUTPUT);
  pinMode(43, OUTPUT);
  
}

void loop()
{
  //wait for a touch on screen to do something
  while(lcd.touchScreen(touch)==0){
      tiempo = millis()-initTime;
      if ((tiempo/1000) > (18000*nTimes)){
        Serial.println("Comprobar correo");
        while (!bFinishedGm){
          nMessages();
        } 
        bFinishedGm = false;        
      }
  } 
  initTouch();
  actionTouch();    
}


//*******************************************************************************************************************
//Convert String to char*
//*******************************************************************************************************************
char * stringToChar(String sCadena)
{
  char *track;
  char tracklngth[sCadena.length() + 1];
  sCadena.toCharArray(tracklngth, sCadena.length() + 1);
  track = tracklngth;
  
  return track;
}




//*******************************************************************************************************************
//inicializa el touch del menu
//*******************************************************************************************************************
void initTouch(){
    //obtain icon number
  if((touch[XCOORD]>26)&&(touch[XCOORD]<98)&&(touch[YCOORD]>94)&&(touch[YCOORD]<166)){ 
    iOp=1;      
  }else if((touch[XCOORD]>129)&&(touch[XCOORD]<201)&&(touch[YCOORD]>94)&&(touch[YCOORD]<166)){
    iOp=2;
  }else if((touch[XCOORD]>224)&&(touch[XCOORD]<296)&&(touch[YCOORD]>94)&&(touch[YCOORD]<166)){
    iOp=3;      
  }
  
}



//*******************************************************************************************************************
//inicializa la Wifly
//*******************************************************************************************************************
void initWifly(){
  WiFly.begin();
 
  Serial.println("Wifi connecting");
  if (!WiFly.join(ssid, passphrase)) {
  //if (!WiFly.join(ssid)) {
    while (1) {
      Serial.println("Wifi not connected");
      // Hang on failure.
    }
  }
  Serial.println("Wifi connected");
}


//*******************************************************************************************************************
//inicializa el LCD
//*******************************************************************************************************************
void initLCD(SMARTGPU lcd){
  lcd.init();  //configure the serial and pinout of arduino board for SMARTGPU support
  lcd.start(); //initialize the SMARTGPU processor
  //lcd.imageSD(0,0,"MENU");
}


//*******************************************************************************************************************
//Realiza la acción adecuada en función de lo seleccionado en pantalla
//*******************************************************************************************************************
void actionTouch(){
  switch (iOp){
    //GMAIL
    case 1:
      switch (valN){
        case '0':
          lcd.imageSD(0,0,"MGmail"); 
          while (!bFinishedGs){
            Subjects();
          } 
          bFinishedGs = false;
          delay(5000);
          lcd.imageSD(0,0,"MENU");
          break;
        case '1':
          lcd.imageSD(0,0,"MENU");
          while (!bFinishedGs){
            Subjects();
          } 
          bFinishedGs = false;
          delay(5000);
          lcd.imageSD(0,0,"MENU1M");
          break;
        case '2':
          lcd.imageSD(0,0,"M2MG");
          while (!bFinishedGs){
            Subjects();
          } 
          bFinishedGs = false; 
          delay(5000);
          lcd.imageSD(0,0,"MENU2M");
          break;
        case '3':
          lcd.imageSD(0,0,"M3MG"); 
          while (!bFinishedGs){
            Subjects();
          } 
          bFinishedGs = false;
          delay(5000);
          lcd.imageSD(0,0,"MENU3M");
          break;
        default:
          lcd.imageSD(0,0,"MnMG"); 
          while (!bFinishedGs){
            Subjects();
          } 
          bFinishedGs = false;
          delay(5000);
          lcd.imageSD(0,0,"MENUnM");
          break;
      }
      iOp=0;
      //initTouch();
      break;
    //WEATHER
    case 2:
      switch (valN){
        case '0':
          lcd.imageSD(0,0,"MWeather");
          while (!bFinishedW){
            Weather();
          } 
          bFinishedW = false;
          delay(5000);
          lcd.imageSD(0,0,"MENU");
          break;
        case '1':
          Serial.println("M1MW");
          lcd.imageSD(0,0,"M1MW");
          while (!bFinishedW){
            Weather();
          } 
          bFinishedW = false;
          delay(5000);
          lcd.imageSD(0,0,"MENU1M");
          break;
        case '2':
          lcd.imageSD(0,0,"M2MW");
          while (!bFinishedW){
            Weather();
          } 
          bFinishedW = false;
          delay(5000);
          lcd.imageSD(0,0,"MENU2M");
          break;
        case '3':
          lcd.imageSD(0,0,"M3MW");
          while (!bFinishedW){
            Weather();
          } 
          bFinishedW = false;
          delay(5000);
          lcd.imageSD(0,0,"MENU3M");
          break;
        default:
          lcd.imageSD(0,0,"MnMW");
          while (!bFinishedW){
            Weather();
          } 
          bFinishedW = false;
          delay(5000);
          lcd.imageSD(0,0,"MENUnM");
          break;
      }
      iOp=0;
      //initTouch();
      break;
    //TRAFFIC
    case 3:
      switch (valN){
        case '0':
          lcd.imageSD(0,0,"MTraffic");
          while (!bFinishedT){
            Traffic();
          } 
          bFinishedT = false; 
          delay(5000);
          lcd.imageSD(0,0,"MENU");
          break;
        case '1':
          lcd.imageSD(0,0,"M1MT");
          while (!bFinishedT){
            Traffic();
          } 
          bFinishedT = false; 
          delay(5000);
          lcd.imageSD(0,0,"MENU1M");
          break;
        case '2':
          lcd.imageSD(0,0,"M2MT");
          while (!bFinishedT){
            Traffic();
          } 
          bFinishedT = false; 
          delay(5000);
          lcd.imageSD(0,0,"MENU2M");
          break;
        case '3':
          lcd.imageSD(0,0,"M3MT");
          while (!bFinishedT){
            Traffic();
          } 
          bFinishedT = false; 
          delay(5000);
          lcd.imageSD(0,0,"MENU3M");
          break;
        default:
          lcd.imageSD(0,0,"MnMT");
          lcd.imageSD(0,0,"TSim");
          //while (!bFinishedT){
          //  Traffic();
          //} 
          bFinishedT = false; 
          delay(5000);
          lcd.imageSD(0,0,"MENUnM");
          break;
      }
      iOp=0;
      //initTouch();
      break;
  } 
}






