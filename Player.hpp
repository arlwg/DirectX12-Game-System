#pragma once
#include "Command.h"
#include <map>

class CommandQueue;

class Player
{
public:
	Player();
	void					handleEvent(CommandQueue& commands);
	void					handleRealtimeInput(CommandQueue& commands);

	enum Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		ActionCount
	};

	void					assignKey(Action action, char key);
	char					getAssignedKey(Action action) const;

public:
	void					initializeActions();
	static bool				isRealtimeAction(Action action);
	
public:
	std::map<char, Action>					mKeyBinding;
	std::map<Action, Command>				mActionBinding;
	std::map<char, bool>					mKeyFlag;


};