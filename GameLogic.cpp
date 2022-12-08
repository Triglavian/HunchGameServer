#include "GameLogic.h"

bool GameLogic::gameFlag = false;

GameLogic::GameLogic(int maxClients)
{
	this->maxClients = maxClients;
	clients = new std::list<ClientSocket*>();
	threads = new std::list<HANDLE>();	
	InitializeCriticalSection(&cs);
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
	DeleteCriticalSection(&cs);
}

bool GameLogic::IsMaxClients()
{
	return (clients->size() > 1);
}

void GameLogic::AddClient(ClientSocket* client)
{
	static int index = 0;
	client->GetClientDataRef().clientIndex = index;
	clients->push_back(client);
	events[index++] = CreateEvent(0, 0, 0, 0);	//join evnet
	this->client = client;
	//if (IsMaxClients()) 
	//{
	//	for (auto ev : events)
	//	{
	//		SetEvent(ev);
	//	}
	//}
	client->SetMainState(E_JOIN);
}

bool GameLogic::JoinGame(ClientSocket* currentClient)
{
	//ClientSocket* _currentClient = currentClient;
	//if (!GetPlayerName(_currentClient))
	//{
	//	return false;
	//}	
	//return true;
	return GetPlayerName(currentClient);
}

void GameLogic::SendPlayerList(ClientSocket* currentClient)
{
	std::cout << "Send list to " + currentClient->GetPlayerData().userName << std::endl;
	for (auto client : *clients)
	{
		currentClient->SendDataPacket(P_DATA, client->GetPlayerData());
	}
}

void GameLogic::WaitForOtherPlayers(ClientSocket* currentClient)
{
	//currentClient->SendProtocolPacket(P_WAIT_);
	//while (!IsMaxClients())
	//{
	//	WaitForSingleObject(events[currentClient->GetClientData().clientIndex], INFINITE);
	//}
	if (IsMaxClients()) 
	{
		for (int i = 0; i < maxClients; i++) 
		{
			SetEvent(events[i]);
		}
	}
	if (maxClients > 1) WaitForSingleObject(events[currentClient->GetClientData().clientIndex], INFINITE);
	currentClient->SendProtocolPacket(P_START);
	SendPlayerList(currentClient);
	BeginGame(currentClient);
}

bool GameLogic::GetPlayerName(ClientSocket* currentClient)
{
	//std::string name = "";
	//if (currentClient->RecvStrPacket(name) == SOCKET_ERROR) 
	//{
	//	return false;
	//}
	//if (!currentClient->ValidateProtocol(P_NAME)) 
	//{
	//	return false;
	//}
	//currentClient->SetPlayerName(name);
	if (currentClient->RecvStrPacket(currentClient->GetPlayerDataRef().userName) == SOCKET_ERROR) 
	{
		return false;
	}
	if (!currentClient->ValidateProtocol(P_NAME)) 
	{
		return false;
	}
	std::cout << currentClient->GetPlayerData().userName + " Joined" << std::endl;
	return true;
}

bool GameLogic::BeginGame(ClientSocket* currentClient)
{
	std::cout << currentClient->GetPlayerData().userName + " just started Game" << std::endl;
	HANDLE networking = (HANDLE)_beginthreadex(0, 0, ListenToInput, this, 0, 0);
	bool flag = true;
	while (flag)
	{
		//WaitForMultipleObjects(3, events, false, INFINITE);
		WaitForSingleObject(events[currentClient->GetClientData().clientIndex], INFINITE);
		switch (currentClient->GetClientData().state) 
		{
			case E_IDLE:
				break;
			case E_MYIN:
				//WaitForSingleObject(events[currentClient->GetClientData().clientIndex], INFINITE);
				if (!ValidateClientInput(currentClient)) 
				{
					break;
				}
				UpdateClientDatas(currentClient);
				SetAllClientEvent(currentClient);
				break;
			//case E_OTHERIN:

			//	break;
			case E_RESULT:
				//WaitForSingleObject(events[EV_OTHERIN], INFINITE);
				SendResult(currentClient);
				if (IsGameOver()) 
				{
					currentClient->GetClientDataRef().state = E_QUIT;
				}
				break;
			case E_QUIT:
				if (Disconnect(currentClient)) 
				{
					flag = false;
				}
				break;
		}
	}
	CloseHandle(networking);
	return true;
}

