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
    for(int i=0; i<STATES_QNT; i++){
        stateTransitionMatrix[i][SERVE_NOW] = SERVING_PORTION;
    }

    stateTransitionMatrix[START][LEVEL_GOES_DOWN] = LOW_LEVEL;
    stateTransitionMatrix[START][SERVE_NOW] = SERVING_PORTION;
    stateTransitionMatrix[START][TIMES_MENU_SELECTED] = TIMES_MENU;
    stateTransitionMatrix[START][CONNECT_APP_SELECTED] = CONNECTING_APP;

    
    stateTransitionMatrix[LOW_LEVEL][LEVEL_GOES_UP] = START;

    stateTransitionMatrix[SERVING_PORTION][FINISH_PORTION_SERVE] = START;

    stateTransitionMatrix[TIMES_MENU][GO_BACK] = START;
    stateTransitionMatrix[TIMES_MENU][ADD_TIME_SELECTED] = ADD_TIME_MENU;
    stateTransitionMatrix[TIMES_MENU][REMOVE_TIME_SELECTED] = REMOVE_TIME_MENU;

    stateTransitionMatrix[ADD_TIME_MENU][GO_BACK] = TIMES_MENU;
    stateTransitionMatrix[ADD_TIME_MENU][NEW_TIME_INSERTED] = WAIT_ADD_CONFIRM;

    stateTransitionMatrix[WAIT_ADD_CONFIRM][GO_BACK] = ADD_TIME_MENU;
    stateTransitionMatrix[WAIT_ADD_CONFIRM][NEW_TIME_SAVED] = START;

    stateTransitionMatrix[REMOVE_TIME_MENU][GO_BACK] = TIMES_MENU;
    stateTransitionMatrix[REMOVE_TIME_MENU][TIME_REMOVED] = START;

    stateTransitionMatrix[CONNECTING_APP][APP_CONNECTED] = START;
    stateTransitionMatrix[CONNECTING_APP][GO_BACK] = START;

    
    stateOutputsMatrix[START] = DISPLAY_START;
    stateOutputsMatrix[LOW_LEVEL] =  DISPLAY_LOW_LEVEL;
    stateOutputsMatrix[SERVING_PORTION] = SERVE_PORTION;
    stateOutputsMatrix[TIMES_MENU] = DISPLAY_TIMES_MENU;    
    stateOutputsMatrix[ADD_TIME_MENU] = DISPLAY_ADD_TIME;
    stateOutputsMatrix[WAIT_ADD_CONFIRM] = DISPLAY_ADD_CONFIRM;
    stateOutputsMatrix[REMOVE_TIME_MENU] = DISPLAY_REMOVE_TIME;
    stateOutputsMatrix[CONNECTING_APP]= DISPLAY_CONNECTING_APP;
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
    case DISPLAY_START:
        displayShow("A-HORARIOS      ", 0,0);
        displayShow("B-SERVIR C-APP  ", 1,0);
        break;
    case DISPLAY_LOW_LEVEL:
        displayShow("NIVEL BAIXO!    ", 0,0);
        displayShow("                ", 1,0);
        break;
    case SERVE_PORTION:
        displayShow("SERVINDO...     ", 0,0);
        displayShow("                ", 1,0);
        dispenserServePortion();
        break;
    case DISPLAY_TIMES_MENU:
        displayShow("A-NOVO  C-VOLTAR", 0,0);
        displayShow("B-EXCLUIR       ", 1,0);
        break;
    case DISPLAY_ADD_TIME:
        displayShow("  INSIRA:  00:00", 0,0);
        displayShow("B-VOLTAR        ", 1,0);
        break;
    case DISPLAY_ADD_CONFIRM:
        displayShow("A-SALVAR:", 0,0);
        displayShow("B-CANCELAR        ", 1,0);
        break;
    case DISPLAY_REMOVE_TIME:
        displayShow("C<<  SELEC  >>D ",0,0);
        displayShow("A-APAGAR B-VOLTA", 1,0);
        break;
    case DISPLAY_CONNECTING_APP:
        displayShow("ID:1234  B-VOLTA",0,0);
        displayShow("Aguardando...   ", 1,0);
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
