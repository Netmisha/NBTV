#pragma once
#include "Chat.h"
#include "Network.h"


class AppManager
{
	Chat chat_;
	Network network_;

public:
	void Work();
	AppManager();
	~AppManager();
};