bool GameLogic::ValidateClientInput(ClientSocket* currentClient)
{
	if (IsAlreadyOver(currentClient))
	{
		ResetState(currentClient);
		return false;
	}
	return true;
}

void GameLogic::ResetState(ClientSocket* currentClient)
{
	currentClient->GetClientDataRef().state = E_IDLE;
}

bool GameLogic::IsAlreadyOver(ClientSocket* currentClient)
{
	return currentClient->GetClientData().data.isTurnOver;
}

void GameLogic::UpFlag(ClientSocket* currentClient)
{
	currentClient->GetClientDataRef().data.isTurnOver = true;
	currentClient->GetClientDataRef().state = E_RESULT;
}

void GameLogic::UpdateClientDatas(ClientSocket* currentClient)
{
	EnterCriticalSection(&cs);
	static int count = 0;
	count++;
	UpFlag(currentClient);
	if (count >= maxClients) 
	{
		currentClient->GetClientDataRef().data.isGameOver = true;
		gameFlag = true;
		LeaveCriticalSection(&cs);
		return;
	}
	UpFlag(currentClient);
	for (auto client : *clients)
	{
		if (client == currentClient) continue;
		client->GetClientDataRef().state = E_RESULT;
		if (abs(client->GetClientData().inputTime - currentClient->GetClientData().inputTime) < delay)
		{
			client->GetClientDataRef().data.isGameOver = currentClient->GetClientDataRef().data.isGameOver = true;
			gameFlag = true;
		}
	}
	LeaveCriticalSection(&cs);
}

void GameLogic::SetAllClientEvent(ClientSocket* currentClient)
{
	for (auto client : *clients)
	{
		client->GetClientDataRef().state = E_RESULT;
		SetEvent(events[currentClient->GetClientData().clientIndex]);
		//SetEvent(events[EV_OTHERIN]);
	}
}

bool GameLogic::IsGameOver()
{
	return gameFlag;
}

void GameLogic::SendResult(ClientSocket* currentClient)
{
	Protocol protocol = IsGameOver() ? P_GAMEOVER : P_ENDTURN;
	currentClient->SendProtocolPacket(protocol);
	SendPlayerList(currentClient);
	SetEvent(events[currentClient->GetClientData().clientIndex]);
}

bool GameLogic::Disconnect(ClientSocket* currentClient)
{
	return currentClient->RecvProtocolPacket() == SOCKET_ERROR;
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
	ClientSocket* _client = logic->client;
	while (true) 
	{	
		if (_client->RecvProtocolPacket() == SOCKET_ERROR)
		{
			break;	//escape if disconnected
		}
		if (!_client->ValidateProtocol(P_TURN)) 
		{
			std::cout << "invalid protocol exit" << std::endl;
			break;
		}
		_client->GetClientDataRef().state = E_MYIN;
		time(&_client->GetClientDataRef().inputTime);
		//_client->GetPlayerDataRef().isTurnOver = true;
		//for (auto client : *logic->clients)
		//{
		//	if (client == _client) continue;
		//	if (abs(client->GetClientData().inputTime - _client->GetClientData().inputTime) < 10) 
		//	{
		//		client->GetPlayerDataRef().isGameOver = _client->GetPlayerDataRef().isGameOver = true;
		//		logic->gameFlag = true;
		//	}
		//}
		//for (auto client : *logic->clients) 
		//{
		//	if (_client != client) 
		//	{
		//		client->GetClientDataRef().state = E_IDLE;
		//	}
		//}
		//for (auto client : *logic->clients) 
		//{
		//	SetEvent(logic->events[_client->GetClientData().clientIndex]);
		//}
		SetEvent(logic->events[_client->GetClientData().clientIndex]);
	}
	return 0;
}
