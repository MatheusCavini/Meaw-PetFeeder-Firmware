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
#include <TimeLib.h>

// Task handles
TaskHandle_t TaskStateMachine;
TaskHandle_t TaskLevelCheck;
TaskHandle_t TaskAlarmCheck;

int state;
int event;
int output;
int lastMinuteCheck;
int currentMinuteCheck;

int qnt; 
int *listSavedHours;
int *listSavedMinutes;

// Tarefas
void stateMachineTask(void *pvParameters);
void keyboardReadTask(void *pvParameters);
void levelCheckTask(void *pvParameters);
void alarmCheckTask(void *pvParameters);

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
    event = NO_EVENT;
    lastMinuteCheck = -1;

    listSavedHours = (int*)malloc(30 * sizeof(int));
    listSavedMinutes = (int*)malloc(30 * sizeof(int));
    
    retrieveAllTimes(listSavedHours, listSavedMinutes);
    Serial.println("Horários salvos:");
    for(int i = 0; i < qnt; i++) {
        Serial.print(listSavedHours[i]);
        Serial.print(":");
        Serial.println(listSavedMinutes[i]);
    }

    // Cria as tasks
    xTaskCreate(stateMachineTask, "State Machine Task", 2048, NULL, 1, &TaskStateMachine);
    xTaskCreate(levelCheckTask, "Level Check Task", 2048, NULL, 1, &TaskLevelCheck);
    xTaskCreate(alarmCheckTask, "Alarm Check Task", 2048, NULL, 1, &TaskAlarmCheck);
}

void loop() {
    //Nada, tudo implementado com FreeRTOS
}

void stateMachineTask(void *pvParameters) {
    while (1) {

        keyboardReadCycle();
        
        //Executa o Loop de atualização da Máquina de Estados

        //Eventos internos são processados com prioridade
        event = getInternEvent();
        if (event == NO_EVENT) {
            event = getEvent();
        }
        
        state = getNextState(state, event);
        output = getOutput(state);
        handleOutput(output);

        Serial.print("Estado: ");
        Serial.println(state);
        Serial.print("Evento: ");
        Serial.println(event);

      
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


void levelCheckTask(void *pvParameters) {
    while (1) {
        //Verifica o nível de ração a cada 30seg (podemos calibrar)
        checkLevel();
        vTaskDelay(pdMS_TO_TICKS(30000));
    }
}

void alarmCheckTask(void *pvParameters) {
    while (1) {
        timeClient.update();
        currentMinuteCheck = minute();
        if (currentMinuteCheck != lastMinuteCheck) {
            checkAlarm();
            lastMinuteCheck = currentMinuteCheck;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}