#include "AppManager.h"

#include <iostream>
void AppManager::Work()
{
    network_.PrepareNetwork();
    Thread network_loop(Network::StartNetwork, &network_);

    chat_.SetUserInfo(6, "Anton");
    chat_.Activate();
	
    chat_.GetInputThread().Join();

    network_.StopNetwork();
    chat_.IOfflineMsg();
    network_loop.Join();


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
    chat_.SetFM(&fm_);
    network_.SetFM(&fm_);
}
