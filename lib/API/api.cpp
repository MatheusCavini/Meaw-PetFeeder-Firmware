#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "api.h"
#include "stateMachine.h"
#include "timesDB.h"

AsyncWebServer server(80);


//Inicialização da API e seus Endpoints
void api_init(){
    //Endpoint de conexão com App. Método GET
    server.on("/1234/connect", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.println("New device connect on terminal");
        request->send(200, "text/plain", "Device connected");
        addEvent(APP_CONNECTED);
    });

    //Endpoint de listagem de horários. Método GET
    server.on("/1234/getTimes", HTTP_GET, [](AsyncWebServerRequest *request){
        StaticJsonDocument<200> jsonDoc;
        JsonArray timeArray = jsonDoc.to<JsonArray>();

        for (int i = 0; i < qnt; i++) {
            // Cria um objeto JSON para cada horário
            JsonObject timeObj = timeArray.createNestedObject();
            timeObj["ID"] = i;
            timeObj["hour"] = listSavedHours[i];
            timeObj["minute"] = listSavedMinutes[i];
        }
        String jsonString;
        serializeJson(jsonDoc, jsonString);

        request->send(200, "application/json", jsonString);
    });

    //Endpoint de nível de ração. Método GET
    server.on("/1234/getLevel", HTTP_GET, [](AsyncWebServerRequest *request){
        StaticJsonDocument<200> jsonDoc;
        JsonArray levelArray = jsonDoc.to<JsonArray>();

        JsonObject levelObj = levelArray.createNestedObject();
        levelObj["level"] = lastLevel;
        String jsonString;
        serializeJson(jsonDoc, jsonString);
        request->send(200, "application/json", jsonString);
    });

    //Endpoint de Servir Agora. Método POST
    server.on("/1234/serveNow", HTTP_POST, [](AsyncWebServerRequest *request){
        Serial.println("Pedido de servir agora...");
        addEvent(SERVE_NOW);
        request->send(200, "text/plain", "Serving...");
    });

    //Endpoint de deleção de horário. Méotod POST
    server.on("/1234/delete", HTTP_POST, [](AsyncWebServerRequest *request){

        if (request->hasParam("id")) {
            int id = request->getParam("id")->value().toInt();
            
            removeTime(id);

            request->send(200, "text/plain", "Deletion successful");
        } else {
            request->send(400, "text/plain", "ID parameter missing");
        }
    });

    //Endpoint de adição de horário. Método POST
    server.on("/1234/addTime", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("hour") && request->hasParam("minute")) {
            int hour = request->getParam("hour")->value().toInt();
            int minute = request->getParam("minute")->value().toInt();

            saveTime(hour, minute);

            request->send(200, "text/plain", "Time received successfully");
        } else {
            Serial.println("No data received");
            request->send(400, "text/plain", "No data received");
        }
    });

    server.begin();

    Serial.print("Server IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("\n\n");
}