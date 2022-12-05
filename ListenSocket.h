#pragma once
#include <WinSock2.h>
#include "Int.h"

class ListenSocket
{
public:	
	ListenSocket(const Int& portNum = 9000);
	~ListenSocket();
	bool IsInvalidSock();		//create and validate invalid socket creation error
	bool IsUnbindedSocket();	//validate unbinded socket error 
	bool IsListeningFailed();	//validate incommlete connection with client
	SOCKET GetSocket();			//socket value returner
private:
	SOCKET listenSocket;
	sockaddr_in serverAddr;
	sockaddr_in clientAddr;
	Int cAddrSize;
	Int result;
	Int port;
	void BindSocket();			//bind socket to connect
	void ListenToConnection();	//keep listening until receive any client's connection
	ListenSocket(const ListenSocket& listenSocket) = delete;	//don't use
};

