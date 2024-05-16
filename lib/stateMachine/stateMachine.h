#ifndef STATE_MACHINE_INCLUDE
#define STATE_MACHINE_INCLUDED

#define STATES_QNT 3
#define EVENTS_QNT 3

//DEFINES DOS NUMEROS RELATIVOS A CADA ESTADO
#define IDLE 0
#define LOW_LEVEL 1
#define SERVING_PORTION 2

//DEFINES DOS NUMEROS RELATIVOS A CADA EVENTO
#define NO_EVENT 0
#define LEVEL_GOES_DOWN 1
#define LEVEL_GOES_UP 2
#define FINISH_PORTION_SERVE 3

//DEFINES DOS NUMEROS RELATIVOS AS SAIDAS
#define NO_OUTPUT 0
#define DISPLAY_IDLE 1
#define DISPLAY_LOW_LEVEL 2
#define SERVE_PORTION 4

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