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
    qnt = retrieveQNT();
    for(int i=0; i<qnt; i++){
        retrieveTime(i, Hlist, Mlist);
        Hlist++;
        Mlist++;
    }


}

//Adiciona um horário na base de dados
void saveTime(int H, int M){
    int storedListSize = retrieveQNT();
    String keyH = "dataH" + String(storedListSize);
    String keyM = "dataM" + String(storedListSize);
    preferences.putInt(keyH.c_str(), H);
    preferences.putInt(keyM.c_str(), M);
    preferences.putInt("listSize", storedListSize+1);
    delay(100);
    retrieveAllTimes(listSavedHours, listSavedMinutes);
   
}

//Remove um dado de um determinado índice
void removeTime(int index) {
    int storedListSize = retrieveQNT();

    // Verificar se o índice é válido
    if (index < 0 || index >= storedListSize) {
        Serial.println("Índice inválido");
        return;
    }

    // Remover o horário no índice especificado e ajustar os índices dos demais horários
    for (int i = index; i < storedListSize - 1; i++) {
        String keyH_current = "dataH" + String(i);
        String keyM_current = "dataM" + String(i);
        String keyH_next = "dataH" + String(i + 1);
        String keyM_next = "dataM" + String(i + 1);

        int nextH = preferences.getInt(keyH_next.c_str(), 0);
        int nextM = preferences.getInt(keyM_next.c_str(), 0);

        preferences.putInt(keyH_current.c_str(), nextH);
        preferences.putInt(keyM_current.c_str(), nextM);
    }

    // Remover os dados do último índice
    String keyH_last = "dataH" + String(storedListSize - 1);
    String keyM_last = "dataM" + String(storedListSize - 1);
    preferences.remove(keyH_last.c_str());
    preferences.remove(keyM_last.c_str());

    // Atualizar o tamanho da lista
    preferences.putInt("listSize", storedListSize - 1);

    delay(100);
    retrieveAllTimes(listSavedHours, listSavedMinutes);
    
}

//Apaga todos os dados da base
void timesDB_reset(){
    Serial.println("Removendo todos os dados e inicialização...");
    preferences.clear();
}

