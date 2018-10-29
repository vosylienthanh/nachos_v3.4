#ifndef PCB_H
#define PCB_H

#include "synch.h"

#define PCB_MAXNAMESIZE 50

class PCB {
private:
    char _filename[PCB_MAXNAMESIZE];
    Thread *_thread;
    int _parentID;
    int _processID;
    Semaphore *_joinsem;
    Semaphore *_exitsem;
    Semaphore *_mutex;
    int _joinid;
    int _joinexitcode;
    int _numwait;
    int _isExit;
public:
    PCB();
    ~PCB();
    int Exec(char *filename, int processID, int parentID);    // create and exec a new thread. return 0 if success and -1 if failure
    void JoinWait(int joinid);
    void JoinRelease(int joinid, int joinexitcode);
    void ExitWait();
    void ExitRelease();
    void IncNumWait();
    void DecNumWait();
    int GetParentID();
    int GetProcessID();
    int GetJoinID();
    int GetJoinExitCode();
    char* GetFileName();
    Thread* GetThread();
};

#endif
