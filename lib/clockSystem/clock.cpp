#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include "clock.h"
#include "stateMachine.h"

// WiFi credentials
const char* ssid = "SHICA";
const char* password = "sim2sim2";

// NTP settings
const long utcOffsetInSeconds = -3*3600; // Adjust according to your timezone
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);



// Initialize WiFi and time
void clock_init() {
    // Connect to WiFi
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);

  // Configure WiFi to use WPA2 Enterprise with PEAP/MSCHAPv2
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
        Serial.print(WiFi.status());
    }
    Serial.println();
    Serial.println("WiFi connected.");

    // Initialize NTP client
    timeClient.begin();
    timeClient.update();

    // Set system time from NTP
    setTime(timeClient.getEpochTime());
    Serial.println("Time synchronized with NTP.");
    Serial.print(hour());
    Serial.println(minute());
}

//Verifica o horário atual e gera evento se for igual a um listado
void checkAlarm() {
    timeClient.update();
    int currentHour = hour();
    int currentMinute = minute(); 

    for (int i = 0; i < qnt; i++) {
        if (currentHour == listSavedHours[i] &&
            currentMinute == listSavedMinutes[i]) {
                addEvent(SERVE_NOW);
        }
    }
}