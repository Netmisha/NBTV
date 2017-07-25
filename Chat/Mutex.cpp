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
    if(IsValid())
        WaitForSingleObject(mutex_handle_, INFINITE);
}

void Mutex::Unlock()const
{
    if(IsValid())
        ReleaseMutex(mutex_handle_);
}

bool Mutex::TryLock()const
{
    //WAIT_OBJECT_0 - object is signaled
    if(IsValid())
        return (WAIT_OBJECT_0 == WaitForSingleObject(mutex_handle_, 0));
}

bool Mutex::IsValid()const
{
    return (mutex_handle_ != NULL);
}