/**************************************************************************
  Wi-Fi Smart Irrigation System based on Souliss IoT Framework

  PIN RELE' VALVOLE : 
  RELE_1     3  
  RELE_2     5  
  RELE_3     4  
  RELE_4     0  
  RELE_5     10 
  RELE_6     13 
  RELE_7     14 
  RELE_8     16 

  This example is only supported on ESP8266.
  Developed by mcbittech & DarioCdj
***************************************************************************/
// Let the IDE point to the Souliss framework
#include "SoulissFramework.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>
#include <FS.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>


// Configure the Souliss framework
#include "bconf/MCU_ESP8266.h"              /** Load the code directly on the ESP8266 */
#include "preferences.h"

#if(DYNAMIC_CONNECTION)
#define USEEEPROM_INSKETCH
#define USEEEPROM             1
#include "conf/RuntimeGateway.h"            // This node is a Peer and can became a Gateway at runtime
#include "conf/DynamicAddressing.h"         // Use dynamically assigned addresses
//#include "conf/WEBCONFinterface.h"          // Enable the WebConfig interface
#include "conf/WEBCONFASYNCinterface.h"     // Enable the WebConfig interface
#include "connection_dynamic.h"
#else
#include "conf/IPBroadcast.h"
#include "connection_static.h"
#endif

#include "Souliss.h"
#include "constants.h"
#include "language.h"
#include "ntp.h"
#include <TimeLib.h>
#include "read_save.h"
#include "datalogger.h"


double analogValue = 0.0;
double analogVolts = 0.0;

//WBServer
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//#ifndef DYNAMIC_CONNECTION
 AsyncWebServer server(80);
//#endif
AsyncWebSocket ws("/ws");
AsyncEventSource events("/events");
File fsUploadFile;
uint8_t buff{};

String S_setpoint_WBS="22.4";
bool S_rele1status_WBS=0;
bool S_rele2status_WBS=0;
bool S_rele3status_WBS=0;
bool S_rele4status_WBS=0;
bool S_rele5status_WBS=0;
bool S_rele6status_WBS=0;
bool S_rele7status_WBS=0;
bool S_rele8status_WBS=0;
String S_humidity_WBS="58.8";
bool S_sensoreattivo_WBS=0;
int S_numerovalvole_WBS=0;
String S_filena_WBS;
/////////////////////////////////////////////////////////////////////////////////////////////////////////

float setpoint=0;
float sensorValue =0;

boolean bClock;
boolean bProgrammi;
int tZone;
boolean bDayLightSavingTime;
boolean sensoreattivo;
int numerovalvole;

