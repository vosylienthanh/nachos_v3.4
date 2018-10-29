// sem.h
#ifndef SEM_H
#define SEM_H

#include "synch.h"

#define SEM_MAXNAMESIZE 50                 // the max size of the name of semaphore

class Sem {
private:
    char _name[SEM_MAXNAMESIZE];
    Semaphore *_sem;
public:
    Sem();
    ~Sem();
    int Create(char *name, int semval);    // create a new semaphore. return 0 if success and -1 if failure
    void Delete();                         // delete the semaphore if exist
    void Wait();
    void Signal();
    char* GetName();
};

#endif
