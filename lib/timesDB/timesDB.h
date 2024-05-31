#ifndef TIMES_DB_INCLUDED
#define TIMES_DB_INCLUDED

void timesDB_init();
void retrieveTime(int index, int *H, int *M);
void saveTime(int H, int M);
int retrieveQNT();
void timesDB_reset();
void removeTime(int index);
void retrieveAllTimes(int *Hlist, int *Mlist);
extern int *listSavedHours;
extern int *listSavedMinutes;
extern int qnt;

#endif