void setup()
{

  
  
  SERIAL_OUT.begin(115200);
  SERIAL_OUT.println("SSI - Souliss Smart Irrigation starting...");
  SERIAL_OUT.print("Version:");SERIAL_OUT.println(VERSION);
  Serial.setDebugOutput(true);  //debug WBServer

  //SPIFFS
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  SPIFFS.begin();
  File  ssi_spiffs_verifica = SPIFFS.open("/ssi_settings.json", "r");

  if (!ssi_spiffs_verifica) {
    Serial.println(" ");
    Serial.println("Non riesco a leggere ssi_settings.json! formatto la SPIFFS...");
    SPIFFS.format();
    Serial.println("Spiffs formatted");
    ReadAllSettingsFromPreferences();
  }
  else
  {
    ReadAllSettingsFromSPIFFS();
  }
    S_sensoreattivo_WBS=sensoreattivo;
    S_numerovalvole_WBS=numerovalvole;
    

  Initialize();
  
  extern AsyncWebServer server;
  #if(DYNAMIC_CONNECTION)
    DYNAMIC_CONNECTION_Init();
  #else
  #if(DHCP_OPTION)
    STATIC_CONNECTION_Init_DHCP();
  #else
    STATIC_CONNECTION_Init_STATICIP();
  #endif
  #endif


  //*************************************************************************

  #define RELAY_OFF 1// GPIO value to write to turn off attached relay
  #define RELAY_ON 0// GPIO value to write to turn off attached relay

  // Define output pins
  pinMode(RELE_1, OUTPUT); 
  pinMode(RELE_2, OUTPUT); 
  pinMode(RELE_3, FUNCTION_3); // trasforma la RX in gpio3
  pinMode(RELE_3, OUTPUT); 
  pinMode(RELE_4, OUTPUT); 
  pinMode(RELE_5, OUTPUT); 
  pinMode(RELE_6, OUTPUT); 
  pinMode(RELE_7, OUTPUT); 
  pinMode(RELE_8, OUTPUT); 
  

  //Put all RELE OFF
  digitalWrite(RELE_1,RELAY_OFF);
  digitalWrite(RELE_2,RELAY_OFF);
  digitalWrite(RELE_3,RELAY_OFF);
  digitalWrite(RELE_4,RELAY_OFF);
  digitalWrite(RELE_5,RELAY_OFF);
  digitalWrite(RELE_6,RELAY_OFF);
  digitalWrite(RELE_7,RELAY_OFF);
  digitalWrite(RELE_8,RELAY_OFF);
  
  //*************************************************************************
  //*************************************************************************
  if (numerovalvole== 2) {
  Set_T11(SLOT_VALVOLA_1);
  Set_T11(SLOT_VALVOLA_2);
  };
  if (numerovalvole== 4) {
  Set_T11(SLOT_VALVOLA_1);
  Set_T11(SLOT_VALVOLA_2);
  Set_T11(SLOT_VALVOLA_3);
  Set_T11(SLOT_VALVOLA_4);
  };
  if (numerovalvole== 8) {
  Set_T11(SLOT_VALVOLA_1);
  Set_T11(SLOT_VALVOLA_2);
  Set_T11(SLOT_VALVOLA_3);
  Set_T11(SLOT_VALVOLA_4);
  Set_T11(SLOT_VALVOLA_5);
  Set_T11(SLOT_VALVOLA_6);
  Set_T11(SLOT_VALVOLA_7);
  Set_T11(SLOT_VALVOLA_8);
  };
  
  Set_T53(SLOT_IGROMETRO);
  
  LeggiSensore();
  

  //NTP
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  initNTP();
  delay(1000);
  //*************************************************************************
  //*************************************************************************

  //OTA-WBServer  
  setup_OTA_WBServer();

 
 
}

