//Data structure to manage files in one process in Nachos
#ifndef FTABLE_H
#define FTABLE_H
#include "openfile.h"

#define MAX_FILE 8

class FTable {
private:
    OpenFile* _pFile[MAX_FILE];
    int _type[MAX_FILE];
    int _curOffset[MAX_FILE];
public:
    FTable();
    ~FTable();
    int Open(OpenFile* pFile, int type);
    int Close(int fid);
    int getType(int fid);
    int ReadChar(char &c, int fid);
    int WriteChar(char &c, int fid);
    int Seek(int pos, int fid);
};
#endif
