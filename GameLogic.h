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
	GameLogic(int maxClients = 2);
	~GameLogic();
	bool IsMaxClients();
	void AddClient(ClientSocket* client);
	bool JoinGame(ClientSocket* currentClient);
	void WaitForOtherPlayers(ClientSocket* currentClient);
	//static unsigned int __stdcall CurrentClientEvent(ClientSocket* currentClient);
	//static unsigned int __stdcall OtherClientEvent(ClientSocket* currentClient);
private:
//	enum class EventId 
//	{
//		JOIN,
//		BEGIN,
//		INPUT,
//		MYIN,
//		OTHERIN,
//		MAX
//#define EV_JOIN (int)EventId::JOIN
//#define EV_START (int)EventId::BEGIN
//#define EV_INPUT (int)EventId::INPUT
//#define EV_MYIN (int)EventId::MYIN
//#define EV_OTHERIN (int)EventId::OTHERIN
//	};
	int maxClients;
	std::list<ClientSocket*>* clients = nullptr;
	std::list<HANDLE>* threads;
	HANDLE events[2];
	CRITICAL_SECTION cs;
	//CRITICAL_SECTION inputSection;
	ClientSocket* client;
	int delay = 20;
	static bool gameFlag;

	//myinput
	void SendPlayerList(ClientSocket* currentClient);
	bool GetPlayerName(ClientSocket* currentClient);
	bool BeginGame(ClientSocket* currentClient);
	bool ValidateClientInput(ClientSocket* currentClient);
	void ResetState(ClientSocket* currentClient);
	bool IsAlreadyOver(ClientSocket* currentClient);
	void UpFlag(ClientSocket* currentClient);
	void UpdateClientDatas(ClientSocket* currentClient);
	void SetAllClientEvent(ClientSocket* currentClient);

	//result
	bool IsGameOver();
	void SendResult(ClientSocket* currentClient);
	bool Disconnect(ClientSocket* currentClient);

	static unsigned int __stdcall ListenToInput(void* _this);
	GameLogic(const GameLogic&) = delete;
};