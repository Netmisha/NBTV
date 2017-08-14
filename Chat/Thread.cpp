#include "Thread.h"

Thread::Thread() : thread_handle_(INVALID_HANDLE_VALUE) {}

Thread::Thread(thread_function func, void * params)
{
	BeginThread(func, params);
}

Thread::~Thread(){}

bool Thread::BeginThread(thread_function function, void *params)
{
    thread_handle_ = CreateThread(NULL,                             //security
                                  0,                                //default stack size
                                  (LPTHREAD_START_ROUTINE)function, //function
                                  params,                           //params
                                  0,                                //flags
                                  NULL);                            //out - thread id
    return (thread_handle_ != NULL);
}

void Thread::Join()const
{
    WaitForSingleObject(thread_handle_, INFINITE);
}

bool Thread::TryJoin()const
{
    bool result = false;
    if(WaitForSingleObject(thread_handle_, 0) == WAIT_OBJECT_0)
    {
        result = true;
    }
    return result;
}