#pragma once
#include "StateIdentifiers.h"
#include "ResourceIdentifiers.h"

#include <memory>
#include "Common/GameTimer.h"
#include "Command.h"
#include "Player.hpp"
#include "SceneNode.hpp"

namespace sf
{
	class RenderWindow;
}

class StateStack;
class Player;
class Game;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	struct Context
	{
		Context(Game* game, Player* player);

		Game* mGame;
		Player* mPlayer;
	};


public:
	State(StateStack* stack, Context* context);
	virtual ~State();

	virtual void		buildState() = 0;
	virtual void		draw() = 0;
	virtual bool		update(const GameTimer& dt) = 0;
	virtual bool		handleEvent(Command& event) = 0;


protected:
	void				requestStackPush(States::ID stateID);
	void				requestStackPop();
	void				requestStateClear();

	Context*				getContext() const;


public:
	StateStack*			mStack;
	Context*				mContext;
	std::unique_ptr<SceneNode> mSceneGraph;

	std::vector<std::unique_ptr<RenderItem>> mAllRitems;
};

#pragma endregion