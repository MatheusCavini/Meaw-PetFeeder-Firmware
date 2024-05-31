#ifndef CLOCK_INCLUDED
#define CLOCK_INCLUDED

extern int qnt;
extern int *listSavedHours;
extern int *listSavedMinutes;

void clock_init();
void checkAlarm();

#endif