
#ifndef THREAD_H
#define THREAD_H

#include "Defines.h"
#include <process.h>
#include <Windows.h>

typedef unsigned(*thread_function)(void*);

class DLL Thread
{
public:
    Thread();
    Thread(thread_function func, void *params);
    ~Thread();

    //starts thread with specific function
    //and vprt to params
    bool BeginThread(thread_function func, void *params);
    //joines thread
    void Join()const;
    //returns true if thread is joined
    //false otherwise
    bool TryJoin()const;

private:

    HANDLE thread_handle_;
};

#endif // !THREAD_H