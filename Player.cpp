#include "Player.hpp"
#include "CommandQueue.h"
#include "Aircraft.hpp"
#include "Common/MathHelper.h"
#include "Common/d3dApp.h"
#include <map>
#include <string>
#include <algorithm>
#include <stdio.h>

using namespace DirectX;
struct AircraftMover
{
	AircraftMover(float vx, float vy, float vz)
		: velocity(vx, vy, vz)
	{
	}

	void operator() (Aircraft& aircraft, const GameTimer&) const
	{
		aircraft.move(velocity);

		if (GetAsyncKeyState('E') & 0x8000)
		{
			bool hit = false;
			if (!hit)
			{
				aircraft.setWorldRotation(-0.3f, aircraft.getWorldRotation().y, aircraft.getWorldRotation().z);
			}
		}
		if (GetAsyncKeyState('Q') & 0x8000)
		{
			bool hit = false;
			if (!hit)
			{

			
				aircraft.setWorldRotation(0.3f, aircraft.getWorldRotation().y, aircraft.getWorldRotation().z);
			}
		}
		if (GetAsyncKeyState('A') & 0x8000)
		{
			bool hit = false;
			if (!hit)
			{
				
				aircraft.setWorldRotation(aircraft.getWorldRotation().x, -0.1, 0.5);
			}
		}
		if ((GetAsyncKeyState('D') & 0x8000))
		{
			bool hit = false;
			if (!hit)
			{
				
				aircraft.setWorldRotation(aircraft.getWorldRotation().x, 0.1, -0.5);
			}
		}

		if ((!(GetAsyncKeyState('D') & 0x8000) && !(GetAsyncKeyState('A') & 0x8000)) ||
			((GetAsyncKeyState('D') & 0x8000) && (GetAsyncKeyState('A') & 0x8000)))
		{
			bool hit = false;
			if (!hit)
			{
				
				aircraft.setWorldRotation(aircraft.getWorldRotation().x, 0, 0);
			}
		}
		if ((!(GetAsyncKeyState('E') & 0x8000) && !(GetAsyncKeyState('Q') & 0x8000)) ||
			((GetAsyncKeyState('E') & 0x8000) && (GetAsyncKeyState('Q') & 0x8000)))
		{
			bool hit = false;
			if (!hit)
			{
				
				aircraft.setWorldRotation(0, aircraft.getWorldRotation().y, aircraft.getWorldRotation().z);
			}
		}
	}

	XMFLOAT3 velocity;
};

Player::Player()
{
	// Set key bindings
	mKeyBinding[VK_LEFT] = MoveLeft;
	mKeyBinding[VK_RIGHT] = MoveRight;
	mKeyBinding[VK_UP] = MoveUp;
	mKeyBinding[VK_DOWN] = MoveDown;

	mKeyBinding['A'] = MoveLeft;
	mKeyBinding['D'] = MoveRight;
	mKeyBinding['W'] = MoveForward;
	mKeyBinding['S'] = MoveBackward;
	mKeyBinding['E'] = MoveUp;
	mKeyBinding['Q'] = MoveDown;

	// Set action bindings
	initializeActions();

	for (auto pair : mKeyBinding)
	{
		mKeyFlag[pair.first] = false;
	}

	// Assign categories to aircraft(player)
	for (auto& pair : mActionBinding)
		pair.second.category = Category::PlayerAircraft;
}

void Player::handleEvent(CommandQueue& commands)
{
	for (auto pair : mKeyBinding)
	{
		if (!isRealtimeAction(pair.second))
		{
			if (mKeyFlag[pair.first])
			{
				if (!GetAsyncKeyState(pair.first))
				{
					//UP
					mKeyFlag[pair.first] = false; 
				}
			}
			else
			{
				if (GetAsyncKeyState(pair.first) & 0x8000)
				{
					//DOWN
					mKeyFlag[pair.first] = true; 
					commands.push(mActionBinding[pair.second]);
				}
			}
		}
	}
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	for (auto pair : mKeyBinding)
	{
		if (GetAsyncKeyState(pair.first) & 0x8000 && isRealtimeAction(pair.second))
		{
			//HOLD
			commands.push(mActionBinding[pair.second]);
		}
	}
}

void Player::assignKey(Action action, char key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

char Player::getAssignedKey(Action action) const
{
	for (auto pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return 0x00;
}

void Player::initializeActions()
{
	const float playerSpeed = 5.f;

	mActionBinding[MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.f, 0.0f));
	mActionBinding[MoveRight].action = derivedAction<Aircraft>(AircraftMover(+playerSpeed, 0.f, 0.0f));
	mActionBinding[MoveForward].action = derivedAction<Aircraft>(AircraftMover(0.f, 0.0f, +playerSpeed));
	mActionBinding[MoveBackward].action = derivedAction<Aircraft>(AircraftMover(0.f, 0.0f, -playerSpeed));
	mActionBinding[MoveUp].action = derivedAction<Aircraft>(AircraftMover(0.f, +playerSpeed,0.0f ));
	mActionBinding[MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.f, -playerSpeed, 0.0f));
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
	case MoveLeft:
	case MoveRight:
	case MoveDown:
	case MoveUp:
	case MoveForward:
	case MoveBackward:
		return true;

	default:
		return false;
	}
}

#pragma endregion
