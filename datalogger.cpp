#include "constants.h"
#include "preferences.h"
#include "datalogger.h"
#include "FS.h"
#include "ntp.h"
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>



/*getNTPday()
 * 1.Sunday 2.Monday 3.Tuesday 4.Wednesday 5.Thursday 6.Friday 7.Saturday
 * SPIFFS.exists(path)
 * SPIFFS.remove(path)
 */
int iDay_prev;
 

void save_datalogger(float setpoint,float sensorValue,bool rele1status,bool rele2status,bool rele3status,bool rele4status,bool rele5status,bool rele6status,bool rele7status,bool rele8status) {
  int iDay = getNTPday();
  String S_TimeDate = digitalClockDisplay_WBS();
  String S_filename = "/datalog/ssi_datalogger" + String(getNTPday()) + ".csv";
  
  /*
   Serial.print("iDay. ");Serial.println(iDay);
  Serial.print("iDay_prev. ");Serial.println(iDay_prev);
  */
  if(iDay_prev > 0 && (iDay != iDay_prev)) {
    if(SPIFFS.exists(S_filename)==1)  {
      Serial.print("    Delete older file: ");Serial.println(S_filename);
      SPIFFS.remove(S_filename);
    }
  }
  

  File ssi_datalogger = SPIFFS.open(S_filename, "a");
  if (!ssi_datalogger) {
    Serial.print(S_filename);Serial.println(" open failed");
  }
  Serial.print(S_filename);Serial.print(" size: ");Serial.println(ssi_datalogger.size());
  ssi_datalogger.print(S_TimeDate);ssi_datalogger.print(",");ssi_datalogger.print(setpoint,1);ssi_datalogger.print(",");ssi_datalogger.print(sensorValue,1);ssi_datalogger.print(",");ssi_datalogger.print(rele1status,1);ssi_datalogger.print(",");ssi_datalogger.print(rele2status,1);ssi_datalogger.print(",");ssi_datalogger.print(rele3status,1);ssi_datalogger.print(",");ssi_datalogger.print(rele4status,1);ssi_datalogger.print(",");ssi_datalogger.print(rele5status,1);ssi_datalogger.print(",");ssi_datalogger.print(rele6status,1);ssi_datalogger.print(",");ssi_datalogger.print(rele7status,1);ssi_datalogger.print(",");ssi_datalogger.println(rele8status);
  Serial.print("Salvo linea datalogger -> ");Serial.print(S_TimeDate);Serial.print(",");Serial.print(setpoint,1);Serial.print(",");Serial.print(sensorValue,1);Serial.print(",");Serial.print(rele1status,1);Serial.print(",");Serial.print(rele2status,1);Serial.print(",");Serial.print(rele3status,1);Serial.print(",");Serial.print(rele4status,1);Serial.print(",");Serial.print(rele5status,1);Serial.print(",");Serial.print(rele6status,1);Serial.print(",");Serial.print(rele7status,1);Serial.print(",");Serial.println(rele8status);
  Serial.print(S_filename);Serial.print(" size: ");Serial.println(ssi_datalogger.size());
  yield();
  ssi_datalogger.close();
  iDay_prev = getNTPday();
  ////////////////////////
  #ifdef TTD
   ///*
      Serial.println("SEND KEEPALIVE");
      HTTPClient clienthttp_SSI;
      const char* host="http://www.google-analytics.com/collect";
      String eventData = "v=1&t=event&tid=UA-89261240-1&cid=555&ec=SSI"+String(VERSION)+"&ea=KEEPALIVE&el="+String(ESP.getChipId(), HEX);
      clienthttp_ssi.begin(host);
      clienthttp_SSI.addHeader("User-agent", "Mozilla/5.0 (X11; Linux x86_64; rv:12.0) Gecko/20100101 Firefox/21.0");
      clienthttp_SSI.POST(eventData);
      clienthttp_SSI.writeToStream(&Serial);
      clienthttp_SSI.end();
      Serial.println("KEEPALIVE CLOSED");
      //*/
  #endif    
}

