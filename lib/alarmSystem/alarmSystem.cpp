#include <Arduino.h>
#include "displaySystem.h"
#include "alarmSystem.h"
#include "stateMachine.h"

#define SOUND_SPEED 0.034 //cm/us

const int trigPin = 18;
const int echoPin = 5;


long duration;
float distanceCm;

void alarm_init(){
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    digitalWrite(trigPin, LOW);
}

float measureLevel(){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
  
    // Calculate the distance
    distanceCm = duration * SOUND_SPEED/2;
    return distanceCm;
}

int lastLevel = HIGH;

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