#include "GameLogic.h"

bool GameLogic::gameFlag = false;

GameLogic::GameLogic(int maxClients)
{
	this->maxClients = maxClients;
	threads = new std::list<HANDLE>();
	events[EV_JOIN] = CreateEvent(0, 0, 0, 0);	//join evnet
	events[EV_START] = CreateEvent(0, 0, 0, 0);	//start event
	events[EV_INPUT] = CreateEvent(0, 0, 0, 0);	//recv stream
	events[EV_MYIN] = CreateEvent(0, 0, 0, 0);	//myinput stream
	events[EV_OTHERIN] = CreateEvent(0, 0, 0, 0);	//otherinput stream
	currentCount = 0;
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
		IsMaxClients() ? SetEvent(events[EV_START]) : SetEvent(events[EV_JOIN]);
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
		WaitForSingleObject(events[EV_JOIN], INFINITE);
	}
	WaitForSingleObject(events[EV_START], INFINITE);
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
	HANDLE input = (HANDLE)_beginthreadex(0, 0, ListenToInput, this, 0, 0);
	bool flag = true;
	while (flag)
	{
		//WaitForMultipleObjects(3, events, false, INFINITE);
		WaitForSingleObject(events[EV_INPUT], INFINITE);
		switch (currentClient->GetClientData().state) 
		{
			case E_IDLE:
				break;
			case E_MYIN:
				WaitForSingleObject(events[EV_MYIN], INFINITE);
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
	CloseHandle(input);
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
		SetEvent(events[EV_INPUT]);
		//SetEvent(events[EV_OTHERIN]);
	}
}

bool GameLogic::IsGameOver()
{
	GameLogic::gameFlag;
}

void GameLogic::SendResult(ClientSocket* currentClient)
{
	Protocol protocol = IsGameOver() ? P_GAMEOVER : P_ENDTURN;
	currentClient->SendDataPacket(protocol, currentClient->GetPlayerData());
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
	
	while (true) 
	{	
		if (logic->client->RecvProtocolPacket() == SOCKET_ERROR) break;	//escape if disconnected
		if (!logic->client->ValidateProtocol(P_TURN)) 
		{
			std::cout << "invalid protocol exit" << std::endl;
			break;
		}
		logic->client->GetPlayerDataRef().isTurnOver = true;
		logic->client->GetClientDataRef().state = E_MYIN;
		time(&logic->client->GetClientDataRef().inputTime);
		for (auto client : *logic->clients)
		{
			if (client == logic->client) continue;
			if (abs(client->GetClientData().inputTime - logic->client->GetClientData().inputTime) < 10) 
			{
				client->GetPlayerDataRef().isGameOver = logic->client->GetPlayerDataRef().isGameOver = true;
				logic->gameFlag = true;
			}
		}
		for (auto client : *logic->clients) 
		{
			if (logic->client != client) 
			{
				client->GetClientDataRef().state = E_IDLE;
			}
		}
		for (auto client : *logic->clients) 
		{
			SetEvent(logic->events[EV_INPUT]);
		}
		SetEvent(logic->events[EV_MYIN]);
	}
	return 0;
}
