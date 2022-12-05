#pragma once

#include <list>
#include <vector>
#include <thread>
#include <ctime>

#include "LogicParameter.h"
#include "ClientSocket.h"	

class GameLogic
{
public:
	GameLogic(int maxClients = 5);
	~GameLogic();
	bool IsMaxClients();
	void AddClient(ClientSocket* client);
	bool JoinGame(ClientSocket* currentClient);
	//static unsigned int __stdcall CurrentClientEvent(ClientSocket* currentClient);
	//static unsigned int __stdcall OtherClientEvent(ClientSocket* currentClient);
private:
	int maxClients;
	std::list<ClientSocket*>* clients = nullptr;
	std::list<HANDLE>* threads;
	std::vector<HANDLE> events;	//0 : join, 1 : start, 2 : recv stream
	CRITICAL_SECTION inputSection;
	int currentCount;
	time_t interval;
	ClientSocket* client;

	void SendCurrentPlayerList(ClientSocket* currentClient);
	void WaitForOtherPlayers(ClientSocket* currentClient);
	bool GetPlayerName(ClientSocket* currentClient);
	bool BeginGame(ClientSocket* currentClient);
	static unsigned int __stdcall ListenToInput(void* _this);
	GameLogic(const GameLogic&) = delete;
};

