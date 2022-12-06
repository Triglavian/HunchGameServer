#pragma once
#include "PlayerData.h"
#include "PlayerState.h"

struct ClientData
{
	PlayerData data;
	PlayerState	state = E_IDLE;
	time_t inputTime = 0;
};

