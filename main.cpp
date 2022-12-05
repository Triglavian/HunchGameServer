#include "ServerBase.h"
#include <ctime>
int main(int argc, char* argv[])
{
	ServerBase* server = new ServerBase();
	server->Run();
	if (server != nullptr) delete server;
}