#include "Packet.h"

Packet::Packet()
{
	memset(buf, NULL, BUFSIZE);
	packetSize = 0;
}

Packet::~Packet()
{

}
