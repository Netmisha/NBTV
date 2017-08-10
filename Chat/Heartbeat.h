#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include "Thread.h"

class Heartbeat
{
public:
    Heartbeat();
    ~Heartbeat();

    void Start(unsigned int interval_msec);
    void Stop();

    static unsigned HeartbeatStartup(void* this_ptr)

private:
    volatile bool is_working_;
    Thread heartbeat_thread_;
};

#endif // !HEARTBEAT_H