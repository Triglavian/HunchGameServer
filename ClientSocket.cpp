#include "ClientSocket.h"

ClientSocket::ClientSocket(const SOCKET& _socket, const sockaddr_in& _clientAddr)
{
	this->_socket = _socket;
	this->packetHandler = new PacketHandler();
	this->mainState = E_MAIN;
	this->clientAddr = _clientAddr;
	this->protocol = P_INVALID;

	char address[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientAddr.sin_addr, address, INET_ADDRSTRLEN);
	std::string strAddr = address;
	ServerLogger::PrintLog("Connected : Ip = " + strAddr + ", Port : " + Int(ntohs(clientAddr.sin_port)).ToString());
	//data = new ClientData();
}

ClientSocket::~ClientSocket()
{
	if (packetHandler != nullptr) delete packetHandler;
	char address[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientAddr.sin_addr, address, INET_ADDRSTRLEN);
	//if (data != nullptr) delete data;
	std::string strAddr = address;
	ServerLogger::PrintLog("Disconnected : Ip = " + strAddr + ", Port : " + Int(ntohs(clientAddr.sin_port)).ToString());
}

SOCKET ClientSocket::GetSocket()
{
	return _socket;
}

MainState ClientSocket::GetMainState()
{
	return mainState;
}
Protocol ClientSocket::GetProtocol()
{
	return protocol;
}

PlayerData ClientSocket::GetPlayerData()
{
	return data.data;
}

PlayerData& ClientSocket::GetPlayerDataRef()
{
	return data.data;
}

ClientData ClientSocket::GetClientData()
{
	return data;
}

ClientData& ClientSocket::GetClientDataRef()
{
	return data;
}

void ClientSocket::SetMainState(const MainState& _state)
{
	mainState = _state;
}
void ClientSocket::SetProtocol(const Protocol& _protocol)
{
	this->protocol = _protocol;
}

void ClientSocket::SetPlayerName(const std::string& name)
{
	data.data.userName = name;
}

void ClientSocket::SwitchState()
{
	if (packetHandler->RecvProtocolPacket(_socket, protocol) < 0) 
	{
		mainState = E_DISCON;
		return;
	}
	switch (protocol)
	{
		//logic
		case P_JOIN:
			mainState = E_JOIN;
			break;
		case P_DISCON:
			mainState = E_DISCON;
			break;
		default:
			packetHandler->SendProtocolPacket(_socket, P_INVALID);
			mainState = E_MAIN;
			break;
	}
}

bool ClientSocket::ValidateProtocol(const Protocol& protocol)
{
	return this->protocol == protocol;
}


Int ClientSocket::SendProtocolPacket(const Protocol& protocol) const
{
	return packetHandler->SendProtocolPacket(_socket, protocol);
}

Int ClientSocket::SendProtocolPacket() const
{
	return packetHandler->SendProtocolPacket(_socket, this->protocol);;
}
 
Int ClientSocket::SendBoolPacket(const Protocol& protocol, const bool& flag) const
{
	return packetHandler->SendBoolPacket(_socket, protocol, flag);
}

Int ClientSocket::SendCharPacket(const Protocol& protocol, const Char& _char) const
{
	return packetHandler->SendCharPacket(_socket, protocol, _char.ToChar());
}

Int ClientSocket::SendIntPacket(const Protocol& protocol, const Int& _int) const
{
	return packetHandler->SendIntPacket(_socket, protocol, _int.ToInt());
}

Int ClientSocket::SendStrPacket(const Protocol& protocol, const std::string& str) const
{
	return packetHandler->SendStrPacket(_socket, protocol, str);
}

Int ClientSocket::SendCStrPacket(const Protocol& protocol, const Char data[], const Int& size)
{
	return packetHandler->SendCStrPacket(_socket, protocol, data, size);
}

Int ClientSocket::SendDataPacket(const Protocol& protocol, const PlayerData& data)
{
	return packetHandler->SendDataPacket(_socket, protocol, data);
}

Int ClientSocket::RecvProtocolPacket()
{
	return packetHandler->RecvProtocolPacket(_socket, protocol);
}

Int ClientSocket::RecvBoolPacket(bool& data)
{
	return packetHandler->RecvBoolPacket(_socket, protocol, data);
}

Int ClientSocket::RecvCharPacket(Char& data)
{
	return packetHandler->RecvCharPacket(_socket, protocol, data.ToCharRef());
}

Int ClientSocket::RecvIntPacket(Int& data)
{
	return packetHandler->RecvIntPacket(_socket, protocol, data.ToIntRef());
}

Int ClientSocket::RecvStrPacket(std::string& data)
{
	return packetHandler->RecvStrPacket(_socket, protocol, data);
}

Int ClientSocket::RecvCStrPacket(Char* data, Int size)
{
	return packetHandler->RecvCStrPacket(_socket, protocol, data, size);
}