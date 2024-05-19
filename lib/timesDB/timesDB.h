#ifndef TIMES_DB_INCLUDED
#define TIMES_DB_INCLUDED

void timesDB_init();
void retrieveTime(int index, int *H, int *M);
void saveTime(int H, int M);
int retrieveQNT();
void timesDB_reset();
void retrieveAllTimes(int *Hlist, int *Mlist);

#endif