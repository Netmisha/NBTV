#include "Thread.h"

Thread::Thread() : thread_handle_(INVALID_HANDLE_VALUE) {}

Thread::Thread(thread_function func, void * params)
{
	BeginThread(func, params);
}

Thread::~Thread(){}

bool Thread::BeginThread(thread_function function, void *params)
{
    thread_handle_ = (HANDLE)_beginthread(function, 0, params);
    return thread_handle_ != INVALID_HANDLE_VALUE;
}

void Thread::Join()const
{
    WaitForSingleObject(thread_handle_, INFINITE);
}