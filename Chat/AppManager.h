#pragma once

#include "Network.h"
#include "Chat.h"


class AppManager
{
	Chat chat_;
	Network network_;

public:
	void Work();
	AppManager();
	~AppManager();
};

