#include <Arduino.h>
#include "constants.h"
#include "read_save.h"
#include "FS.h"
#include <ArduinoJson.h>

void save_spiffs_prefs(int json_bClock, int json_timeZone, int json_DayLightSavingTime, int json_bProgrammi, float json_setpoint, int json_SensoreAttivo, int json_NumeroValvole) {
  SPIFFS.begin();
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["Orologio"] = json_bClock;
  root["Tzone"] = json_timeZone;
  root["DayLightSavingTime"] = json_DayLightSavingTime;
  root["Programmi"] = json_bProgrammi;
  root["Setpoint"] = json_setpoint;
  root["SensoreAttivo"] = json_setpoint;
  root["NumeroValvole"] = json_setpoint;
  Serial.print("Ecco i dati in json: ");
  root.printTo(Serial);
  char buffer[256];
  root.printTo(buffer, sizeof(buffer));
  //Serial.println();

  // open file for writing
  File ssi_spiffs = SPIFFS.open("/ssi_settings.json", "w");
  if (!ssi_spiffs) {
    Serial.println("ssi_settings.json open failed");
  }
  //qui salvo il buffer su file
  ssi_spiffs.println(buffer);
  Serial.print("Salvo in SPIFFS il buffer con i settings :"); Serial.println(buffer);
  delay(1);
  //chiudo il file
  ssi_spiffs.close();
}

int read_spiffs_prefs(const char*  valuedaleggere) {
  File  ssi_spiffs_inlettura = SPIFFS.open("/ssi_settings.json", "r");
  if (!ssi_spiffs_inlettura) {
    Serial.println("ssi_settings.json open failed");
    return 0;
  }
  String risultato = ssi_spiffs_inlettura.readStringUntil('\n');
  //Serial.print("Ho letto dal file : ");Serial.println(risultato);
  char json[200];
  risultato.toCharArray(json, 200);
  //Serial.print("Ecco l'array json convertito: ");Serial.println(json);
  StaticJsonBuffer<200> jsonBuffer_inlettura;
  JsonObject& root_inlettura = jsonBuffer_inlettura.parseObject(json);
  if (!root_inlettura.success()) {
    Serial.println("parseObject() failed");
    return 0;
  }
  //leggo il valore e lo parso:
  int risultatoparsed = root_inlettura[valuedaleggere];
  #ifdef DEBUG
  Serial.print("Spiffs Json parsed value of "); Serial.print(valuedaleggere); Serial.print(" :");
  Serial.println(risultatoparsed);
  #endif
  ssi_spiffs_inlettura.close();
  return risultatoparsed;
}


void spiffs_Reset() {
  Serial.println("Reset SSI");
  Serial.print("SPIFFS Formatting... ");
  if (SPIFFS.format()) {
    Serial.println("OK");
  } else {
    Serial.println("Fail");
  }

}



