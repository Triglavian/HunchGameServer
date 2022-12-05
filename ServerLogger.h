#pragma once
#include <iostream>

class ServerLogger
{
public:
	static void PrintLog(const std::string& msg);
private:
	ServerLogger() = delete;
	ServerLogger(const ServerLogger&) = delete;
	~ServerLogger() = delete;
};

