#pragma once
enum class MainState 
{
	E_MAIN,
	E_JOIN,
	E_EXIT,
	E_DISCONNECTED,

#define E_MAIN		MainState::E_MAIN	
#define E_JOIN		MainState::E_JOIN
#define E_EXIT		MainState::E_EXIT
#define E_DISCON	MainState::E_DISCONNECTED
};