#include "PacketHandler.h"

PacketHandler::PacketHandler()
{

}

PacketHandler::~PacketHandler()
{

}

int PacketHandler::SendProtocolPacket(const SOCKET& _socket, const Protocol& protocol) 
{
	ClearPacket();
	char* ptr = packet.buf + sizeof(int);
	int packetSize = 0;

	memcpy(ptr, &protocol, sizeof(protocol));
	ptr = packet.buf;
	packetSize += sizeof(protocol);

	memcpy(ptr, &packetSize, sizeof(packetSize));
	packetSize += sizeof(packetSize);
	return send(_socket, packet.buf, packetSize, 0);
}

int PacketHandler::SendBoolPacket(const SOCKET& _socket, const Protocol& protocol, const bool& data) 
{
	return SendPacket(_socket, protocol, data);
}

int PacketHandler::SendCharPacket(const SOCKET& _socket, const Protocol& protocol, const char& data) 
{
	return SendPacket(_socket, protocol, data);
}

int PacketHandler::SendIntPacket(const SOCKET& _socket, const Protocol& protocol, const int& data) 
{
	return SendPacket(_socket, protocol, data);
}

int PacketHandler::SendStrPacket(const SOCKET& _socket, const Protocol& protocol, const std::string& data) 
{
	ClearPacket();
	char* ptr = packet.buf + sizeof(int);
	int packetSize = 0;
	int dataSize = data.size();

	memcpy(ptr, &protocol, sizeof(protocol));
	ptr += sizeof(protocol);
	packetSize += sizeof(protocol);

	memcpy(ptr, &dataSize, sizeof(dataSize));
	ptr += sizeof(dataSize);
	packetSize += sizeof(dataSize);

	memcpy(ptr, data.c_str(), dataSize);
	packetSize += dataSize;

	ptr = packet.buf;
	packet.packetSize = packetSize;
	memcpy(ptr, &packetSize, sizeof(packetSize));

	return send(_socket, packet.buf, packet.packetSize + sizeof(int), 0);
}

int PacketHandler::SendCStrPacket(const SOCKET& socket, const Protocol& protocol, const Char data[], const Int& size)
{
	char* ptr = packet.buf + sizeof(int);
	int packetSize = 0;

	memcpy(ptr, &protocol, sizeof(protocol));
	ptr += sizeof(protocol);
	packetSize += sizeof(protocol);

	memcpy(ptr, &size, sizeof(size));
	ptr += sizeof(size);
	packetSize += sizeof(size);

	memcpy(ptr, data, size.ToInt());
	packetSize += size.ToInt();

	ptr = packet.buf;
	packet.packetSize = packetSize;
	memcpy(ptr, &packetSize, sizeof(packetSize));

	return send(socket, packet.buf, packet.packetSize + sizeof(int), 0);
}

int PacketHandler::SendDataPacket(const SOCKET& _socket, const Protocol& protocol, const PlayerData data)
{
	return SendPacket(_socket, protocol, data);
}

int PacketHandler::RecvProtocolPacket(const SOCKET& _socket, Protocol& protocol)
{
	ClearPacket();
	int retval = 0;
	int packetSize = 0;
	retval = recv(_socket, (char*)&packetSize, sizeof(int), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		ServerLogger::PrintLog("recv() error");
		return SOCKET_ERROR;
	}
	retval = recv(_socket, packet.buf, packetSize, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		ServerLogger::PrintLog("recv() error");
		return SOCKET_ERROR;
	}
	memcpy(&protocol, packet.buf, sizeof(protocol));
	return retval;
}

int PacketHandler::RecvBoolPacket(const SOCKET& _socket, Protocol& protocol, bool& data)
{
	return RecvPacket(_socket, protocol, data);
}

int PacketHandler::RecvCharPacket(const SOCKET& _socket, Protocol& protocol, char& data)
{
	return RecvPacket(_socket, protocol, data);
}

int PacketHandler::RecvIntPacket(const SOCKET& _socket, Protocol& protocol, int& data)
{
	return RecvPacket(_socket, protocol, data);
}

int PacketHandler::RecvStrPacket(const SOCKET& _socket, Protocol& protocol, std::string& data)
{
	ClearPacket();
	int retval = 0;
	int packetSize = 0;
	retval = recv(_socket, (char*)&packetSize, sizeof(int), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		ServerLogger::PrintLog("recv() error");
		return SOCKET_ERROR;
	}
	retval = recv(_socket, packet.buf, packetSize, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		ServerLogger::PrintLog("recv() error");
		return SOCKET_ERROR;
	}
	char* ptr = packet.buf;
	data.clear();

	memcpy(&protocol, ptr, sizeof(protocol));
	ptr += sizeof(protocol);

	int dataSize = 0;
	memcpy(&dataSize, ptr, sizeof(dataSize));
	ptr += sizeof(dataSize);

	//memcpy(data, ptr, dataSize);
	data.copy(ptr, dataSize);
	//memcpy(&data, )
	data = ptr;
	memcpy(&data, ptr, dataSize);
	ptr += dataSize;
	return retval;
}

int PacketHandler::RecvCStrPacket(const SOCKET& socket, Protocol& protocol, Char* data, Int size)
{
	int retval = 0;
	int packetSize = 0;
	retval = recv(socket, (char*)&packetSize, sizeof(int), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		ServerLogger::PrintLog("recv() error");
		return SOCKET_ERROR;
	}
	retval = recv(socket, packet.buf, packetSize, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		ServerLogger::PrintLog("recv() error");
		return SOCKET_ERROR;
	}
	char* ptr = packet.buf;

	memset(data, 0, sizeof(*data));
	memcpy(&protocol, ptr, sizeof(protocol));
	ptr += sizeof(protocol);

	int dataSize = 0;
	memcpy(&dataSize, ptr, sizeof(dataSize));
	ptr += sizeof(dataSize);

	memcpy(data, ptr, dataSize);
	return retval;
}

void PacketHandler::ClearPacket()
{
	ZeroMemory(packet.buf, BUFSIZE);
}
