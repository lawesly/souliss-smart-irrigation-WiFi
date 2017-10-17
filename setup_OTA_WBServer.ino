#include "constants.h"
#include "preferences.h"
#include "ntp.h"


void setup_OTA_WBServer(){

  // Init the OTA + WebServer
  // Set Hostname.
  String hostNAME(HOSTNAME);
  hostNAME += String(ESP.getChipId(), HEX);
  SERIAL_OUT.print("set OTA+WiFi hostname: "); SERIAL_OUT.println(hostNAME);
  WiFi.hostname(hostNAME);
  ArduinoOTA.onStart([]() { events.send("Update Start", "ota"); });
  ArduinoOTA.onEnd([]() { events.send("Update End", "ota"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    char p[32];
    sprintf(p, "Progress: %u%%\n", (progress/(total/100)));
    events.send(p, "ota");
  });
  ArduinoOTA.onError([](ota_error_t error) {
    if(error == OTA_AUTH_ERROR) events.send("Auth Failed", "ota");
    else if(error == OTA_BEGIN_ERROR) events.send("Begin Failed", "ota");
    else if(error == OTA_CONNECT_ERROR) events.send("Connect Failed", "ota");
    else if(error == OTA_RECEIVE_ERROR) events.send("Recieve Failed", "ota");
    else if(error == OTA_END_ERROR) events.send("End Failed", "ota");
  });
  ArduinoOTA.setHostname((const char *)hostNAME.c_str());
  ArduinoOTA.begin();

  MDNS.addService("http","tcp",80);

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  events.onConnect([](AsyncEventSourceClient *clientSSI){
    clientSSI->send("hello!",NULL,millis(),1000);
  });
  server.addHandler(&events);

  

  server.addHandler(new SPIFFSEditor(http_username,http_password));

  server.on("/", [](AsyncWebServerRequest *request)
    {       

        if (!request->authenticate(http_username,http_password))          
            {
            Serial.printf("Autentication required \n");
            return request->requestAuthentication();//request->redirect("/login");
            }
        else{
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/index.htm", "text/html");
       request->send(response);
        }
    }); 
    
   server.on("/index.htm", [](AsyncWebServerRequest *request)
    {       

        if (!request->authenticate(http_username,http_password))          
            {
            Serial.printf("Autentication required \n");
            return request->requestAuthentication();//request->redirect("/login");
            }
        else{
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/index.htm", "text/html");
       request->send(response);
        }
    });
     server.on("/programlist.htm", [](AsyncWebServerRequest *request)
    {       
        if (!request->authenticate(http_username,http_password))          
            {
            Serial.printf("Autentication required \n");
            return request->requestAuthentication();//request->redirect("/login");
            }
        else{
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/programlist.htm", "text/html");
       request->send(response);
        }
    });
      server.on("/programinput.htm", [](AsyncWebServerRequest *request)
    {       
        if (!request->authenticate(http_username,http_password))          
            {
            Serial.printf("Autentication required \n");
            return request->requestAuthentication();//request->redirect("/login");
            }
        else{
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/programinput.htm", "text/html");
       request->send(response);
        }
    });
     server.on("/programmod.htm", [](AsyncWebServerRequest *request)
    {       
        if (!request->authenticate(http_username,http_password))          
            {
            Serial.printf("Autentication required \n");
            return request->requestAuthentication();//request->redirect("/login");
            }
        else{
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/programmod.htm", "text/html");
       request->send(response);
        }
    });
    server.on("/setting.htm", [](AsyncWebServerRequest *request)
    {       
        if (!request->authenticate(http_username,http_password))          
            {
            Serial.printf("Autentication required \n");
            return request->requestAuthentication();//request->redirect("/login");
            }
        else{
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/setting.htm", "text/html");
       request->send(response);
        }
    });
    server.on("/graph.htm", [](AsyncWebServerRequest *request)
    {       
        if (!request->authenticate(http_username,http_password))          
            {
            Serial.printf("Autentication required \n");
            return request->requestAuthentication();//request->redirect("/login");
            }
        else{
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/graph.htm", "text/html");
       request->send(response);
        }
    });
  
  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("GET /heap");
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });
    
  server.on("/valvola1ON", HTTP_GET, [](AsyncWebServerRequest *request){
    S_rele1status_WBS=1;
    mInput(SLOT_VALVOLA_1) = Souliss_T1n_OnCmd; //accende il relè
    Serial.printf("\nGET /valvola1ON");
    request->redirect("/index.htm");
  });
   server.on("/valvola1OFF", HTTP_GET, [](AsyncWebServerRequest *request){
    S_rele1status_WBS=0;
    mInput(SLOT_VALVOLA_1) = Souliss_T1n_OffCmd; //accende il relè
    Serial.printf("\nGET /valvola1OFF");
    request->redirect("/index.htm");
  });
  server.on("/valvola2ON", HTTP_GET, [](AsyncWebServerRequest *request){
    S_rele2status_WBS=1;
    mInput(SLOT_VALVOLA_2) = Souliss_T1n_OnCmd; //accende il relè
    Serial.printf("\nGET /valvola2ON");
    request->redirect("/index.htm");
  });
   server.on("/valvola2OFF", HTTP_GET, [](AsyncWebServerRequest *request){
    S_rele2status_WBS=0;
    mInput(SLOT_VALVOLA_2) = Souliss_T1n_OffCmd; //accende il relè
    Serial.printf("\nGET /valvola2OFF");
    request->redirect("/index.htm");
  });
  server.on("/valvola3ON", HTTP_GET, [](AsyncWebServerRequest *request){
    S_rele3status_WBS=1;
    mInput(SLOT_VALVOLA_3) = Souliss_T1n_OnCmd; //accende il relè
    Serial.printf("\nGET /valvola3ON");
    request->redirect("/index.htm");
  });
   server.on("/valvola3OFF", HTTP_GET, [](AsyncWebServerRequest *request){
    S_rele3status_WBS=0;
    mInput(SLOT_VALVOLA_3) = Souliss_T1n_OffCmd; //accende il relè
    Serial.printf("\nGET /valvola3OFF");
    request->redirect("/index.htm");
  });
  server.on("/valvola4ON", HTTP_GET, [](AsyncWebServerRequest *request){
    S_rele4status_WBS=1;
    mInput(SLOT_VALVOLA_4) = Souliss_T1n_OnCmd; //accende il relè
    Serial.printf("\nGET /valvola4ON");
    request->redirect("/index.htm");
  });
   server.on("/valvola4OFF", HTTP_GET, [](AsyncWebServerRequest *request){
    S_rele4status_WBS=0;
    mInput(SLOT_VALVOLA_4) = Souliss_T1n_OffCmd; //accende il relè
    Serial.printf("\nGET /valvola4OFF");
    request->redirect("/index.htm");
  });
  server.on("/valvola5ON", HTTP_GET, [](AsyncWebServerRequest *request){
    S_rele5status_WBS=1;
    mInput(SLOT_VALVOLA_5) = Souliss_T1n_OnCmd; //accende il relè
    Serial.printf("\nGET /valvola5ON");
    request->redirect("/index.htm");
  });
   server.on("/valvola5OFF", HTTP_GET, [](AsyncWebServerRequest *request){
    S_rele5status_WBS=0;
    mInput(SLOT_VALVOLA_5) = Souliss_T1n_OffCmd; //accende il relè
    Serial.printf("\nGET /valvola5OFF");
    request->redirect("/index.htm");
  });
  server.on("/valvola6ON", HTTP_GET, [](AsyncWebServerRequest *request){
    S_rele6status_WBS=1;
    mInput(SLOT_VALVOLA_6) = Souliss_T1n_OnCmd; //accende il relè
    Serial.printf("\nGET /valvola6ON");
    request->redirect("/index.htm");
  });
   server.on("/valvola6OFF", HTTP_GET, [](AsyncWebServerRequest *request){
    S_rele6status_WBS=0;
    mInput(SLOT_VALVOLA_6) = Souliss_T1n_OffCmd; //accende il relè
    Serial.printf("\nGET /valvola6OFF");
    request->redirect("/index.htm");
  });
  server.on("/valvola7ON", HTTP_GET, [](AsyncWebServerRequest *request){
    S_rele7status_WBS=1;
    mInput(SLOT_VALVOLA_7) = Souliss_T1n_OnCmd; //accende il relè
    Serial.printf("\nGET /valvola7ON");
    request->redirect("/index.htm");
  });
   server.on("/valvola7OFF", HTTP_GET, [](AsyncWebServerRequest *request){
    S_rele7status_WBS=0;
    mInput(SLOT_VALVOLA_7) = Souliss_T1n_OffCmd; //accende il relè
    Serial.printf("\nGET /valvola7OFF");
    request->redirect("/index.htm");
  });
  server.on("/valvola8ON", HTTP_GET, [](AsyncWebServerRequest *request){
    S_rele8status_WBS=1;
    mInput(SLOT_VALVOLA_8) = Souliss_T1n_OnCmd; //accende il relè
    Serial.printf("\nGET /valvola8ON");
    request->redirect("/index.htm");
  });
   server.on("/valvola8OFF", HTTP_GET, [](AsyncWebServerRequest *request){
    S_rele8status_WBS=0;
    mInput(SLOT_VALVOLA_8) = Souliss_T1n_OffCmd; //accende il relè
    Serial.printf("\nGET /valvola8OFF");
    request->redirect("/index.htm");
  });
  server.on("/all", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("\nGET /all");
    String json = "{";
    json += "\"heap\":"+String(ESP.getFreeHeap());  
    json += ", \"S_setpoint_WBS\":"+S_setpoint_WBS;
    json += ", \"S_humidity_WBS\":"+S_humidity_WBS;
    json += ", \"S_rele1status_WBS\":" +String(S_rele1status_WBS);
    json += ", \"S_rele2status_WBS\":" +String(S_rele2status_WBS);
    json += ", \"S_rele3status_WBS\":" +String(S_rele3status_WBS);
    json += ", \"S_rele4status_WBS\":" +String(S_rele4status_WBS);
    json += ", \"S_rele5status_WBS\":" +String(S_rele5status_WBS);
    json += ", \"S_rele6status_WBS\":" +String(S_rele6status_WBS);
    json += ", \"S_rele7status_WBS\":" +String(S_rele7status_WBS);
    json += ", \"S_rele8status_WBS\":" +String(S_rele8status_WBS);
    json += ", \"S_sensoreattivo_WBS\":" +String(S_sensoreattivo_WBS);
    json += ", \"S_numerovalvole_WBS\":" +String(S_numerovalvole_WBS);
    json += "}";
    Serial.printf("Json: \n");
    request->send(200, "application/json", json);
  });

  server.on("/getFileList", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("GET /getFileList");
    Serial.println("Ecco la lista dei programmi:");
    int i=0; 
    String ListaProgrammi = "";
    Dir dir = SPIFFS.openDir("/programs/");
      ListaProgrammi += "[";
      int primo = 0;
      while (dir.next()) {
        if (primo==0) {
        ListaProgrammi += "\"";          
        }
        else
        {
        ListaProgrammi += ",\"";            
        }
        ListaProgrammi += dir.fileName();
        ListaProgrammi += "\"";
        primo = 1;
      }
      ListaProgrammi += "]";
    
      Serial.println(ListaProgrammi);

  // open file for writing
  File ssi_spiffs = SPIFFS.open("/listaprogrammi.json", "w");
  if (!ssi_spiffs) {
    Serial.println("listaprogrammi.json open failed");
  }
  //qui salvo il buffer su file
  ssi_spiffs.println(ListaProgrammi);
  Serial.print("Salvo in SPIFFS il buffer con i programmi :"); Serial.println(ListaProgrammi);
  delay(1);
  //chiudo il file
  ssi_spiffs.close();
      
      request->send(200, "text/html",ListaProgrammi);
  });

