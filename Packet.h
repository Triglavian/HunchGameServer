#pragma once

#include <iostream>

class Packet
{
public:
	friend class PacketHandler;
	Packet();
	~Packet();
private:
#define BUFSIZE 4096
	char buf[BUFSIZE];
	int packetSize;
};