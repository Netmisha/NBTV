#ifndef MUTEX_H
#define MUTEX_H

#include <Windows.h>

class Mutex
{
public:
    Mutex();
    ~Mutex();

    void Lock()const;
    void Unlock()const;
    //tries to lock the thread
    //returns true if success
    //false otherwise, don't block thread execution
    bool TryLock()const;

private:
    HANDLE mutex_handle_;
};

#endif // !MUTEX_H