#ifndef STATE_MACHINE_INCLUDE
#define STATE_MACHINE_INCLUDED

#define STATES_QNT 6
#define EVENTS_QNT 10

//DEFINES DOS NUMEROS RELATIVOS A CADA ESTADO
#define START 0
#define LOW_LEVEL 1
#define SERVING_PORTION 2
#define TIMES_MENU 3
#define ADD_TIME_MENU 4
#define WAIT_ADD_CONFIRM 5


//DEFINES DOS NUMEROS RELATIVOS A CADA EVENTO
#define NO_EVENT 0
#define LEVEL_GOES_DOWN 1
#define LEVEL_GOES_UP 2
#define FINISH_PORTION_SERVE 3
#define SERVE_NOW 4
#define TIMES_MENU_SELECTED 5
#define GO_BACK 6
#define ADD_TIME_SELECTED 7
#define NEW_TIME_INSERTED 8
#define NEW_TIME_SAVED 9


//DEFINES DOS NUMEROS RELATIVOS AS SAIDAS
#define NO_OUTPUT 0
#define DISPLAY_START 1
#define DISPLAY_LOW_LEVEL 2
#define SERVE_PORTION 3
#define DISPLAY_TIMES_MENU 4
#define DISPLAY_ADD_TIME 5
#define DISPLAY_ADD_CONFIRM 6


void stateMachine_init();
int getNextState(int state, int event);
int getOutput(int state);
void handleOutput(int output);
void eventQ_init();
int getEvent();
int getInternEvent();
void addEvent(int event);
void addInternEvent(int event);


#endif