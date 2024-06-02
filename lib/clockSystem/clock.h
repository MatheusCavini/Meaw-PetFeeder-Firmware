#ifndef CLOCK_INCLUDED
#define CLOCK_INCLUDED
#include <WiFiUdp.h>
#include <NTPClient.h>

extern int qnt;
extern int *listSavedHours;
extern int *listSavedMinutes;

void clock_init();
void checkAlarm();

extern NTPClient timeClient; 

#endif