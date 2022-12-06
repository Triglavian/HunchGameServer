#pragma once
#include "Protocol.h"
#include "Packet.h"
#include <winsock2.h>
#include "Char.h"
#include "Int.h"
#include <cstring>
#include "ServerLogger.h"
#include "ClientData.h"

class PacketHandler
{
public:
	PacketHandler();
	~PacketHandler();
	
	//send packet
	int SendProtocolPacket(const SOCKET& _socket, const Protocol& protocol);
	int SendBoolPacket(const SOCKET& _socket, const Protocol& protocol, const bool& data);
	int SendCharPacket(const SOCKET& _socket, const Protocol& protocol, const char& data);
	int SendIntPacket(const SOCKET& _socket, const Protocol& protocol, const int& data);
	int SendStrPacket(const SOCKET& _socket, const Protocol& protocol, const std::string& data);
	int SendCStrPacket(const SOCKET& socket, const Protocol& protocol, const Char data[], const Int& size);
	int SendDataPacket(const SOCKET& _socket, const Protocol& protocol, const PlayerData& data);
	//receive packet
	int RecvProtocolPacket(const SOCKET& _socket, Protocol& protocol);
	int RecvBoolPacket(const SOCKET& _socket, Protocol& protocol, bool& data);
	int RecvCharPacket(const SOCKET& _socket, Protocol& protocol, char& data);
	int RecvIntPacket(const SOCKET& _socket, Protocol& protocol, int& data);
	int RecvStrPacket(const SOCKET& _socket, Protocol& protocol, std::string& data);
	int RecvCStrPacket(const SOCKET& socket, Protocol& protocol, Char* data, Int size);

private:
	Packet packet;
	void ClearPacket();
	template <class Data>
	int SendPacket(const SOCKET& _socket, const Protocol& protocol, const Data& data) ;
	template <class Data>
	int RecvPacket(const SOCKET& _socket, Protocol& protocol, Data& data);
};

template<class Data>
inline int PacketHandler::SendPacket(const SOCKET& _socket, const Protocol& protocol, const Data& data)  
{
	ClearPacket();
	char* ptr = packet.buf + sizeof(int);
	int packetSize = 0;
	int dataSize = sizeof(data);

	memcpy(ptr, &protocol, sizeof(protocol));
	ptr += sizeof(protocol);
	packetSize += sizeof(protocol);

	memcpy(ptr, &dataSize, sizeof(dataSize));
	ptr += sizeof(dataSize);
	packetSize += sizeof(dataSize);

	memcpy(ptr, &data, dataSize);
	packetSize += dataSize;

	ptr = packet.buf;
	packet.packetSize = packetSize;
	memcpy(ptr, &packetSize, sizeof(packetSize));

	return send(_socket, packet.buf, packet.packetSize + sizeof(int), 0);

}

template<class Data>
inline int PacketHandler::RecvPacket(const SOCKET& _socket, Protocol& protocol, Data& data)
{
	ClearPacket();
	int retval = 0;
	int packetSize = 0;
	retval = recv(_socket, (char*)&packetSize, sizeof(int), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		ServerLogger::PrintLog("recv() error");
		return -1;
	}
	retval = recv(_socket, packet.buf, packetSize, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		ServerLogger::PrintLog("recv() error");
		return -1;
	}
	char* ptr = packet.buf;

	memset(&data, 0, sizeof(data));
	memcpy(&protocol, ptr, sizeof(protocol));
	ptr += sizeof(protocol);

	int dataSize = 0;
	memcpy(&dataSize, ptr, sizeof(dataSize));
	ptr += sizeof(dataSize);

	memcpy(&data, ptr, dataSize);
	return retval;
}