void loop()
{
  EXECUTEFAST() {
    UPDATEFAST();


    FAST_510ms() {
     
      if (numerovalvole== 2) {
      Logic_T11(SLOT_VALVOLA_1);
      Logic_T11(SLOT_VALVOLA_2);
      };
      if (numerovalvole== 4) {
      Logic_T11(SLOT_VALVOLA_1);
      Logic_T11(SLOT_VALVOLA_2);
      Logic_T11(SLOT_VALVOLA_3);
      Logic_T11(SLOT_VALVOLA_4);
      };
      if (numerovalvole== 8) {
      Logic_T11(SLOT_VALVOLA_1);
      Logic_T11(SLOT_VALVOLA_2);
      Logic_T11(SLOT_VALVOLA_3);
      Logic_T11(SLOT_VALVOLA_4);
      Logic_T11(SLOT_VALVOLA_5);
      Logic_T11(SLOT_VALVOLA_6);
      Logic_T11(SLOT_VALVOLA_7);
      Logic_T11(SLOT_VALVOLA_8);
      };

      if (numerovalvole== 2) {
      LowDigOut(RELE_1, Souliss_T1n_Coil,SLOT_VALVOLA_1);
      S_rele1status_WBS=(mOutput(SLOT_VALVOLA_1) & Souliss_T1n_Coil);//to WBServer
      LowDigOut(RELE_2, Souliss_T1n_Coil,SLOT_VALVOLA_2);
      S_rele2status_WBS=(mOutput(SLOT_VALVOLA_2) & Souliss_T1n_Coil);//to WBServer
      };
      if (numerovalvole== 4) {
      LowDigOut(RELE_1, Souliss_T1n_Coil,SLOT_VALVOLA_1);
      S_rele1status_WBS=(mOutput(SLOT_VALVOLA_1) & Souliss_T1n_Coil);//to WBServer
      LowDigOut(RELE_2, Souliss_T1n_Coil,SLOT_VALVOLA_2);
      S_rele2status_WBS=(mOutput(SLOT_VALVOLA_2) & Souliss_T1n_Coil);//to WBServer
      LowDigOut(RELE_3, Souliss_T1n_Coil,SLOT_VALVOLA_3);
      S_rele3status_WBS=(mOutput(SLOT_VALVOLA_3) & Souliss_T1n_Coil);//to WBServer
      LowDigOut(RELE_4, Souliss_T1n_Coil,SLOT_VALVOLA_4);
      S_rele4status_WBS=(mOutput(SLOT_VALVOLA_4) & Souliss_T1n_Coil);//to WBServer
      };
      
      if (numerovalvole== 8) {
      LowDigOut(RELE_1, Souliss_T1n_Coil,SLOT_VALVOLA_1);
      S_rele1status_WBS=(mOutput(SLOT_VALVOLA_1) & Souliss_T1n_Coil);//to WBServer
      LowDigOut(RELE_2, Souliss_T1n_Coil,SLOT_VALVOLA_2);
      S_rele2status_WBS=(mOutput(SLOT_VALVOLA_2) & Souliss_T1n_Coil);//to WBServer
      LowDigOut(RELE_3, Souliss_T1n_Coil,SLOT_VALVOLA_3);
      S_rele3status_WBS=(mOutput(SLOT_VALVOLA_3) & Souliss_T1n_Coil);//to WBServer
      LowDigOut(RELE_4, Souliss_T1n_Coil,SLOT_VALVOLA_4);
      S_rele4status_WBS=(mOutput(SLOT_VALVOLA_4) & Souliss_T1n_Coil);//to WBServer
      LowDigOut(RELE_5, Souliss_T1n_Coil,SLOT_VALVOLA_5);
      S_rele4status_WBS=(mOutput(SLOT_VALVOLA_4) & Souliss_T1n_Coil);//to WBServer
      LowDigOut(RELE_6, Souliss_T1n_Coil,SLOT_VALVOLA_6);
      S_rele4status_WBS=(mOutput(SLOT_VALVOLA_4) & Souliss_T1n_Coil);//to WBServer
      LowDigOut(RELE_7, Souliss_T1n_Coil,SLOT_VALVOLA_7);
      S_rele4status_WBS=(mOutput(SLOT_VALVOLA_4) & Souliss_T1n_Coil);//to WBServer
      LowDigOut(RELE_8, Souliss_T1n_Coil,SLOT_VALVOLA_8);
      S_rele4status_WBS=(mOutput(SLOT_VALVOLA_4) & Souliss_T1n_Coil);//to WBServer
      };
    }

      #if(DYNAMIC_CONNECTION)
       DYNAMIC_CONNECTION_fast();
      #else
        STATIC_CONNECTION_fast();
      #endif
  }

  EXECUTESLOW() {
    UPDATESLOW();

      
       SLOW_x10s(59){
      //DATALOGGER
      save_datalogger(setpoint,sensorValue,S_rele1status_WBS,S_rele2status_WBS,S_rele3status_WBS,S_rele4status_WBS,S_rele5status_WBS,S_rele6status_WBS,S_rele7status_WBS,S_rele8status_WBS);
    }

 SLOW_x10s(61){
    
    ReadAllSettingsFromSPIFFS();
    LeggiSensore();
  }
    
       
       SLOW_10s() {        // We handle the light timer with a 10 seconds base time
           
            if (numerovalvole== 2) {
            Timer_T11(RELE_1); 
            Timer_T11(RELE_2);
            };
            
            if (numerovalvole== 4) {
            Timer_T11(RELE_1); 
            Timer_T11(RELE_2);
            Timer_T11(RELE_3);
            Timer_T11(RELE_4);
            };
            
            if (numerovalvole== 8) {
            Timer_T11(RELE_1); 
            Timer_T11(RELE_2);
            Timer_T11(RELE_3);
            Timer_T11(RELE_4);
            Timer_T11(RELE_5);
            Timer_T11(RELE_6);
            Timer_T11(RELE_7);
            Timer_T11(RELE_8);
            };            
       }
      SLOW_15m() {
        //NTP
        yield();
        initNTP();
      }

      #if(DYNAMIC_CONNECTION==1)
      DYNAMIC_CONNECTION_slow();
      #endif
  }

  // Look for a new sketch to update over the air
  ArduinoOTA.handle();
  yield();
}


void SSI_Reset() {  
  spiffs_Reset();
  ESP.reset();
}
  

