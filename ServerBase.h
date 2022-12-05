#pragma once
//#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <process.h>

#include <functional>
#include <vector>	
#include <list>

#include "ListenSocket.h"
#include "ClientSocket.h"
#include "ServerLogger.h"

#include "Char.h"
#include "Int.h"
#include "Float.h"
#include "Double.h"
#include "GameLogic.h"

class ServerBase
{
public:
	ServerBase(Int port = 9000);
	~ServerBase();
	void Run();
private:
	//base data
	WSADATA wsa;
	Int port;
	Int result;
	ListenSocket* lSocket;
	SOCKET listenSocket;
	sockaddr_in clientAddr;
	std::vector<HANDLE*>* threadHandles;
	std::vector<ClientSocket*>* cSockets;	
	std::list<GameLogic*>* gameLogics;
	CRITICAL_SECTION handleSection;
	CRITICAL_SECTION cSocketSection;
	CRITICAL_SECTION gameSection;

	//base function
	bool InitializeServer();
	bool IsWSAStartFailed();
	void AcceptClients();

	//service
	static unsigned int __stdcall StateSwitch(void* obj);
};