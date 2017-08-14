#include "Mutex.h"

Mutex::Mutex()
{
    mutex_handle_ = CreateMutex(NULL, FALSE, NULL);
}

Mutex::~Mutex(){}

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

int Mutex::TryLock()const
{
    //WAIT_OBJECT_0 - object is signaled
    if(IsValid())
        return (int)(WAIT_OBJECT_0 == WaitForSingleObject(mutex_handle_, 0));

    return -1;
}

bool Mutex::IsValid()const
{
    return (mutex_handle_ != NULL);
}