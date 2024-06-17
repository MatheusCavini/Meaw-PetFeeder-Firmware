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
    lcd.setCursor(column, row);
    lcd.print(string);
}

//Limpa o display
void displayClear(){
    lcd.clear();
}

//Exibe um horário no formato HH:MM na posição (row, column)
void displayTime(int H, int M, int row, int column){
    char hour[3];
    char min[3];

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

    char timeFormat[6]; 
    sprintf(timeFormat, "%s:%s", hour, min);
    lcd.setCursor(column, row);
    lcd.print(timeFormat);
}