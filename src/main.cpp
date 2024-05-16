#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "displaySystem.h"
#include "alarmSystem.h"
#include "stateMachine.h"
 
int state;
int event;
int output;
unsigned long lastLevelCheck;
 
void setup() {
    //Inicializa todos os módulos
    display_init();
    alarm_init();
    stateMachine_init();
    eventQ_init();
    state = IDLE;
    event =  NO_EVENT;
    lastLevelCheck = millis();
}


void loop() {
    //Verifica o nível de ração a cada 30seg (podemos calibrar)
    unsigned long now = millis();
    if(now - lastLevelCheck > 30000){
        checkLevel();
        lastLevelCheck = now;
    }

    //Executa o Loop de atualização da Máquina de Estados

    //Eventos internos são processados com prioridade
    event = getInternEvent();
    if(event == NO_EVENT){
        event = getEvent();
    }

    state = getNextState(state, event);
    output = getOutput(state);
    handleOutput(output);
    delay(100);
}