#include "Mutex.h"

Mutex::Mutex()
{
    mutex_handle_ = CreateMutex(NULL, FALSE, NULL);
}

Mutex::~Mutex()
{
    if(mutex_handle_ != INVALID_HANDLE_VALUE)
        CloseHandle(mutex_handle_);
}

void Mutex::Lock()const
{
    WaitForSingleObject(mutex_handle_, INFINITE);
}

void Mutex::Unlock()const
{
    ReleaseMutex(mutex_handle_);
}

bool Mutex::TryLock()const
{
    //WAIT_OBJECT_0 - object is signaled
    return (WAIT_OBJECT_0 == WaitForSingleObject(mutex_handle_, 0));
}