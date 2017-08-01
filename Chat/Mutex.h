#ifndef MUTEX_H
#define MUTEX_H

#include "Defines.h"
#include <Windows.h>

class DLL_EXP Mutex
{
public:
    Mutex();
    ~Mutex();

    void Lock()const;
    void Unlock()const;
    //tries to lock the thread
    //returns 1 if success
    //0 otherwise, don't block thread execution
    //-1 if mutex is not valid
    int TryLock()const;

    bool IsValid()const;

private:
    HANDLE mutex_handle_;
};

#endif // !MUTEX_H