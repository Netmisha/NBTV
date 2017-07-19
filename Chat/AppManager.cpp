#include "AppManager.h"

#include <iostream>
void AppManager::Work()
{
    network_.PrepareNetwork();
    Thread network_loop(Network::StartNetwork, &network_);

    chat_.SetUserInfo(6, "Anton");
    chat_.Activate();
	
    chat_.GetInputThread().Join();

	std::cout << "ok";
	//at the end
}


AppManager::~AppManager(){}

AppManager::AppManager()
{
    chat_.SetNetwork(&network_);
    network_.SetChat(&chat_);
}
