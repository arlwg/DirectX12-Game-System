#include "State.h"
#include "StateStack.h"


State::Context::Context(Game* game, Player* player)
	: mGame(game)
	 , mPlayer(player)
{
}

State::State(StateStack* stack, Context* context)
	: mStack(stack)
	, mContext(context)
	, mSceneGraph(std::make_unique<SceneNode>(context->mGame))
{
}

State::~State() = default;

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStateClear()
{
	mStack->clearStates();
}

State::Context* State::getContext() const
{
	return mContext;
}

#pragma endregion
