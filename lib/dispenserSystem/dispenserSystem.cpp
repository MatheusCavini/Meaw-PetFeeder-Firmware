#include <Arduino.h>
#include "dispenserSystem.h"
#include <Stepper.h>
#include "stateMachine.h"

const int stepsPerRevolution = 2048;

// ULN2003 Motor Driver Pins
#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 17

Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

void dispenser_init(){
    myStepper.setSpeed(10);
}

void dispenserOpen(){
    myStepper.step(stepsPerRevolution/8);
}

void dispenserClose(){
    myStepper.step(-stepsPerRevolution/8);
}

void releaseMotor(){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

//Função que usa as definidas anteriormente (abrir, fechar, release)
//para servir uma porção
void dispenserServePortion(){
    delay(1000);
    dispenserOpen();
    releaseMotor();
    delay(600);
    dispenserClose();
    delay(500);
    releaseMotor();
    addInternEvent(FINISH_PORTION_SERVE);
}