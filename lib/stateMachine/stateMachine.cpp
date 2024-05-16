#include <Arduino.h>
#include "displaySystem.h"
#include "alarmSystem.h"
#include "stateMachine.h"
#include "dispenserSystem.h"

//Matriz que relaciona estado atual e evento com o próximo estado
int stateTransitionMatrix[STATES_QNT][EVENTS_QNT];

//Matriz que relaciona o estado atual com a saída (Moore)
int stateOutputsMatrix[STATES_QNT];

//Inicialização das Matrizes e mapeamento das relações
void stateMachine_init(){
    for(int i=0; i<STATES_QNT; i++){
        for(int j=0; j<EVENTS_QNT; j++){
            stateTransitionMatrix[i][j] = i;
            stateOutputsMatrix[i] = NO_OUTPUT;
        }
    }

    stateTransitionMatrix[IDLE][LEVEL_GOES_DOWN] = LOW_LEVEL;
    stateOutputsMatrix[IDLE] = DISPLAY_IDLE;

    stateTransitionMatrix[LOW_LEVEL][LEVEL_GOES_UP] = IDLE;
    stateOutputsMatrix[LOW_LEVEL] =  DISPLAY_LOW_LEVEL;

    stateTransitionMatrix[SERVE_PORTION][FINISH_PORTION_SERVE] = IDLE;
    stateOutputsMatrix[SERVING_PORTION] = SERVE_PORTION;
}

//Consulta a matriz de transição e retorna o próximo estado
int getNextState(int state, int event){
    return stateTransitionMatrix[state][event];
}

//Consulta a matriz de saídas e retorna a saída
int getOutput(int state){
    return stateOutputsMatrix[state];
}

//Executa uma ação de saída (ex: display, motores, led)
void handleOutput(int output){
    switch (output)
    {
    case DISPLAY_IDLE:
        displayShow("ESTADO IDLE", 0,0);
        break;
    case DISPLAY_LOW_LEVEL:
        displayShow("NIVEL BAIXO!",0,0);
        break;
    case SERVE_PORTION:
        displayShow("SERVINDO...", 0,0);
        dispenserServePortion();
        break;
    
    default:
        break;
    }
}


//Criação das filas de eventos internos e gerais
#define MAX_EVENTS 50
int EventsQueue[MAX_EVENTS];
int InternEventsQueue[MAX_EVENTS];
int eventCount;
int internEventCount;

//Inicialização das filas
void eventQ_init(){
    for(int i=0; i<MAX_EVENTS; i++){
        EventsQueue[i]= NO_EVENT;
        InternEventsQueue[i] = NO_EVENT;
        eventCount = 0;
        internEventCount = 0;
    }
}


//Funções de inserção de evento
void addEvent(int event){
    int i;
    for(i=0; i<eventCount; i++);
    EventsQueue[i] = event;
    eventCount ++;
}

void addInternEvent(int event){
    int i;
    for(i=0; i<internEventCount; i++);
    InternEventsQueue[i] = event;
    internEventCount ++;
}


//Funções de leitura + dequeue de eventos em fila
int getEvent(){
    if (eventCount == 0) {
        return NO_EVENT; // No events in the queue
    } else {
        int firstEvent = EventsQueue[0]; // Get the first event
        // Shift elements to the left to remove the first event
        for (int i = 0; i < eventCount - 1; i++) {
            EventsQueue[i] = EventsQueue[i + 1];
        }
        EventsQueue[eventCount - 1] = NO_EVENT; // Clear the last element
        eventCount--; // Decrement the event count
        return firstEvent; // Return the dequeued event
    }
}

int getInternEvent(){
    if (internEventCount == 0) {
        return NO_EVENT; // No events in the queue
    } else {
        int firstEvent = InternEventsQueue[0]; // Get the first event
        // Shift elements to the left to remove the first event
        for (int i = 0; i < internEventCount - 1; i++) {
            InternEventsQueue[i] = InternEventsQueue[i + 1];
        }
        InternEventsQueue[internEventCount - 1] = NO_EVENT; // Clear the last element
        internEventCount--; // Decrement the event count
        return firstEvent; // Return the dequeued event
    }
}
