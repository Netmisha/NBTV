#ifndef MUTEX_H
#define MUTEX_H

#include <Windows.h>

class Mutex
{
public:
    Mutex();
    ~Mutex();

    inline void Lock()const;
    inline void Unlock()const;
    //tries to lock the thread
    //returns true if success
    //false otherwise, don't block thread execution
    inline bool TryLock()const;

    inline bool IsValid()const;

private:
    HANDLE mutex_handle_;
};

#endif // !MUTEX_H