server.on("/DeleteProgram", HTTP_GET, [](
  
  AsyncWebServerRequest *request){
    Serial.println("GET /DeleteProgram");
    Serial.println("Cancello il programma");
  //cancello il file  
  //Check if GET parameter exists

int params = request->params();
if(request->hasParam("file")) {
  AsyncWebParameter* p = request->getParam("file");  
  //const char * nomefile = (char*)p->value().c_str(); //QUESTO E' IL PUNTO DOVE HAI LASCIATO AGOSTINO
  String nomefile = p->value(); 
   Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
  if (SPIFFS.exists(nomefile) == true) {
      bool ssi_spiffs = SPIFFS.remove(nomefile);
        if (!ssi_spiffs) {
          Serial.print("file '");
          Serial.print(nomefile);
          Serial.println("' non trovato!");
       request->send(404, "text/html","Non trovo il programma da cancellare");
        }
        else
        {
          Serial.print("file '");
          Serial.print(nomefile);
          Serial.println("' cancellato!");
      request->send(200, "text/html","Ok programma cancellato!");
        }
      delay(1);

      
  } else {
          Serial.print("file '");
          Serial.println("' non trovato!");
       request->send(404, "text/html","Non trovo il programma da cancellare");
      //!?request->redirect("/programlist.htm");
  }
    } else {
          Serial.print("file '");
          //Serial.print(nomefile);
          Serial.println("' errore!");
      request->send(500, "text/html","Errore");
      
  }
});

    server.serveStatic("/", SPIFFS, "/"); //.setDefaultFile("index.htm");

  server.onNotFound([](AsyncWebServerRequest *request){
    Serial.printf("NOT_FOUND: ");
    if(request->method() == HTTP_GET)
      Serial.printf("GET");
    else if(request->method() == HTTP_POST)
      Serial.printf(" POST http://%s%s\n", "Host",request->host().c_str());
      //Serial.printf("POST");
    else if(request->method() == HTTP_DELETE)
      Serial.printf("DELETE");
    else if(request->method() == HTTP_PUT)
      Serial.printf("PUT");
    else if(request->method() == HTTP_PATCH)
      Serial.printf("PATCH");
    else if(request->method() == HTTP_HEAD)
      Serial.printf("HEAD");
    else if(request->method() == HTTP_OPTIONS)
      Serial.printf("OPTIONS");
    else
      Serial.printf("UNKNOWN");
    Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

    if(request->contentLength()){
      Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
      Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
    }

    int headers = request->headers();
    int i;
    for(i=0;i<headers;i++){
      AsyncWebHeader* h = request->getHeader(i);
      Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
    }

    int params = request->params();
    for(i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isFile()){
        Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
      } else if(p->isPost()){
        Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      } else {
        Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }

    request->send(404);
  });
  server.onFileUpload([](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
    if(!index)
      Serial.printf("UploadStart: %s\n", filename.c_str());
    Serial.printf("%s", (const char*)data);
    if(final)
      Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index+len);
  });
  
  server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
      AsyncWebHeader* h = request->getHeader("Referer");
      Serial.printf("MyHeader: %s\n", h->value().c_str());
      
      String cerca=h->value().c_str();
        if (cerca.indexOf("setting")==-1)
        { 
        Serial.printf("Setting non trovato: %s\n", h->value().c_str());
        } 
        else{
            Serial.printf("Setting trovato: %s\n", h->value().c_str());
            if(!index)
            Serial.printf("Request : ",request);
            Serial.printf("BodyStart: %u\n", index);
            Serial.printf("scrivo file: ssi_setting.json");
            Serial.printf("%s", (const char*)data);
            String S_filena_WBS = "/ssi_settings.json";
            fsUploadFile = SPIFFS.open(S_filena_WBS, "w");
            if (!fsUploadFile) 
              Serial.println("file open failed");
            fsUploadFile.printf("%s",(const char*)data);  
            if(index + len == total)
              //Serial.printf("BodyEnd: %u\n", total);
              fsUploadFile.close();
              delay (1000);
              request->redirect("/salvatosetting.htm");
              delay (1000);
              ReadAllSettingsFromSPIFFS();
        } 
        if (cerca.indexOf("programmod")==-1)
        { 
        Serial.printf("Modifica Programma non trovata: %s\n", h->value().c_str());
        } 
        else{
            Serial.printf("Modifica trovata: %s\n", h->value().c_str());
            if(!index)
            Serial.printf("Request : ",request);
            Serial.printf("BodyStart: %u\n", index);
            Serial.printf("scrivo file: /programs/ssi_program1.json");
            Serial.printf("%s", (const char*)data);
            String S_filena_WBS = "/programs/ssi_program1.json";
            fsUploadFile = SPIFFS.open(S_filena_WBS, "w");
            if (!fsUploadFile) 
              Serial.println("file open failed");
            fsUploadFile.printf("%s",(const char*)data);  
            if(index + len == total)
              //Serial.printf("BodyEnd: %u\n", total);
              fsUploadFile.close();
              delay (1000);
              request->redirect("/salvatoprogramma.htm");
              delay (1000);
              // qui sicuramente andrà la funzione per ricaricare i programmi dopo averli modificati ed attivarli....
              //ReadAllSettingsFromSPIFFS();
        } 
        if (cerca.indexOf("programinput")==-1)
        { 
        Serial.printf("Inserimento Programma non trovato: %s\n", h->value().c_str());
        } 
        else{
                  Serial.printf("Inserimento Programma trovato: %s\n", h->value().c_str());
                  #define FILEPREFIX_LEN strlen("/programs/ssi_program")
                  //Serial.println("Ecco i file:");
                  String str = "";
                  Dir dir = SPIFFS.openDir("/programs/");
                  //funzione per trovare prossimo numero per filename del programma
                  int max_num = 0;
                  while (dir.next()) {
                  //Serial.println(dir.fileName());
                  int extpos = dir.fileName().indexOf(".json"); // indice della sottostring ".json" nel nome del file
                      if(extpos > -1 ){  // solo se l'hai trovata
                        int num = dir.fileName().substring(FILEPREFIX_LEN, extpos).toInt(); // estrai la stringa che rappresenta il numero e convertila a intero
                        if(num > max_num){ max_num = num; } 
                        }
                  }
            //Serial.print("Max is: ");Serial.println(max_num);
            
             if(!index)
            Serial.printf("Request : ",request);
            Serial.printf("BodyStart: %u\n", index);
            Serial.printf("%s", (const char*)data);
            String S_filena_WBS = "/programs/ssi_program" + String(++max_num) + ".json";
            //Serial.print("Prossimo file da salvare: ");Serial.println(S_filena_WBS);
            fsUploadFile = SPIFFS.open(S_filena_WBS, "w");
            if (!fsUploadFile) 
              Serial.println("file open failed");
            fsUploadFile.printf("%s",(const char*)data);  
            if(index + len == total)
              //Serial.printf("BodyEnd: %u\n", total);
              fsUploadFile.close();
              delay (1000);
              request->redirect("/salvatoprogramma.htm");
              delay (1000);
            } 
});
          


  //Client
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  #ifdef TTD
    Serial.println("SEND BOOTUP");
    HTTPClient clienthttp_SSI;
    const char* host="http://www.google-analytics.com/collect";
    String eventData = "v=1&t=event&tid=UA-89261240-1&cid=555&ec=SSI"+String(VERSION)+"&ea=BOOTUP&el="+String(ESP.getChipId(),HEX);
    clienthttp_SSI.begin(host);
    clienthttp_SSI.addHeader("User-agent", "Mozilla/5.0 (X11; Linux x86_64; rv:12.0) Gecko/20100101 Firefox/21.0");
    clienthttp_SSI.POST(eventData);
    clienthttp_SSI.writeToStream(&Serial);
    clienthttp_SSI.end();
    Serial.println("BOOTUP CLOSED");
  #endif
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  server.begin();
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
}  
