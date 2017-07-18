#include "AppManager.h"


#include <iostream>
void AppManager::Work()
{
    network_.PrepareNetwork();
    std::thread network_loop(Network::StartNetwork, &network_);

    chat_.SetUserInfo(6, "Anton");
    chat_.Activate();
	
    chat_.GetInputThread().join();

	network_loop.detach();
	std::cout << "ok";
	//at the end
}


AppManager::~AppManager()
{

}

AppManager::AppManager()
{
    chat_.SetNetwork(&network_);
    network_.SetChat(&chat_);
}
