#pragma once

enum class Protocol
{
	DISCONNECTION = 0,	//disconnection protocol, recivec if opps socket is closed
	INVALIDPROT,
	PLAYERNAME,
	PLAYERCOUNT,
	PLAYERDATA,
	JOIN,
	WAIT,
	START,
	TURN,

#define P_DISCON	Protocol::DISCONNECTION
#define P_INVALID	Protocol::INVALIDPROT
#define P_COUNT		Protocol::PLAYERCOUNT
#define P_NAME		Protocol::PLAYERNAME
#define P_DATA		Protocol::PLAYERDATA
#define P_JOIN		Protocol::JOIN
#define P_WAIT_		Protocol::WAIT
#define P_START		Protocol::START
#define P_TURN		Protocol::TURN
};