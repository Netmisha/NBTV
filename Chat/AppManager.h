#pragma once

#include "Network.h"
#include "Chat.h"
#include "FileManager.h"

class AppManager
{
	Chat chat_;
	Network network_;
    FileManager fm_;

public:
	void Work();
	AppManager();
	~AppManager();
};

