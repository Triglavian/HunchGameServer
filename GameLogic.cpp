#include "GameLogic.h"

GameLogic::GameLogic(int maxClients)
{
	this->maxClients = maxClients;
	threads = new std::list<HANDLE>();
	events.emplace_back(CreateEvent(0, 0, 0, 0));	//join evnet
	events.emplace_back(CreateEvent(0, 0, 0, 0));	//start event
	events.emplace_back(CreateEvent(0, 0, 0, 0));	//recv stream
	currentCount = 0;
	interval = 5;
}

GameLogic::~GameLogic()
{
	if (clients != nullptr) delete clients;
	if (threads != nullptr) 
	{
		while (!threads->empty())
		{
			CloseHandle(threads->back());
			threads->pop_back();
		}
		delete threads;
	}
}

bool GameLogic::IsMaxClients()
{
	return (clients->size() > 1);
}

void GameLogic::AddClient(ClientSocket* client)
{
	clients->push_back(client);
	this->client = client;
	for (auto client : *clients) 
	{
		IsMaxClients() ? SetEvent(events[1]) : SetEvent(events[0]);
	}
}

bool GameLogic::JoinGame(ClientSocket* currentClient)
{
	ClientSocket* _currentClient = currentClient;
	if (!GetPlayerName(_currentClient))
	{
		return false;
	}
	WaitForOtherPlayers(_currentClient);
	return BeginGame(currentClient);
}

void GameLogic::SendCurrentPlayerList(ClientSocket* currentClient)
{
	std::vector<PlayerData>* data = new std::vector<PlayerData>();
	for (auto client : *clients) 
	{
		data->push_back(client->GetPlayerData());
	}
	currentClient->SendIntPacket(P_COUNT, data->size());
	for (auto _data : *data) 
	{
		currentClient->SendDataPacket(P_DATA, _data);
	}
	if (data != nullptr) delete data;
}

void GameLogic::WaitForOtherPlayers(ClientSocket* currentClient)
{
	currentClient->SendProtocolPacket(P_WAIT_);
	while (!IsMaxClients())
	{
		WaitForSingleObject(events[0], INFINITE);
	}
	WaitForSingleObject(events[1], INFINITE);
	currentClient->SendProtocolPacket(P_START);
}

bool GameLogic::GetPlayerName(ClientSocket* currentClient)
{
	std::string name;
	if (currentClient->RecvStrPacket(name) == SOCKET_ERROR) 
	{
		return false;
	}
	if (!currentClient->ValidateProtocol(P_NAME)) 
	{
		return false;
	}
	currentClient->SetPlayerName(name);
	return true;
}

bool GameLogic::BeginGame(ClientSocket* currentClient)
{
	
}

unsigned int __stdcall GameLogic::ListenToInput(void* _this)
{
	/*
	LogicParameter* paramInstance = static_cast<LogicParameter*>(_this);
	GameLogic* logicInst = paramInstance->logic;	//#1
	GameLogic* logicInst = static_cast<GameLogic*>(paramInstance->logic);	//#2
	GameLogic* logicInst = static_cast<GameLogic*>(static_cast<LogicParameter*>(_this)->logic);	//#3
	*/
	//LogicParameter* paramInstance = static_cast<LogicParameter*>(_this);
	//GameLoigc* logic_ = paramInstance->logic;
	//ClientSocket* client = paramInstance->client;
	GameLogic* logic = static_cast<GameLogic*>(_this);
	
	while (true) 
	{	
		if (logic->client->RecvProtocolPacket() == SOCKET_ERROR) break;	//escape if disconnected
		if (!logic->client->ValidateProtocol(P_TURN)) 
		{
			std::cout << "invalid protocol exit" << std::endl;
			break;
		}
		logic->client->GetPlayerDataRef().isOver = true;
		logic->client->GetClientDataRef().state = E_MYIN;
		for (auto client : *logic->clients) 
		{
			if (logic->client != client) 
			{
				client->GetClientDataRef().state = E_OTHERIN;
			}
		}
		SetEvent(logic->events[2]);
	}
}
