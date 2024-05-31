#include <Arduino.h>
#include "stateMachine.h"
#include "Keypad.h"
#include "keyboard.h"
#include "displaySystem.h"
#include "math.h"
#include "timesDB.h"

#define ROWS  4
#define COLS  4

char keyMap[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

uint8_t rowPins[ROWS] = {13, 12, 14, 27}; 
uint8_t colPins[COLS] = {26, 25, 33, 32};

Keypad keypad = Keypad(makeKeymap(keyMap), rowPins, colPins, ROWS, COLS );

char key;
int H = 0;
int M = 0;
int clicks = 0;
int eraseID = -1;


//O tratamento dos inputs de teclado gera eventos diferentes em função do estado atual
char keyboardReadCycle(){
    key = keypad.getKey(); 
    if(key){
    switch (state)
        {
            case START:
                if(key == 'A'){
                    addEvent(TIMES_MENU_SELECTED);
                }
                else if(key == 'B'){
                    addEvent(SERVE_NOW);
                }else if(key == 'C'){
                    addEvent(CONNECT_APP_SELECTED);
                }
                break;
            
            case TIMES_MENU:
                if(key == 'A'){
                    addEvent(ADD_TIME_SELECTED);
                }else if(key == 'B'){
                    addEvent(REMOVE_TIME_SELECTED);
                }else if(key == 'C'){
                    addEvent(GO_BACK);
                }
                break;
            
            case ADD_TIME_MENU:
                //Espera a inserção de 4 dados: HH:MM
                while(clicks<4){
                    displayTime(H, M, 0, 11);
                    if(key == 'B'){
                        clicks = 0;
                        H = 0;
                        M = 0;
                        addEvent(GO_BACK);
                        key = 0;
                        break;
                    }else if(key >= 48 && key <= 57){
                        if(clicks == 0 || clicks ==1){
                            H += pow(10, 1-clicks)*(key-'0');
                        }else{
                            M += pow(10, 1-clicks+2)*(key-'0');
                        }
                        clicks++;
                    }
                    delay(100);
                    key = keypad.getKey();
                }
                displayTime(H, M, 0, 11);
                if(clicks >=4){addEvent(NEW_TIME_INSERTED);};
                break;

            case WAIT_ADD_CONFIRM:
                if(key == 'A'){
                    if(H<24 && M<60){
                        saveTime(H, M);
                        displayShow("SALVO COM       ",0,0);
                        displayShow("SUCESSO!        ",1,0);
                        delay(2000);
                        addEvent(NEW_TIME_SAVED);
                    }else{
                        displayShow("HORARIO         ",0,0);
                        displayShow("INVALIDO!       ",1,0);
                        delay(2000);
                        addEvent(GO_BACK);
                    }
                    H = 0;
                    M = 0;
                    clicks =0;
                }else if(key == 'B'){
                    H = 0;
                    M = 0;
                    clicks =0;
                    addEvent(GO_BACK);
                }
                break;
            
            case REMOVE_TIME_MENU:
                while(true){
                    if(key == 'B'){
                        eraseID =-1;
                        addEvent(GO_BACK);
                        break;
                    }else if(key == 'D' && eraseID<qnt-1){
                        eraseID ++;
                        displayTime(listSavedHours[eraseID], listSavedMinutes[eraseID],0,5);
                    }else if(key == 'C' && eraseID>0){
                        eraseID --;
                        displayTime(listSavedHours[eraseID], listSavedMinutes[eraseID],0,5);
                    }else if(key == 'A' && eraseID> -1){
                        displayShow("HORARIO  ",0,0);
                        displayTime(listSavedHours[eraseID], listSavedMinutes[eraseID],0,9);
                        displayShow("APAGADO!        ",1,0);
                        removeTime(eraseID);
                        delay(2000);
                        eraseID = -1;
                        addEvent(TIME_REMOVED);
                        break;

                    }
                    delay(100);
                    key = keypad.getKey();
                }
                break;
            
            default:
                break;
        }
    }
    return key;
    
}