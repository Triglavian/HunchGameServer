#pragma once

enum class PlayerState 
{
	IDLE,
	MYINPUT,
	OTHERINPUT,
	RESULT,
	QUIT,

#define E_IDLE		PlayerState::IDLE
#define E_MYIN		PlayerState::MYINPUT
#define E_OTHERIN	PlayerState::OTHERINPUT
#define E_RESULT	PlayerState::RESULT
#define E_QUIT		PlayerState::QUIT
};