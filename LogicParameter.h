#pragma once

class ClientSocket;
class GameLoigc;

class LogicParameter {
public:
	GameLoigc* logic;
	ClientSocket* client;
};