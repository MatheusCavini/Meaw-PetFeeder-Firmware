#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "displaySystem.h"
#include "alarmSystem.h"
#include "stateMachine.h"
 
int state;
int event;
int output;
 
void setup() {
 display_init();
 alarm_init();
 stateMachine_init();
 eventQ_init();
 state = IDLE;
 event =  NO_EVENT;
}
 
void loop() {

    checkLevel();
    event = getEvent();
    state = getNextState(state, event);
    output = getOutput(state);
    handleOutput(output);
    delay(100);


}