#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "displaySystem.h"
#include "alarmSystem.h"
#include "stateMachine.h"
#include "dispenserSystem.h"
#include "keyboard.h"
#include "timesDB.h"
#include "Preferences.h"
#include "clock.h"
#include "api.h"
 
int state;
int event;
int output;
unsigned long lastLevelCheck;
unsigned long lastAlarmCheck;
int qnt; // Declare globally so it's accessible in the setup function and header
int *listSavedHours;
int *listSavedMinutes;

 
void setup() {
    //Inicializa todos os módulos
    Serial.begin(9600);
    clock_init();
    display_init();
    alarm_init();
    dispenser_init();
    timesDB_init();
    eventQ_init();
    stateMachine_init();
    api_init();
    state = START;
    event =  NO_EVENT;
    lastLevelCheck = millis();
    lastAlarmCheck = millis();
    

    listSavedHours = (int*)malloc(30 * sizeof(int));
    listSavedMinutes = (int*)malloc(30 * sizeof(int));
    
    retrieveAllTimes(listSavedHours, listSavedMinutes);
    Serial.println("Horários salvos:");
    for(int i=0; i<qnt; i++){
        Serial.print(listSavedHours[i]);
        Serial.print(":");
        Serial.println(listSavedMinutes[i]);
    }
}


void loop() {
    //Verifica o nível de ração a cada 30seg (podemos calibrar)
    unsigned long now = millis();
    if(now - lastLevelCheck > 30000){
        checkLevel();
        lastLevelCheck = now;
    }

    if(now - lastAlarmCheck > 50000){
        checkAlarm();
        lastAlarmCheck = now;
    }

    keyboardReadCycle();

    //Executa o Loop de atualização da Máquina de Estados

    //Eventos internos são processados com prioridade
    event = getInternEvent();
    if(event == NO_EVENT){
        event = getEvent();
    }

    state = getNextState(state, event);
    output = getOutput(state);
    handleOutput(output);

    Serial.print("Estado: ");
    Serial.println(state);
    Serial.print("Evento: ");
    Serial.println(event);
    delay(100);
}