//WBServer
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if(type == WS_EVT_CONNECT){
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    client->printf("Hello Client %u :)", client->id());
    client->ping();
  } else if(type == WS_EVT_DISCONNECT){
    Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
  } else if(type == WS_EVT_ERROR){
    Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } else if(type == WS_EVT_PONG){
    Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
  } else if(type == WS_EVT_DATA){
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    String msg = "";
    if(info->final && info->index == 0 && info->len == len){
      //the whole message is in a single frame and we got all of it's data
      Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);

      if(info->opcode == WS_TEXT){
        for(size_t i=0; i < info->len; i++) {
          msg += (char) data[i];
        }
      } else {
        char buff[3];
        for(size_t i=0; i < info->len; i++) {
          sprintf(buff, "%02x ", (uint8_t) data[i]);
          msg += buff ;
        }
      }
      Serial.printf("%s\n",msg.c_str());

      if(info->opcode == WS_TEXT)
        client->text("I got your text message");
      else
        client->binary("I got your binary message");
    } else {
      //message is comprised of multiple frames or the frame is split into multiple packets
      if(info->index == 0){
        if(info->num == 0)
          Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
        Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);

      if(info->opcode == WS_TEXT){
        for(size_t i=0; i < info->len; i++) {
          msg += (char) data[i];
        }
      } else {
        char buff[3];
        for(size_t i=0; i < info->len; i++) {
          sprintf(buff, "%02x ", (uint8_t) data[i]);
          msg += buff ;
        }
      }
      Serial.printf("%s\n",msg.c_str());

      if((info->index + len) == info->len){
        Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if(info->final){
          Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          if(info->message_opcode == WS_TEXT)
            client->text("I got your text message");
          else
            client->binary("I got your binary message");
        }
      }
    }
  }
}
void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    Serial.printf("Void HandleBody:");
  if(!index){
    Serial.printf("BodyStart: %u B\n", total);
  }
  for(size_t i=0; i<len; i++){
    Serial.write(data[i]);
  }
  if(index + len == total){
    Serial.printf("BodyEnd: %u B\n", total);
  }
  yield();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void ReadAllSettingsFromPreferences() {
  SERIAL_OUT.println("Read ALL Preferences value....");
  bClock = CLOCK;
  bProgrammi= PROGRAMMI;
  tZone = TIME_ZONE;
  bDayLightSavingTime = DAYLIGHTSAVINGTIME;
  setpoint = SETPOINT;
  sensoreattivo = SENSOREATTIVO;
  numerovalvole = NUMEROVALVOLE;
  save_spiffs_prefs( bClock, tZone, bDayLightSavingTime, bProgrammi, setpoint,sensoreattivo, numerovalvole);
  
}
void ReadAllSettingsFromSPIFFS() {
  //SPIFFS
  
  SERIAL_OUT.println(" ");
  SERIAL_OUT.println("Read All Settings From SPIFFS.... ");
  bClock = read_spiffs_prefs("Orologio");
  tZone = read_spiffs_prefs("Tzone");
  bDayLightSavingTime = read_spiffs_prefs("DayLightSavingTime");
  bProgrammi = read_spiffs_prefs("Programmi");
  setpoint=read_spiffs_prefs("Setpoint");
  S_setpoint_WBS=setpoint;//to WBServer
  sensoreattivo=read_spiffs_prefs("SensoreAttivo");
  S_sensoreattivo_WBS=sensoreattivo;
  numerovalvole=read_spiffs_prefs("NumeroValvole");
  S_numerovalvole_WBS=numerovalvole;
  
}

void LeggiSensore() {
  if (sensoreattivo == 1) {
          S_sensoreattivo_WBS = sensoreattivo;
          Logic_T53(SLOT_IGROMETRO);
      
          analogValue = analogRead(A0); // read the analog signal
      
          // convert the analog signal to voltage
          // the ESP2866 A0 reads between 0 and ~3 volts, producing a corresponding value
          // between 0 and 1024. The equation below will convert the value to a voltage value.
      
          analogVolts = (analogValue * 3.08) / 1024;
        
          // now get our chart value by converting the analog (0-1024) value to a value between 0 and 100.
          // the value of 400 was determined by using a dry moisture sensor (not in soil, just in air).
          // When dry, the moisture sensor value was approximately 400. This value might need adjustment
          // for fine tuning of the chartValue.
        
          sensorValue = (analogValue * 100) / 1024; //with above 400 is not good, using 1024
        
          // now reverse the value so that the value goes up as moisture increases
          // the raw value goes down with wetness, we want our chart to go up with wetness
          sensorValue = 100 - sensorValue;
      
          
          SERIAL_OUT.print("Valore Igrometro : "); SERIAL_OUT.println(sensorValue); //Stampa a schermo il valore
          
          ImportAnalog(SLOT_IGROMETRO,&sensorValue);
          S_humidity_WBS=sensorValue;//to WBServer
          S_setpoint_WBS=setpoint;//to WBServer
     }
     else 
     {
      S_sensoreattivo_WBS =0;
     }
}

