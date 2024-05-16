#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "displaySystem.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);


//Inicialização do módulo do display
void display_init(){
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0,0);
}


//Exibição no display na posição (row, column)
void displayShow(char string[], int row, int column){
    lcd.setCursor(row,column);
    lcd.print(string);
}

void displayTime(int H, int M, int S){
    char hour[3];
    char min[3];
    char sec[3];

    if(H<10){
        sprintf(hour, "0%d", H);
    }else{
        sprintf(hour, "%d", H);
    }

    if(M<10){
        sprintf(min, "0%d", M);
    }else{
        sprintf(min, "%d", M);
    }

    if(S<10){
        sprintf(sec, "0%d", S);
    }else{
        sprintf(sec, "%d", S);
    }

    
    char timeFormat[9]; 
    sprintf(timeFormat, "%s:%s:%s", hour, min, sec);
    lcd.print(timeFormat);
}