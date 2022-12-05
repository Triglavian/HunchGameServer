#include "ListenSocket.h"

ListenSocket::ListenSocket(const Int& portNum)
{
	listenSocket = 0;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	ZeroMemory(&clientAddr, sizeof(clientAddr));
	cAddrSize = 0;
	result = 0;
	port = portNum;
}

ListenSocket::~ListenSocket()
{
	closesocket(listenSocket);
}

bool ListenSocket::IsInvalidSock()	//create and validate invalid socket creation error
{
	listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port.ToInt());	//linker error
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	cAddrSize = sizeof(serverAddr);
	return listenSocket == INVALID_SOCKET;
}

bool ListenSocket::IsUnbindedSocket()	//validate unbinded socket error 
{
	return bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR;
}

bool ListenSocket::IsListeningFailed()	//validate incommlete connection with client
{
	return listen(listenSocket, SOMAXCONN) == SOCKET_ERROR;
}

void ListenSocket::BindSocket() //bind socket to connect
{
	result = bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
}

void ListenSocket::ListenToConnection()	//keep listening until receive any client's connection
{
	result = listen(listenSocket, SOMAXCONN);
}

SOCKET ListenSocket::GetSocket()	//socket value returner
{
	return listenSocket;
}
