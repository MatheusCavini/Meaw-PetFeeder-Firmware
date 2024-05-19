#include <Arduino.h>
#include "timesDB.h"
#include "Preferences.h"

Preferences preferences;

//Inicializa base de dados e a variável de quantidade de horários salvos, caso não exista
void timesDB_init(){
    preferences.begin("my-app", false);
    int storedListSize = preferences.getInt("listSize", -1);
    if(storedListSize == -1){
        preferences.putInt("listSize", 0);
        Serial.println("Base de dados inicializada pela primeira vez");
    }else{
        Serial.println("Base de dados já inicializada");
        Serial.print("Número de dados: ");
        Serial.println(storedListSize);
    }
}

//Retorna a Hora e Minuto de um horário de índice index
void retrieveTime(int index, int *H, int *M){
     String keyH = "dataH" + String(index);
     String keyM = "dataM" + String(index);
     *H = preferences.getInt(keyH.c_str(), 0);
     *M = preferences.getInt(keyM.c_str(), 0);
}

//Retorna quantos horários há na base de dados
int retrieveQNT(){
    int storedListSize = preferences.getInt("listSize", 0);
    return storedListSize;
}

//Salva todas as horas e minutos armazenados, em listas passadas como ponteiros
void retrieveAllTimes(int *Hlist, int *Mlist){
    int qnt = retrieveQNT();
    for(int i=0; i<qnt; i++){
        retrieveTime(i, Hlist, Mlist);
        Hlist++;
        Mlist++;
    }


}

//Adiciona um horário na base de dados
void saveTime(int H, int M){
    int storedListSize = preferences.getInt("listSize", 0);
    String keyH = "dataH" + String(storedListSize);
    String keyM = "dataM" + String(storedListSize);
    preferences.putInt(keyH.c_str(), H);
    preferences.putInt(keyM.c_str(), M);
    preferences.putInt("listSize", storedListSize+1);
}

//Apaga todos os dados da base
void timesDB_reset(){
    Serial.println("Removendo todos os dados e inicialização...");
    preferences.clear();
}