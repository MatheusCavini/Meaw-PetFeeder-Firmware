#include <Arduino.h>
#include "displaySystem.h"
#include "alarmSystem.h"
#include "stateMachine.h"

#define SOUND_SPEED 0.034 //cm/us

const int trigPin = 18;
const int echoPin = 5;


long duration;
float distanceCm;

//Inicialização do módulo de alarme
void alarm_init(){
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    digitalWrite(trigPin, LOW);
}


//Medição da distância em CM
float measureLevel(){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    //TigPin em HIGH por 10ms
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    //Mede o tempo de retorno da onda
    duration = pulseIn(echoPin, HIGH);
  
    //Calcula a distância
    distanceCm = duration * SOUND_SPEED/2;
    return distanceCm;
}


//Variável que armazena o nível da última medição:
//HIGH = suficiente
//LOW = insuficiente
int lastLevel = HIGH;


//Avalia o nível de forma binária e gera evento quando troca de nível
void checkLevel(){
    float dist =  measureLevel();
    int newLevel;
    
    if(dist > 10){
        newLevel = HIGH;
    }else{
        newLevel = LOW;
    }

    if(newLevel != lastLevel){
        if(newLevel == HIGH){
            addEvent(LEVEL_GOES_UP);
        }else{
            addEvent(LEVEL_GOES_DOWN);
        }
    }

    lastLevel = newLevel;
}