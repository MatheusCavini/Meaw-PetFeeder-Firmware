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
 
int state;
int event;
int output;
unsigned long lastLevelCheck;
 
void setup() {
    //Inicializa todos os módulos
    Serial.begin(9600);
    display_init();
    alarm_init();
    dispenser_init();
    timesDB_init();
    eventQ_init();
    stateMachine_init();
    state = START;
    event =  NO_EVENT;
    lastLevelCheck = millis();
    delay(2000);
    

    
    /* int qnt = retrieveQNT();
    int H[qnt];
    int M[qnt];
    retrieveAllTimes(H, M);
    Serial.println("Horários salvos:");
    for(int i; i<qnt; i++){
        Serial.print(H[i]);
        Serial.print(":");
        Serial.println(M[i]);
    } */
}


void loop() {
    //Verifica o nível de ração a cada 30seg (podemos calibrar)
    unsigned long now = millis();
    if(now - lastLevelCheck > 30000){
        checkLevel();
        lastLevelCheck = now;
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