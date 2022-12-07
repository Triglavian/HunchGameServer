#include "GameLogic.h"

bool GameLogic::gameFlag = false;

GameLogic::GameLogic(int maxClients)
{
	this->maxClients = maxClients;
	clients = new std::list<ClientSocket*>();
	threads = new std::list<HANDLE>();
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
	WaitForSingleObject(events[currentClient->GetClientData().clientIndex], INFINITE);
	currentClient->SendProtocolPacket(P_START);
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
	return true;
}

bool GameLogic::BeginGame(ClientSocket* currentClient)
{
	HANDLE networking = (HANDLE)_beginthreadex(0, 0, ListenToInput, this, 0, 0);
	bool flag = true;
	SendResult(currentClient);
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
				UpFlag(currentClient);
				SetOtherClientEvent(currentClient);
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

void GameLogic::SetOtherClientEvent(ClientSocket* currentClient)
{
	for (auto client : *clients)
	{
		if (client == currentClient) continue;
		client->GetClientDataRef().state = E_RESULT;
		SetEvent(events[currentClient->GetClientData().clientIndex]);
		//SetEvent(events[EV_OTHERIN]);
	}
}

bool GameLogic::IsGameOver()
{
	return GameLogic::gameFlag;
}

void GameLogic::SendResult(ClientSocket* currentClient)
{
	Protocol protocol = IsGameOver() ? P_GAMEOVER : P_ENDTURN;
	for (auto client : *clients) 
	{
		currentClient->SendDataPacket(protocol, client->GetPlayerData());
	}
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
		if (_client->RecvProtocolPacket() == SOCKET_ERROR) break;	//escape if disconnected
		if (!_client->ValidateProtocol(P_TURN)) 
		{
			std::cout << "invalid protocol exit" << std::endl;
			break;
		}
		_client->GetPlayerDataRef().isTurnOver = true;
		_client->GetClientDataRef().state = E_MYIN;
		time(&_client->GetClientDataRef().inputTime);
		for (auto client : *logic->clients)
		{
			if (client == _client) continue;
			if (abs(client->GetClientData().inputTime - _client->GetClientData().inputTime) < 10) 
			{
				client->GetPlayerDataRef().isGameOver = _client->GetPlayerDataRef().isGameOver = true;
				logic->gameFlag = true;
			}
		}
		for (auto client : *logic->clients) 
		{
			if (_client != client) 
			{
				client->GetClientDataRef().state = E_IDLE;
			}
		}
		for (auto client : *logic->clients) 
		{
			SetEvent(logic->events[_client->GetClientData().clientIndex]);
		}
		SetEvent(logic->events[_client->GetClientData().clientIndex]);
	}
	return 0;
}
