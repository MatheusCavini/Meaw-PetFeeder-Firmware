#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "api.h"
#include "stateMachine.h"
#include "timesDB.h"

AsyncWebServer server(80);


void api_init(){
    server.on("/1234/connect", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.println("New device connect on terminal");
        request->send(200, "text/plain", "Device connected");
        addEvent(APP_CONNECTED);
    });

    server.on("/1234/getTimes", HTTP_GET, [](AsyncWebServerRequest *request){
        // Create a JSON document
        StaticJsonDocument<200> jsonDoc;

        // Create a JSON array within the document
        JsonArray timeArray = jsonDoc.to<JsonArray>();

        // Populate the JSON array with the saved times
        for (int i = 0; i < qnt; i++) {
            // Create a JSON object for each time
            JsonObject timeObj = timeArray.createNestedObject();
            timeObj["ID"] = i;
            timeObj["hour"] = listSavedHours[i];
            timeObj["minute"] = listSavedMinutes[i];
        }

        // Serialize the JSON document to a string
        String jsonString;
        serializeJson(jsonDoc, jsonString);

        // Send the JSON response
        request->send(200, "application/json", jsonString);
    });

    server.on("/1234/getLevel", HTTP_GET, [](AsyncWebServerRequest *request){
        // Create a JSON document
        StaticJsonDocument<200> jsonDoc;

        // Create a JSON array within the document
        JsonArray levelArray = jsonDoc.to<JsonArray>();

        
        JsonObject levelObj = levelArray.createNestedObject();
        levelObj["level"] = lastLevel; 
        

        // Serialize the JSON document to a string
        String jsonString;
        serializeJson(jsonDoc, jsonString);

        // Send the JSON response
        request->send(200, "application/json", jsonString);
    });

    server.on("/1234/serveNow", HTTP_POST, [](AsyncWebServerRequest *request){
        Serial.println("Pedido de servir agora...");
        addEvent(SERVE_NOW);
        request->send(200, "text/plain", "Serving...");
    });

    server.on("/1234/delete", HTTP_POST, [](AsyncWebServerRequest *request){
        // Extract the ID from the URL parameters
        if (request->hasParam("id")) {
            int id = request->getParam("id")->value().toInt();
            

            removeTime(id);

            request->send(200, "text/plain", "Deletion successful");
        } else {
            request->send(400, "text/plain", "ID parameter missing");
        }
    });

    server.on("/1234/addTime", HTTP_POST, [](AsyncWebServerRequest *request) {
        // Check if there's data available
        if (request->hasParam("hour") && request->hasParam("minute")) {
            // Extract hour and minute from the request parameters
            int hour = request->getParam("hour")->value().toInt();
            int minute = request->getParam("minute")->value().toInt();

            // Do something with hour and minute
            saveTime(hour, minute);

            // Respond to the client
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