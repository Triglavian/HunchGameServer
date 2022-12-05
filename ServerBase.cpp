#include "ServerBase.h"

ServerBase::ServerBase(Int port)
{
	result = WSAStartup(MAKEWORD(2, 2), &wsa);
	this->port = port;
	threadHandles = new std::vector<HANDLE*>();
	lSocket = new ListenSocket(this->port);
	listenSocket = 0;
	ZeroMemory(&clientAddr, sizeof(clientAddr));
	cSockets = new std::vector<ClientSocket*>();
	gameLogics = new std::list<GameLogic*>();
	GameLogic* game = new GameLogic();
	gameLogics->push_back(game);
	InitializeCriticalSection(&handleSection);
	InitializeCriticalSection(&cSocketSection);
	InitializeCriticalSection(&gameSection);
}

ServerBase::~ServerBase()
{
	if (threadHandles != nullptr) 
	{
		for (auto obj : *threadHandles)
		{
			CloseHandle(obj);
		}
		delete threadHandles;
	}
	if (cSockets != nullptr) 
	{
		for (auto* obj : *cSockets) 
		{
			delete obj;
		}
		delete cSockets;
	}
	if (gameLogics != nullptr) 
	{
		for (auto* obj : *gameLogics) 
		{
			delete obj;
		}
		delete gameLogics;
	}
	if (lSocket != nullptr) delete lSocket;
	DeleteCriticalSection(&handleSection);
	DeleteCriticalSection(&cSocketSection);
	DeleteCriticalSection(&gameSection);
}

void ServerBase::Run()
{
	if (!InitializeServer())
	{
		//error handler
		return;
	}
	AcceptClients();
}

bool ServerBase::InitializeServer()
{
	if (IsWSAStartFailed()) 
	{
		//error handler
		ServerLogger::PrintLog("Failed to start up wsa");
		return false;
	}
	if (lSocket->IsInvalidSock()) 
	{
		//error handler
		ServerLogger::PrintLog("Failed to init listen socket");
		return false;
	}
	if (lSocket->IsUnbindedSocket()) 
	{
		//error handler
		ServerLogger::PrintLog("Failed to bind listen socket");
		return false;
	}
	if (lSocket->IsListeningFailed())
	{
		//error handler
		ServerLogger::PrintLog("Failed to listen connection");
		return false;
	}
	return true;
}

bool ServerBase::IsWSAStartFailed()
{
	return result != 0;
}

void ServerBase::AcceptClients()
{
	int addrLen = sizeof(clientAddr);
	HANDLE threadHandle;
	ZeroMemory(&clientAddr, sizeof(clientAddr));
 	while (true) 
	{
		listenSocket = accept(lSocket->GetSocket(), (sockaddr*)&clientAddr, &addrLen);
		EnterCriticalSection(&handleSection);
		threadHandles->emplace_back(&threadHandle);
		threadHandle = (HANDLE)_beginthreadex(0, 0, ServerBase::StateSwitch, this, 0, 0);
		threadHandles->erase(std::find(threadHandles->begin(), threadHandles->end(), &threadHandle));
		LeaveCriticalSection(&handleSection);
		CloseHandle(threadHandle);
	}
	//WaitForMultipleObjects(threadHandles->size(), , 1, INFINITE);
}

unsigned int __stdcall ServerBase::StateSwitch(void* obj)
{
	ServerBase* server = static_cast<ServerBase*>(obj);
	EnterCriticalSection(&server->cSocketSection);
	EnterCriticalSection(&server->gameSection);
	ClientSocket* cSocket = new ClientSocket(server->listenSocket, server->clientAddr);
	server->cSockets->emplace_back(cSocket);
	if (server->gameLogics->back()->IsMaxClients()) 
	{
		server->gameLogics->emplace_back(new GameLogic());
	}
	server->gameLogics->back()->AddClient(cSocket);
	LeaveCriticalSection(&server->gameSection);
	LeaveCriticalSection(&server->cSocketSection);
	bool flag = true;
	while (flag)
	{
		switch (cSocket->GetMainState()) 
		{
			case E_MAIN:
				cSocket->SwitchState();
				break;
			case E_JOIN:

				break;
			case E_DISCON:
				flag = false;
				break;
		}
	}
	EnterCriticalSection(&server->cSocketSection);
	server->cSockets->erase(std::find(server->cSockets->begin(), server->cSockets->end(), cSocket));
	LeaveCriticalSection(&server->cSocketSection);
	delete cSocket;
	return 0;
}
